//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2006      Stephan Hirmer (shirmer@cct.lsu.edu)
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ADAPTORS_DEFAULT_FILE_DEFAULT_NAME_SPACE_DIR_IMPL_HPP
#define ADAPTORS_DEFAULT_FILE_DEFAULT_NAME_SPACE_DIR_IMPL_HPP

#include <map>
#include <vector>

#include <boost/regex.hpp>
#include <boost/filesystem/operations.hpp>

#include <saga/saga/util.hpp>
#include <saga/saga/exception.hpp>
#include <saga/saga/url.hpp>
#include <saga/saga/path_leaf.hpp>
#include <saga/impl/config.hpp>

#include <saga/saga/adaptors/bulk_container.hpp>
#include <saga/saga/uuid.hpp>

#include "default_namespace_entry.hpp"
#include "default_namespace_dir.hpp"
#include "common_helpers.hpp"

#include "default_namespace_entry_impl.hpp"

#include <sys/types.h>
#include <sys/stat.h>

#if defined(BOOST_HAS_UNISTD_H)
#include <unistd.h>
#endif

#ifndef  MAX_PATH
# define MAX_PATH _POSIX_PATH_MAX
#endif

/*
 * constructor
 */
template <typename Base>
inline
namespace_dir_cpi_impl<Base>::namespace_dir_cpi_impl (saga::impl::proxy* p,
        saga::impl::v1_0::cpi_info const& info, saga::ini::ini const& glob_ini,
        saga::ini::ini const& adap_ini, TR1::shared_ptr<saga::adaptor> adaptor,
        saga::impl::v1_0::cpi::flags flags)
:   base_cpi(p, info, glob_ini, adap_ini, adaptor, flags)
{
}

/*
 * destructor
 */
template <typename Base>
inline
namespace_dir_cpi_impl<Base>::~namespace_dir_cpi_impl (void)
{
}

/*
 * SAGA API functions
 */

///////////////////////////////////////////////////////////////////////////////
template <typename Base>
inline void
namespace_dir_cpi_impl<Base>::sync_exists (bool & exists, saga::url url)
{
  namespace fs = boost::filesystem;

  // verify this file can be handled
  if (!::detail::file_islocal(url))
  {
    SAGA_OSSTREAM strm;
    strm << "namespace_dir_cpi_impl<Base>::sync_exists: "
        "cannot handle remote target directory: " << url.get_url();
    SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
  }

  instance_data data (this);
  this->check_if_open ("namespace_dir_cpi_impl<Base>::sync_exists", data->location_);

  // verify current working directory is local
  saga::url dir_url(data->location_);
  if (!::detail::file_islocal(url))
  {
    SAGA_OSSTREAM strm;
    strm << "namespace_dir_cpi_impl<Base>::sync_exists: "
        "cannot handle remote current directory: " << url.get_url();
    SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
  }

  try {
#if BOOST_FILESYSTEM_VERSION == 3
    fs::path name (saga::url::unescape(::detail::get_filepath(url)));
    fs::path path (saga::url::unescape(::detail::get_filepath(data->location_)));
#else
    fs::path name (saga::url::unescape(::detail::get_filepath(url)), fs::native);
    fs::path path (saga::url::unescape(::detail::get_filepath(data->location_)), fs::native);
#endif

    if ( ! name.has_root_path () )
      path /= name;
    else
      path = name;

    detail::canonize(path);
    exists = fs::exists (path);
  }
  catch (boost::system::system_error const& e) {
    SAGA_ADAPTOR_THROW(
      dir_url.get_string() + ": caught filesystem exception: " + e.what(),
      saga::NoSuccess);
  }
}

template <typename Base>
inline void
namespace_dir_cpi_impl<Base>::sync_is_dir (bool & is_dir, saga::url url)
{
  namespace fs = boost::filesystem;

  // verify the file can be handled
  if (!::detail::file_islocal(url))
  {
    SAGA_OSSTREAM strm;
    strm << "namespace_dir_cpi_impl<Base>::sync_is_dir: "
        "cannot handle remote target entry: " << url.get_url();
    SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
  }

  instance_data data (this);
  this->check_if_open ("namespace_dir_cpi_impl<Base>::sync_is_dir", data->location_);

  // verify current working directory is local
  saga::url dir_url(data->location_);
  if (!::detail::file_islocal(url))
  {
    SAGA_OSSTREAM strm;
    strm << "namespace_dir_cpi_impl<Base>::sync_is_dir: "
        "cannot handle remote current directory: " << url.get_url();
    SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
  }

  try {
#if BOOST_FILESYSTEM_VERSION == 3
    fs::path name (saga::url::unescape(::detail::get_filepath(url)));
    fs::path path (saga::url::unescape(::detail::get_filepath(data->location_)));
#else
    fs::path name (saga::url::unescape(::detail::get_filepath(url)), fs::native);
    fs::path path (saga::url::unescape(::detail::get_filepath(data->location_)), fs::native);
#endif
    if ( ! name.has_root_path () )
      path /= name;
    else
      path = name;

    detail::canonize(path);

    // avoid race conditions between exists and is_directory
    typename mutex_type::scoped_lock lock(this->mtx_);

    is_dir = fs::exists (path) && fs::is_directory (path);
  }
  catch (boost::system::system_error const& e) {
    SAGA_ADAPTOR_THROW(
      dir_url.get_string() + ": caught filesystem exception: " + e.what(),
      saga::NoSuccess);
  }
}

template <typename Base>
inline void
namespace_dir_cpi_impl<Base>::sync_is_entry (bool & is_entry, saga::url url)
{
  namespace fs = boost::filesystem;

  // verify the file can be handled
  if (!::detail::file_islocal(url))
  {
    SAGA_OSSTREAM strm;
    strm << "namespace_dir_cpi_impl<Base>::sync_is_entry: "
        "cannot handle remote target entry: " << url.get_url();
    SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
  }

  instance_data data (this);
  this->check_if_open ("namespace_dir_cpi_impl<Base>::sync_is_entry", data->location_);

  // verify current working directory is local
  saga::url dir_url(data->location_);
  if (!::detail::file_islocal(url))
  {
    SAGA_OSSTREAM strm;
    strm << "namespace_dir_cpi_impl<Base>::sync_is_entry: "
        "cannot handle remote current directory: " << url.get_url();
    SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
  }

  try {
#if BOOST_FILESYSTEM_VERSION == 3
    fs::path name (saga::url::unescape(::detail::get_filepath(url)));
    fs::path path (saga::url::unescape(::detail::get_filepath(data->location_)));
#else
    fs::path name (saga::url::unescape(::detail::get_filepath(url)), fs::native);
    fs::path path (saga::url::unescape(::detail::get_filepath(data->location_)), fs::native);
#endif

    if ( ! name.has_root_path () )
      path /= name;
    else
      path = name;

    detail::canonize(path);

    // avoid race conditions between exists and is_directory
    typename mutex_type::scoped_lock lock(this->mtx_);

    is_entry = fs::exists (path) && ! fs::is_directory (path);
  }
  catch (boost::system::system_error const& e) {
    SAGA_ADAPTOR_THROW(
      dir_url.get_string() + ": caught filesystem exception: " + e.what(),
      saga::NoSuccess);
  }
}

template <typename Base>
inline void
namespace_dir_cpi_impl<Base>::sync_is_file (bool & is_file, saga::url url)
{
  sync_is_entry (is_file, url);
}
template <typename Base>
inline void
namespace_dir_cpi_impl<Base>::sync_is_link (bool & is_link, saga::url url)
{
  namespace fs = boost::filesystem;

  instance_data data (this);
  this->check_if_open ("namespace_dir_cpi_impl<Base>::sync_is_link", data->location_);

  // verify current working directory is local
  saga::url dir_url(data->location_);
  if (!::detail::file_islocal(url))
  {
    SAGA_OSSTREAM strm;
    strm << "namespace_dir_cpi_impl<Base>::sync_is_link: "
        "cannot handle remote current directory: " << url.get_url();
    SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
  }

  // verify the file can be handled
  if (!::detail::file_islocal(url))
  {
    SAGA_OSSTREAM strm;
    strm << "namespace_dir_cpi_impl<Base>::sync_is_link: "
        "cannot handle remote target entry: " << url.get_url();
    SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
  }

  try {
#if BOOST_FILESYSTEM_VERSION == 3
    fs::path name (saga::url::unescape(::detail::get_filepath(url)));
    fs::path path (saga::url::unescape(::detail::get_filepath(data->location_)));
#else
    fs::path name (saga::url::unescape(::detail::get_filepath(url)), fs::native);
    fs::path path (saga::url::unescape(::detail::get_filepath(data->location_)), fs::native);
#endif
    if ( ! name.has_root_path () )
      path /= name;
    else
      path = name;

    detail::canonize(path);

    // avoid race conditions between exists and symbolic_link_exists
    typename mutex_type::scoped_lock lock(this->mtx_);

#if BOOST_VERSION >= 103400
    is_link = fs::exists (path) && fs::symbolic_link_exists(path);
#elif defined(linux) || defined(__linux) || defined(__linux__) || defined(SAGA_APPLE)
    {
      is_link = false;
      struct stat out;
      lstat (path.string ().c_str (), &out);

      is_link = S_ISLNK(out.st_mode) ? true : false;
    }
#else
    SAGA_ADAPTOR_THROW("Not implemented.", saga::NotImplemented);
#endif
  }
  catch (boost::system::system_error const& e) {
    SAGA_ADAPTOR_THROW(
      dir_url.get_string() + ": caught filesystem exception: " + e.what(),
      saga::NoSuccess);
  }
}

///////////////////////////////////////////////////////////////////////////////
template <typename Base>
inline void
namespace_dir_cpi_impl<Base>::sync_change_dir (saga::impl::void_t&, saga::url new_dir)
{
    namespace fs = boost::filesystem;

    // verify the file can be handled
    if (!::detail::file_islocal(new_dir))
    {
        SAGA_OSSTREAM strm;
        strm << "namespace_dir_cpi_impl<Base>::sync_change_dir: "
            "cannot handle remote target directory: " << new_dir.get_url();
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
    }

    instance_data data (this);
    this->check_if_open ("namespace_dir_cpi_impl<Base>::sync_change_dir", data->location_);

    // verify current working directory is local
    saga::url url(data->location_);
    if (!::detail::file_islocal(url))
    {
        SAGA_OSSTREAM strm;
        strm << "namespace_dir_cpi_impl<Base>::sync_change_dir: "
            "cannot handle remote current directory: " << url.get_url();
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
    }

    try {
#if BOOST_FILESYSTEM_VERSION == 3
    fs::path name (saga::url::unescape(::detail::get_filepath(new_dir)));
    fs::path path (saga::url::unescape(::detail::get_filepath(data->location_)));
#else
    fs::path name (saga::url::unescape(::detail::get_filepath(new_dir)), fs::native);
    fs::path path (saga::url::unescape(::detail::get_filepath(data->location_)), fs::native);
#endif
        if ( ! name.has_root_path () )
            path /= name;
        else
            path = name;

        detail::canonize(path);

        // avoid race conditions between exists and is_directory
        typename mutex_type::scoped_lock lock(this->mtx_);

        if (!(fs::exists(path) && fs::is_directory (path)))
        {
            SAGA_ADAPTOR_THROW(
                path.string () + ": doesn't refer to a directory object",
                saga::DoesNotExist);
        }

        saga::url u(data->location_);
        u.set_path(path.string());
        data->location_ = u.get_url();
    }
    catch (boost::system::system_error const& e) {
        SAGA_ADAPTOR_THROW(
            new_dir.get_string() + ": caught filesystem exception: " + e.what(),
            saga::NoSuccess);
    }
}

///////////////////////////////////////////////////////////////////////////////
template <typename Base>
inline void
namespace_dir_cpi_impl<Base>::sync_list(std::vector <saga::url> & retval,
    std::string pattern, int flags)
{
    if (flags & ~(saga::filesystem::Recursive|saga::filesystem::Dereference)) 
    {
        SAGA_OSSTREAM strm;
        strm << "namespace_dir_cpi_impl<Base>::sync_list: "
            "invalid flag specified: " << std::hex << flags;
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::BadParameter);
    }

    if (flags & saga::filesystem::Dereference) 
    {
        SAGA_OSSTREAM strm;
        strm << "namespace_dir_cpi_impl<Base>::sync_find: "
            "dereferencing symlinks is not implemented";
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::NotImplemented);
    }

    namespace fs = boost::filesystem;

    instance_data data (this);
    this->check_if_open ("namespace_dir_cpi_impl<Base>::sync_list", data->location_);

    // verify current working directory is local
    saga::url url(data->location_);
    if (!::detail::file_islocal(url))
    {
        SAGA_OSSTREAM strm;
        strm << "namespace_dir_cpi_impl<Base>::sync_list: "
            "cannot handle remote current directory: " << url.get_url();
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
    }

    try {
#if BOOST_FILESYSTEM_VERSION == 3
        fs::path path (::detail::get_filepath(data->location_));
#else
        fs::path path (::detail::get_filepath(data->location_), fs::native);        
#endif
        std::string host(::detail::get_scheme_authority(data->location_));

        if (flags & saga::filesystem::Recursive) {
            fs::recursive_directory_iterator dir_end;
            for (fs::recursive_directory_iterator dir_it(path); dir_it != dir_end; ++dir_it)
            {
                //retval.push_back(host + saga::detail::leaf((*dir_it))); see --> Spec. p. 141
                saga::url p(saga::url::escape(saga::detail::leaf(*dir_it)));
                retval.push_back(p);
            }
        }
        else {
            fs::directory_iterator dir_end;
            for (fs::directory_iterator dir_it(path); dir_it != dir_end; ++dir_it)
            {
                //retval.push_back(host + saga::detail::leaf((*dir_it))); see --> Spec. p. 141
                saga::url p(saga::url::escape(saga::detail::leaf(*dir_it)));
                retval.push_back(p);
            }
        }
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
namespace_dir_cpi_impl<Base>::sync_find(std::vector <saga::url> & retval,
    std::string entry, int flags)
{
    if (flags & ~(saga::filesystem::Recursive|saga::filesystem::Dereference)) 
    {
        SAGA_OSSTREAM strm;
        strm << "namespace_dir_cpi_impl<Base>::sync_find: "
            "invalid flag specified: " << std::hex << flags;
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::BadParameter);
    }

    if (flags & saga::filesystem::Dereference) 
    {
        SAGA_OSSTREAM strm;
        strm << "namespace_dir_cpi_impl<Base>::sync_find: "
            "dereferencing symlinks is not implemented";
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::NotImplemented);
    }

    saga::url location;
    {
        instance_data data (this);
        location = data->location_;
    }
    this->check_if_open ("namespace_dir_cpi_impl<Base>::sync_find", location);

    // verify current working directory is local
    if (!::detail::file_islocal(location))
    {
        SAGA_OSSTREAM strm;
        strm << "namespace_dir_cpi_impl<Base>::sync_find: "
            "cannot handle remote current directory: " << location.get_url();
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
    }

    std::string pattern(::detail::regex_from_pattern(entry));
    boost::regex rx (pattern);

    try {
        namespace fs = boost::filesystem;

        // all wildcards are relative to the current directory
        if (flags & saga::filesystem::Recursive) {
            fs::recursive_directory_iterator dir_end;
            for (fs::recursive_directory_iterator dir_it(saga::url::unescape(location.get_path())); 
                 dir_it != dir_end; ++dir_it)
            {
#if BOOST_FILESYSTEM_VERSION > 2
                std::string p ((*dir_it).path().string());
#else
                std::string p ((*dir_it).string());
#endif
                boost::smatch what;
                if (boost::regex_match(p, what, rx)) {
                    saga::url u(saga::url::escape(saga::detail::leaf(*dir_it)));
                    retval.push_back(u);
                }
            }
        }
        else {
            fs::directory_iterator dir_end;
            for (fs::directory_iterator dir_it(saga::url::unescape(location.get_path())); 
                 dir_it != dir_end; ++dir_it)
            {
#if BOOST_FILESYSTEM_VERSION > 2
                std::string p ((*dir_it).path().string());
#else
                std::string p ((*dir_it).string());
#endif
                boost::smatch what;
                if (boost::regex_match(p, what, rx)) {
                    saga::url u(saga::url::escape(saga::detail::leaf(*dir_it)));
                    retval.push_back(u);
                }
            }
        }
    }
    catch (boost::system::system_error const& e) {
        SAGA_ADAPTOR_THROW(
            std::string("namespace_dir_cpi_impl<Base>::sync_find: ") + 
            location.get_string() + ": caught filesystem exception: " + e.what(),
            saga::NoSuccess);
    }
}

///////////////////////////////////////////////////////////////////////////////
template <typename Base>
inline void
namespace_dir_cpi_impl<Base>::sync_copy(saga::impl::void_t & ret, saga::url source,
    saga::url destination, int open_mode)
{
    namespace fs = boost::filesystem;

    instance_data data (this);
    this->check_if_open ("namespace_dir_cpi_impl<Base>::sync_copy", data->location_);

    // verify current working directory is local
    saga::url url(data->location_);
    if (!::detail::file_islocal(url))
    {
        SAGA_OSSTREAM strm;
        strm << "namespace_dir_cpi_impl<Base>::sync_copy: "
            "cannot handle remote current directory: " << url.get_url();
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
    }

    // verify the files can be handled
    if (!::detail::file_islocal(source))
    {
        SAGA_OSSTREAM strm;
        strm << "namespace_dir_cpi_impl<Base>::sync_copy: "
            "cannot handle remote source file: " << source.get_url();
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
    }

    if (!::detail::file_islocal(destination))
    {
        SAGA_OSSTREAM strm;
        strm << "namespace_dir_cpi_impl<Base>::sync_copy: "
            "cannot handle remote destination file: " << destination.get_url();
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
    }

    try {
        // handle the files
#if BOOST_FILESYSTEM_VERSION == 3
        fs::path src_location (saga::url::unescape(::detail::get_filepath(data->location_)));
#else
        fs::path src_location (saga::url::unescape(::detail::get_filepath(data->location_)), fs::native);
#endif
        fs::path dst_location (src_location);

        // complete paths
#if BOOST_FILESYSTEM_VERSION == 3
        fs::path src_path  (saga::url::unescape(::detail::get_filepath(source)));
        fs::path dest_path (saga::url::unescape(::detail::get_filepath(destination)));
#else
        fs::path src_path  (saga::url::unescape(::detail::get_filepath(source)), fs::native);
        fs::path dest_path (saga::url::unescape(::detail::get_filepath(destination)), fs::native);

#endif

        if ( ! src_path.has_root_path () )
            src_location /= src_path;
        else
            src_location = src_path;

        detail::canonize(src_location);

        if ( ! dest_path.has_root_path () )
            dst_location /= dest_path;
        else
            dst_location = dest_path;

        detail::canonize(dst_location);

        // avoid race conditions between exists and the copy operation
        typename mutex_type::scoped_lock lock(this->mtx_);

        if (fs::is_directory(src_location)) {
            // src location refers to a is a directory
            detail::copy_directory(src_location, dst_location, open_mode);
        }
        else {
            if (fs::is_directory(dst_location))
                dst_location /= saga::detail::leaf(src_location);

            // remove the file/directory if it exists and we should overwrite
            if ((open_mode & saga::name_space::Overwrite) && fs::exists(dst_location)) {
                fs::remove(dst_location);

                // if destination still exists raise an error
                if (fs::exists(dst_location)) {
                    SAGA_OSSTREAM strm;
                    if (open_mode & saga::name_space::Overwrite) {
                        strm << "namespace_dir_cpi_impl<Base>::sync_copy: "
                                "target file still exists after deleting: " 
                             << destination.get_url();
                        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::NoSuccess);
                    }
                    else {
                        strm << "namespace_dir_cpi_impl<Base>::sync_copy: "
                                "target file already exists: " << destination.get_url();
                        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::AlreadyExists);
                    }
                }
            }

            fs::copy_file(src_location, dst_location);
        }
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
namespace_dir_cpi_impl<Base>::sync_move(saga::impl::void_t & ret, saga::url source,
    saga::url destination, int open_mode)
{
    namespace fs = boost::filesystem;

    instance_data data (this);
    this->check_if_open ("namespace_dir_cpi_impl<Base>::sync_move", data->location_);

    // verify current working directory is local
    saga::url url(data->location_);
    if (!::detail::file_islocal(url))
    {
        SAGA_OSSTREAM strm;
        strm << "namespace_dir_cpi_impl<Base>::sync_move: "
            "cannot handle remote current directory: " << url.get_url();
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
    }

    // verify the files can be handled
    if (!::detail::file_islocal(source))
    {
      SAGA_OSSTREAM strm;
      strm << "namespace_dir_cpi_impl<Base>::sync_move: "
          "cannot handle remote source file: " << source.get_url();
      SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
    }

    if (!::detail::file_islocal(destination))
    {
      SAGA_OSSTREAM strm;
      strm << "namespace_dir_cpi_impl<Base>::sync_move: "
          "cannot handle remote destination file: " << destination.get_url();
      SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
    }

    try {
        // handle the files
#if BOOST_FILESYSTEM_VERSION == 3
        fs::path src_location (saga::url::unescape(::detail::get_filepath(data->location_)));
        fs::path src_path  (saga::url::unescape(::detail::get_filepath(source)));
        fs::path dest_path (saga::url::unescape(::detail::get_filepath(destination)));
#else
        fs::path src_location (saga::url::unescape(::detail::get_filepath(data->location_)), fs::native);
        fs::path src_path  (saga::url::unescape(::detail::get_filepath(source)), fs::native);
        fs::path dest_path (saga::url::unescape(::detail::get_filepath(destination)), fs::native);

#endif
        fs::path dst_location (src_location);

        // complete paths

        if ( ! src_path.has_root_path () )
            src_location /= src_path;
        else
            src_location = src_path;

        detail::canonize(src_location);

        if ( ! dest_path.has_root_path () )
            dst_location /= dest_path;
        else
            dst_location = dest_path;

        detail::canonize(dst_location);

        bool is_src_dir = fs::is_directory(src_location);
        bool is_dst_dir = fs::is_directory(dst_location);

        if (!is_src_dir && is_dst_dir)
            dst_location /= saga::detail::leaf(src_location);

        // avoid race conditions between exists and the copy operation
        typename mutex_type::scoped_lock lock(this->mtx_);

        if ((open_mode & saga::name_space::Overwrite) && fs::exists(dst_location)) {
            if (is_dst_dir)
                fs::remove_all(dst_location);
            else
                fs::remove(dst_location);
        }

        // if destination still exists raise an error
        if (fs::exists(dst_location)) {
            SAGA_OSSTREAM strm;
            if (open_mode & saga::name_space::Overwrite) {
                strm << "namespace_dir_cpi_impl<Base>::sync_move: "
                        "target file/directory still exists after deleting: " 
                     << destination.get_url();
                SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::NoSuccess);
            }
            else {
                strm << "namespace_dir_cpi_impl<Base>::sync_copy: "
                        "target file already exists: " << destination.get_url();
                SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::AlreadyExists);
            }
        }

        saga::detail::remove_trailing_dot(src_location);
        fs::rename(src_location, dst_location);
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
namespace_dir_cpi_impl<Base>::sync_remove (saga::impl::void_t & ret, int flags)
{
    namespace fs = boost::filesystem;

    instance_data data (this);
    check_if_open ("namespace_dir_cpi_impl<Base>::sync_remove", data->location_);

    // verify current working directory is local
    saga::url url(data->location_);
    if (!::detail::file_islocal(url))
    {
        SAGA_OSSTREAM strm;
        strm << "namespace_dir_cpi_impl<Base>::sync_remove: "
            "cannot handle remote entry: " << url.get_url();
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
    }

    if (!(flags & saga::filesystem::Recursive))
    {
        SAGA_OSSTREAM strm;
        strm << "namespace_dir_cpi_impl<Base>::sync_remove: "
            "need to specify flag saga::filesystem::Recursive "
            "in order to delete a directory.";
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::BadParameter);
    }

    try {
#if BOOST_FILESYSTEM_VERSION == 3
        fs::path src_location (saga::url::unescape(::detail::get_filepath(data->location_)));
#else
        fs::path src_location (saga::url::unescape(::detail::get_filepath(data->location_)), fs::native);
#endif
        saga::detail::remove_trailing_dot(src_location);
        remove_all(src_location);
    }
    catch (boost::system::system_error const& e) {
        SAGA_ADAPTOR_THROW(
            url.get_string() + ": caught filesystem exception: " + e.what(),
            saga::NoSuccess);
    }
}

template <typename Base>
inline void
namespace_dir_cpi_impl<Base>::sync_remove(saga::impl::void_t & ret, saga::url url,
                                          int flags)
{
    namespace fs = boost::filesystem;

    // verify the file can be handled
    if (!::detail::file_islocal(url))
    {
        SAGA_OSSTREAM strm;
        strm << "namespace_dir_cpi_impl<Base>::sync_remove: "
            "cannot handle remote target file: " << url.get_url();
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
    }

    instance_data data (this);

    // verify current working directory is local
    saga::url dir_url(data->location_);
    if (!::detail::file_islocal(url))
    {
        SAGA_OSSTREAM strm;
        strm << "namespace_dir_cpi_impl<Base>::sync_remove: "
            "cannot handle remote current directory: " << url.get_url();
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
    }

    try {
#if BOOST_FILESYSTEM_VERSION == 3
        fs::path src_location (saga::url::unescape(::detail::get_filepath(data->location_)));
#else
        fs::path src_location (saga::url::unescape(::detail::get_filepath(data->location_)), fs::native);
#endif

        // complete paths
#if BOOST_FILESYSTEM_VERSION == 3
        fs::path src_path  (saga::url::unescape(::detail::get_filepath(url)));
#else
        fs::path src_path  (saga::url::unescape(::detail::get_filepath(url)), fs::native);
#endif
        if ( ! src_path.has_root_path () )
            src_location /= src_path;
        else
            src_location = src_path;

        detail::canonize(src_location);

        // avoid race conditions between exists and the remove operation
        typename mutex_type::scoped_lock lock(this->mtx_);

        if (!fs::exists(src_location)) {
            SAGA_ADAPTOR_THROW("directory::remove: Can't remove directory: "
              "Does not exist", saga::DoesNotExist);
        }

        if (fs::is_directory(src_location)) {
            if (saga::name_space::Recursive != flags && !fs::is_empty(src_location))
            {
                SAGA_ADAPTOR_THROW("directory::remove: Can't remove directory. "
                    "Please use recursive mode!", saga::BadParameter);
            }
            else {
                saga::detail::remove_trailing_dot(src_location);
                remove_all(src_location);
            }
        }
        else {
             fs::remove(src_location);
        }
    }
    catch (boost::system::system_error const& e) {
        SAGA_ADAPTOR_THROW(
            dir_url.get_string() + ": caught filesystem exception: " + e.what(),
            saga::NoSuccess);
    }
}

template <typename Base>
inline void
namespace_dir_cpi_impl<Base>::sync_make_dir(saga::impl::void_t & ret, saga::url url,
    int open_mode)
{
    namespace fs = boost::filesystem;

    // verify the file can be handled
    if (!::detail::file_islocal(url))
    {
        SAGA_OSSTREAM strm;
        strm << "namespace_dir_cpi_impl<Base>::sync_make_dir: "
            "cannot handle remote target directory: " << url.get_url();
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
    }

    instance_data data (this);
    this->check_if_open ("namespace_dir_cpi_impl<Base>::sync_make_dir", data->location_);

    // verify current working directory is local
    saga::url dir_url(data->location_);
    if (!::detail::file_islocal(url))
    {
        SAGA_OSSTREAM strm;
        strm << "namespace_dir_cpi_impl<Base>::sync_make_dir: "
            "cannot handle remote current directory: " << url.get_url();
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
    }

#if BOOST_FILESYSTEM_VERSION == 3
        fs::path src_location (saga::url::unescape(::detail::get_filepath(data->location_)));
#else
        fs::path src_location (saga::url::unescape(::detail::get_filepath(data->location_)), fs::native);
#endif

    // complete paths
#if BOOST_FILESYSTEM_VERSION == 3
        fs::path src_path  (saga::url::unescape(::detail::get_filepath(url)));
#else
        fs::path src_path  (saga::url::unescape(::detail::get_filepath(url)), fs::native);
#endif

    if ( ! src_path.has_root_path () )
        src_location /= src_path;
    else
        src_location = src_path;

    detail::canonize(src_location);

    try
    {
        saga::detail::remove_trailing_dot(src_location);
        fs::create_directory(src_location);
    }
    catch(fs::filesystem_error const& e)
    {
        SAGA_ADAPTOR_THROW(e.what(), saga::NoSuccess);
    }
}

template <typename Base>
inline void
namespace_dir_cpi_impl<Base>::sync_link(saga::impl::void_t & ret,
    saga::url source, saga::url symlink, int open_mode)
{
  //#if BOOST_VERSION >= 103500
  //We're always using boost::filesystem >= 1.35 since it's part of our externals. 
  //It *would* be nice to have an extra #define that could be querried but commenting
  // this out *shouldn't* cause any problems.
  // verify current working directory is local
    saga::url url;
    {
        instance_data data (this);
        url = data->location_;
    }
    this->check_if_open ("namespace_dir_cpi_impl<Base>::sync_link", url);

    if (!::detail::file_islocal(url))
    {
        SAGA_OSSTREAM strm;
        strm << "namespace_dir_cpi_impl<Base>::sync_link: "
            "cannot handle remote current directory: " << url.get_url();
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
    }

    // verify the names can be handled
    if (!::detail::file_islocal(source))
    {
      SAGA_OSSTREAM strm;
      strm << "namespace_dir_cpi_impl<Base>::sync_link: "
              "cannot handle remote target name for a symlink: "
           << source.get_url();
      SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
    }

    if (!::detail::file_islocal(symlink))
    {
      SAGA_OSSTREAM strm;
      strm << "namespace_dir_cpi_impl<Base>::sync_link: "
              "cannot handle remote destination as symlink name: "
           << symlink.get_url();
      SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
    }

    // handle the files
    namespace fs = boost::filesystem;

    try {
#if BOOST_FILESYSTEM_VERSION == 3
        fs::path src_location (saga::url::unescape(::detail::get_filepath(url)));
#else
        fs::path src_location (saga::url::unescape(::detail::get_filepath(url)), fs::native);
#endif
        fs::path symlink_location (src_location);

        // complete paths
#if BOOST_FILESYSTEM_VERSION == 3
        fs::path src_path  (saga::url::unescape(::detail::get_filepath(source)));
        fs::path symlink_path (saga::url::unescape(::detail::get_filepath(symlink)));
#else
        fs::path src_path  (saga::url::unescape(::detail::get_filepath(source)), fs::native);
        fs::path symlink_path (saga::url::unescape(::detail::get_filepath(symlink)), fs::native);
#endif

        if (!src_path.has_root_path())
            src_location /= src_path;
        else
            src_location = src_path;

        detail::canonize(src_location);

        if (!symlink_path.has_root_path())
            symlink_location /= symlink_path;
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
//#else
//    SAGA_ADAPTOR_THROW("namespace_dir_cpi_impl<Base>::sync_link not yet implemented",
//                       saga::NotImplemented);
//#endif
}

///////////////////////////////////////////////////////////////////////////////
template <typename Base>
inline void
namespace_dir_cpi_impl<Base>::sync_copy_wildcard_helper(saga::url const& location,
    std::string const& source, saga::url const& destination, int open_mode)
{
    namespace fs = boost::filesystem;

    {
        instance_data data(this);
        this->check_if_open ("namespace_dir_cpi_impl<Base>::sync_copy_wildcard_helper", data->location_);
    }

    try {
        // handle the files
#if BOOST_FILESYSTEM_VERSION == 3
        fs::path src_location (saga::url::unescape(::detail::get_filepath(location)));
        fs::path src_path  (saga::url::unescape(::detail::get_filepath(source)));
        fs::path dest_path (saga::url::unescape(::detail::get_filepath(destination)));
#else
        fs::path src_location (saga::url::unescape(::detail::get_filepath(location)), fs::native);
        fs::path src_path  (saga::url::unescape(::detail::get_filepath(source)), fs::native);
        fs::path dest_path (saga::url::unescape(::detail::get_filepath(destination)), fs::native);
#endif

        fs::path dst_location (src_location);

        // complete paths
        
        if ( ! src_path.has_root_path () )
            src_location /= src_path;
        else
            src_location = src_path;

        detail::canonize(src_location);

        if ( ! dest_path.has_root_path () )
            dst_location /= dest_path;
        else
            dst_location = dest_path;

        detail::canonize(dst_location);

        if (fs::is_directory(src_location)) {
            // src location refers to a is a directory
            detail::copy_directory(src_location, dst_location, open_mode);
        }
        else {
            bool is_dst_dir = fs::is_directory(dst_location);
            if (is_dst_dir)
                dst_location /= saga::detail::leaf(src_location);

            // remove the file/directory if it exists and we should overwrite
            if ((open_mode & saga::name_space::Overwrite) && fs::exists(dst_location)) {
                if (is_dst_dir)
                    fs::remove_all(dst_location);
                else
                    fs::remove(dst_location);
            }

            // if destination still exists raise an error
            if (fs::exists(dst_location)) {
                SAGA_OSSTREAM strm;
                if (open_mode & saga::name_space::Overwrite) {
                    strm << "namespace_dir_cpi_impl<Base>::sync_copy_wildcard: "
                            "target file/directory still exists after deleting: " 
                         << destination.get_url();
                    SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::NoSuccess);
                }
                else {
                    strm << "namespace_dir_cpi_impl<Base>::sync_copy_wildcard: "
                            "target file already exists: " << destination.get_url();
                    SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::AlreadyExists);
                }
            }

            fs::copy_file(src_location, dst_location);
        }
    }
    catch (boost::system::system_error const& e) {
        SAGA_ADAPTOR_THROW(
            location.get_string() + ": caught filesystem exception: " + e.what(),
            saga::NoSuccess);
    }
}

template <typename Base>
inline void
namespace_dir_cpi_impl<Base>::sync_copy_wildcard(saga::impl::void_t& ret,
    std::string source, saga::url destination, int open_mode)
{
    namespace fs = boost::filesystem;

    saga::url location;
    {
        instance_data data (this);
        location = data->location_;
    }
    this->check_if_open ("namespace_dir_cpi_impl<Base>::sync_copy_wildcard", location);

    // verify current working directory is local
    if (!::detail::file_islocal(location))
    {
        SAGA_OSSTREAM strm;
        strm << "namespace_dir_cpi_impl<Base>::sync_copy_wildcard: "
            "cannot handle remote current directory: " << location.get_url();
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
    }

    // verify the files can be handled
    if (!::detail::file_islocal(destination))
    {
      SAGA_OSSTREAM strm;
      strm << "namespace_dir_cpi_impl<Base>::sync_copy_wildcard: "
          "cannot handle remote destination file: " << destination.get_url();
      SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
    }

    // loop over all matching source files
    std::string pattern(::detail::regex_from_pattern(source));
    boost::regex rx (pattern);

    // avoid race conditions between exists and the copy operation
    typename mutex_type::scoped_lock lock(this->mtx_);

    try {
        // all wildcards are relative to the current directory
        fs::directory_iterator dir_end;
        for (fs::directory_iterator dir_it(saga::url::unescape(location.get_path())); 
             dir_it != dir_end; ++dir_it)
        {
#if BOOST_FILESYSTEM_VERSION > 2
            std::string p ((*dir_it).path().string());
#else
            std::string p ((*dir_it).string());
#endif
            boost::smatch what;
            if (boost::regex_match(p, what, rx))
                sync_copy_wildcard_helper(location, p, destination, open_mode);
        }
    }
    catch (boost::system::system_error const& e) {
        SAGA_ADAPTOR_THROW(
            location.get_string() + ": caught filesystem exception: " + e.what(),
            saga::NoSuccess);
    }
}

///////////////////////////////////////////////////////////////////////////////
template <typename Base>
inline void
namespace_dir_cpi_impl<Base>::sync_move_wildcard_helper(saga::url const& location,
    std::string const& source, saga::url const& destination, int open_mode)
{
    namespace fs = boost::filesystem;

    {
        instance_data data(this);
        this->check_if_open ("namespace_dir_cpi_impl<Base>::sync_move_wildcard_helper", data->location_);
    }

    try {
    
#if BOOST_FILESYSTEM_VERSION == 3
        fs::path src_location (saga::url::unescape(::detail::get_filepath(location)));
        fs::path src_path  (saga::url::unescape(::detail::get_filepath(source)));
        fs::path dest_path (saga::url::unescape(::detail::get_filepath(destination)));
#else
        fs::path src_location (saga::url::unescape(::detail::get_filepath(location)), fs::native);
        fs::path src_path  (saga::url::unescape(::detail::get_filepath(source)), fs::native);
        fs::path dest_path (saga::url::unescape(::detail::get_filepath(destination)), fs::native);
#endif

        fs::path dst_location (src_location);


        if ( ! src_path.has_root_path () )
            src_location /= src_path;
        else
            src_location = src_path;

        detail::canonize(src_location);

        if ( ! dest_path.has_root_path () )
            dst_location /= dest_path;
        else
            dst_location = dest_path;

        detail::canonize(dst_location);

        bool is_src_dir = fs::is_directory(src_location);
        bool is_dst_dir = fs::is_directory(dst_location);

        if (!is_src_dir && is_dst_dir)
            dst_location /= saga::detail::leaf(src_location);

        if ((open_mode & saga::name_space::Overwrite) && fs::exists(dst_location)) {
            if (is_dst_dir)
                fs::remove_all(dst_location);
            else
                fs::remove(dst_location);
        }

        // if destination still exists raise an error
        if (fs::exists(dst_location)) {
            SAGA_OSSTREAM strm;
            if (open_mode & saga::name_space::Overwrite) {
                strm << "namespace_dir_cpi_impl<Base>::sync_move_wildcard: "
                        "target file/directory still exists after deleting: " 
                     << destination.get_url();
                SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::NoSuccess);
            }
            else {
                strm << "namespace_dir_cpi_impl<Base>::sync_move_wildcard: "
                        "target file already exists: " << destination.get_url();
                SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::AlreadyExists);
            }
        }

        saga::detail::remove_trailing_dot(src_location);
        fs::rename(src_location, dst_location);
    }
    catch (boost::system::system_error const& e) {
        SAGA_ADAPTOR_THROW(
            location.get_string() + ": caught filesystem exception: " + e.what(),
            saga::NoSuccess);
    }
}

template <typename Base>
inline void
namespace_dir_cpi_impl<Base>::sync_move_wildcard(saga::impl::void_t& ret,
    std::string source, saga::url destination, int open_mode)
{
    namespace fs = boost::filesystem;

    saga::url location;
    {
        instance_data data (this);
        location = data->location_;
    }
    this->check_if_open ("namespace_dir_cpi_impl<Base>::sync_move_wildcard", location);

    // verify current working directory is local
    if (!::detail::file_islocal(location))
    {
        SAGA_OSSTREAM strm;
        strm << "namespace_dir_cpi_impl<Base>::sync_move_wildcard: "
            "cannot handle remote current directory: " << location.get_url();
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
    }

    // verify the files can be handled
    if (!::detail::file_islocal(destination))
    {
        SAGA_OSSTREAM strm;
        strm << "namespace_dir_cpi_impl<Base>::sync_copy_wildcard: "
            "cannot handle remote destination file: " << destination.get_url();
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
    }

    // loop over all matching source files
    std::string pattern(::detail::regex_from_pattern(source));
    boost::regex rx (pattern);

    // avoid race conditions between exists and the move operation
    typename mutex_type::scoped_lock lock(this->mtx_);

    try {
        // all wildcards are relative to the current directory
        fs::directory_iterator dir_end;
        for (fs::directory_iterator dir_it(saga::url::unescape(location.get_path())); 
             dir_it != dir_end; ++dir_it)
        {
#if BOOST_FILESYSTEM_VERSION > 2
            std::string p ((*dir_it).path().string());
#else
            std::string p ((*dir_it).string());
#endif
            boost::smatch what;
            if (boost::regex_match(p, what, rx))
                sync_move_wildcard_helper(location, p, destination, open_mode);
        }
    }
    catch (boost::system::system_error const& e) {
        SAGA_ADAPTOR_THROW(
            location.get_string() + ": caught filesystem exception: " + e.what(),
            saga::NoSuccess);
    }
}

///////////////////////////////////////////////////////////////////////////////
template <typename Base>
inline void
namespace_dir_cpi_impl<Base>::sync_remove_wildcard_helper(
    saga::url const& location, std::string const& source, int flags)
{
    namespace fs = boost::filesystem;

    {
        instance_data data(this);
        this->check_if_open ("namespace_dir_cpi_impl<Base>::sync_remove_wildcard_helper", data->location_);
    }

    try {
    
#if BOOST_FILESYSTEM_VERSION == 3
        fs::path src_location (saga::url::unescape(::detail::get_filepath(location)));
        fs::path src_path  (saga::url::unescape(::detail::get_filepath(source)));
#else
        fs::path src_location (saga::url::unescape(::detail::get_filepath(location)), fs::native);
        fs::path src_path  (saga::url::unescape(::detail::get_filepath(source)), fs::native);
#endif
    
        if (!src_path.has_root_path())
            src_location /= src_path;
        else
            src_location = src_path;

        detail::canonize(src_location);

        if (!fs::exists(src_location)) {
            SAGA_ADAPTOR_THROW("directory::remove_wildcard: Can't remove directory: "
              "Does not exist", saga::DoesNotExist);
        }

        if (fs::is_directory(src_location)) {
            if (saga::name_space::Recursive != flags && !fs::is_empty(src_location))
            {
                SAGA_ADAPTOR_THROW("directory::remove_wildcard: Can't remove directory. "
                    "Please use recursive mode!", saga::BadParameter);
            }
            else {
                remove_all(src_location);
            }
        }
        else {
             fs::remove(src_location);
        }
    }
    catch (boost::system::system_error const& e) {
        SAGA_ADAPTOR_THROW(
            location.get_string() + ": caught filesystem exception: " + e.what(),
            saga::NoSuccess);
    }
}

template <typename Base>
inline void
namespace_dir_cpi_impl<Base>::sync_remove_wildcard(saga::impl::void_t& ret,
    std::string source, int flags)
{
    namespace fs = boost::filesystem;

    saga::url location;
    {
        instance_data data (this);
        location = data->location_;
    }
    this->check_if_open ("namespace_dir_cpi_impl<Base>::sync_remove_wildcard", location);

    // verify current working directory is local
    if (!::detail::file_islocal(location))
    {
        SAGA_OSSTREAM strm;
        strm << "namespace_dir_cpi_impl<Base>::sync_remove_wildcard: "
            "cannot handle remote current directory: " << location.get_url();
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
    }

    // loop over all matching files
    std::string pattern(::detail::regex_from_pattern(source));
    boost::regex rx (pattern);

    // avoid race conditions between exists and the remove operation
    typename mutex_type::scoped_lock lock(this->mtx_);

    try {
        // all wildcards are relative to the current directory
        fs::directory_iterator dir_end;
        for (fs::directory_iterator dir_it(saga::url::unescape(location.get_path())); 
             dir_it != dir_end; ++dir_it)
        {
#if BOOST_FILESYSTEM_VERSION > 2
            std::string p ((*dir_it).path().string());
#else
            std::string p ((*dir_it).string());
#endif
            boost::smatch what;
            if (boost::regex_match(p, what, rx))
                sync_remove_wildcard_helper(location, p, flags);
        }
    }
    catch (boost::system::system_error const& e) {
        SAGA_ADAPTOR_THROW(
            location.get_string() + ": caught filesystem exception: " + e.what(),
            saga::NoSuccess);
    }
}

///////////////////////////////////////////////////////////////////////////////
//#if BOOST_VERSION >= 103500
//We're always using boost::filesystem >= 1.35 since it's part of our externals. 
//It *would* be nice to have an extra #define that could be querried but commenting
// this out *shouldn't* cause any problems.
template <typename Base>
inline void
namespace_dir_cpi_impl<Base>::sync_link_wildcard_helper(saga::url const& location,
    std::string const& source, saga::url const& symlink, int open_mode)
{
    {
        instance_data data(this);
        this->check_if_open ("namespace_dir_cpi_impl<Base>::sync_link_wildcard_helper", data->location_);
    }

    namespace fs = boost::filesystem;
    try {
    
#if BOOST_FILESYSTEM_VERSION == 3
        fs::path path (saga::url::unescape(::detail::get_filepath(source)));
        fs::path target  (saga::url::unescape(::detail::get_filepath(symlink)));
#else
        fs::path path (saga::url::unescape(::detail::get_filepath(source)), fs::native);
        fs::path target (saga::url::unescape(::detail::get_filepath(symlink)), fs::native);
#endif
    
        fs::create_symlink(path, target);
    }
    catch (boost::system::system_error const& e) {
        SAGA_ADAPTOR_THROW(
            location.get_string() + ": caught filesystem exception: " + e.what(),
            saga::NoSuccess);
    }
}
//#endif

template <typename Base>
inline void
namespace_dir_cpi_impl<Base>::sync_link_wildcard(saga::impl::void_t& ret,
    std::string source, saga::url symlink, int open_mode)
{
//#if BOOST_VERSION >= 103500
//We're always using boost::filesystem >= 1.35 since it's part of our externals. 
//It *would* be nice to have an extra #define that could be querried but commenting
// this out *shouldn't* cause any problems.
    namespace fs = boost::filesystem;

    saga::url location;
    {
        instance_data data (this);
        location = data->location_;
    }
    this->check_if_open ("namespace_dir_cpi_impl<Base>::sync_link_wildcard", location);

    // verify current working directory is local
    if (!::detail::file_islocal(location))
    {
        SAGA_OSSTREAM strm;
        strm << "namespace_dir_cpi_impl<Base>::sync_link_wildcard: "
            "cannot handle remote current directory: " << location.get_url();
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
    }

    // verify the files can be handled
    if (!::detail::file_islocal(symlink))
    {
        SAGA_OSSTREAM strm;
        strm << "namespace_dir_cpi_impl<Base>::sync_link_wildcard: "
                "cannot handle remote destination as symlink name: "
             << symlink.get_url();
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
    }

    // loop over all matching source files
    std::string pattern(::detail::regex_from_pattern(source));
    boost::regex rx (pattern);

    // avoid race conditions between exists and the link operation
    typename mutex_type::scoped_lock lock(this->mtx_);

    try {
        // all wildcards are relative to the current directory
        fs::directory_iterator dir_end;
        for (fs::directory_iterator dir_it(saga::url::unescape(location.get_path())); 
             dir_it != dir_end; ++dir_it)
        {
#if BOOST_FILESYSTEM_VERSION > 2
            std::string p ((*dir_it).path().string());
#else
            std::string p ((*dir_it).string());
#endif
            boost::smatch what;
            if (boost::regex_match(p, what, rx))
                sync_link_wildcard_helper(location, p, symlink, open_mode);
        }
    }
    catch (boost::system::system_error const& e) {
        SAGA_ADAPTOR_THROW(
            location.get_string() + ": caught filesystem exception: " + e.what(),
            saga::NoSuccess);
    }
//#else
//    SAGA_ADAPTOR_THROW("namespace_dir_cpi_impl<Base>::sync_link_wildcard "
//        "not yet implemented", saga::NotImplemented);
//#endif
}

///////////////////////////////////////////////////////////////////////////////
template <typename Base>
inline void
namespace_dir_cpi_impl<Base>::sync_read_link(saga::url& ret, saga::url source)
{
  namespace fs = boost::filesystem;

  // verify the file can be handled
  if (!::detail::file_islocal(source))
  {
    SAGA_OSSTREAM strm;
    strm << "namespace_dir_cpi_impl<Base>::sync_read_link: "
        "cannot handle remote target file: " << source.get_url();
    SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
  }

  saga::url location;
  {
    instance_data data (this);
    location = data->location_;
  }
  this->check_if_open ("namespace_dir_cpi_impl<Base>::sync_read_link", location);

  // verify current working directory is local
  if (!::detail::file_islocal(location))
  {
    SAGA_OSSTREAM strm;
    strm << "namespace_dir_cpi_impl<Base>::sync_read_link: "
        "cannot handle remote current directory: " << location.get_url();
    SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
  }

  try {
#if BOOST_FILESYSTEM_VERSION == 3
        fs::path name (saga::url::unescape(::detail::get_filepath(source)));
        fs::path path  (saga::url::unescape(::detail::get_filepath(location)));
#else
        fs::path name (saga::url::unescape(::detail::get_filepath(source)), fs::native);
        fs::path path (saga::url::unescape(::detail::get_filepath(location)), fs::native);
#endif

      if ( ! name.has_root_path () )
        path /= name;
      else
        path = name;

      detail::canonize(path);

#if defined(linux) || defined(__linux) || defined(__linux__) || defined(SAGA_APPLE)
        {
            char* buf = (char*) calloc (sizeof (char), MAX_PATH + 1);
            readlink (path.string ().c_str (), buf, MAX_PATH);
            ret = std::string (buf);
            free (buf);
        }
#else
        SAGA_ADAPTOR_THROW("namespace_dir_cpi_impl<Base>::sync_read_link not yet implemented",
                           saga::NotImplemented);
#endif
    }
    catch (boost::system::system_error const& e) {
        SAGA_ADAPTOR_THROW(
            location.get_string() + ": caught filesystem exception: " + e.what(),
            saga::NoSuccess);
    }
}

///////////////////////////////////////////////////////////////////////////////
template <typename Base>
inline void
namespace_dir_cpi_impl<Base>::sync_get_num_entries(std::size_t& ret)
{
  {
    instance_data data(this);
    this->check_if_open ("namespace_dir_cpi_impl<Base>::sync_get_num_entries", data->location_);
  }

  entries_.clear();
  sync_list(entries_, "*", saga::name_space::None);
  ret = entries_.size();
}

template <typename Base>
inline void
namespace_dir_cpi_impl<Base>::sync_get_entry(saga::url& ret, std::size_t entry)
{
  {
    instance_data data(this);
    this->check_if_open ("namespace_dir_cpi_impl<Base>::sync_get_entry", data->location_);
  }

  if (entry >= entries_.size())
  {
    SAGA_ADAPTOR_THROW("namespace_dir_cpi_impl<Base>::sync_get_entry: "
        "reqested entry does not exist.", saga::BadParameter);
  }

  ret = entries_[entry];
}

///////////////////////////////////////////////////////////////////////////////
template <typename Base>
inline void 
namespace_dir_cpi_impl<Base>::sync_open (
    saga::name_space::entry& entry, saga::url name_to_open, int openmode)
{
  namespace fs = boost::filesystem;

  instance_data data (this);
  this->check_if_open ("dir_cpi_impl::sync_open", data->location_);

  // verify the file can be handled
  if (!::detail::file_islocal(name_to_open))
  {
    SAGA_OSSTREAM strm;
    strm << "dir_cpi_impl::sync_open: "
      "cannot handle remote target directory: " << name_to_open.get_url();
    SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
  }

  saga::url file_url(data->location_);

  if (!::detail::file_islocal(file_url))
  {
    SAGA_OSSTREAM strm;
    strm << "dir_cpi_impl::sync_open: "
      "cannot handle remote current directory: " << file_url.get_url();
    SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
  }

  try {
  
#if BOOST_FILESYSTEM_VERSION == 3
      fs::path name (saga::url::unescape(::detail::get_filepath(name_to_open)));
      fs::path path (saga::url::unescape(file_url.get_path()));
#else
      fs::path name (saga::url::unescape(::detail::get_filepath(name_to_open)), fs::native);
      fs::path path (saga::url::unescape(file_url.get_path()), fs::native);
#endif

      if ( ! name.has_root_path () ) {
        path /= name;
        file_url.set_path(path.string());
      }
      else {
        path = name;
        file_url = saga::url(name.string());
      }
      
      if ( fs::exists(path) && fs::is_directory (path) ) {
        SAGA_ADAPTOR_THROW(path.string () + ": doesn't refer to a file object",
          saga::DoesNotExist);
      }

      // is_entry # FIXME: what? (also below...) -- AM 
      entry = saga::filesystem::file (this->get_proxy()->get_session(), file_url.get_url(), openmode);
    }
    catch (boost::system::system_error const& e) {
        SAGA_ADAPTOR_THROW(
            file_url.get_string() + ": caught filesystem exception: " + e.what(),
            saga::NoSuccess);
    }
}

///////////////////////////////////////////////////////////////////////////////
template <typename Base>
inline void 
namespace_dir_cpi_impl<Base>::sync_open_dir(saga::name_space::directory & entry, 
                                 saga::url       name_to_open, 
                                 int               openmode)
{
  namespace fs = boost::filesystem;

  instance_data data (this);
  this->check_if_open ("dir_cpi_impl::sync_open_dir", data->location_);

  // verify the file can be handled
  if (!::detail::file_islocal(name_to_open))
  {
    SAGA_OSSTREAM strm;
    strm << "dir_cpi_impl::sync_open_dir: "
      "cannot handle remote target directory: " << name_to_open.get_url();
    SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
  }

  saga::url file_url(data->location_);

  if (!::detail::file_islocal(file_url))
  {
    SAGA_OSSTREAM strm;
    strm << "dir_cpi_impl::sync_open_dir: "
      "cannot handle remote current directory: " << file_url.get_url();
    SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
  }

  try {
  
#if BOOST_FILESYSTEM_VERSION == 3
      fs::path name (saga::url::unescape(::detail::get_filepath(name_to_open)));
      fs::path path (saga::url::unescape(file_url.get_path()));
#else
      fs::path name (saga::url::unescape(::detail::get_filepath(name_to_open)), fs::native);
      fs::path path (saga::url::unescape(file_url.get_path()), fs::native);
#endif


      if ( ! name.has_root_path () ) {
        path /= name;
        file_url.set_path(path.string());
      }
      else {
        path = name;
        file_url = saga::url(name.string());
      }

      if ( fs::exists(path) && !fs::is_directory(path)) {
        SAGA_ADAPTOR_THROW(path.string () + ": doesn't refer to a file object",
          saga::DoesNotExist);
      }

      entry = saga::filesystem::directory (this->get_proxy()->get_session(), file_url.get_url(), 
                               openmode);
  }
  catch (boost::system::system_error const& e) {
      SAGA_ADAPTOR_THROW(
          file_url.get_string() + ": caught filesystem exception: " + e.what(),
          saga::NoSuccess);
  }
}

#if ! defined (SAGA_DEFAULT_FILE_ADAPTOR_NO_ASYNCS)
///////////////////////////////////////////////////////////////////////////////
// This adaptor implements the async functions based on its own synchronous
// functions for testing purposes only. SInce there is no principal need
// to do so, we allow these to be 'switched off'.

template <typename Base>
inline saga::task
namespace_dir_cpi_impl<Base>::async_exists (saga::url url)
{
    return saga::adaptors::task("namespace_dir_cpi::sync_exists",
        this->shared_from_this(), &namespace_dir_cpi_impl<Base>::sync_exists,
        url);
}

template <typename Base>
inline saga::task
namespace_dir_cpi_impl<Base>::async_is_dir (saga::url url)
{
    return saga::adaptors::task("namespace_dir_cpi::sync_is_dir",
        this->shared_from_this(), &namespace_dir_cpi_impl<Base>::sync_is_dir,
        url);
}

template <typename Base>
inline saga::task
namespace_dir_cpi_impl<Base>::async_is_entry (saga::url url)
{
    return saga::adaptors::task("namespace_dir_cpi::sync_is_entry",
        this->shared_from_this(), &namespace_dir_cpi_impl<Base>::sync_is_entry,
        url);
}

template <typename Base>
inline saga::task
namespace_dir_cpi_impl<Base>::async_is_link (saga::url url)
{
    return saga::adaptors::task("namespace_dir_cpi::sync_is_link",
        this->shared_from_this(), &namespace_dir_cpi_impl<Base>::sync_is_link,
        url);
}

template <typename Base>
inline saga::task
namespace_dir_cpi_impl<Base>::async_change_dir (saga::url new_dir)
{
    return saga::adaptors::task("namespace_dir_cpi::sync_change_dir",
        this->shared_from_this(), &namespace_dir_cpi_impl<Base>::sync_change_dir,
        new_dir);
}

template <typename Base>
inline saga::task
namespace_dir_cpi_impl<Base>::async_copy(saga::url source,
    saga::url destination, int flags)
{
    return saga::adaptors::task("namespace_dir_cpi::copy",
                                 this->shared_from_this(),
                                 &namespace_dir_cpi_impl<Base>::sync_copy,
                                 source,
                                 destination,
                                 flags
#if ! defined (SAGA_DEFAULT_FILE_ADAPTOR_NO_BULKS)
                               , &namespace_dir_cpi_impl<Base>::prepare_copy
#endif
                             );
}

template <typename Base>
inline saga::task
namespace_dir_cpi_impl<Base>::async_move(saga::url source,
    saga::url destination, int flags)
{
    return saga::adaptors::task("namespace_dir_cpi::move",
                                 this->shared_from_this(),
                                 &namespace_dir_cpi_impl<Base>::sync_move,
                                 source,
                                 destination,
                                 flags
#if ! defined (SAGA_DEFAULT_FILE_ADAPTOR_NO_BULKS)
                               , &namespace_dir_cpi_impl<Base>::prepare_move
#endif
                            );
}

template <typename Base>
inline saga::task
namespace_dir_cpi_impl<Base>::async_remove(saga::url url, int flags)
{
    return saga::adaptors::task("namespace_dir_cpi::remove",
                                 this->shared_from_this(),
                                 &namespace_dir_cpi_impl<Base>::sync_remove,
                                 url,
                                 flags
#if ! defined (SAGA_DEFAULT_FILE_ADAPTOR_NO_BULKS)
                               , &namespace_dir_cpi_impl<Base>::prepare_remove
#endif
                            );
}

template <typename Base>
inline saga::task
namespace_dir_cpi_impl<Base>::async_make_dir(saga::url url, int open_mode)
{
    return saga::adaptors::task("namespace_dir_cpi::make_dir",
        this->shared_from_this(), &namespace_dir_cpi_impl<Base>::sync_make_dir,
        url, open_mode);
}

template <typename Base>
inline saga::task
namespace_dir_cpi_impl<Base>::async_link(saga::url source, saga::url url,
    int open_mode)
{
    return saga::adaptors::task("namespace_dir_cpi::link",
        this->shared_from_this(), &namespace_dir_cpi_impl<Base>::sync_link,
        source, url, open_mode);
}

template <typename Base>
inline saga::task
namespace_dir_cpi_impl<Base>::async_read_link(saga::url source)
{
    return saga::adaptors::task("namespace_dir_cpi::read_link",
        this->shared_from_this(), &namespace_dir_cpi_impl<Base>::sync_read_link,
        source);
}

template <typename Base>
inline saga::task
namespace_dir_cpi_impl<Base>::async_get_num_entries()
{
    return saga::adaptors::task("namespace_dir_cpi::get_num_entries",
        this->shared_from_this(), &namespace_dir_cpi_impl<Base>::sync_get_num_entries);
}

template <typename Base>
inline saga::task
namespace_dir_cpi_impl<Base>::async_get_entry(std::size_t entry)
{
    return saga::adaptors::task("namespace_dir_cpi::get_entry",
        this->shared_from_this(), &namespace_dir_cpi_impl<Base>::sync_get_entry,
        entry);
}

template <typename Base>
inline saga::task
namespace_dir_cpi_impl<Base>::async_list(std::string pattern, int flags)
{
    return saga::adaptors::task("namespace_dir_cpi::list",
        this->shared_from_this(), &namespace_dir_cpi_impl<Base>::sync_list,
        pattern, flags);
}

template <typename Base>
inline saga::task
namespace_dir_cpi_impl<Base>::async_find(std::string entry, int flags)
{
    return saga::adaptors::task("namespace_dir_cpi::find",
        this->shared_from_this(), &namespace_dir_cpi_impl<Base>::sync_find,
        entry, flags);
}

template <typename Base>
inline saga::task 
namespace_dir_cpi_impl<Base>::async_open (saga::url name, int openmode)
{
    return saga::adaptors::task("namespace_dir_cpi::open",
        this->shared_from_this(), &namespace_dir_cpi_impl<Base>::sync_open,
        name, openmode);
}

template <typename Base>
inline saga::task 
namespace_dir_cpi_impl<Base>::async_open_dir (saga::url name, int openmode)
{
    return saga::adaptors::task("namespace_dir_cpi::open_dir",
        this->shared_from_this(), &namespace_dir_cpi_impl<Base>::sync_open_dir,
        name, openmode);
}
///////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// bulk ops
#if ! defined (SAGA_DEFAULT_FILE_ADAPTOR_NO_BULKS)
// This adaptor implements the bulk functions for testing purposes only. Since
// there is no principal need to do so, we allow these to be 'switched off'.
template <typename Base>
inline bool
namespace_dir_cpi_impl<Base>::prepare_copy(saga::impl::void_t & ret,
                                           saga::url source,
                                           saga::url destination,
                                           int open_mode,
                                           saga::uuid task_uuid/*=NULL*/)
{
    SAGA_LOG(SAGA_VERBOSE_LEVEL_BLURB) 
        << "Reached bulk_prepare for ns_dir::copy";

    create_bulk_container_or_add_params(this, this->bulk_,
        &namespace_dir_cpi_impl<Base>::bulk_copy,
        TR1::ref (ret), source, destination, open_mode, task_uuid );
    return true;
}

template <typename Base>
inline std::vector<saga::uuid>
namespace_dir_cpi_impl<Base>::bulk_copy(
                      std::vector<TR1::reference_wrapper <saga::impl::void_t> >& rets,
                      std::vector<saga::url>& sources,
                      std::vector<saga::url>& dests,
                      std::vector<int>& modes, std::vector<saga::uuid>& uuids)
{
    SAGA_LOG(SAGA_VERBOSE_LEVEL_BLURB) 
        << "Reached bulk_copy for ns_dir::copy";

    std::vector<TR1::reference_wrapper <saga::impl::void_t> >::iterator ret_it = rets.begin();
    std::vector<saga::url>::iterator src_it    = sources.begin();
    std::vector<saga::url>::iterator dest_it   = dests.begin();
    std::vector<int>::iterator mod_it    = modes.begin();
    std::vector<saga::uuid>::iterator  id_it     = uuids.begin();
    saga::impl::void_t r;

    std::vector<saga::uuid> ret;

    while ( ret_it  != rets.end()  && src_it != sources.end() &&
            dest_it != dests.end() && mod_it != modes.end() && id_it != uuids.end() )
    {
        saga::url src = *src_it;
        saga::url dst = *dest_it;
        int m   = *mod_it;
        this->sync_copy(r, src, dst, m);

        ret.push_back(*id_it);

        ++ret_it;
        ++src_it;
        ++dest_it;
        ++mod_it;
        ++id_it;
    }
    return ret;
}

template <typename Base>
inline bool
namespace_dir_cpi_impl<Base>::prepare_move(saga::impl::void_t & ret,
                                           saga::url source,
                                           saga::url destination,
                                           int open_mode,
                                           saga::uuid task_uuid/*=NULL*/)
{
    SAGA_LOG(SAGA_VERBOSE_LEVEL_BLURB) 
        << "Reached bulk_prepare for ns_dir::move";

    create_bulk_container_or_add_params(this, this->bulk_,
        &namespace_dir_cpi_impl<Base>::bulk_move,
        TR1::ref (ret), source, destination, open_mode, task_uuid );
    return true;
}


template <typename Base>
inline std::vector<saga::uuid>
namespace_dir_cpi_impl<Base>::bulk_move(
                      std::vector<TR1::reference_wrapper <saga::impl::void_t> >& rets,
                      std::vector<saga::url>& sources,
                      std::vector<saga::url>& dests,
                      std::vector<int>& modes, std::vector<saga::uuid>& uuids)
{
    SAGA_LOG(SAGA_VERBOSE_LEVEL_BLURB) 
        << "Reached bulk_copy for ns_dir::move";

    std::vector<TR1::reference_wrapper <saga::impl::void_t> >::iterator ret_it = rets.begin();
    std::vector<saga::url>::iterator src_it    = sources.begin();
    std::vector<saga::url>::iterator dest_it   = dests.begin();
    std::vector<int>::iterator mod_it            = modes.begin();
    std::vector<saga::uuid>::iterator  id_it     = uuids.begin();
    saga::impl::void_t r;

    std::vector<saga::uuid> ret;

    while ( ret_it  != rets.end()  && src_it != sources.end() &&
            dest_it != dests.end() && mod_it != modes.end() && id_it != uuids.end() )
    {
        saga::url src = (*src_it);
        saga::url dst = (*dest_it);
        int         m   = (*mod_it);
        this->sync_move(r, src, dst, m);

        ret.push_back(*id_it);

        ++ret_it;
        ++src_it;
        ++dest_it;
        ++mod_it;
        ++id_it;
    }
    return ret;
}


template <typename Base>
inline bool
namespace_dir_cpi_impl<Base>::prepare_remove(saga::impl::void_t & ret, saga::url url,
                                             int open_mode,
                                             saga::uuid task_uuid/*=NULL*/)
{
    SAGA_LOG(SAGA_VERBOSE_LEVEL_BLURB) 
        << "Reached bulk_prepare for ns_dir::remove";

    create_bulk_container_or_add_params(this, this->bulk_,
        &namespace_dir_cpi_impl<Base>::bulk_remove, TR1::ref (ret),
        url, open_mode, task_uuid );
    return true;
}

template <typename Base>
inline std::vector<saga::uuid>
namespace_dir_cpi_impl<Base>::bulk_remove(
                    std::vector<TR1::reference_wrapper <saga::impl::void_t> >& rets,
                    std::vector<saga::url>& urls,
                    std::vector<int>& modes,
                    std::vector<saga::uuid>& uuids)
{
    SAGA_LOG(SAGA_VERBOSE_LEVEL_BLURB) 
        << "Reached bulk_remove for ns_dir::remove";

    std::vector<TR1::reference_wrapper <saga::impl::void_t> >::iterator ret_it = rets.begin();
    std::vector<saga::url>::iterator url_it    = urls.begin();
    std::vector<int>::iterator         mod_it    = modes.begin();
    std::vector<saga::uuid>::iterator  id_it     = uuids.begin();

    saga::impl::void_t r;

    std::vector<saga::uuid> ret;

    while ( ret_it  != rets.end()  && url_it != urls.end() &&
            mod_it  != modes.end() && id_it != uuids.end() )
    {
        saga::url url = (*url_it);
        int         m   = (*mod_it);
        this->sync_remove(r, url,  m);

        ret.push_back(*id_it);

        ++ret_it;
        ++url_it;
        ++mod_it;
        ++id_it;
    }

   return ret;
}

#endif // ! SAGA_DEFAULT_FILE_ADAPTOR_NO_BULKS
#endif // ! SAGA_DEFAULT_FILE_ADAPTOR_NO_ASYNCS

#endif // !ADAPTORS_DEFAULT_FILE_DEFAULT_NAME_SPACE_DIR_IMPL_HPP
