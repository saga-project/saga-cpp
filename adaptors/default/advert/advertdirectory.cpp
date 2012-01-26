//  Copyright (c) 2005-2011 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 
#include <map>
#include <vector>

#include <boost/tokenizer.hpp>
#include <boost/filesystem/path.hpp>

#include <saga/saga/util.hpp>
#include <saga/saga/path_leaf.hpp>
#include <saga/saga/exception.hpp>
#include <saga/saga/url.hpp>
#include <saga/saga/adaptors/task.hpp>

#include <saga/impl/config.hpp>
#include <saga/impl/exception_list.hpp>

#include "config.hpp"
#include "advertdirectory.hpp"
#include "common_helpers.hpp"

///////////////////////////////////////////////////////////////////////////////
// wrap database calls and catch all exceptions
#define SAGA_ADVERTDIR_DBACTION(name, f)                                      \
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
        strm << "advert::advertdirectory_cpi_impl::" #name ": "               \
             << e.what();                                                     \
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::NoSuccess);   \
    }                                                                         \
    /**/
    
///////////////////////////////////////////////////////////////////////////////
namespace advert
{

///////////////////////////////////////////////////////////////////////////////
//  constructor
advertdirectory_cpi_impl::advertdirectory_cpi_impl (
        proxy* p, cpi_info const& info,
        saga::ini::ini const& glob_ini, saga::ini::ini const& adap_ini,
        TR1::shared_ptr<saga::adaptor> adaptor)
  : base_cpi (p, info, adaptor, cpi::Noflags),
//     monitor_handle_(0),
    db_(detail::get_db_type<instance_data>(adap_ini, p), 
        detail::get_db_connectstring<instance_data>(adap_ini, p->get_session(), p))
{

    // first usage of this adaptor
    instance_data data (this);
    saga::url advert_url(data->location_);
    std::string url(saga::url::unescape(advert_url.get_path()));
    if (url.empty())
        url = "/";      // root only

    SAGA_LOG(SAGA_VERBOSE_LEVEL_DEBUG) 
        << "Trying to create advert directory instance for: "
        << clean_url (advert_url);

    // we understand absolute paths only
    if ('/' != url[0]) 
    {
        SAGA_OSSTREAM strm;
        strm << "advert::advertdirectory_cpi_impl::init: "
                "cannot handle relative advert directory name: " 
             << advert_url.get_string();
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::IncorrectURL);
    }

    // we understand adname's only for localhost (if sqlite3)
    std::string host(advert_url.get_host());
    if (!host.empty() && !detail::host_is_valid(db_.get_dbtype(), host))
    {
        SAGA_OSSTREAM strm;
        strm << "advert::advertdirectory_cpi_impl::init: "
                "cannot handle advert directory host: " 
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
        strm << "advert::advertdirectory_cpi_impl::init: "
                "cannot handle advert directory scheme: " << advert_url.get_string();
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
    }

    database_scope db_scope(this, db_);

    saga::advert::flags mode = (saga::advert::flags)data->mode_;
    if (((mode & saga::advert::Create) || 
         (mode & saga::advert::CreateParents)) && 
        (mode & saga::advert::Exclusive)) 
    {
        if (db_.exists_directory(url))
        {
            SAGA_OSSTREAM strm;
            strm << "advert::advertdirectory_cpi_impl::init: "
                    "advert already exists: " << advert_url.get_string();
            SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::AlreadyExists);
        }
    }

    // Create advert if saga::advert::directory::create flag is given
    if ((mode & saga::advert::Create) || 
        (mode & saga::advert::CreateParents))
    {
        SAGA_ADVERTDIR_DBACTION(advertdirectory_cpi_impl,
            (db_.create_directory(url, (mode & saga::advert::CreateParents) ? true : false)));

        // no need to create this entry twice
        data->mode_ &= ~(saga::advert::Create | 
                         saga::advert::CreateParents);
    }

    if (!db_.exists_directory(url))
    { 
        SAGA_OSSTREAM strm;
        strm << "advert::advertdirectory_cpi_impl::init: "
                "advert does not exist: " << advert_url.get_string();
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::DoesNotExist);
    }

    opened_ = true;

//     setup_monitoring();

    SAGA_LOG(SAGA_VERBOSE_LEVEL_DEBUG) 
        << "Created advert directory instance for: "
        << clean_url (advert_url);
}

///////////////////////////////////////////////////////////////////////////////
// void advertdirectory_cpi_impl::setup_monitoring()
// {
//     // register a new work item to be called 100ms from now
//     monitor_handle_ = saga::adaptors::register_workitem(
//         TR1::bind(&advertdirectory_cpi_impl::monitor_entry, this, _1), 
//         saga::adaptors::duration_type(boost::posix_time::milliseconds(100)),
//         get_session());
// }

///////////////////////////////////////////////////////////////////////////////
// void advertdirectory_cpi_impl::monitor_entry(boost::system::error_code const& e)
// {
//     monitor_handle_ = 0;    // no need to delete anymore
// 
//     if (!e) return;         // some error occurred
// 
//     try {
//         std::string key("utime");
//         std::string adname;
//         {
//             instance_data data (this);
//             adname = detail::get_filepath(data->location_);
//         }
//         
//         bool exists = false;
//             
//     // fetch modification time
//         std::string utime;
//         mutex_type::scoped_lock l(mtx_);
// 
//         {
//         // read entry
//             database_scope db_scope(this, db_);
//             exists = false;
//             SAGA_ADVERTDIR_DBACTION(attribute_exists, 
//                 (db_.attribute_exists(adname, key, exists)));
// 
//             if (!exists) {
//                 saga::monitorable monitor (this->proxy_);
//                 saga::adaptors::metric m (
//                     monitor.get_metric(saga::advert::metrics::directory_deleted_entry));
//                 m.set_attribute(saga::attributes::metric_value, adname);
//                 m.fire();
//             }
//             else {
//                 bool is_vector = false;
//                 SAGA_ADVERTDIR_DBACTION(get_attribute, 
//                     (db_.get_attribute(adname, key, utime, is_vector)));
//             }
//         }
// 
//     // if the modification time has been changed, fire metric
//         if (exists && !last_modification_time_.empty() && 
//             utime != last_modification_time_) 
//         {
//             saga::monitorable monitor (this->proxy_);
//             saga::adaptors::metric m (
//                 monitor.get_metric(saga::advert::metrics::directory_modified_entry));
//             m.set_attribute(saga::attributes::metric_value, adname);
//             m.fire();
//             last_modification_time_ = utime;
//         }
// 
//     // reschedule this work item, the current work item will be unregistered 
//     // automatically
//         if (exists)
//             setup_monitoring();
//     }
//     catch (...) {
//         // just ignore any error in the monitoring thread
//     }
// }

///////////////////////////////////////////////////////////////////////////////
//  destructor
advertdirectory_cpi_impl::~advertdirectory_cpi_impl (void)
{
//     BOOST_ASSERT(0 == monitor_handle_);    // close should have been called

    opened_ = false;
    try {
        mutex_type::scoped_lock lock(mtx_);
        database_scope db_scope(this, db_);
        SAGA_ADVERTDIR_DBACTION(~advertdirectory_cpi_impl, db_.close(false));
    }
    catch (...) {
        // ignore any errors here, we can't report errors from the destructor
    }
}

///////////////////////////////////////////////////////////////////////////////
//  SAGA CPI functions 
void advertdirectory_cpi_impl::check_if_open (std::string const& functionname, 
        saga::url const& location)
{
    if (!opened_)
    {
        SAGA_OSSTREAM strm;
        strm << functionname << ": advert directory is not in open state: " 
             << location.get_string();
        SAGA_ADAPTOR_THROW (SAGA_OSSTREAM_GETSTRING (strm), saga::IncorrectState);
    }
}


///////////////////////////////////////////////////////////////////////////////
// attribute functions
void 
advertdirectory_cpi_impl::sync_attribute_exists(bool& ret, std::string key)
{
    std::string adname;
    {
        instance_data data (this);
        check_if_open("advertdirectory_cpi_impl::sync_attribute_exists", data->location_);
        adname = detail::get_filepath(data->location_);
    }
    
    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_ADVERTDIR_DBACTION(attribute_exists,
        (db_.attribute_exists(adname, key, ret)));
}

void 
advertdirectory_cpi_impl::sync_attribute_is_readonly(bool& ret, std::string key)
{
    instance_data data (this);
    check_if_open("advertdirectory_cpi_impl::sync_attribute_is_readonly", data->location_);
    ret = !(data->mode_ & saga::advert::Read);
}

void 
advertdirectory_cpi_impl::sync_attribute_is_writable(bool& ret, std::string key)
{
    instance_data data (this);
    check_if_open("advertdirectory_cpi_impl::sync_attribute_is_writable", data->location_);
    ret = (data->mode_ & saga::advert::Write) ? true : false;
}

///////////////////////////////////////////////////////////////////////////////
void 
advertdirectory_cpi_impl::check_permissions(saga::advert::flags flags,
    char const* name, std::string const& adname)
{
    instance_data data (this);
    if (!(data->mode_ & flags)) {
        SAGA_OSSTREAM strm;
        strm << "advert::advertdirectory_cpi_impl::" << name << ": "
                "could not access(" 
             << ((flags & saga::advert::Read) ? "read" : "write")
             << ") attribute for advert: " 
             << adname;
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), 
            saga::PermissionDenied);
    }
}

void 
advertdirectory_cpi_impl::sync_attribute_is_vector(bool& ret, std::string key)
{
    std::string adname;
    {
        instance_data data (this);
        check_if_open("advertdirectory_cpi_impl::sync_attribute_is_vector", data->location_);
        adname = detail::get_filepath(data->location_);
    }
    
    // check permissions (throws on mismatch)
    check_permissions(saga::advert::Read, "attribute_is_vector", adname);
    
    // read attribute from db 
    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    std::string val;
    SAGA_ADVERTDIR_DBACTION(attribute_is_vector,
        (db_.get_attribute(adname, key, val, ret)));
}

void 
advertdirectory_cpi_impl::sync_attribute_is_extended(bool& ret, std::string key)
{
    instance_data data (this);
    check_if_open("advertdirectory_cpi_impl::sync_attribute_is_extended", data->location_);
    ret = (data->mode_ & saga::advert::Write) ? true : false;
}

///////////////////////////////////////////////////////////////////////////////
void 
advertdirectory_cpi_impl::sync_get_attribute(std::string& ret, std::string key)
{
    std::string adname;
    {
        instance_data data (this);
        check_if_open("advertdirectory_cpi_impl::sync_get_attribute", data->location_);
        adname = detail::get_filepath(data->location_);
    }
    
    // check permissions (throws on mismatch)
    check_permissions(saga::advert::Read, "get_attribute", adname);

    // read attribute from db 
    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    bool is_vector = false;
    std::string val;
    SAGA_ADVERTDIR_DBACTION(get_attribute,
        (db_.get_attribute(adname, key, val, is_vector)));

    if (!is_vector) {
        ret = val;
    }
    else {
        SAGA_OSSTREAM strm;
        strm << "advert::advertdirectory_cpi_impl::sync_get_attribute: "
                "attribute '" << key << "' is a vector attribute for: " << adname;
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), 
            saga::IncorrectState);
    }
}

///////////////////////////////////////////////////////////////////////////////
void 
advertdirectory_cpi_impl::sync_get_vector_attribute(std::vector<std::string>& ret, 
    std::string key)
{
    std::string adname;
    {
        instance_data data (this);
        check_if_open("advertdirectory_cpi_impl::sync_get_vector_attribute", data->location_);
        adname = detail::get_filepath(data->location_);
    }
    
    // check permissions (throws on mismatch)
    check_permissions(saga::advert::Read, "get_vector_attribute", adname);

    // read attribute from db 
    std::string val;
    
    bool is_vector = false;
    bool result    = false;

    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_ADVERTDIR_DBACTION(get_vector_attribute,
        (result = db_.get_attribute(adname, key, val, is_vector)));
        
    if (result) {
        if (is_vector) {
            detail::tokenize_values(val, ret);
        }
        else {
            SAGA_OSSTREAM strm;
            strm << "advert::advertdirectory_cpi_impl::sync_get_vector_attribute: "
                    "attribute '" << key << "' is not a vector attribute for: " 
                 << adname;
            SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), 
                saga::IncorrectState);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void 
advertdirectory_cpi_impl::sync_set_attribute(saga::impl::void_t&, std::string key, 
    std::string val)
{
    std::string adname;
    {
        instance_data data (this);
        check_if_open("advertdirectory_cpi_impl::sync_set_attribute", data->location_);
        adname = detail::get_filepath(data->location_);
    }

    // check permissions (throws on mismatch)
    check_permissions(saga::advert::Write, "set_attribute", adname);
    
    // write attribute to db
    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_ADVERTDIR_DBACTION(set_attribute,
        (db_.set_attribute(adname, key, val, false)));
}

///////////////////////////////////////////////////////////////////////////////
void 
advertdirectory_cpi_impl::sync_set_vector_attribute(saga::impl::void_t&, std::string key, 
    std::vector<std::string> val)
{
    std::string adname;
    {
        instance_data data (this);
        check_if_open("advertdirectory_cpi_impl::sync_set_vector_attribute", data->location_);
        adname = detail::get_filepath(data->location_);
    }

    // check permissions (throws on mismatch)
    check_permissions(saga::advert::Write, "get_vector_attribute", adname);
    
    // write attribute to db
    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_ADVERTDIR_DBACTION(set_vector_attribute,
        (db_.set_attribute(adname, key, detail::concat_values(val), true)));
}

///////////////////////////////////////////////////////////////////////////////
void 
advertdirectory_cpi_impl::sync_remove_attribute(saga::impl::void_t&, std::string key)
{
    std::string adname;
    {
        instance_data data (this);
        check_if_open("advertdirectory_cpi_impl::sync_remove_attribute", data->location_);
        adname = detail::get_filepath(data->location_);
    }

    // check permissions (throws on mismatch)
    check_permissions(saga::advert::Write, "remove_attribute", adname);
    
    // remove attribute from db
    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_ADVERTDIR_DBACTION(remove_attribute,
        (db_.remove_attribute(adname, key)));
}

///////////////////////////////////////////////////////////////////////////////
void 
advertdirectory_cpi_impl::sync_list_attributes(std::vector<std::string>& keys)
{
    std::string adname;
    {
        instance_data data (this);
        check_if_open("advertdirectory_cpi_impl::sync_list_attributes", data->location_);
        adname = detail::get_filepath(data->location_);
    }

    // check permissions (throws on mismatch)
    check_permissions(saga::advert::Read, "list_attributes", adname);
    
    // get attributes from db
    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_ADVERTDIR_DBACTION(list_attributes, (db_.list_attributes(adname, keys)));
}

///////////////////////////////////////////////////////////////////////////////
void 
advertdirectory_cpi_impl::sync_find_attributes(std::vector<std::string>& keys, 
    std::string pattern)
{
    std::string adname;
    {
        instance_data data (this);
        check_if_open("advertdirectory_cpi_impl::sync_find_attributes", data->location_);
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

    SAGA_ADVERTDIR_DBACTION(find_attributes,
        (db_.find_attributes(adname, detail::regex_from_pattern(kpat), 
            detail::regex_from_pattern(vpat), &keys)));
}

///////////////////////////////////////////////////////////////////////////////
// namespace_entry functions
void 
advertdirectory_cpi_impl::sync_get_url(saga::url& url)
{
    instance_data data (this);
    check_if_open("advertdirectory_cpi_impl::sync_get_url", data->location_);

    {
        mutex_type::scoped_lock lock(mtx_);
        database_scope db_scope(this, db_);

        SAGA_ADVERTDIR_DBACTION(get_url, db_.verify_status()); // throws when db has been closed
    }

    url = data->location_;
}

void 
advertdirectory_cpi_impl::sync_get_cwd(saga::url& url)
{
    saga::url result;
    {
        instance_data data (this);
        check_if_open("advertdirectory_cpi_impl::sync_get_cwd", data->location_);
        result = data->location_;
    }

    {
        mutex_type::scoped_lock lock(mtx_);
        database_scope db_scope(this, db_);

        SAGA_ADVERTDIR_DBACTION(get_cwd, db_.verify_status()); // throws when db has been closed
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
advertdirectory_cpi_impl::sync_get_name(saga::url& url)
{
    saga::url result;
    {
        instance_data data (this);
        check_if_open("advertdirectory_cpi_impl::sync_get_name", data->location_);
        result = data->location_;
    }

    {
        mutex_type::scoped_lock lock(mtx_);
        database_scope db_scope(this, db_);

        SAGA_ADVERTDIR_DBACTION(get_name, db_.verify_status()); // throws when db has been closed
    }

    std::string dirpath(detail::get_filepath(result));
    std::string::size_type pos = dirpath.find_last_of('/');
    if (pos != std::string::npos)
        dirpath = dirpath.substr(0, pos);

    boost::filesystem::path p(dirpath);
    url.set_path(saga::detail::leaf(p));
}

void 
advertdirectory_cpi_impl::sync_is_dir(bool& ret)
{
    {
        instance_data data (this);
        check_if_open("advertdirectory_cpi_impl::sync_is_dir", data->location_);
    }

    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_ADVERTDIR_DBACTION(is_dir, db_.verify_status()); // throws when db has been closed
    ret = true;
}

void 
advertdirectory_cpi_impl::sync_is_entry(bool& ret)
{
    {
        instance_data data (this);
        check_if_open("advertdirectory_cpi_impl::sync_is_entry", data->location_);
    }

    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_ADVERTDIR_DBACTION(is_entry, db_.verify_status()); // throws when db has been closed
    ret = false;
}

void 
advertdirectory_cpi_impl::sync_is_link(bool& ret)
{
    {
        instance_data data (this);
        check_if_open("advertdirectory_cpi_impl::sync_is_link", data->location_);
    }

    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_ADVERTDIR_DBACTION(is_link, db_.verify_status()); // throws when db has been closed
    ret = false;                // we don't support links
}

void 
advertdirectory_cpi_impl::sync_remove(saga::impl::void_t&, int flags)
{
    std::string adname;
    {
        instance_data data (this);
        check_if_open("advertdirectory_cpi_impl::sync_remove", data->location_);
        adname = detail::get_filepath(data->location_);
    }

    if (!(flags & saga::advert::Recursive)) {
        SAGA_ADAPTOR_THROW(
            "advert::advertdirectory_cpi_impl::sync_remove: "
            "Recursive flag was not specified while attempting to delete a "
            "directory", saga::BadParameter);
    }

    // check permissions (throws on mismatch)
    check_permissions(saga::advert::Write, "remove", adname);

    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_ADVERTDIR_DBACTION(remove, (db_.remove_directory(adname), db_.close()));
    opened_ = false;
}

void advertdirectory_cpi_impl::sync_close(saga::impl::void_t&, double)
{
//     if (0 != monitor_handle_) {
//         try {
//             saga::adaptors::unregister_workitem(monitor_handle_, get_session());
//         }
//         catch (...) {
//             // ignore any errors here, the item might have been unregistered 
//             // already  
//         }
//     }

    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    opened_ = false;
    SAGA_ADVERTDIR_DBACTION(close, db_.close());
}

///////////////////////////////////////////////////////////////////////////////
//  namespace_dir functions
///////////////////////////////////////////////////////////////////////////////
namespace 
{
    // find all matching entries recursively
    void 
    find_recursive(database& db, std::string const& adname, 
        std::string const& pattern, std::string const& cwd,
        std::vector<std::string>& ret)
    {
        std::vector<std::string> entries;
        db.list(adname, pattern, entries, !cwd.empty());
        
        std::vector<std::string>::iterator end = entries.end();
        for (std::vector<std::string>::iterator it = entries.begin(); 
             it != end; ++it)
        {
            std::string entry(adname.empty() ? "/" : adname);
            if (entry.find_last_of('/') != entry.size()-1)
                entry += '/';
            entry += *it;

            bool is_dir = false;
            if (0 == db.find(entry, is_dir, true)) {
            // this should not happen!
                SAGA_OSSTREAM strm;
                strm << "advert::advertdirectory_cpi_impl::find_recursive: "
                        "Database inconsistency detected: " << entry;
                SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                    saga::NoSuccess);
            }
            
            std::string rel_entry(cwd.empty() ? *it : cwd + '/' + *it);
            if (is_dir)     // analyze this directory
                find_recursive(db, entry, pattern, rel_entry, ret);

        // found a matching entry
            ret.push_back(rel_entry);
        }
    }

    // the difference between list_recursive and find_recursve is, that list
    // applies the pattern in the top level directory only
    void 
    list_recursive(database& db, std::string const& adname, 
        std::string const& pattern, std::string const& cwd,
        std::vector<std::string>& ret)
    {
        std::vector<std::string> entries;
        db.list(adname, pattern, entries);
        
        std::vector<std::string>::iterator end = entries.end();
        for (std::vector<std::string>::iterator it = entries.begin(); 
             it != end; ++it)
        {
            std::string entry(adname.empty() ? "/" : adname);
            if (entry.find_last_of('/') != entry.size()-1)
                entry += '/';
            entry += *it;
            
            bool is_dir = false;
            if (0 == db.find(entry, is_dir, true)) {
            // this should not happen!
                SAGA_OSSTREAM strm;
                strm << "advert::advertdirectory_cpi_impl::list_recursive: "
                        "Database inconsistency detected: " << entry;
                SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                    saga::NoSuccess);
            }

            std::string rel_entry(cwd.empty() ? *it : cwd + '/' + *it);
            if (is_dir)     // analyze this directory
                find_recursive(db, entry, ".*", rel_entry, ret);

        // found a matching entry
            ret.push_back(rel_entry);
        }
    }
}

void 
advertdirectory_cpi_impl::sync_list(std::vector<saga::url>& ret, 
    std::string pattern, int flags)
{
    std::string adname;
    std::string basename;
    {
        instance_data data (this);
        check_if_open("advertdirectory_cpi_impl::sync_list", data->location_);
        adname = detail::get_filepath(data->location_);
        basename = data->location_.get_string();
    }

    // check permissions (throws on mismatch)
    check_permissions(saga::advert::Read, "list", adname);
    
    if (flags & saga::advert::Dereference) {
        SAGA_OSSTREAM strm;
        strm << "advert::advertdirectory_cpi_impl::sync_list: "
                "Dereferencing links is not supported.";
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), 
            saga::NotImplemented);
    }

//     std::string entry(basename.empty() ? "/" : basename);
//     if (entry.find_last_of('/') != entry.size()-1)
//         entry += '/';

    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    std::vector<std::string> entries;
    if (flags & saga::advert::Recursive) {
    // invoke list recursively
        SAGA_ADVERTDIR_DBACTION(list,
            (list_recursive(db_, adname, detail::regex_from_pattern(pattern), 
                "", entries)));
    }
    else {
        // return the matching entries
        SAGA_ADVERTDIR_DBACTION(list,
            (db_.list(adname, detail::regex_from_pattern(pattern), entries)));
    }

    // create result vector
    std::vector<std::string>::iterator end = entries.end();
    for (std::vector<std::string>::iterator it = entries.begin(); 
         it != end; ++it)
    {
        ret.push_back(saga::url(/*entry + */*it));
    }
}

void 
advertdirectory_cpi_impl::sync_find(std::vector<saga::url>& ret, 
    std::string pattern, int flags)
{
    std::string adname;
    std::string basename;
    {
        instance_data data (this);
        check_if_open("advertdirectory_cpi_impl::sync_find", data->location_);
        adname = detail::get_filepath(data->location_);
        basename = data->location_.get_string();
    }

    // check permissions (throws on mismatch)
    check_permissions(saga::advert::Read, "find", adname);
    
    if (flags & saga::advert::Dereference) {
        SAGA_OSSTREAM strm;
        strm << "advert::advertdirectory_cpi_impl::sync_find: "
                "Dereferencing links is not supported.";
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), 
            saga::NotImplemented);
    }

    std::string entry(basename.empty() ? "/" : basename);
    if (entry.find_last_of('/') != entry.size()-1)
        entry += '/';

    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    std::vector<std::string> entries;
    if (flags & saga::advert::Recursive) {
    // invoke list recursively
        SAGA_ADVERTDIR_DBACTION(find,
            (find_recursive(db_, adname, detail::regex_from_pattern(pattern), 
                "", entries)));
    }
    else {
    // same as list
        SAGA_ADVERTDIR_DBACTION(find,
            (db_.list(adname, detail::regex_from_pattern(pattern), entries)));
    }

    // create result vector
    std::vector<std::string>::iterator end = entries.end();
    for (std::vector<std::string>::iterator it = entries.begin(); 
         it != end; ++it)
    {
        ret.push_back(saga::url(entry + *it));
    }
}

void 
advertdirectory_cpi_impl::sync_exists(bool& ret, saga::url entry)
{
    std::string adname;
    {
        instance_data data (this);
        check_if_open("advertdirectory_cpi_impl::sync_exists", data->location_);
        adname = detail::get_filepath(data->location_);
    }

    // check permissions (throws on mismatch)
    check_permissions(saga::advert::Read, "exists", adname);
    
    std::string entry_path (detail::get_filepath(entry));
    if ('/' != entry_path[0]) 
        entry_path = adname + "/" + entry_path;   // relative path given
        
    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_ADVERTDIR_DBACTION(exists, (ret = db_.exists_other(entry_path)));
}

void 
advertdirectory_cpi_impl::sync_is_dir(bool& ret, saga::url entry)
{
    std::string adname;
    {
        instance_data data (this);
        check_if_open("advertdirectory_cpi_impl::sync_is_dir", data->location_);
        adname = detail::get_filepath(data->location_);
    }

    // check permissions (throws on mismatch)
    check_permissions(saga::advert::Read, "is_dir", adname);

    std::string entry_path (detail::get_filepath(entry));
    if ('/' != entry_path[0]) 
        entry_path = adname + "/" + entry_path;   // relative path given
        
    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_ADVERTDIR_DBACTION(is_dir,
        (ret = db_.exists_directory_other(entry_path, false)));
}

void 
advertdirectory_cpi_impl::sync_is_entry(bool& ret, saga::url entry)
{
    std::string adname;
    {
        instance_data data (this);
        check_if_open("advertdirectory_cpi_impl::sync_is_entry", data->location_);
        adname = detail::get_filepath(data->location_);
    }

    // check permissions (throws on mismatch)
    check_permissions(saga::advert::Read, "is_entry", adname);
    
    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    std::string entry_path (detail::get_filepath(entry));
    if ('/' == entry_path[0]) {
    // absolute path given
        SAGA_ADVERTDIR_DBACTION(is_entry,
            (ret = db_.exists_entry_other(entry_path, false)));   
    }
    else {
    // relative path given
        SAGA_ADVERTDIR_DBACTION(is_entry,
            (ret = db_.exists_entry_other(adname + "/" + entry_path, false)));  
    }
}

void 
advertdirectory_cpi_impl::sync_is_link(bool& ret, saga::url)
{
    instance_data data (this);
    check_if_open("advertdirectory_cpi_impl::sync_is_link", data->location_);

    ret = false;    // we don't support links
}

void 
advertdirectory_cpi_impl::sync_get_num_entries(std::size_t& num_entries)
{
    std::string adname;
    {
        instance_data data (this);
        check_if_open("advertdirectory_cpi_impl::sync_get_num_entries", data->location_);
        adname = detail::get_filepath(data->location_);
    }

    // check permissions (throws on mismatch)
    check_permissions(saga::advert::Read, "get_num_entries", adname);

    // return the matching entries
    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    std::vector<std::string> entries;
    SAGA_ADVERTDIR_DBACTION(get_num_entries, 
        (db_.list(adname, detail::regex_from_pattern("*"), entries)));
    num_entries = (unsigned int)entries.size();
}

void 
advertdirectory_cpi_impl::sync_get_entry(saga::url& entry, std::size_t num)
{
    std::string adname;
    {
        instance_data data (this);
        check_if_open("advertdirectory_cpi_impl::sync_get_entry", data->location_);
        adname = detail::get_filepath(data->location_);
    }

    // check permissions (throws on mismatch)
    check_permissions(saga::advert::Read, "get_num_entries", adname);
    
    // return the matching entries
    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    std::vector<std::string> entries;
    SAGA_ADVERTDIR_DBACTION(get_entry, 
        (db_.list(adname, detail::regex_from_pattern("*"), entries)));
    if (num < 0 || num >= entries.size()) {
        SAGA_OSSTREAM strm;
        strm << "advert::advertdirectory_cpi_impl::sync_get_entry: "
                "bad entry number for: " << adname;
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), 
            saga::BadParameter);
    }
    entry = entries[num];
}

void 
advertdirectory_cpi_impl::sync_remove(saga::impl::void_t&, saga::url entry, 
    int flags)
{
    std::string adname;
    {
        instance_data data (this);
        check_if_open("advertdirectory_cpi_impl::sync_remove", data->location_);
        adname = detail::get_filepath(data->location_);
    }

    // check permissions (throws on mismatch)
    check_permissions(saga::advert::Write, "remove", adname);
    
    // now (recursively) remove the entry
    std::string entry_path (detail::get_filepath(entry));
    if ('/' != entry_path[0]) 
        entry_path = adname + "/" + entry_path;     // relative path given

    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    bool exists_entry = false;
    SAGA_ADVERTDIR_DBACTION(get_entry, 
        (exists_entry = db_.exists_entry_other(entry_path, false)));

    bool exists_dir = false;
    SAGA_ADVERTDIR_DBACTION(get_entry, 
        (exists_dir = db_.exists_directory_other(entry_path, false)));

    if (exists_entry) {
        if (flags & saga::advert::Recursive) {
            SAGA_ADAPTOR_THROW(
                "advert::advertdirectory_cpi_impl::sync_remove: "
                "Recursive flag was specified while attempting to delete a "
                "file", saga::BadParameter);
        }
        SAGA_ADVERTDIR_DBACTION(remove, db_.remove_other(entry_path));
    }
    else if (exists_dir) {
        if (!(flags & saga::advert::Recursive)) {
            SAGA_ADAPTOR_THROW(
                "advert::advertdirectory_cpi_impl::sync_remove: "
                "Recursive flag was not specified while attempting to delete a "
                "directory", saga::BadParameter);
        }
        SAGA_ADVERTDIR_DBACTION(remove, db_.remove_directory_other(entry_path));
    }
    else {
        SAGA_ADAPTOR_THROW(
            "advert::advertdirectory_cpi_impl::sync_remove: "
            "Attempting to delete a non-existing directory: " + entry_path, 
            saga::BadParameter);
    }
}

void 
advertdirectory_cpi_impl::sync_open(saga::name_space::entry& ret, 
    saga::url entry, int flags)
{
    saga::url adname;
    {
        instance_data data (this);
        check_if_open("advertdirectory_cpi_impl::sync_open", data->location_);
        adname = data->location_.clone();
    }

    // now open the entry
    std::string entry_path (detail::get_filepath(entry));
    if ('/' != entry_path[0]) 
        adname.set_path(saga::url::unescape(adname.get_path()) + "/" + entry_path);     // relative path given
    else
        adname.set_path(entry_path);

    ret = saga::advert::entry(this->get_proxy()->get_session(), adname.get_url(), 
        flags);
}

void 
advertdirectory_cpi_impl::sync_open_dir(saga::name_space::directory& ret, 
    saga::url entry, int flags)
{
    saga::url adname;
    {
        instance_data data (this);
        check_if_open("advertdirectory_cpi_impl::sync_open_dir", data->location_);
        adname = data->location_.clone();
    }

    // now open the directory
    std::string entry_path (detail::get_filepath(entry));
    if ('/' != entry_path[0]) 
        adname.set_path(saga::url::unescape(adname.get_path()) + "/" + entry_path);     // relative path given
    else
        adname.set_path(entry_path);

    ret = saga::advert::directory(this->get_proxy()->get_session(), 
        adname.get_url(), flags);
}

void 
advertdirectory_cpi_impl::sync_change_dir(saga::impl::void_t&, saga::url dir)
{
    saga::url adname;
    {
        instance_data data (this);
        check_if_open("advertdirectory_cpi_impl::sync_change_dir", data->location_);
        adname = data->location_.clone();
    }

    // check permissions (throws on mismatch)
    check_permissions(saga::advert::Write, "change_dir", adname.get_string());

    std::string dir_path (detail::get_filepath(dir));
    if ('/' != dir_path[0]) 
        adname.set_path(saga::url::unescape(adname.get_path()) + "/" + dir_path);     // relative path given
    else
        adname.set_path(dir_path);

    // verify the directory existence
    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_ADVERTDIR_DBACTION(change_dir, 
        db_.change_directory(saga::url::unescape(adname.get_path())));

    // change directory 
    {
        instance_data data (this);
        data->location_ = adname.get_url();
    }
}

void 
advertdirectory_cpi_impl::sync_make_dir(saga::impl::void_t&, saga::url dir, 
    int flags)
{
    std::string adname;
    {
        instance_data data (this);
        check_if_open("advertdirectory_cpi_impl::sync_make_dir", data->location_);
        adname = detail::get_filepath(data->location_);
    }

    // check permissions (throws on mismatch)
    check_permissions(saga::advert::Write, "make_dir", adname);

    std::string dir_path (detail::get_filepath(dir));
    if ('/' != dir_path[0]) 
        dir_path = adname + "/" + dir_path;     // relative path given

    bool result = false;

    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_ADVERTDIR_DBACTION(make_dir, 
        (result = db_.exists_directory_other(dir_path)));

    if (result) {
        // make sure directory does not exist if Exclusive is given
        if (flags & saga::advert::Exclusive) {
            SAGA_ADAPTOR_THROW(
                "advert::advertdirectory_cpi_impl::sync_make_dir: "
                "Exclusive flag was specified while the directory already exists",
                saga::AlreadyExists);
        }
    }
    else {
        // create the new directory
        SAGA_ADVERTDIR_DBACTION(make_dir, 
            (db_.create_new_directory(dir_path, 
                (flags & saga::advert::CreateParents) ? true : false)));
    }
}

///////////////////////////////////////////////////////////////////////////////
//  directory functions
void 
advertdirectory_cpi_impl::sync_open(saga::advert::entry& ret, saga::url entry, 
    int flags)
{
    sync_open((saga::name_space::entry&)ret, entry, flags);
}

void 
advertdirectory_cpi_impl::sync_open_dir(saga::advert::directory& ret, 
    saga::url entry, int flags)
{
    sync_open_dir((saga::name_space::directory&)ret, entry, flags);
}

void 
advertdirectory_cpi_impl::sync_find(std::vector<saga::url>& ret, 
    std::string pattern, std::vector<std::string> patterns, int flags)
{
    std::string adname;
    std::string basename;
    {
        instance_data data (this);
        check_if_open("advertdirectory_cpi_impl::sync_find", data->location_);
        adname = detail::get_filepath(data->location_);
        basename = data->location_.get_string();
    }

    // check permissions (throws on mismatch)
    check_permissions(saga::advert::Read, "find", adname);
    
    if (flags & saga::advert::Dereference) {
        SAGA_OSSTREAM strm;
        strm << "advert::advertdirectory_cpi_impl::sync_find: "
                "Dereferencing links is not supported.";
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), 
            saga::NotImplemented);
    }

    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    std::vector<std::string> paths;
    if (flags & saga::advert::Recursive) {
    // invoke list recursively
        SAGA_ADVERTDIR_DBACTION(find, 
            (find_recursive(db_, adname, detail::regex_from_pattern(pattern), 
                "", paths)));
    }
    else {
    // same as list
        SAGA_ADVERTDIR_DBACTION(find, 
            (db_.list(adname, detail::regex_from_pattern(pattern), paths)));
    }

// look for matching keys in the database, iterating for all found paths
    std::string entry(basename.empty() ? "/" : basename);
    if (entry.find_last_of('/') != entry.size()-1)
        entry += '/';

    if (!patterns.empty()) {
    // generate regex patterns for given key and value patterns    
        std::vector<std::string> kpat, vpat;
        detail::parse_patterns(this, patterns, kpat, vpat);

        std::vector<std::string> kpatrx, vpatrx;
        detail::regex_from_patterns(kpat, kpatrx);
        detail::regex_from_patterns(vpat, vpatrx);

    // look for matching keys in the database, iterating for all found paths
        std::string entry(basename.empty() ? "/" : basename);
        if (entry.find_last_of('/') != entry.size()-1)
            entry += '/';

        std::vector<std::string>::iterator end = paths.end();
        for (std::vector<std::string>::iterator it = paths.begin(); it != end; ++it)
        {
            std::string e(adname + *it);
            bool keys_exist = false;
            SAGA_ADVERTDIR_DBACTION(find,
                (keys_exist = db_.find_attributes(e, kpatrx, vpatrx, NULL, true, true))); 
            if (keys_exist)
                ret.push_back(entry + *it);
        }
    }
    else {
    // no additional attribute based criteria given, return all existing
        std::vector<std::string>::iterator end = paths.end();
        for (std::vector<std::string>::iterator it = paths.begin(); it != end; ++it)
        {
            ret.push_back(entry + *it);
        }
    }
}

std::string 
advertdirectory_cpi_impl::clean_url (const saga::url & u)
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


