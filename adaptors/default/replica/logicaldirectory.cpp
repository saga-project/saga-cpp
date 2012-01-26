//  Copyright (c) 2005-2011 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 
#include <map>
#include <vector>

#include <boost/tokenizer.hpp>

#include <saga/saga/util.hpp>
#include <saga/saga/path_leaf.hpp>
#include <saga/saga/exception.hpp>
#include <saga/saga/url.hpp>
#include <saga/saga/adaptors/task.hpp>

#include <saga/impl/config.hpp>
#include <saga/impl/exception_list.hpp>

#include "config.hpp"
#include "logicaldirectory.hpp"
#include "common_helpers.hpp"

///////////////////////////////////////////////////////////////////////////////
// wrap database calls and catch all exceptions
#define SAGA_LOGICALDIR_DBACTION(name, f)                                     \
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
        strm << "replica::logical_directory_cpi_impl::" #name ": "            \
             << e.what();                                                     \
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::NoSuccess);   \
    }                                                                         \
    /**/

///////////////////////////////////////////////////////////////////////////////
namespace replica
{

///////////////////////////////////////////////////////////////////////////////
//  constructor
logical_directory_cpi_impl::logical_directory_cpi_impl (
        proxy* p, cpi_info const& info,
        saga::ini::ini const& glob_ini, saga::ini::ini const& adap_ini,
        TR1::shared_ptr<saga::adaptor> adaptor)
:   base_cpi (p, info, adaptor, cpi::Noflags),
    db_(detail::get_db_type<instance_data>(adap_ini, p), 
        detail::get_db_connectstring<instance_data>(adap_ini, p->get_session(), p))
{
    database_scope db_scope(this, db_);
    instance_data data (this);
    saga::url lfn_url(data->location_);
    std::string url(saga::url::unescape(lfn_url.get_path()));
    if (url.empty())
        url = "/";      // root only

    // we understand absolute paths only
    if ('/' != url[0]) 
    {
        SAGA_OSSTREAM strm;
        strm << "replica::logical_directory_cpi_impl::init: "
                "cannot handle relative logical directory name: " 
             << lfn_url.get_string();
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::IncorrectURL);
    }
    
    // we understand lfn's only for localhost (if sqlite3)
    std::string host(lfn_url.get_host());
    if (!host.empty() && !detail::host_is_valid(db_.get_dbtype(), host))
    {
        SAGA_OSSTREAM strm;
        strm << "replica::logical_directory_cpi_impl::init: "
                "cannot handle logical directory name: " 
             << lfn_url.get_string();
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
    }

    // we understand only 'any' and 'lfn' url schemes
    std::string scheme(lfn_url.get_scheme());
    if (!scheme.empty() && scheme != "lfn" && scheme != "any" && scheme != "replica")
    {
        SAGA_OSSTREAM strm;
        strm << "replica::logical_directory_cpi_impl::init: "
                "cannot handle logical directory name: " << lfn_url.get_string();
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
    }
    
    saga::replica::flags mode = (saga::replica::flags)data->mode_;
    if (((mode & saga::replica::Create) || 
         (mode & saga::replica::CreateParents)) && 
        (mode & saga::replica::Exclusive)) 
    {
        if (db_.exists_directory(url))
        {
            SAGA_OSSTREAM strm;
            strm << "replica::logical_directory_cpi_impl::init: "
                    "replica entry already exists: " << lfn_url.get_string();
            SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::AlreadyExists);
        }
    }

    // Create replica entry if saga::logical_directory::create flag is given
    if ((mode & saga::replica::Create) || 
        (mode & saga::replica::CreateParents))
    {
        SAGA_LOGICALDIR_DBACTION(logical_directory_cpi_impl,
            (db_.create_directory(url, (mode & saga::replica::CreateParents) ? true : false)));

        // no need to create this entry twice
        data->mode_ &= ~(saga::replica::Create | 
                         saga::replica::CreateParents);
    }

    if (!db_.exists_directory(url))
    { 
        SAGA_OSSTREAM strm;
        strm << "replica::logical_directory_cpi_impl::init: "
                "replica entry does not exist: " << lfn_url.get_string();
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::DoesNotExist);
    }

    opened_ = true;
}

///////////////////////////////////////////////////////////////////////////////
//  destructor
logical_directory_cpi_impl::~logical_directory_cpi_impl (void)
{
    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_LOGICALDIR_DBACTION(~logical_directory_cpi_impl, db_.close(false));
    opened_ = false;
}

///////////////////////////////////////////////////////////////////////////////
//  SAGA CPI functions 
void logical_directory_cpi_impl::check_if_open (std::string const& functionname, 
        saga::url const& location)
{
    if (!opened_)
    {
        SAGA_OSSTREAM strm;
        strm << functionname << ": replica directory is not in open state: " 
             << location.get_string();
        SAGA_ADAPTOR_THROW (SAGA_OSSTREAM_GETSTRING (strm), saga::IncorrectState);
    }
}

///////////////////////////////////////////////////////////////////////////////
// attribute functions
void 
logical_directory_cpi_impl::sync_attribute_exists(bool& ret, std::string key)
{
    std::string lfn;
    {
        instance_data data (this);
        check_if_open("logical_directory_cpi_impl::sync_attribute_exists", data->location_);
        lfn = detail::get_filepath(data->location_);
    }

    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_LOGICALDIR_DBACTION(attribute_exists,
        (db_.attribute_exists(lfn, key, ret)));
}

void 
logical_directory_cpi_impl::sync_attribute_is_readonly(bool& ret, std::string key)
{
    instance_data data (this);
    check_if_open("logical_directory_cpi_impl::sync_attribute_is_readonly", data->location_);
    ret = !(data->mode_ & saga::replica::Read);
}

void 
logical_directory_cpi_impl::sync_attribute_is_writable(bool& ret, std::string key)
{
    instance_data data (this);
    check_if_open("logical_directory_cpi_impl::sync_attribute_is_writable", data->location_);
    ret = (data->mode_ & saga::replica::Write) ? true : false;
}

///////////////////////////////////////////////////////////////////////////////
void 
logical_directory_cpi_impl::check_permissions(saga::replica::flags flags,
    char const* name, std::string const& lfn)
{
    instance_data data (this);
    if (!(data->mode_ & flags)) {
        SAGA_OSSTREAM strm;
        strm << "replica::logical_directory_cpi_impl::" << name << ": "
                "could not access(" 
             << ((flags & saga::replica::Read) ? "read" : "write")
             << ") attribute for replica entry: " 
             << lfn;
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), 
            saga::PermissionDenied);
    }
}

void 
logical_directory_cpi_impl::sync_attribute_is_vector(bool& ret, std::string key)
{
    std::string lfn;
    {
        instance_data data (this);
        check_if_open("logical_directory_cpi_impl::sync_attribute_is_vector", data->location_);
        lfn = detail::get_filepath(data->location_);
    }

    // check permissions (throws on mismatch)
    check_permissions(saga::replica::Read, "attribute_is_vector", lfn);

    // read attribute from db 
    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    std::string val;
    SAGA_LOGICALDIR_DBACTION(attribute_is_vector,
        (db_.get_attribute(lfn, key, val, ret)));
}

void 
logical_directory_cpi_impl::sync_attribute_is_extended(bool& ret, std::string key)
{
    instance_data data (this);
    check_if_open("logical_directory_cpi_impl::sync_attribute_is_extended", data->location_);
    ret = (data->mode_ & saga::replica::Write) ? true : false;
}

///////////////////////////////////////////////////////////////////////////////
void 
logical_directory_cpi_impl::sync_get_attribute(std::string& ret, std::string key)
{
    std::string lfn;
    {
        instance_data data (this);
        check_if_open("logical_directory_cpi_impl::sync_get_attribute", data->location_);
        lfn = detail::get_filepath(data->location_);
    }

    // check permissions (throws on mismatch)
    check_permissions(saga::replica::Read, "get_attribute", lfn);

    // read attribute from db 
    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    bool is_vector = false;
    std::string val;
    SAGA_LOGICALDIR_DBACTION(get_attribute,
        (db_.get_attribute(lfn, key, val, is_vector)));

    if (!is_vector) {
        ret = val;
    }
    else {
        SAGA_OSSTREAM strm;
        strm << "replica::logical_directory_cpi_impl::sync_get_attribute: "
                "attribute '" << key << "' is a vector attribute for: " << lfn;
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), 
            saga::IncorrectState);
    }
}

///////////////////////////////////////////////////////////////////////////////
void 
logical_directory_cpi_impl::sync_get_vector_attribute(std::vector<std::string>& ret, 
    std::string key)
{
    std::string lfn;
    {
        instance_data data (this);
        check_if_open("logical_directory_cpi_impl::sync_get_vector_attribute", data->location_);
        lfn = detail::get_filepath(data->location_);
    }
    
    // check permissions (throws on mismatch)
    check_permissions(saga::replica::Read, "get_vector_attribute", lfn);

    // read attribute from db 
    std::string val;
    bool is_vector = false;
    bool result    = false;

    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_LOGICALDIR_DBACTION(get_vector_attribute,
        (result = db_.get_attribute(lfn, key, val, is_vector)));

    if (result) {
        if (is_vector) {
            detail::tokenize_values(val, ret);
        }
        else {
            SAGA_OSSTREAM strm;
            strm << "replica::logical_directory_cpi_impl::sync_get_vector_attribute: "
                    "attribute '" << key << "' is not a vector attribute for: " 
                 << lfn;
            SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), 
                saga::IncorrectState);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void 
logical_directory_cpi_impl::sync_set_attribute(saga::impl::void_t&, std::string key, 
    std::string val)
{
    std::string lfn;
    {
        instance_data data (this);
        check_if_open("logical_directory_cpi_impl::sync_set_attribute", data->location_);
        lfn = detail::get_filepath(data->location_);
    }

    // check permissions (throws on mismatch)
    check_permissions(saga::replica::Write, "set_attribute", lfn);

    // write attribute to db
    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_LOGICALDIR_DBACTION(set_attribute,
        (db_.set_attribute(lfn, key, val, false)));
}

///////////////////////////////////////////////////////////////////////////////
void 
logical_directory_cpi_impl::sync_set_vector_attribute(saga::impl::void_t&, std::string key, 
    std::vector<std::string> val)
{
    std::string lfn;
    {
        instance_data data (this);
        check_if_open("logical_directory_cpi_impl::sync_set_vector_attribute", data->location_);
        lfn = detail::get_filepath(data->location_);
    }

    // check permissions (throws on mismatch)
    check_permissions(saga::replica::Write, "get_vector_attribute", lfn);

    // write attribute to db
    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_LOGICALDIR_DBACTION(set_vector_attribute,
        (db_.set_attribute(lfn, key, detail::concat_values(val), true)));
}

///////////////////////////////////////////////////////////////////////////////
void 
logical_directory_cpi_impl::sync_remove_attribute(saga::impl::void_t&, std::string key)
{
    std::string lfn;
    {
        instance_data data (this);
        check_if_open("logical_directory_cpi_impl::sync_remove_attribute", data->location_);
        lfn = detail::get_filepath(data->location_);
    }

    // check permissions (throws on mismatch)
    check_permissions(saga::replica::Write, "remove_attribute", lfn);

    // remove attribute from db
    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_LOGICALDIR_DBACTION(remove_attribute,
        (db_.remove_attribute(lfn, key)));
}

///////////////////////////////////////////////////////////////////////////////
void 
logical_directory_cpi_impl::sync_list_attributes(std::vector<std::string>& keys)
{
    std::string lfn;
    {
        instance_data data (this);
        check_if_open("logical_directory_cpi_impl::sync_list_attributes", data->location_);
        lfn = detail::get_filepath(data->location_);
    }

    // check permissions (throws on mismatch)
    check_permissions(saga::replica::Read, "list_attributes", lfn);

    // get attributes from db
    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_LOGICALDIR_DBACTION(list_attributes, (db_.list_attributes(lfn, keys)));
}

///////////////////////////////////////////////////////////////////////////////
void 
logical_directory_cpi_impl::sync_find_attributes(std::vector<std::string>& keys, 
    std::string pattern)
{
    std::string lfn;
    {
        instance_data data (this);
        check_if_open("logical_directory_cpi_impl::sync_find_attributes", data->location_);
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

    SAGA_LOGICALDIR_DBACTION(find_attributes,
        (db_.find_attributes(lfn, detail::regex_from_pattern(kpat), 
            detail::regex_from_pattern(vpat), &keys)));
}

///////////////////////////////////////////////////////////////////////////////
// namespace_entry functions
void 
logical_directory_cpi_impl::sync_get_url(saga::url& url)
{
    instance_data data (this);
    check_if_open("logical_directory_cpi_impl::sync_get_url", data->location_);

    {
        mutex_type::scoped_lock lock(mtx_);
        database_scope db_scope(this, db_);

        SAGA_LOGICALDIR_DBACTION(get_url, db_.verify_status()); // throws when db has been closed
    }
    url = data->location_.clone();
}

void 
logical_directory_cpi_impl::sync_get_cwd(saga::url& url)
{
    saga::url result;
    {
        instance_data data (this);
        check_if_open("logical_directory_cpi_impl::sync_get_cwd", data->location_);
        result = data->location_;
    }

    {
        mutex_type::scoped_lock lock(mtx_);
        database_scope db_scope(this, db_);

        SAGA_LOGICALDIR_DBACTION(get_cwd, db_.verify_status()); // throws when db has been closed
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
logical_directory_cpi_impl::sync_get_name(saga::url& url)
{
    saga::url result;
    {
        instance_data data (this);
        check_if_open("logical_directory_cpi_impl::sync_get_name", data->location_);
        result = data->location_;
    }

    {
        mutex_type::scoped_lock lock(mtx_);
        database_scope db_scope(this, db_);

        SAGA_LOGICALDIR_DBACTION(get_name, db_.verify_status()); // throws when db has been closed
    }

    std::string dirpath(detail::get_filepath(result));
    std::string::size_type pos = dirpath.find_last_of('/');
    if (pos != std::string::npos)
        dirpath = dirpath.substr(0, pos);

    boost::filesystem::path p(dirpath);
    url.set_path(saga::detail::leaf(p));
}

void 
logical_directory_cpi_impl::sync_is_dir(bool& ret)
{
    {
        instance_data data (this);
        check_if_open("logical_directory_cpi_impl::sync_is_dir", data->location_);
    }

    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_LOGICALDIR_DBACTION(is_dir, db_.verify_status()); // throws when db has been closed
    ret = true;
}

void 
logical_directory_cpi_impl::sync_is_entry(bool& ret)
{
    {
        instance_data data (this);
        check_if_open("logical_directory_cpi_impl::sync_is_entry", data->location_);
    }

    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_LOGICALDIR_DBACTION(is_entry, db_.verify_status()); // throws when db has been closed
    ret = false;
}

void 
logical_directory_cpi_impl::sync_is_link(bool& ret)
{
    {
        instance_data data (this);
        check_if_open("logical_directory_cpi_impl::sync_is_link", data->location_);
    }

    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_LOGICALDIR_DBACTION(is_link, db_.verify_status()); // throws when db has been closed
    ret = false;                // we don't support links
}

void 
logical_directory_cpi_impl::sync_remove(saga::impl::void_t&, int flags)
{
    std::string lfn;
    {
        instance_data data (this);
        check_if_open("logical_directory_cpi_impl::sync_remove", data->location_);
        lfn = detail::get_filepath(data->location_);
    }

    if (!(flags & saga::replica::Recursive)) {
        SAGA_ADAPTOR_THROW(
            "replica::logical_directory_cpi_impl::sync_remove: "
            "Recursive flag was not specified while attempting to delete a "
            "directory", saga::BadParameter);
    }

    // check permissions (throws on mismatch)
    check_permissions(saga::replica::Write, "remove", lfn);

    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_LOGICALDIR_DBACTION(remove, (db_.remove_directory(lfn), db_.close()));
    opened_ = false;
}

void logical_directory_cpi_impl::sync_close(saga::impl::void_t&, double)
{
    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_LOGICALDIR_DBACTION(close, db_.close());
    opened_ = false;
}

///////////////////////////////////////////////////////////////////////////////
//  namespace_dir functions
///////////////////////////////////////////////////////////////////////////////
namespace 
{
    // find all matching entries recursively
    void 
    find_recursive(database& db, std::string const& lfn, 
        std::string const& pattern, std::string const& cwd,
        std::vector<std::string>& ret)
    {
        std::vector<std::string> entries;
        db.list(lfn, pattern, entries);
        
        std::vector<std::string>::iterator end = entries.end();
        for (std::vector<std::string>::iterator it = entries.begin(); 
             it != end; ++it)
        {
            std::string entry(lfn.empty() ? "/" : lfn);
            if (entry.find_last_of('/') != entry.size()-1)
                entry += '/';
            entry += *it;

            bool is_dir = false;
            if (0 == db.find(entry, is_dir)) {
            // this should not happen!
                SAGA_OSSTREAM strm;
                strm << "replica::logical_directory_cpi_impl::find_recursive: "
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
    list_recursive(database& db, std::string const& lfn, 
        std::string const& pattern, std::string const& cwd,
        std::vector<std::string>& ret)
    {
        std::vector<std::string> entries;
        db.list(lfn, pattern, entries);
        
        std::vector<std::string>::iterator end = entries.end();
        for (std::vector<std::string>::iterator it = entries.begin(); 
             it != end; ++it)
        {
            std::string entry(lfn.empty() ? "/" : lfn);
            if (entry.find_last_of('/') != entry.size()-1)
                entry += '/';
            entry += *it;
            
            bool is_dir = false;
            if (0 == db.find(entry, is_dir)) {
            // this should not happen!
                SAGA_OSSTREAM strm;
                strm << "replica::logical_directory_cpi_impl::list_recursive: "
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
logical_directory_cpi_impl::sync_list(std::vector<saga::url>& ret, 
    std::string pattern, int flags)
{
    std::string lfn;
    std::string basename;
    {
        instance_data data (this);
        check_if_open("logical_directory_cpi_impl::sync_list", data->location_);
        lfn = detail::get_filepath(data->location_);
        basename = data->location_.get_string();
    }

    // check permissions (throws on mismatch)
    check_permissions(saga::replica::Read, "list", lfn);
    
    if (flags & saga::replica::Dereference) {
        SAGA_OSSTREAM strm;
        strm << "replica::logical_directory_cpi_impl::sync_list: "
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
    if (flags & saga::replica::Recursive) {
    // invoke list recursively
        SAGA_LOGICALDIR_DBACTION(list,
            (list_recursive(db_, lfn, detail::regex_from_pattern(pattern), 
                "", entries)));
    }
    else {
        // return the matching entries
        SAGA_LOGICALDIR_DBACTION(list,
            (db_.list(lfn, detail::regex_from_pattern(pattern), entries)));
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
logical_directory_cpi_impl::sync_find(std::vector<saga::url>& ret, 
    std::string pattern, int flags)
{
    std::string lfn;
    std::string basename;
    {
        instance_data data (this);
        check_if_open("logical_directory_cpi_impl::sync_find", data->location_);
        lfn = detail::get_filepath(data->location_);
        basename = data->location_.get_string();
    }

    // check permissions (throws on mismatch)
    check_permissions(saga::replica::Read, "find", lfn);
    
    if (flags & saga::replica::Dereference) {
        SAGA_OSSTREAM strm;
        strm << "replica::logical_directory_cpi_impl::sync_find: "
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
    if (flags & saga::replica::Recursive) {
    // invoke list recursively
        SAGA_LOGICALDIR_DBACTION(find,
            (find_recursive(db_, lfn, detail::regex_from_pattern(pattern), 
                "", entries)));
    }
    else {
    // same as list
        SAGA_LOGICALDIR_DBACTION(find,
            (db_.list(lfn, detail::regex_from_pattern(pattern), entries)));
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
logical_directory_cpi_impl::sync_exists(bool& ret, saga::url entry)
{
    std::string lfn;
    {
        instance_data data (this);
        check_if_open("logical_directory_cpi_impl::sync_exists", data->location_);
        lfn = detail::get_filepath(data->location_);
    }

    // check permissions (throws on mismatch)
    check_permissions(saga::replica::Read, "exists", lfn);
    
    std::string entry_path (detail::get_filepath(entry));
    if ('/' != entry_path[0]) 
        entry_path = lfn + "/" + entry_path;   // relative path given

    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_LOGICALDIR_DBACTION(exists, (ret = db_.exists(entry_path)));
}

void 
logical_directory_cpi_impl::sync_is_dir(bool& ret, saga::url entry)
{
    std::string lfn;
    {
        instance_data data (this);
        check_if_open("logical_directory_cpi_impl::sync_is_dir", data->location_);
        lfn = detail::get_filepath(data->location_);
    }

    // check permissions (throws on mismatch)
    check_permissions(saga::replica::Read, "is_dir", lfn);
    
    std::string entry_path (detail::get_filepath(entry));
    if ('/' != entry_path[0]) 
        entry_path = lfn + "/" + entry_path;   // relative path given

    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_LOGICALDIR_DBACTION(is_dir,
        (ret = db_.exists_directory(entry_path, false)));
}

void 
logical_directory_cpi_impl::sync_is_entry(bool& ret, saga::url entry)
{
    std::string lfn;
    {
        instance_data data (this);
        check_if_open("logical_directory_cpi_impl::sync_is_entry", data->location_);
        lfn = detail::get_filepath(data->location_);
    }

    // check permissions (throws on mismatch)
    check_permissions(saga::replica::Read, "is_entry", lfn);

    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    std::string entry_path (detail::get_filepath(entry));
    if ('/' == entry_path[0]) {
    // absolute path given
        SAGA_LOGICALDIR_DBACTION(is_entry,
            (ret = db_.exists_entry(entry_path, false)));   
    }
    else {
    // relative path given
        SAGA_LOGICALDIR_DBACTION(is_entry,
            (ret = db_.exists_entry(lfn + "/" + entry_path, false)));  
    }
}

void 
logical_directory_cpi_impl::sync_is_link(bool& ret, saga::url)
{
    instance_data data (this);
    check_if_open("logical_directory_cpi_impl::sync_is_link", data->location_);

    ret = false;    // we don't support links
}

void 
logical_directory_cpi_impl::sync_get_num_entries(std::size_t& num_entries)
{
    std::string lfn;
    {
        instance_data data (this);
        check_if_open("logical_directory_cpi_impl::sync_get_num_entries", data->location_);
        lfn = detail::get_filepath(data->location_);
    }

    // check permissions (throws on mismatch)
    check_permissions(saga::replica::Read, "get_num_entries", lfn);

    // return the matching entries
    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    std::vector<std::string> entries;
    SAGA_LOGICALDIR_DBACTION(get_num_entries, 
        (db_.list(lfn, detail::regex_from_pattern("*"), entries)));
    num_entries = (unsigned int)entries.size();
}

void 
logical_directory_cpi_impl::sync_get_entry(saga::url& entry, std::size_t num)
{
    std::string lfn;
    {
        instance_data data (this);
        check_if_open("logical_directory_cpi_impl::sync_get_entry", data->location_);
        lfn = detail::get_filepath(data->location_);
    }

    // check permissions (throws on mismatch)
    check_permissions(saga::replica::Read, "get_num_entries", lfn);

    // return the matching entries
    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    std::vector<std::string> entries;
    SAGA_LOGICALDIR_DBACTION(get_entry, 
        (db_.list(lfn, detail::regex_from_pattern("*"), entries)));
    if (num < 0 || num >= entries.size()) {
        SAGA_OSSTREAM strm;
        strm << "replica::logical_directory_cpi_impl::sync_get_entry: "
                "bad entry number for: " << lfn;
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), 
            saga::BadParameter);
    }
    entry = entries[num];
}

void 
logical_directory_cpi_impl::sync_remove(saga::impl::void_t&, saga::url entry, 
    int flags)
{
    std::string lfn;
    {
        instance_data data (this);
        check_if_open("logical_directory_cpi_impl::sync_remove", data->location_);
        lfn = detail::get_filepath(data->location_);
    }

    // check permissions (throws on mismatch)
    check_permissions(saga::replica::Write, "remove", lfn);
    
    // now (recursively) remove the entry
    std::string entry_path (detail::get_filepath(entry));
    if ('/' != entry_path[0]) 
        entry_path = lfn + "/" + entry_path;     // relative path given

    bool exists_entry = false;
    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_LOGICALDIR_DBACTION(get_entry, 
        (exists_entry = db_.exists_entry(entry_path, false)));
        
    bool exists_dir = false;
    SAGA_LOGICALDIR_DBACTION(get_entry, 
        (exists_dir = db_.exists_directory(entry_path, false)));
        
    if (exists_entry) {
        if (flags & saga::replica::Recursive) {
            SAGA_ADAPTOR_THROW(
                "replica::logical_directory_cpi_impl::sync_remove: "
                "Recursive flag was specified while attempting to delete a "
                "file", saga::BadParameter);
        }
        SAGA_LOGICALDIR_DBACTION(remove, db_.remove(entry_path));
    }
    else if (exists_dir) {
        if (!(flags & saga::replica::Recursive)) {
            SAGA_ADAPTOR_THROW(
                "replica::logical_directory_cpi_impl::sync_remove: "
                "Recursive flag was not specified while attempting to delete a "
                "directory", saga::BadParameter);
        }
        SAGA_LOGICALDIR_DBACTION(remove, db_.remove_directory(entry_path));
    }
    else {
        SAGA_ADAPTOR_THROW(
            "replica::logical_directory_cpi_impl::sync_remove: "
            "Attempting to delete a non-existing directory: " + entry_path, 
            saga::DoesNotExist);
    }
}

void 
logical_directory_cpi_impl::sync_open(saga::name_space::entry& ret, 
    saga::url entry, int flags)
{
    saga::url lfn;
    {
        instance_data data (this);
        check_if_open("logical_directory_cpi_impl::sync_open", data->location_);
        lfn = data->location_.clone();
    }

    // now open the entry
    std::string entry_path (detail::get_filepath(entry));
    if ('/' != entry_path[0]) 
        lfn.set_path(saga::url::unescape(lfn.get_path()) + "/" + entry_path);     // relative path given
    else
        lfn.set_path(entry_path);

    ret = saga::replica::logical_file(this->get_proxy()->get_session(), 
        lfn.get_url(), flags);
}

void 
logical_directory_cpi_impl::sync_open_dir(saga::name_space::directory& ret, 
    saga::url entry, int flags)
{
    saga::url lfn;
    {
        instance_data data (this);
        check_if_open("logical_directory_cpi_impl::sync_open_dir", data->location_);
        lfn = data->location_.clone();
    }

    // now open the directory
    std::string entry_path (detail::get_filepath(entry));
    if ('/' != entry_path[0]) 
        lfn.set_path(saga::url::unescape(lfn.get_path()) + "/" + entry_path);     // relative path given
    else
        lfn.set_path(entry_path);

    ret = saga::replica::logical_directory(this->get_proxy()->get_session(), 
        lfn.get_url(), flags);
}

void 
logical_directory_cpi_impl::sync_change_dir(saga::impl::void_t&, saga::url dir)
{
    saga::url lfn;
    {
        instance_data data (this);
        check_if_open("logical_directory_cpi_impl::sync_change_dir", data->location_);
        lfn = data->location_.clone();
    }
    
    // check permissions (throws on mismatch)
    check_permissions(saga::replica::Write, "change_dir", lfn.get_string());
    
    std::string dir_path (detail::get_filepath(dir));
    if ('/' != dir_path[0]) 
        lfn.set_path(saga::url::unescape(lfn.get_path()) + "/" + dir_path);     // relative path given
    else
        lfn.set_path(dir_path);

    // verify the directory existence
    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_LOGICALDIR_DBACTION(change_dir, 
        db_.exists_directory(saga::url::unescape(lfn.get_path())));

    // change directory 
    {
        instance_data data (this);
        data->location_ = lfn.get_url();
    }
}

void 
logical_directory_cpi_impl::sync_make_dir(saga::impl::void_t&, saga::url dir, 
    int flags)
{
    std::string lfn;
    {
        instance_data data (this);
        check_if_open("logical_directory_cpi_impl::sync_make_dir", data->location_);
        lfn = detail::get_filepath(data->location_);
    }

    // check permissions (throws on mismatch)
    check_permissions(saga::replica::Write, "make_dir", lfn);
    
    std::string dir_path (detail::get_filepath(dir));
    if ('/' != dir_path[0]) 
        dir_path = lfn + "/" + dir_path;     // relative path given

    bool result = false;

    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    SAGA_LOGICALDIR_DBACTION(make_dir, 
        (result = db_.exists_directory(dir_path)));

    if (result) {
        // make sure directory does not exist if Exclusive is given
        if (flags & saga::replica::Exclusive) {
            SAGA_ADAPTOR_THROW(
                "replica::logical_directory_cpi_impl::sync_make_dir: "
                "Exclusive flag was specified while the directory already exists",
                saga::AlreadyExists);
        }
    }
    else {    
        // create the new directory
        SAGA_LOGICALDIR_DBACTION(make_dir, 
            (db_.create_directory(dir_path, 
                (flags & saga::replica::CreateParents) ? true : false)));
    }
}

///////////////////////////////////////////////////////////////////////////////
//  logical_directory functions
void 
logical_directory_cpi_impl::sync_is_file(bool& ret, saga::url name)
{
    sync_is_entry(ret, name);
}

void 
logical_directory_cpi_impl::sync_open(saga::replica::logical_file& ret, 
    saga::url entry, int flags)
{
    sync_open((saga::name_space::entry&)ret, entry, flags);
}

void 
logical_directory_cpi_impl::sync_open_dir(saga::replica::logical_directory& ret, 
    saga::url entry, int flags)
{
    sync_open_dir((saga::name_space::directory&)ret, entry, flags);
}

void 
logical_directory_cpi_impl::sync_find(std::vector<saga::url>& ret, 
    std::string pattern, std::vector<std::string> patterns, int flags)
{
    std::string lfn;
    std::string basename;
    {
        instance_data data (this);
        check_if_open("logical_directory_cpi_impl::sync_find", data->location_);
        lfn = detail::get_filepath(data->location_);
        basename = data->location_.get_string();
    }

    // check permissions (throws on mismatch)
    check_permissions(saga::replica::Read, "find", lfn);
    
    if (flags & saga::replica::Dereference) {
        SAGA_OSSTREAM strm;
        strm << "logical_file::logical_directory_cpi_impl::sync_find: "
                "Dereferencing links is not supported.";
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), 
            saga::NotImplemented);
    }

    mutex_type::scoped_lock lock(mtx_);
    database_scope db_scope(this, db_);

    std::vector<std::string> paths;
    if (flags & saga::replica::Recursive) {
    // invoke list recursively
        SAGA_LOGICALDIR_DBACTION(find, 
            (find_recursive(db_, lfn, detail::regex_from_pattern(pattern), "", paths)));
    }
    else {
    // same as list
        SAGA_LOGICALDIR_DBACTION(find, 
            (db_.list(lfn, detail::regex_from_pattern(pattern), paths)));
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

        std::vector<std::string>::iterator end = paths.end();
        for (std::vector<std::string>::iterator it = paths.begin(); it != end; ++it)
        {
            std::string e (lfn + *it);
            int key_count = 0;
            SAGA_LOGICALDIR_DBACTION(find,
                (key_count = db_.find_attributes(e, kpatrx, vpatrx, NULL, true))); 
            if (key_count)
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

///////////////////////////////////////////////////////////////////////////////
}   // namespace replica


