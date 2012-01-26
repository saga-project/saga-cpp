//  Copyright (c) 2005-2011 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 
#include <map>
#include <vector>

#include <boost/assert.hpp>
#include <boost/tokenizer.hpp>

#include <saga/saga-defs.hpp>

#include <saga/saga/util.hpp>
#include <saga/saga/path_leaf.hpp>
#include <saga/saga/exception.hpp>
#include <saga/saga/url.hpp>
#include <saga/saga/adaptors/task.hpp>
#include <saga/saga/adaptors/serialization.hpp>
#include <saga/saga/adaptors/register_workitem.hpp>

#include <saga/impl/config.hpp>
#include <saga/impl/exception_list.hpp>

#include "advert.hpp"
#include "common_helpers.hpp"

///////////////////////////////////////////////////////////////////////////////
// wrap database calls and catch all exceptions
#define SAGA_ADVERT_DBACTION(name, f)                                         \
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
        strm << "advert::advert_cpi_impl::" #name ": "                        \
             << e.what();                                                     \
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::NoSuccess);   \
    }                                                                         \
    /**/
    
///////////////////////////////////////////////////////////////////////////////
namespace advert
{

///////////////////////////////////////////////////////////////////////////////
//  constructor
advert_cpi_impl::advert_cpi_impl (proxy* p, cpi_info const& info,
        saga::ini::ini const& glob_ini, saga::ini::ini const& adap_ini,
        TR1::shared_ptr<saga::adaptor> adaptor)
  : base_cpi (p, info, adaptor, cpi::Noflags),
    monitor_handle_(0),
    session_(p->get_session()),
    opened_(false),
    db_(detail::get_db_type<instance_data>(adap_ini, p), 
        detail::get_db_connectstring<instance_data>(adap_ini, p->get_session(), p))
  {
    saga::url advert_url;
    std::string url;
    {
        // first usage of this adaptor
        instance_data data (this);
        advert_url = data->location_.clone();
        url = saga::url::unescape(advert_url.get_path());
        if (url.empty())
            url = "/";      // root only

        SAGA_LOG(SAGA_VERBOSE_LEVEL_DEBUG) 
            << "Trying  to create advert entry instance for: "
            << clean_url (advert_url);

        // we understand adverts only for localhost
        std::string host(advert_url.get_host());
        if (!host.empty() && !detail::host_is_valid(db_.get_dbtype(), host))
        {
            SAGA_OSSTREAM strm;
            strm << "advert::advert_cpi_impl::init: "
                    "cannot handle advert entry host: " 
                 << advert_url.get_string();
            SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
        }

        // we understand only 'any' and 'advert' url schemes
        std::string scheme(advert_url.get_scheme());
        if ( !scheme.empty ()   && 
             scheme != "advert" && 
             scheme != "any"    )
        {
            SAGA_OSSTREAM strm;
            strm << "advert::advert_cpi_impl::init: "
                    "cannot handle advert entry scheme: " << advert_url.get_string();
            SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
        }
    }

    database_scope db_scope(this, db_);

    {    
        instance_data data (this);

        saga::advert::flags mode = (saga::advert::flags)data->mode_;
        if (((mode & saga::advert::Create) || (mode & saga::advert::CreateParents)) && 
            (mode & saga::advert::Exclusive)) 
        {
            if (db_.exists_entry(url))
            {
                SAGA_OSSTREAM strm;
                strm << "advert::advert_cpi_impl::init: "
                        "advert already exists: " << advert_url.get_string();
                SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::AlreadyExists);
            }
        }

        // Create advert if saga::advert::create flag is given
        if ((mode & saga::advert::Create) || (mode & saga::advert::CreateParents))
        {
            SAGA_ADVERT_DBACTION(advert_cpi_impl, 
                (db_.create_entry(url, (mode & saga::advert::CreateParents) ? true : false)));

            // no need to create this entry twice
            data->mode_ &= ~(saga::advert::Create | saga::advert::CreateParents);
        }
    }

    bool result = false;

    SAGA_ADVERT_DBACTION(advert_cpi_impl, 
        (result = db_.exists_entry(url)));

    if (!result)
    { 
        SAGA_OSSTREAM strm;
        strm << "advert::advert_cpi_impl::init: "
                "advert does not exist: " << advert_url.get_string();
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::DoesNotExist);
    }

    opened_ = true;

    setup_monitoring();

    SAGA_LOG(SAGA_VERBOSE_LEVEL_DEBUG) 
        << "Created advert entry instance for: "
        << clean_url (advert_url);
}

///////////////////////////////////////////////////////////////////////////////
void advert_cpi_impl::setup_monitoring()
{
    // register a new work item to be called 100ms from now
    monitor_handle_ = saga::adaptors::register_workitem(
        TR1::bind(&advert_cpi_impl::monitor_entry, this, TR1::placeholders::_1), 
        saga::adaptors::duration_type(boost::posix_time::milliseconds(100)),
        session_);
}

///////////////////////////////////////////////////////////////////////////////
void advert_cpi_impl::monitor_entry(boost::system::error_code const& e)
{
    monitor_handle_ = 0;    // no need to delete anymore

    if (!e) return;         // some error occurred

    try {
        std::string key("utime");
        std::string adname;
        {
            instance_data data (this);
            adname = detail::get_filepath(data->location_);
        }
        
        bool exists = false;
            
    // fetch modification time
        std::string utime;
        mutex_type::scoped_lock l(mtx_);

        {
        // read entry
            database_scope db_scope(this, db_);
            exists = false;
            SAGA_ADVERT_DBACTION(attribute_exists, 
                (db_.attribute_exists(adname, key, exists)));

            if (!exists) {
                saga::monitorable monitor (this->proxy_);
                saga::adaptors::metric m (
                    monitor.get_metric(saga::advert::metrics::advert_deleted));
                m.set_attribute(saga::attributes::metric_value, adname);
                m.fire();
            }
            else {
                bool is_vector = false;
                SAGA_ADVERT_DBACTION(get_attribute, 
                    (db_.get_attribute(adname, key, utime, is_vector)));
            }
        }

    // if the modification time has been changed, fire metric
        if (exists && !last_modification_time_.empty() && 
            utime != last_modification_time_) 
        {
            saga::monitorable monitor (this->proxy_);
            saga::adaptors::metric m (
                monitor.get_metric(saga::advert::metrics::advert_modified));
            m.set_attribute(saga::attributes::metric_value, adname);
            m.fire();
            last_modification_time_ = utime;
        }

    // reschedule this work item, the current work item will be unregistered 
    // automatically
        if (exists)
            setup_monitoring();
    }
    catch (...) {
        // just ignore any error in the monitoring thread
    }
}

///////////////////////////////////////////////////////////////////////////////
//  destructor
advert_cpi_impl::~advert_cpi_impl (void)
{
    opened_ = false;
    try {
        mutex_type::scoped_lock lock(mtx_);

        if (0 != monitor_handle_) {
            try {
                saga::adaptors::workitem_cookie_handle monitor_handle = monitor_handle_;
                monitor_handle_ = 0;
                saga::adaptors::unregister_workitem(monitor_handle, session_);
            }
            catch (...) {
                // ignore any errors here, the item might have been unregistered 
                // already  
            }
        }

        database_scope db_scope(this, db_);
        SAGA_ADVERT_DBACTION(~advert_cpi_impl, db_.close(false));
    }
    catch (...) {
        // ignore any errors here, we can't report errors from the destructor
    }
}

///////////////////////////////////////////////////////////////////////////////
//  SAGA CPI functions 
void advert_cpi_impl::check_if_open (std::string const& functionname, 
        saga::url const& location)
{
    if (!opened_)
    {
        SAGA_OSSTREAM strm;
        strm << functionname << ": advert entry is not in open state: " 
             << location.get_string();
        SAGA_ADAPTOR_THROW (SAGA_OSSTREAM_GETSTRING (strm), saga::IncorrectState);
    }
}

///////////////////////////////////////////////////////////////////////////////
// attribute functions
void 
advert_cpi_impl::sync_attribute_exists(bool& ret, std::string key)
{
    std::string adname;
    {
        instance_data data (this);
        check_if_open("advert_cpi_impl::sync_attribute_exists", data->location_);
        adname = detail::get_filepath(data->location_);
    }

    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_ADVERT_DBACTION(attribute_exists, 
        (db_.attribute_exists(adname, key, ret)));
}

void 
advert_cpi_impl::sync_attribute_is_readonly(bool& ret, std::string key)
{
    instance_data data (this);
    check_if_open("advert_cpi_impl::sync_attribute_is_readonly", data->location_);
    ret = !(data->mode_ & saga::advert::Write);
}

void 
advert_cpi_impl::sync_attribute_is_writable(bool& ret, std::string key)
{
    instance_data data (this);
    check_if_open("advert_cpi_impl::sync_attribute_is_writable", data->location_);
    ret = (data->mode_ & saga::advert::Write) ? true : false;
}

///////////////////////////////////////////////////////////////////////////////
void 
advert_cpi_impl::check_permissions(saga::advert::flags flags,
    char const* name, std::string const& adname)
{
    instance_data data (this);
    if (!(data->mode_ & flags)) {
        SAGA_OSSTREAM strm;
        strm << "advert::advert_cpi_impl::" << name << ": "
                "could not access(" 
             << ((flags == saga::advert::Read) ? "read" : "write")
             << ") attribute for advert: " 
             << adname;
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), 
            saga::PermissionDenied);
    }
}

void 
advert_cpi_impl::sync_attribute_is_vector(bool& ret, std::string key)
{
    std::string adname;
    {
        instance_data data (this);
        check_if_open("advert_cpi_impl::sync_attribute_is_vector", data->location_);
        adname = detail::get_filepath(data->location_);
    }
    
    // check permissions (throws on mismatch)
    check_permissions(saga::advert::Read, "attribute_is_vector", adname);
    
    // read attribute from db 
    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    std::string val;
    SAGA_ADVERT_DBACTION(attribute_is_vector, 
        (db_.get_attribute(adname, key, val, ret)));
}

void 
advert_cpi_impl::sync_attribute_is_extended(bool& ret, std::string key)
{
    instance_data data (this);
    check_if_open("advert_cpi_impl::sync_attribute_is_extended", data->location_);
    ret = (data->mode_ & saga::advert::Write) ? true : false;
}

///////////////////////////////////////////////////////////////////////////////
void 
advert_cpi_impl::sync_get_attribute(std::string& ret, std::string key)
{
    std::string adname;
    {
        instance_data data (this);
        check_if_open("advert_cpi_impl::sync_get_attribute", data->location_);
        adname = detail::get_filepath(data->location_);
    }
    
    // check permissions (throws on mismatch)
    check_permissions(saga::advert::Read, "get_attribute", adname);

    // read attribute from db 
    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    bool is_vector = false;
    std::string val;
    SAGA_ADVERT_DBACTION(get_attribute, 
        (db_.get_attribute(adname, key, val, is_vector)));

    if (!is_vector) {
        ret = val;
    }
    else {
        SAGA_OSSTREAM strm;
        strm << "advert::advert_cpi_impl::sync_get_attribute: "
                "attribute '" << key << "' is a vector attribute for: " << adname;
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), 
            saga::IncorrectState);
    }
}

///////////////////////////////////////////////////////////////////////////////
void 
advert_cpi_impl::sync_get_vector_attribute(std::vector<std::string>& ret, 
    std::string key)
{
    std::string adname;
    {
        instance_data data (this);
        check_if_open("advert_cpi_impl::sync_get_vector_attribute", data->location_);
        adname = detail::get_filepath(data->location_);
    }

    // check permissions (throws on mismatch)
    check_permissions(saga::advert::Read, "get_vector_attribute", adname);

    // read attribute from db 
    std::string val;
    bool is_vector = false;
    bool result    = false;

    {
        mutex_type::scoped_lock lock(mtx_);
        database_scope db_scope(this, db_);

        SAGA_ADVERT_DBACTION(get_vector_attribute, 
            (result = db_.get_attribute(adname, key, val, is_vector)));
    }        
    if (result) {
        if (is_vector) {
            detail::tokenize_values(val, ret);
        }
        else {
            SAGA_OSSTREAM strm;
            strm << "advert::advert_cpi_impl::sync_get_vector_attribute: "
                    "attribute '" << key << "' is not a vector attribute for: " 
                 << adname;
            SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), 
                saga::IncorrectState);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void 
advert_cpi_impl::sync_set_attribute(saga::impl::void_t&, std::string key, 
    std::string val)
{
    std::string adname;
    {
        instance_data data (this);
        check_if_open("advert_cpi_impl::sync_set_attribute", data->location_);
        adname = detail::get_filepath(data->location_);
    }

    // check permissions (throws on mismatch)
    check_permissions(saga::advert::Write, "set_attribute", adname);
    
    // write attribute to db
    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_ADVERT_DBACTION(set_attribute, 
        (db_.set_attribute(adname, key, val, false)));
}

///////////////////////////////////////////////////////////////////////////////
void 
advert_cpi_impl::sync_set_vector_attribute(saga::impl::void_t&, std::string key, 
    std::vector<std::string> val)
{
    std::string adname;
    {
        instance_data data (this);
        check_if_open("advert_cpi_impl::sync_set_vector_attribute", data->location_);
        adname = detail::get_filepath(data->location_);
    }

    // check permissions (throws on mismatch)
    check_permissions(saga::advert::Write, "get_vector_attribute", adname);
    
    // write attribute to db
    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_ADVERT_DBACTION(set_vector_attribute, 
        (db_.set_attribute(adname, key, detail::concat_values(val), true)));
}

///////////////////////////////////////////////////////////////////////////////
void 
advert_cpi_impl::sync_remove_attribute(saga::impl::void_t&, std::string key)
{
    std::string adname;
    {
        instance_data data (this);
        check_if_open("advert_cpi_impl::sync_remove_attribute", data->location_);
        adname = detail::get_filepath(data->location_);
    }

    // check permissions (throws on mismatch)
    check_permissions(saga::advert::Write, "remove_attribute", adname);
    
    // remove attribute from db
    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_ADVERT_DBACTION(remove_attribute, 
        (db_.remove_attribute(adname, key)));
}

///////////////////////////////////////////////////////////////////////////////
void 
advert_cpi_impl::sync_list_attributes(std::vector<std::string>& keys)
{
    std::string adname;
    {
        instance_data data (this);
        check_if_open("advert_cpi_impl::sync_list_attributes", data->location_);
        adname = detail::get_filepath(data->location_);
    }

    // check permissions (throws on mismatch)
    check_permissions(saga::advert::Read, "list_attributes", adname);
    
    // get attributes from db
    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_ADVERT_DBACTION(list_attributes, 
        (db_.list_attributes(adname, keys)));
}

///////////////////////////////////////////////////////////////////////////////
void 
advert_cpi_impl::sync_find_attributes(std::vector<std::string>& keys, 
    std::string pattern)
{
    std::string adname;
    {
        instance_data data (this);
        check_if_open("advert_cpi_impl::sync_find_attributes", data->location_);
        adname = detail::get_filepath(data->location_);
    }

    // check permissions (throws on mismatch)
    check_permissions(saga::advert::Read, "find_attributes", adname);
    
    // extract patterns from 'key=value' expression
    std::string kpat, vpat;
    detail::parse_pattern(this, pattern, kpat, vpat);

    // look for matching keys in the database
    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_ADVERT_DBACTION(find_attributes, 
        (db_.find_attributes(adname, detail::regex_from_pattern(kpat), 
            detail::regex_from_pattern(vpat), &keys)));
}

///////////////////////////////////////////////////////////////////////////////
// namespace_entry functions
void 
advert_cpi_impl::sync_get_url(saga::url& url)
{
    instance_data data (this);
    check_if_open("advert_cpi_impl::sync_get_url", data->location_);

    {
        mutex_type::scoped_lock lock(mtx_);
        database_scope db_scope(this, db_);

        SAGA_ADVERT_DBACTION(get_url, (db_.verify_status()));  // throws when db is closed
    }

    url = data->location_.clone();
}

// spec: "For not-directory type entries, the URL returned on get_url MUST
//        equal the concatenation of the return values of get_cwd() and 
//        get_name()"
void 
advert_cpi_impl::sync_get_cwd(saga::url& url)
{
    saga::url result;
    {
        instance_data data (this);
        check_if_open("advert_cpi_impl::sync_get_cwd", data->location_);
        result = data->location_;
    }

    {
        mutex_type::scoped_lock lock(mtx_);
        database_scope db_scope(this, db_);

        SAGA_ADVERT_DBACTION(get_cwd, (db_.verify_status()));  // throws when db is closed
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
advert_cpi_impl::sync_get_name(saga::url& url)
{
    saga::url result;
    {
        instance_data data (this);
        check_if_open("advert_cpi_impl::sync_get_name", data->location_);
        result = data->location_;
    }

    {
        mutex_type::scoped_lock lock(mtx_);
        database_scope db_scope(this, db_);

        SAGA_ADVERT_DBACTION(get_name, (db_.verify_status()));  // throws when db is closed
    }

    std::string dirpath(detail::get_filepath(result));
    std::string::size_type pos = dirpath.find_last_of('/');
    if (pos != std::string::npos)
        dirpath = dirpath.substr(pos + 1);

    boost::filesystem::path p(dirpath);
    url.set_path(saga::detail::leaf(p));
}

void 
advert_cpi_impl::sync_is_dir(bool& ret)
{
    {
        instance_data data (this);
        check_if_open("advert_cpi_impl::sync_is_dir", data->location_);
    }

    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_ADVERT_DBACTION(is_dir, (db_.verify_status()));     // throws when db is closed
    ret = false;
}

void 
advert_cpi_impl::sync_is_entry(bool& ret)
{
    {
        instance_data data (this);
        check_if_open("advert_cpi_impl::sync_is_entry", data->location_);
    }

    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_ADVERT_DBACTION(is_entry, (db_.verify_status()));   // throws when db is closed
    ret = true;
}

void 
advert_cpi_impl::sync_is_link(bool& ret)
{
    {
        instance_data data (this);
        check_if_open("advert_cpi_impl::sync_is_link", data->location_);
    }

    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_ADVERT_DBACTION(is_link, (db_.verify_status()));    // throws when db is closed
    ret = false;                // we don't support links
}

void 
advert_cpi_impl::sync_remove(saga::impl::void_t&, int flags)
{
    std::string adname;
    {
        instance_data data (this);
        check_if_open("advert_cpi_impl::sync_remove", data->location_);
        adname = detail::get_filepath(data->location_);
    }

    if (flags & saga::advert::Recursive) {
        SAGA_ADAPTOR_THROW(
            "advert::advert_cpi_impl::sync_remove: "
            "Recursive flag was specified while attempting to delete a file",
            saga::BadParameter);
    }

    // check permissions (throws on mismatch)
    check_permissions(saga::advert::Write, "remove", adname);

    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_ADVERT_DBACTION(remove, (db_.remove(adname), db_.close()));
    opened_ = false;
}

void advert_cpi_impl::sync_close(saga::impl::void_t&, double)
{
    mutex_type::scoped_lock lock(mtx_);

    if (0 != monitor_handle_) {
        try {
            saga::adaptors::workitem_cookie_handle monitor_handle = monitor_handle_;
            monitor_handle_ = 0;
            saga::adaptors::unregister_workitem(monitor_handle, session_);
        }
        catch (...) {
            // ignore any errors here, the item might have been unregistered 
            // already  
        }
    }

    database_scope db_scope(this, db_);

    opened_ = false;
    SAGA_ADVERT_DBACTION(close, db_.close());
}


///////////////////////////////////////////////////////////////////////////////
// advert functions
void 
advert_cpi_impl::sync_store_object(saga::impl::void_t&, saga::object obj)
{
    std::string adname;
    {
        instance_data data (this);
        check_if_open("advert_cpi_impl::sync_store_object", data->location_);
        adname = detail::get_filepath(data->location_);
    }

    // check permissions (throws on mismatch)
    check_permissions(saga::advert::Write, "store_object", adname);
    
    // serialize and store object
    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_ADVERT_DBACTION(store_object, 
        (db_.store_data(adname, saga::adaptors::serialize(obj))));
}

void 
advert_cpi_impl::sync_retrieve_object(saga::object& ret, saga::session s)
{
    std::string adname;
    {
        instance_data data (this);
        check_if_open("advert_cpi_impl::sync_retrieve_object", data->location_);
        adname = detail::get_filepath(data->location_);
    }

    // check permissions (throws on mismatch)
    check_permissions(saga::advert::Read, "retireve_object", adname);
    
    // retrieve serialized object
    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    std::string data;
    SAGA_ADVERT_DBACTION(retrieve_object, (db_.retrieve_data(adname, data)));
    ret = saga::adaptors::deserialize(s, data);
}

void 
advert_cpi_impl::sync_store_string(saga::impl::void_t&, std::string str)
{
    std::string adname;
    {
        instance_data data (this);
        check_if_open("advert_cpi_impl::sync_store_string", data->location_);
        adname = detail::get_filepath(data->location_);
    }

    // check permissions (throws on mismatch)
    check_permissions(saga::advert::Write, "store_object", adname);
    
    // store string
    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_ADVERT_DBACTION(store_object, (db_.store_data(adname, str)));
}

void 
advert_cpi_impl::sync_retrieve_string(std::string& ret)
{
    std::string adname;
    {
        instance_data data (this);
        check_if_open("advert_cpi_impl::sync_retrieve_string", data->location_);
        adname = detail::get_filepath(data->location_);
    }

    // check permissions (throws on mismatch)
    check_permissions(saga::advert::Read, "retireve_object", adname);
    
    // retrieve string
    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_ADVERT_DBACTION(retrieve_object, (db_.retrieve_data(adname, ret)));
}

std::string 
advert_cpi_impl::clean_url (const saga::url & u)
{
  saga::url tmp = u;
  if ( ! tmp.get_password ().empty () )
  {
    tmp.set_password ("************");
  }

  return tmp.get_string ();
}

///////////////////////////////////////////////////////////////////////////////
}   // namespace advert

