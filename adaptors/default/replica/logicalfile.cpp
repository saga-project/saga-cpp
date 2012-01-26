//  Copyright (c) 2005-2011 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 
#include <map>
#include <vector>

#include <boost/assert.hpp>
#include <boost/tokenizer.hpp>

#include <saga/saga/util.hpp>
#include <saga/saga/path_leaf.hpp>
#include <saga/saga/exception.hpp>
#include <saga/saga/url.hpp>
#include <saga/saga/adaptors/task.hpp>
#include <saga/saga/filesystem.hpp>

#include <saga/impl/config.hpp>
#include <saga/impl/exception_list.hpp>

#include "config.hpp"
#include "logicalfile.hpp"
#include "common_helpers.hpp"

///////////////////////////////////////////////////////////////////////////////
// wrap database calls and catch all exceptions
#define SAGA_REPLICA_DBACTION(name, f)                                        \
    try {                                                                     \
        (f);                                                                  \
    }                                                                         \
    catch (saga::adaptors::exception const& e) {                              \
    /* just rethrow using this cpi instance */                                \
        saga::impl::exception_list exceptions;                                \
        exceptions.add(e);                                                    \
        SAGA_ADAPTOR_THROW_PLAIN_LIST(this, exceptions);                      \
    }                                                                         \
    catch (std::exception const& e) {                                         \
    /* wrap this error in a SAGA adaptor exception */                         \
        SAGA_OSSTREAM strm;                                                   \
        strm << "replica::logical_file_cpi_impl::" #name ": "                 \
             << e.what();                                                     \
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::NoSuccess);   \
    }                                                                         \
    /**/

///////////////////////////////////////////////////////////////////////////////
namespace replica
{

///////////////////////////////////////////////////////////////////////////////
//  constructor
logical_file_cpi_impl::logical_file_cpi_impl (proxy* p, cpi_info const& info,
        saga::ini::ini const& glob_ini, saga::ini::ini const& adap_ini,
        TR1::shared_ptr<saga::adaptor> adaptor)
:   base_cpi (p, info, adaptor, cpi::Noflags),
    db_(detail::get_db_type<instance_data>(adap_ini, p), 
        detail::get_db_connectstring<instance_data>(adap_ini, p->get_session(), p)),
    thrd_(NULL), 
    cond_(NULL),
    thread_alive_(false), cancel_thread_(false), opened_(false)
{
    database_scope db_scope(this, db_);
    saga::url lfn_url;
    std::string url;
    {
        // first usage of this adaptor
        instance_data data (this);
        lfn_url = data->location_.clone();
        url = saga::url::unescape(lfn_url.get_path());
        if (url.empty())
            url = "/";      // root only

        // we understand lfn's only for localhost
        std::string host(lfn_url.get_host());
        if (!host.empty() && !detail::host_is_valid(db_.get_dbtype(), host))
        {
            SAGA_OSSTREAM strm;
            strm << "replica::logical_file_cpi_impl::init: "
                    "cannot handle logical file name: " 
                 << lfn_url.get_string();
            SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::IncorrectURL);
        }

        // we understand only 'any' and 'lfn' url schemes
        std::string scheme(lfn_url.get_scheme());
        if (!scheme.empty() && scheme != "lfn" && scheme != "any" && scheme != "replica")
        {
            SAGA_OSSTREAM strm;
            strm << "replica::logical_file_cpi_impl::init: "
                    "cannot handle logical file name: " << lfn_url.get_string();
            SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
        }
        
        saga::replica::flags mode = (saga::replica::flags)data->mode_;
        if (((mode & saga::replica::Create) || 
             (mode & saga::replica::CreateParents)) && 
            (mode & saga::replica::Exclusive)) 
        {
            if (db_.exists_entry(url))
            {
                SAGA_OSSTREAM strm;
                strm << "replica::logical_file_cpi_impl::init: "
                        "replica entry already exists: " << lfn_url.get_string();
                SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
            }
        }

        // Create replica entry if saga::replica::create flag is given
        if ((mode & saga::replica::Create) || 
            (mode & saga::replica::CreateParents))
        {
            SAGA_REPLICA_DBACTION(logical_file_cpi_impl, 
                (db_.create_entry(url, 
                    (mode & saga::replica::CreateParents) ? true : false)));
            data->mode_ &= ~saga::replica::Create;   // no need to create this entry twice
        }
    }
    
    bool result = false;

    SAGA_REPLICA_DBACTION(logical_file_cpi_impl, 
        (result = db_.exists_entry(url)));
        
    if (!result)
    { 
        SAGA_OSSTREAM strm;
        strm << "replica::logical_file_cpi_impl::init: "
                "replica entry does not exist: " << lfn_url.get_string();
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::DoesNotExist);
    }

    opened_ = true;

    // set up thread monitoring this entry in the database
    // FIXME: the monittoring thread causes segfaults now and then
    // setup_monitoring_thread();
}

///////////////////////////////////////////////////////////////////////////////
void logical_file_cpi_impl::setup_monitoring_thread()
{
    mutex_type::scoped_lock lock(mtx_);
    
    BOOST_ASSERT(NULL == cond_ && NULL == thrd_); // should be called only once

    // set up separate thread watching for job termination
    thread_alive_ = true;
    cancel_thread_ = false;
    try {
        cond_ = new boost::condition();
        thrd_ = new boost::thread(TR1::bind(&logical_file_cpi_impl::monitor_entry, this));
        cond_->wait(lock);
    }
    catch (...) {
        thread_alive_ = false;
        cancel_thread_ = true;
        delete thrd_;
        thrd_ = NULL;
    }
    delete cond_;
    cond_ = NULL;
}

///////////////////////////////////////////////////////////////////////////////
namespace 
{
    struct handle_flag
    {
        handle_flag(logical_file_cpi_impl::mutex_type& mtx, bool& flag) 
          : mtx_(mtx), flag_(flag)
        {
        }
        ~handle_flag()
        {
            logical_file_cpi_impl::mutex_type::scoped_lock l(mtx_);
            flag_ = false;
        }
        
        logical_file_cpi_impl::mutex_type& mtx_;
        bool& flag_;
    };
}

void logical_file_cpi_impl::monitor_entry()
{
    handle_flag handler(mtx_, thread_alive_);   // 'thread_alive_ = false' on exit
    
    bool first = true;
    try {
        std::string key("utime");
        std::string adname;
        {
            instance_data data (this);
            adname = detail::get_filepath(data->location_);
        }
        
        std::string value;
        bool exists = false;
            
    // fetch modification time
        std::string utime;
        do {
            mutex_type::scoped_lock l(mtx_);

            {
            // read entry
                database_scope db_scope(this, db_);
                exists = false;
                SAGA_REPLICA_DBACTION(attribute_exists, 
                    (db_.attribute_exists(adname, key, exists)));
                if (!exists) 
                    break;    // entry has been deleted
                    
                bool is_vector = false;
                SAGA_REPLICA_DBACTION(get_attribute, 
                    (db_.get_attribute(adname, key, utime, is_vector)));
            }

        // try to find out, if entry has been changed
            if (first) {
                value = utime;
                first = false;
                if (NULL != cond_) 
                    cond_->notify_one();        // initialization is complete
            }
            else {
                boost::xtime xt;
                boost::xtime_get(&xt, boost::TIME_UTC);
                xt.nsec += 10000000;            // sleep max. 10ms
                if (cancel_thread_ || cancel_thrd_.timed_wait(l, xt))
                    return;                     // thread has been canceled

            // if the modification time has been changed, fire metric
                if (utime != value) {
                    saga::monitorable monitor (this->proxy_);
                    saga::adaptors::metric m (
                                              monitor.get_metric(saga::replica::metrics::logical_file_modified));
                    m.set_attribute(saga::attributes::metric_value, adname);
                    m.fire();
                    value = utime;
                }
            }
        } while (true);
            
    // if entry doesn't exist anymore, fire deleted metric
        BOOST_ASSERT(!exists);
        saga::monitorable monitor (this->proxy_);
        saga::adaptors::metric m (
                                  monitor.get_metric(saga::replica::metrics::logical_file_deleted));
        m.set_attribute(saga::attributes::metric_value, adname);
        m.fire();
    }
    catch (...) {
        // just ignore any error in the monitoring thread
        if (first) {
            mutex_type::scoped_lock l(mtx_);
            if (NULL != cond_) 
                cond_->notify_one();        // initialization is complete
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
//  destructor
logical_file_cpi_impl::~logical_file_cpi_impl (void)
{
    mutex_type::scoped_lock lock(mtx_);

    if (NULL != thrd_) {
        if (thread_alive_) {
            cancel_thread_ = true;        // break thread
            cancel_thrd_.notify_one();
            lock.unlock();
            thrd_->join();
        }
        delete thrd_;
        thrd_ = NULL;
    }

    database_scope db_scope(this, db_);
    SAGA_REPLICA_DBACTION(~logical_file_cpi_impl, db_.close(false));
    opened_ = false;
}

///////////////////////////////////////////////////////////////////////////////
//  SAGA CPI functions 
void logical_file_cpi_impl::check_if_open (std::string const& functionname, 
        saga::url const& location)
{
    if (!opened_)
    {
        SAGA_OSSTREAM strm;
        strm << functionname << ": replica entry is not in open state: " 
             << location.get_string();
        SAGA_ADAPTOR_THROW (SAGA_OSSTREAM_GETSTRING (strm), saga::IncorrectState);
    }
}

///////////////////////////////////////////////////////////////////////////////
// attribute functions
void 
logical_file_cpi_impl::sync_attribute_exists(bool& ret, std::string key)
{
    std::string lfn;
    {
        instance_data data (this);
        check_if_open("logical_file_cpi_impl::sync_attribute_exists", data->location_);
        lfn = detail::get_filepath(data->location_);
    }

    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_REPLICA_DBACTION(attribute_exists, 
        (db_.attribute_exists(lfn, key, ret)));
}

void 
logical_file_cpi_impl::sync_attribute_is_readonly(bool& ret, std::string key)
{
    instance_data data (this);
    check_if_open("logical_file_cpi_impl::sync_attribute_is_readonly", data->location_);
    ret = !(data->mode_ & saga::replica::Write);
}

void 
logical_file_cpi_impl::sync_attribute_is_writable(bool& ret, std::string key)
{
    instance_data data (this);
    check_if_open("logical_file_cpi_impl::sync_attribute_is_writable", data->location_);
    ret = (data->mode_ & saga::replica::Write) ? true : false;
}

///////////////////////////////////////////////////////////////////////////////
void 
logical_file_cpi_impl::check_permissions(saga::replica::flags flags,
    char const* name, std::string const& lfn)
{
    instance_data data (this);
    if (!(data->mode_ & flags)) {
        SAGA_OSSTREAM strm;
        strm << "replica::logical_file_cpi_impl::" << name << ": "
                "could not access(" 
             << ((flags == saga::replica::Read) ? "read" : "write")
             << ") attribute for replica entry: " 
             << lfn;
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), 
            saga::PermissionDenied);
    }
}

void 
logical_file_cpi_impl::sync_attribute_is_vector(bool& ret, std::string key)
{
    std::string lfn;
    {
        instance_data data (this);
        check_if_open("logical_file_cpi_impl::sync_attribute_is_vector", data->location_);
        lfn = detail::get_filepath(data->location_);
    }

    // check permissions (throws on mismatch)
    check_permissions(saga::replica::Read, "attribute_is_vector", lfn);
    
    // read attribute from db 
    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    std::string val;
    SAGA_REPLICA_DBACTION(attribute_is_vector, 
        (db_.get_attribute(lfn, key, val, ret)));
}

void 
logical_file_cpi_impl::sync_attribute_is_extended(bool& ret, std::string key)
{
    instance_data data (this);
    check_if_open("logical_file_cpi_impl::sync_attribute_is_extended", data->location_);
    ret = (data->mode_ & saga::replica::Write) ? true : false;
}

///////////////////////////////////////////////////////////////////////////////
void 
logical_file_cpi_impl::sync_get_attribute(std::string& ret, std::string key)
{
    std::string lfn;
    {
        instance_data data (this);
        check_if_open("logical_file_cpi_impl::sync_get_attribute", data->location_);
        lfn = detail::get_filepath(data->location_);
    }
    
    // check permissions (throws on mismatch)
    check_permissions(saga::replica::Read, "get_attribute", lfn);

    // read attribute from db 
    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    std::string val;
    bool is_vector = false;

    SAGA_REPLICA_DBACTION(get_attribute, 
        (db_.get_attribute(lfn, key, val, is_vector)));

    if (!is_vector) {
        ret = val;
    }
    else {
        SAGA_OSSTREAM strm;
        strm << "replica::logical_file_cpi_impl::sync_get_attribute: "
                "attribute '" << key << "' is a vector attribute for: " << lfn;
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), 
            saga::IncorrectState);
    }
}

///////////////////////////////////////////////////////////////////////////////
void 
logical_file_cpi_impl::sync_get_vector_attribute(std::vector<std::string>& ret, 
    std::string key)
{
    std::string lfn;
    {
        instance_data data (this);
        check_if_open("logical_file_cpi_impl::sync_get_vector_attribute", data->location_);
        lfn = detail::get_filepath(data->location_);
    }
    
    // check permissions (throws on mismatch)
    check_permissions(saga::replica::Read, "get_vector_attribute", lfn);

    // read attribute from db 
    std::string val;
    bool is_vector = false;
    bool result = false;

    {
        mutex_type::scoped_lock lock(mtx_);
        database_scope db_scope(this, db_);

        SAGA_REPLICA_DBACTION(get_vector_attribute, 
            (result = db_.get_attribute(lfn, key, val, is_vector)));
    }
    if (result) {
        if (is_vector) {
            detail::tokenize_values(val, ret);
        }
        else {
            SAGA_OSSTREAM strm;
            strm << "replica::logical_file_cpi_impl::sync_get_vector_attribute: "
                    "attribute '" << key << "' is not a vector attribute for: " 
                 << lfn;
            SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), 
                saga::IncorrectState);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void 
logical_file_cpi_impl::sync_set_attribute(saga::impl::void_t&, std::string key, 
    std::string val)
{
    std::string lfn;
    {
        instance_data data (this);
        check_if_open("logical_file_cpi_impl::sync_set_attribute", data->location_);
        lfn = detail::get_filepath(data->location_);
    }

    // check permissions (throws on mismatch)
    check_permissions(saga::replica::Write, "set_attribute", lfn);
    
    // write attribute to db
    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_REPLICA_DBACTION(set_attribute, 
        (db_.set_attribute(lfn, key, val, false)));
}

///////////////////////////////////////////////////////////////////////////////
void 
logical_file_cpi_impl::sync_set_vector_attribute(saga::impl::void_t&, std::string key, 
    std::vector<std::string> val)
{
    std::string lfn;
    {
        instance_data data (this);
        check_if_open("logical_file_cpi_impl::sync_set_vector_attribute", data->location_);
        lfn = detail::get_filepath(data->location_);
    }

    // check permissions (throws on mismatch)
    check_permissions(saga::replica::Write, "get_vector_attribute", lfn);

    // write attribute to db
    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_REPLICA_DBACTION(set_vector_attribute, 
        (db_.set_attribute(lfn, key, detail::concat_values(val), true)));
}

///////////////////////////////////////////////////////////////////////////////
void 
logical_file_cpi_impl::sync_remove_attribute(saga::impl::void_t&, std::string key)
{
    std::string lfn;
    {
        instance_data data (this);
        check_if_open("logical_file_cpi_impl::sync_remove_attribute", data->location_);
        lfn = detail::get_filepath(data->location_);
    }

    // check permissions (throws on mismatch)
    check_permissions(saga::replica::Write, "remove_attribute", lfn);

    // remove attribute from db
    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_REPLICA_DBACTION(remove_attribute, 
        (db_.remove_attribute(lfn, key)));
}

///////////////////////////////////////////////////////////////////////////////
void 
logical_file_cpi_impl::sync_list_attributes(std::vector<std::string>& keys)
{
    std::string lfn;
    {
        instance_data data (this);
        check_if_open("logical_file_cpi_impl::sync_list_attributes", data->location_);
        lfn = detail::get_filepath(data->location_);
    }

    // check permissions (throws on mismatch)
    check_permissions(saga::replica::Read, "list_attributes", lfn);

    // get attributes from db
    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_REPLICA_DBACTION(list_attributes, 
        (db_.list_attributes(lfn, keys)));
}

///////////////////////////////////////////////////////////////////////////////
void 
logical_file_cpi_impl::sync_find_attributes(std::vector<std::string>& keys, 
    std::string pattern)
{
    std::string lfn;
    {
        instance_data data (this);
        check_if_open("logical_file_cpi_impl::sync_find_attributes", data->location_);
        lfn = detail::get_filepath(data->location_);
    }

    // check permissions (throws on mismatch)
    check_permissions(saga::replica::Read, "find_attributes", lfn);

    // extract patterns from 'key=value' expression
    std::string kpat, vpat;
    detail::parse_pattern(this, pattern, kpat, vpat);

    // look for matching keys in the database
    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_REPLICA_DBACTION(find_attributes, 
        (db_.find_attributes(lfn, detail::regex_from_pattern(kpat), 
            detail::regex_from_pattern(vpat), &keys)));
}

///////////////////////////////////////////////////////////////////////////////
// namespace_entry functions
void 
logical_file_cpi_impl::sync_get_url(saga::url& url)
{
    instance_data data (this);
    check_if_open("logical_file_cpi_impl::sync_get_url", data->location_);

    {
        mutex_type::scoped_lock lock(mtx_);
        database_scope db_scope(this, db_);

        SAGA_REPLICA_DBACTION(get_url, (db_.verify_status()));  // throws when db has been closed
    }
    url = data->location_.clone();
}

// spec: "For not-directory type entries, the URL returned on get_url MUST
//        equal the concatenation of the return values of get_cwd() and 
//        get_name()"
void 
logical_file_cpi_impl::sync_get_cwd(saga::url& url)
{
    saga::url result;
    {
        instance_data data (this);
        check_if_open("logical_file_cpi_impl::sync_get_cwd", data->location_);
        result = data->location_;
    }

    {
        mutex_type::scoped_lock lock(mtx_);
        database_scope db_scope(this, db_);

        SAGA_REPLICA_DBACTION(get_cwd, (db_.verify_status()));  // throws when db has been closed
    }

    std::string dirpath(detail::get_filepath(result));
    std::string::size_type pos = dirpath.find_last_of('/');
    if (pos != std::string::npos)
        dirpath = dirpath.substr(0, pos);

    boost::filesystem::path p(dirpath);
    url = result;
    url.set_path(saga::detail::parent(p));
}

void 
logical_file_cpi_impl::sync_get_name(saga::url& url)
{
    saga::url result;
    {
        instance_data data (this);
        check_if_open("logical_file_cpi_impl::sync_get_name", data->location_);
        result = data->location_;
    }

    {
        mutex_type::scoped_lock lock(mtx_);
        database_scope db_scope(this, db_);

        SAGA_REPLICA_DBACTION(get_name, (db_.verify_status()));  // throws when db has been closed
    }

    std::string dirpath(detail::get_filepath(result));
    std::string::size_type pos = dirpath.find_last_of('/');
    if (pos != std::string::npos)
        dirpath = dirpath.substr(pos + 1);

    boost::filesystem::path p(dirpath);
    url.set_path(saga::detail::leaf(p));
}

void 
logical_file_cpi_impl::sync_is_dir(bool& ret)
{
    {
        instance_data data (this);
        check_if_open("logical_file_cpi_impl::sync_is_dir", data->location_);
    }

    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_REPLICA_DBACTION(is_dir, (db_.verify_status()));     // throws when db has been closed
    ret = false;
}

void 
logical_file_cpi_impl::sync_is_entry(bool& ret)
{
    {
        instance_data data (this);
        check_if_open("logical_file_cpi_impl::sync_is_entry", data->location_);
    }

    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_REPLICA_DBACTION(is_entry, (db_.verify_status()));   // throws when db has been closed
    ret = true;
}

void 
logical_file_cpi_impl::sync_is_link(bool& ret)
{
    {
        instance_data data (this);
        check_if_open("logical_file_cpi_impl::sync_is_link", data->location_);
    }

    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_REPLICA_DBACTION(is_link, (db_.verify_status()));    // throws when db has been closed
    ret = false;                // we don't support links
}

void 
logical_file_cpi_impl::sync_remove(saga::impl::void_t&, int flags)
{
    std::string lfn;
    {
        instance_data data (this);
        check_if_open("logical_file_cpi_impl::sync_remove", data->location_);
        lfn = detail::get_filepath(data->location_);
    }

    if (flags & saga::replica::Recursive) {
        SAGA_ADAPTOR_THROW(
            "replica::logical_file_cpi_impl::sync_remove: "
            "Recursive flag was specified while attempting to delete a file",
            saga::BadParameter);
    }

    // check permissions (throws on mismatch)
    check_permissions(saga::replica::Write, "remove", lfn);

    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_REPLICA_DBACTION(remove, (db_.remove(lfn), db_.close()));
    opened_ = false;
}

void logical_file_cpi_impl::sync_close(saga::impl::void_t&, double)
{
    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_REPLICA_DBACTION(close, db_.close());
    opened_ = false;
}


///////////////////////////////////////////////////////////////////////////////
// logical_file functions
void 
logical_file_cpi_impl::sync_list_locations(std::vector<saga::url>& locations)
{
    std::string lfn;
    {
        instance_data data (this);
        check_if_open("logical_file_cpi_impl::sync_list_locations", data->location_);
        lfn = detail::get_filepath(data->location_);
    }

    // check permissions (throws on mismatch)
    check_permissions(saga::replica::Read, "list_locations", lfn);
    
    // get locations from db    
    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    std::vector<std::string> entries;
    SAGA_REPLICA_DBACTION(list_locations, (db_.get_entries(lfn, entries)));

    // create result vector
    std::vector<std::string>::iterator end = entries.end();
    for (std::vector<std::string>::iterator it = entries.begin(); 
         it != end; ++it)
    {
        locations.push_back(saga::url(*it));
    }
}

void 
logical_file_cpi_impl::sync_add_location(saga::impl::void_t&, saga::url location)
{
    std::string lfn;
    {
        instance_data data (this);
        check_if_open("logical_file_cpi_impl::sync_add_location", data->location_);
        lfn = detail::get_filepath(data->location_);
    }

    // check permissions (throws on mismatch)
    check_permissions(saga::replica::Write, "add_location", lfn);
    
    // add new replica
    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_REPLICA_DBACTION(add_location, 
        (db_.add_entry(lfn, location.get_url())));
}

void 
logical_file_cpi_impl::sync_remove_location(saga::impl::void_t&, saga::url location)
{
    std::string lfn;
    {
        instance_data data (this);
        check_if_open("logical_file_cpi_impl::sync_remove_location", data->location_);
        lfn = detail::get_filepath(data->location_);
    }

    // check permissions (throws on mismatch)
    check_permissions(saga::replica::Write, "remove_location", lfn);
    
    // remove replica
    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_REPLICA_DBACTION(remove_location, 
        (db_.remove_entry(lfn, location.get_url())));
}

void 
logical_file_cpi_impl::sync_update_location(saga::impl::void_t&, 
    saga::url oldlocation, saga::url newlocation)
{
    std::string lfn;
    {
        instance_data data (this);
        check_if_open("logical_file_cpi_impl::sync_update_location", data->location_);
        lfn = detail::get_filepath(data->location_);
    }

    // check permissions (throws on mismatch)
    check_permissions(saga::replica::Write, "update_location", lfn);

    // remove old and add new replica
    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_REPLICA_DBACTION(update_location, 
        (db_.update_entry(lfn, oldlocation.get_url(), newlocation.get_url())));
}

void 
logical_file_cpi_impl::sync_replicate(saga::impl::void_t&, saga::url location, 
    int mode)
{
    std::string lfn;
    {
        instance_data data (this);
        check_if_open("logical_file_cpi_impl::sync_replicate", data->location_);
        lfn = detail::get_filepath(data->location_);
    }

    // check permissions (throws on mismatch)
    check_permissions(saga::replica::Read, "replicate", lfn);
    check_permissions(saga::replica::Write, "replicate", lfn);

    // get replicas for the given replica entry
    std::vector<std::string> locations;
    
    bool result = false;
    {
        mutex_type::scoped_lock lock(mtx_);
        database_scope db_scope(this, db_);

        SAGA_REPLICA_DBACTION(replicate, 
            (result = db_.get_entries(lfn, locations)));
    }
    if (!result) {
        SAGA_OSSTREAM strm;
        strm << "replica::logical_file_cpi_impl::replicate: "
                "replica entry: '" << lfn << "' does not contain any replicas.";
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::IncorrectState);
    }
    BOOST_ASSERT(!locations.empty());

    // loop over all replicas trying to copy (replicate) the file
    saga::impl::exception_list exceptions;
    bool copied_file = false;

    std::vector<std::string>::iterator end = locations.end();
    for (std::vector<std::string>::iterator it = locations.begin(); 
         it != end && !copied_file; ++it)
    {
        try {
            saga::filesystem::file f(*it);
            f.copy(location, mode);
        }
        catch (saga::exception const& e) {
            exceptions.add(e);
            continue;
        }
        copied_file = true;
    }

    if (!copied_file) {
        // rethrow the whole error stack 
        SAGA_ADAPTOR_THROW_PLAIN_LIST(this, exceptions);
    }

    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_REPLICA_DBACTION(replicate, (db_.add_entry(lfn, location.get_url())));
}

///////////////////////////////////////////////////////////////////////////////
}   // namespace replica

