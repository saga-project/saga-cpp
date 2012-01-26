//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2006 		Ole Weidner (ole.weidner@aei.mpg.de)
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(ADAPTORS_DEFAULT_FILE_DEFAULT_NAME_SPACE_ENTRY_IMPL_HPP)
#define ADAPTORS_DEFAULT_FILE_DEFAULT_NAME_SPACE_ENTRY_IMPL_HPP

#include <map>
#include <vector>
#include <errno.h>
#include <stdio.h>
#include <limits.h>

#include <boost/system/system_error.hpp>
#include <boost/system/error_code.hpp>

#include <saga/saga/util.hpp>
#include <saga/saga/exception.hpp>
#include <saga/saga/url.hpp>
#include <saga/saga/path_leaf.hpp>
#include <saga/impl/config.hpp>

#include <saga/saga/packages/namespace/namespace_entry.hpp>

#include <saga/saga/adaptors/bulk_container.hpp>

#include "default_namespace_entry.hpp"
#include "common_helpers.hpp"

#ifndef  MAX_PATH
# define MAX_PATH _POSIX_PATH_MAX
#endif

/*
 * constructor
 */
template <typename Base>
inline
namespace_entry_cpi_impl<Base>::namespace_entry_cpi_impl (
        saga::impl::proxy                * p,
        saga::impl::v1_0::cpi_info const & info,
        saga::ini::ini             const & glob_ini,
        saga::ini::ini             const & adap_ini,
        TR1::shared_ptr <saga::adaptor>  adaptor,
        saga::impl::v1_0::cpi::flags       flags)
:   base_cpi (p, info, adaptor, flags),
    is_open_ (true)
{
}

/*
 * destructor
 */
template <typename Base>
inline
namespace_entry_cpi_impl<Base>::~namespace_entry_cpi_impl (void)
{
}

/*
 * SAGA API functions
 */

///////////////////////////////////////////////////////////////////////////////
template <typename Base>
inline void
namespace_entry_cpi_impl<Base>::check_if_open (std::string const& functionname, 
        saga::url const& location)
{
    if (!is_open_)
    {
        SAGA_OSSTREAM strm;
        strm << functionname << ": entry is not in open state: " 
             << location.get_url();
        SAGA_ADAPTOR_THROW (SAGA_OSSTREAM_GETSTRING (strm), saga::IncorrectState);
    }
}

template <typename Base>
inline void
namespace_entry_cpi_impl<Base>::sync_exists (bool & exists)
{
    namespace fs = boost::filesystem;

    instance_data data (this);
    check_if_open ("namespace_entry_cpi_impl<Base>::sync_exists", data->location_);

    // verify current working directory is local
    saga::url url(data->location_);
    if (!::detail::file_islocal(url))
    {
        SAGA_OSSTREAM strm;
        strm << "namespace_entry_cpi_impl<Base>::sync_exists: "
            "cannot handle remote entry: " << url.get_url();
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
    }

    try {
#if BOOST_FILESYSTEM_VERSION == 3
        fs::path path (saga::url::unescape(::detail::get_filepath(data->location_)));
#else
        fs::path path (saga::url::unescape(::detail::get_filepath(data->location_)), fs::native);
#endif
        exists = fs::exists (path);
    }
    catch (boost::system::system_error const& e) {
        SAGA_ADAPTOR_THROW(
            url.get_string() + ": caught filesystem exception: " + e.what(),
            saga::NoSuccess);
    }
}

///////////////////////////////////////////////////////////////////////////////
template <typename Base>
inline void
namespace_entry_cpi_impl<Base>::sync_is_dir (bool & is_dir)
{
    namespace fs = boost::filesystem;

    instance_data data (this);
    check_if_open ("namespace_entry_cpi_impl<Base>::sync_is_dir", data->location_);

    // verify current working directory is local
    saga::url url(data->location_);
    if (!::detail::file_islocal(url))
    {
        SAGA_OSSTREAM strm;
        strm << "namespace_entry_cpi_impl<Base>::sync_is_dir: "
            "cannot handle remote entry: " << url.get_url();
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
    }

    // avoid race conditions between exists and is_directory
    typename mutex_type::scoped_lock lock(this->mtx_);

    try {
#if BOOST_FILESYSTEM_VERSION == 3
        fs::path path (saga::url::unescape(::detail::get_filepath(data->location_)));
#else
        fs::path path (saga::url::unescape(::detail::get_filepath(data->location_)), fs::native);
#endif
        is_dir = fs::exists (path) && fs::is_directory (path);
    }
    catch (boost::system::system_error const& e) {
        SAGA_ADAPTOR_THROW(
            url.get_string() + ": caught filesystem exception: " + e.what(),
            saga::NoSuccess);
    }
}

///////////////////////////////////////////////////////////////////////////////
template <typename Base>
inline void
namespace_entry_cpi_impl<Base>::sync_is_entry (bool & is_entry)
{
    namespace fs = boost::filesystem;

    instance_data data (this);
    check_if_open ("namespace_entry_cpi_impl<Base>::sync_is_entry", data->location_);

    // verify current working directory is local
    saga::url url(data->location_);
    if (!::detail::file_islocal(url))
    {
        SAGA_OSSTREAM strm;
        strm << "namespace_entry_cpi_impl<Base>::sync_is_entry: "
            "cannot handle remote entry: " << url.get_url();
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
    }

    // avoid race conditions between exists and is_directory
    typename mutex_type::scoped_lock lock(this->mtx_);

    try {
#if BOOST_FILESYSTEM_VERSION == 3
        fs::path path (saga::url::unescape(::detail::get_filepath(data->location_)));
#else
        fs::path path (saga::url::unescape(::detail::get_filepath(data->location_)), fs::native);
#endif
        is_entry = fs::exists (path) && ! fs::is_directory (path);
    }
    catch (boost::system::system_error const& e) {
        SAGA_ADAPTOR_THROW(
            url.get_string() + ": caught filesystem exception: " + e.what(),
            saga::NoSuccess);
    }
}

template <typename Base>
inline void
namespace_entry_cpi_impl<Base>::sync_is_file (bool & is_file)
{
    sync_is_entry (is_file);
}

///////////////////////////////////////////////////////////////////////////////
template <typename Base>
inline void
namespace_entry_cpi_impl<Base>::sync_is_link (bool & is_link)
{
    namespace fs = boost::filesystem;

    instance_data data (this);
    check_if_open ("namespace_entry_cpi_impl<Base>::sync_is_link", data->location_);

    // verify current working directory is local
    saga::url url(data->location_);
    if (!::detail::file_islocal(url))
    {
        SAGA_OSSTREAM strm;
        strm << "namespace_entry_cpi_impl<Base>::sync_is_link: "
            "cannot handle remote entry: " << url.get_url();
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
    }

    // avoid race conditions between exists and symbolic_link_exists
    typename mutex_type::scoped_lock lock(this->mtx_);

    try {
#if BOOST_FILESYSTEM_VERSION == 3
        fs::path path (saga::url::unescape(::detail::get_filepath(data->location_)));
#else
        fs::path path (saga::url::unescape(::detail::get_filepath(data->location_)), fs::native);
#endif
        is_link = fs::exists (path) && fs::symbolic_link_exists (path);
    }
    catch (boost::system::system_error const& e) {
        SAGA_ADAPTOR_THROW(
            url.get_string() + ": caught filesystem exception: " + e.what(),
            saga::NoSuccess);
    }
}

///////////////////////////////////////////////////////////////////////////////
template <typename Base>
inline void
namespace_entry_cpi_impl<Base>::sync_get_url (saga::url& url)
{
    saga::url u;
    {
        instance_data data (this);
        u = data->location_;
    }

    // verify current working directory is local
    if (!::detail::file_islocal(u))
    {
        SAGA_OSSTREAM strm;
        strm << "namespace_entry_cpi_impl<Base>::sync_is_dir: "
            "cannot handle remote entry: " << u.get_url();
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
    }

    // complete url
//     if (u.get_scheme().empty())
//         u.set_scheme("file");
//     if (u.get_host().empty())
//         u.set_host("localhost");

    url = u.get_url();
}

///////////////////////////////////////////////////////////////////////////////
template <typename Base>
inline void
namespace_entry_cpi_impl<Base>::sync_get_cwd (saga::url& url)
{
    namespace fs = boost::filesystem;

    instance_data data (this);

    // verify current working directory is local
    saga::url result(data->location_);
    if (!::detail::file_islocal(url))
    {
        SAGA_OSSTREAM strm;
        strm << "namespace_entry_cpi_impl<Base>::sync_get_cwd: "
            "cannot handle remote entry: " << result.get_url();
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
    }

    try {
#if BOOST_FILESYSTEM_VERSION == 3
        fs::path name (saga::url::unescape(result.get_path()));
#else
        fs::path name (saga::url::unescape(result.get_path()), fs::native);
#endif
        if (fs::is_directory (name)) {
            std::string name_str(name.string());
            if (name_str.find_last_of(SAGA_PATH_DELIMITERS) == name_str.size()-1)

#if BOOST_FILESYSTEM_VERSION == 3
                name = fs::path(name_str.substr(0, name_str.size()-1));
#else
                name = fs::path(name_str.substr(0, name_str.size()-1), fs::native);
#endif
        }
        result.set_path(name.branch_path().string());

    // complete url
//     if (result.get_scheme().empty())
//         result.set_scheme("file");
//     if (result.get_host().empty())
//         result.set_host("localhost");

        url = result;
    }
    catch (boost::system::system_error const& e) {
        SAGA_ADAPTOR_THROW(
            url.get_string() + ": caught filesystem exception: " + e.what(),
            saga::NoSuccess);
    }
}

///////////////////////////////////////////////////////////////////////////////
template <typename Base>
inline void
namespace_entry_cpi_impl<Base>::sync_get_name (saga::url& url)
{
    namespace fs = boost::filesystem;

    instance_data data (this);

    // verify current working directory is local
    saga::url current_url(data->location_);
    if (!::detail::file_islocal(current_url))
    {
        SAGA_OSSTREAM strm;
        strm << "namespace_entry_cpi_impl<Base>::sync_get_name: "
            "cannot handle remote entry: " << current_url.get_url();
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
    }

    try {
        saga::url u (::detail::get_filepath(data->location_));
#if BOOST_FILESYSTEM_VERSION == 3
        fs::path name (saga::url::unescape(u.get_path()));
#else
        fs::path name (saga::url::unescape(u.get_path()), fs::native);
#endif        

        if (fs::is_directory (name)) {
        std::string name_str(name.string());

            if (name_str.find_last_of(SAGA_PATH_DELIMITERS) == name_str.size()-1)
#if BOOST_FILESYSTEM_VERSION == 3
                name = fs::path(name_str.substr(0, name_str.size()-1));
#else
                name = fs::path(name_str.substr(0, name_str.size()-1), fs::native);
#endif
        }
        url = saga::detail::leaf(name);
    }
    catch (boost::system::system_error const& e) {
        SAGA_ADAPTOR_THROW(
            current_url.get_string() + ": caught filesystem exception: " + e.what(),
            saga::NoSuccess);
    }
}

///////////////////////////////////////////////////////////////////////////////
template <typename Base>
inline void
namespace_entry_cpi_impl<Base>::sync_copy (saga::impl::void_t & ret,
    saga::url dest, int flags)
{
    namespace fs = boost::filesystem;

    // handle the file
    saga::url url;
    {
        instance_data data (this);
        url = data->location_;
    }
    check_if_open ("namespace_entry_cpi_impl<Base>::sync_copy", url);

    // verify current working directory is local
    if (!::detail::file_islocal(url))
    {
        SAGA_OSSTREAM strm;
        strm << "namespace_entry_cpi_impl<Base>::sync_copy: "
            "cannot handle remote entry: " << url.get_url();
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
    }

    // verify this file can be handled
    if (!::detail::file_islocal(dest))
    {
      SAGA_OSSTREAM strm;
      strm << "namespace_entry_cpi_impl<Base>::sync_copy: "
          "cannot handle remote destination file: " << dest.get_url();
      SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
    }

    try {
#if BOOST_FILESYSTEM_VERSION == 3
        fs::path src_location (saga::url::unescape(::detail::get_filepath(url)));
        fs::path dest_path (saga::url::unescape(::detail::get_filepath(dest)));
#else
        fs::path src_location (saga::url::unescape(::detail::get_filepath(url)), fs::native);
        fs::path dest_path (saga::url::unescape(::detail::get_filepath(dest)), fs::native);
#endif
        fs::path dst_location;

        if ( ! dest_path.has_root_path () )
            dst_location = dest_path;
        else
            dst_location = dest_path;

        detail::canonize(dst_location);

        bool is_src_dir = fs::is_directory(src_location);
        bool is_dst_dir = fs::is_directory(dst_location);

        if (!is_src_dir && is_dst_dir)
        {
            dst_location /= saga::detail::leaf(src_location);
            is_dst_dir = false;
        }

        // avoid race conditions between exists and the copy operation
        typename mutex_type::scoped_lock lock(this->mtx_);

        // remove the file/directory if it exists and we should overwrite
        if ((flags & saga::name_space::Overwrite) && fs::exists(dst_location) && !is_dst_dir) 
        {
            fs::remove(dst_location);
        }

        // if destination still exists raise an error
        if (!is_src_dir && fs::exists(dst_location)) {
            SAGA_OSSTREAM strm;
            if (flags & saga::name_space::Overwrite) {
                strm << "namespace_entry_cpi_impl<Base>::sync_copy: "
                        "target file still exists after deleting: " 
                     << dest.get_url();
                SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::NoSuccess);
            }
            else {
                strm << "namespace_entry_cpi_impl<Base>::sync_copy: "
                        "target file already exists: " << dst_location.string();
                SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::AlreadyExists);
            }
        }

        if (is_src_dir && !fs::exists(dest_path)) {
          // create directory, if needed
          if (flags & saga::filesystem::CreateParents) {
            try {
              saga::detail::remove_trailing_dot(dest_path);
              if (!fs::create_directories (dest_path)) {
                SAGA_ADAPTOR_THROW(dest_path.string () + 
                  ": couldn't create the directory hierarchy", 
                  saga::DoesNotExist);
              }
            }
            catch (boost::system::system_error const& e) {
              if ( e.code () != boost::system::posix_error::file_exists )
              {
                SAGA_ADAPTOR_THROW(dest_path.string () + 
                  ": couldn't create the directory (" + e.what() + ")", 
                  saga::DoesNotExist);
              }
            }
          }
          else if (flags & saga::filesystem::Create) {
            try {
              saga::detail::remove_trailing_dot(dest_path);
              if (!fs::create_directory (dest_path)) {
                SAGA_ADAPTOR_THROW(dest_path.string() + 
                  ": couldn't create the directory", saga::DoesNotExist);
              }
            }
            catch (boost::system::system_error const& e) {
              if (e.code() != boost::system::posix_error::file_exists)
              {
                SAGA_ADAPTOR_THROW(dest_path.string () + 
                  ": couldn't create the directory (" + e.what() + "), "
                  "(did you forget to specify the flag CreateParents?)", 
                  saga::DoesNotExist);
              }
            }
          }
        }

        if (is_src_dir) {
            // src location refers to a is a directory
            detail::copy_directory(src_location, dst_location, flags);
        }
        else {
            fs::copy_file(src_location, dst_location);
        }
    }
    catch (boost::system::system_error const& e) {
        SAGA_ADAPTOR_THROW(
            url.get_string() + ": caught filesystem exception: " + e.what(),
            saga::NoSuccess);
    }
}

template <typename Base>
inline void
namespace_entry_cpi_impl<Base>::sync_link (saga::impl::void_t & ret,
    saga::url symlink, int flags)
{
    saga::url url;
    {
        instance_data data (this);
        url = data->location_;
    }
    check_if_open ("namespace_entry_cpi_impl<Base>::sync_link", url);

    // verify current working directory is local
    if (!::detail::file_islocal(url))
    {
        SAGA_OSSTREAM strm;
        strm << "namespace_entry_cpi_impl<Base>::sync_link: "
            "cannot handle remote entry: " << url.get_url();
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
    }

    // verify this file can be handled
    if (!::detail::file_islocal(symlink))
    {
        SAGA_OSSTREAM strm;
        strm << "namespace_entry_cpi_impl<Base>::sync_link: "
                "cannot handle remote destination as symlink name: "
             << symlink.get_url();
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
    }

    try {
        namespace fs = boost::filesystem;
#if BOOST_FILESYSTEM_VERSION == 3
        fs::path src_location (saga::url::unescape(::detail::get_filepath(url)));
#else
        fs::path src_location (saga::url::unescape(::detail::get_filepath(url)), fs::native);
#endif
        fs::path symlink_location (src_location);

        // complete paths
#if BOOST_FILESYSTEM_VERSION == 3
        fs::path symlink_path (saga::url::unescape(::detail::get_filepath(symlink)));
#else
        fs::path symlink_path (saga::url::unescape(::detail::get_filepath(symlink)), fs::native);
#endif

        if (!symlink_path.has_root_path())
            symlink_location = symlink_path;
        else
            symlink_location = symlink_path;

        detail::canonize(symlink_location);

        fs::create_symlink(src_location, symlink_location);
    }
    catch (boost::system::system_error const& e) {
        SAGA_ADAPTOR_THROW(
            url.get_string() + ": caught filesystem exception: " + e.what(),
            saga::NoSuccess);
    }
}

template <typename Base>
inline void
namespace_entry_cpi_impl<Base>::sync_move (saga::impl::void_t & ret,
    saga::url dest, int flags)
{
    namespace fs = boost::filesystem;

    // verify this file can be handled
    if (!::detail::file_islocal(dest))
    {
      SAGA_OSSTREAM strm;
      strm << "namespace_entry_cpi_impl<Base>::sync_move: "
          "cannot handle remote destination file: " << dest.get_url();
      SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
    }

    instance_data data (this);
    check_if_open ("namespace_entry_cpi_impl<Base>::sync_move", data->location_);

    // verify current working directory is local
    saga::url url(data->location_);
    if (!::detail::file_islocal(url))
    {
        SAGA_OSSTREAM strm;
        strm << "namespace_entry_cpi_impl<Base>::sync_move: "
            "cannot handle remote entry: " << url.get_url();
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
    }

    try {
#if BOOST_FILESYSTEM_VERSION == 3
        fs::path src_location (saga::url::unescape(::detail::get_filepath(data->location_)));
        fs::path dest_path (saga::url::unescape(::detail::get_filepath(dest)));
#else
        fs::path src_location (saga::url::unescape(::detail::get_filepath(data->location_)), fs::native);
        fs::path dest_path (saga::url::unescape(::detail::get_filepath(dest)), fs::native);
#endif
    
        fs::path dst_location (src_location);

        // complete paths

        if (!dest_path.has_root_path())
            dst_location = dest_path;
        else
            dst_location = dest_path;

        detail::canonize(dst_location);

        bool is_src_dir = fs::is_directory(src_location);
        bool is_dst_dir = fs::is_directory(dst_location);

        if (!is_src_dir && is_dst_dir)
            dst_location /= saga::detail::leaf(src_location);

        // avoid race conditions between exists and the move operation
        typename mutex_type::scoped_lock lock(this->mtx_);

        // remove the file if it exists and we should overwrite
        if ((flags & saga::name_space::Overwrite) && fs::exists(dst_location)) {
            if (is_dst_dir)
                fs::remove_all(dst_location);
            else
                fs::remove(dst_location);
        }

        // if destination still exists raise an error
        if (fs::exists(dst_location)) {
            SAGA_OSSTREAM strm;
            if (flags & saga::name_space::Overwrite) {
                strm << "namespace_entry_cpi_impl<Base>::sync_move: "
                        "target file/directory still exists after deleting: " 
                     << dest.get_url();
                SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::NoSuccess);
            }
            else {
                strm << "namespace_entry_cpi_impl<Base>::sync_move: "
                        "target file already exists: " << dest.get_url();
                SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::AlreadyExists);
            }
        }

        fs::rename(src_location, dst_location);
    }
    catch (boost::system::system_error const& e) {
        SAGA_ADAPTOR_THROW(
            url.get_string() + ": caught filesystem exception: " + e.what(),
            saga::NoSuccess);
    }
}

template <typename Base>
inline void
namespace_entry_cpi_impl<Base>::sync_remove (saga::impl::void_t & ret, int flags)
{
    namespace fs = boost::filesystem;

    instance_data data (this);
    check_if_open ("namespace_entry_cpi_impl<Base>::sync_remove", data->location_);

    // verify current working directory is local
    saga::url url(data->location_);
    if (!::detail::file_islocal(url))
    {
        SAGA_OSSTREAM strm;
        strm << "namespace_entry_cpi_impl<Base>::sync_remove: "
            "cannot handle remote entry: " << url.get_url();
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
    }

    if (flags & saga::filesystem::Recursive)
    {
        SAGA_OSSTREAM strm;
        strm << "namespace_entry_cpi_impl<Base>::sync_remove: "
            "cannot handle flag saga::filesystem::Recursive.";
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::BadParameter);
    }

    try {
#if BOOST_FILESYSTEM_VERSION == 3
        fs::path src_location (saga::url::unescape(::detail::get_filepath(data->location_)));
#else
        fs::path src_location (saga::url::unescape(::detail::get_filepath(data->location_)), fs::native);
#endif
        fs::remove(src_location);
    }
    catch (boost::system::system_error const& e) {
        SAGA_ADAPTOR_THROW(
            url.get_string() + ": caught filesystem exception: " + e.what(),
            saga::NoSuccess);
    }
}

template <typename Base>
inline void
namespace_entry_cpi_impl<Base>::sync_close (saga::impl::void_t & ret, double timeout)
{
    is_open_ = false;
    // Nothing to close here...
}

#if ! defined (SAGA_DEFAULT_FILE_ADAPTOR_NO_ASYNCS)
///////////////////////////////////////////////////////////////////////////////
//  This adaptor implements the async functions based on its own synchronous
//  functions for testing purposes only. Since there is no real need
//  to do so, we allow these to be 'switched off'.

template <typename Base>
inline saga::task
namespace_entry_cpi_impl<Base>::async_get_url()
{
    return saga::adaptors::task("namespace_entry_cpi_impl<Base>::sync_get_url",
        this->shared_from_this(), &namespace_entry_cpi_impl<Base>::sync_get_url);
}

template <typename Base>
inline saga::task
namespace_entry_cpi_impl<Base>::async_get_cwd ()
{
    return saga::adaptors::task("namespace_entry_cpi_impl<Base>::sync_get_cwd",
        this->shared_from_this(), &namespace_entry_cpi_impl<Base>::sync_get_cwd);
}

template <typename Base>
inline saga::task
namespace_entry_cpi_impl<Base>::async_get_name()
{
    return saga::adaptors::task("namespace_entry_cpi::get_name",
                                 this->shared_from_this(),
                                 &namespace_entry_cpi_impl<Base>::sync_get_name
#if !defined (SAGA_DEFAULT_FILE_ADAPTOR_NO_BULKS)
                               , &namespace_entry_cpi_impl<Base>::prepare_get_name
#endif
                             );
}

template <typename Base>
inline saga::task
namespace_entry_cpi_impl<Base>::async_exists()
{
    return saga::adaptors::task("namespace_entry_cpi_impl<Base>::sync_exists",
        this->shared_from_this(),  &namespace_entry_cpi_impl<Base>::sync_exists);
}

template <typename Base>
inline saga::task
namespace_entry_cpi_impl<Base>::async_is_dir()
{
    return saga::adaptors::task("namespace_entry_cpi_impl<Base>::sync_is_dir",
        this->shared_from_this(), &namespace_entry_cpi_impl<Base>::sync_is_dir);
}

template <typename Base>
inline saga::task
namespace_entry_cpi_impl<Base>::async_is_entry()
{
    return saga::adaptors::task("namespace_entry_cpi_impl<Base>::sync_is_entry",
        this->shared_from_this(), &namespace_entry_cpi_impl<Base>::sync_is_entry);
}

template <typename Base>
inline saga::task
namespace_entry_cpi_impl<Base>::async_is_link()
{
    return saga::adaptors::task("namespace_entry_cpi_impl<Base>::sync_is_link",
        this->shared_from_this(), &namespace_entry_cpi_impl<Base>::sync_is_link);
}

///////////////////////////////////////////////////////////////////////////////
template <typename Base>
inline saga::task
namespace_entry_cpi_impl<Base>::async_copy(saga::url source, int flags)
{
    return saga::adaptors::task("namespace_entry_cpi_impl<Base>::sync_copy",
        this->shared_from_this(), &namespace_entry_cpi_impl<Base>::sync_copy,
        source, flags);
}

template <typename Base>
inline saga::task
namespace_entry_cpi_impl<Base>::async_link(saga::url target, int flags)
{
    return saga::adaptors::task("namespace_entry_cpi_impl<Base>::sync_link",
        this->shared_from_this(), &namespace_entry_cpi_impl<Base>::sync_link,
        target, flags);
}

template <typename Base>
inline saga::task
namespace_entry_cpi_impl<Base>::async_move (saga::url source, int flags)
{
    return saga::adaptors::task("namespace_entry_cpi_impl<Base>::sync_move",
        this->shared_from_this(), &namespace_entry_cpi_impl<Base>::sync_move,
        source, flags);
}

template <typename Base>
inline saga::task
namespace_entry_cpi_impl<Base>::async_remove(int flags)
{
    return saga::adaptors::task("namespace_entry_cpi_impl<Base>::sync_remove",
        this->shared_from_this(), &namespace_entry_cpi_impl<Base>::sync_remove,
        flags);
}

template <typename Base>
inline saga::task
namespace_entry_cpi_impl<Base>::async_close(double timeout)
{
    return saga::adaptors::task("namespace_entry_cpi_impl<Base>::sync_close",
        this->shared_from_this(), &namespace_entry_cpi_impl<Base>::sync_close,
        timeout);
}

/////////////////////////////////////////////////////////////////////////////
// bulk ops
#if ! defined (SAGA_DEFAULT_FILE_ADAPTOR_NO_BULKS)
// This adaptor implements the bulk functions for testing purposes only. Since
// there is no principal need to do so, we allow these to be 'switched off'.
template <typename Base>
inline bool
namespace_entry_cpi_impl<Base>::prepare_get_name(saga::url& url, saga::uuid task_uuid)
{
    SAGA_LOG(SAGA_VERBOSE_LEVEL_BLURB) 
        << "Reached bulk_prepare";

    create_bulk_container_or_add_params(this, this->bulk_,
        &namespace_entry_cpi_impl<Base>::bulk_get_name, TR1::ref(url), task_uuid);
    return true;
}

template <typename Base>
inline std::vector<saga::uuid>
namespace_entry_cpi_impl<Base>::bulk_get_name(
                            std::vector<TR1::reference_wrapper<saga::url> >& rets,
                            std::vector<saga::uuid>& uuids)
{
    SAGA_LOG(SAGA_VERBOSE_LEVEL_BLURB) 
        << "Reached bulk_get_name for executing the bulk";


    std::vector<TR1::reference_wrapper<saga::url> >::iterator str_it = rets.begin();
    std::vector<saga::uuid>::iterator uuid_it = uuids.begin();

    std::vector<saga::uuid> r;

    while ( (str_it != rets.end()) && (uuid_it != uuids.end()))
    {
        TR1::reference_wrapper<saga::url> ret = *str_it;
        this->sync_get_name(ret);
        r.push_back(*uuid_it);
        ++uuid_it;
        ++str_it;
    }

    return r;
}

#endif // ! SAGA_DEFAULT_FILE_ADAPTOR_NO_BULKS
#endif // ! SAGA_DEFAULT_FILE_ADAPTOR_NO_ASYNCS

#endif // !ADAPTORS_DEFAULT_FILE_DEFAULT_NAME_SPACE_ENTRY_IMPL_HPP

