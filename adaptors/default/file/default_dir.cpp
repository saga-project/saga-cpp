//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <map>
#include <vector>

#include <saga/saga/util.hpp>
#include <saga/saga/exception.hpp>
#include <saga/saga/url.hpp>
#include <saga/impl/config.hpp>
#include <saga/saga/path_leaf.hpp>

#include <boost/system/system_error.hpp>
#include <boost/filesystem/convenience.hpp>

#include "default_dir.hpp"
#include "default_file.hpp"

#include <saga/saga/adaptors/task.hpp>

#include "default_namespace_dir.hpp"
#include "default_namespace_dir_impl.hpp"
#include "common_helpers.hpp"

#ifndef  MAX_PATH
# define MAX_PATH _POSIX_PATH_MAX
#endif

/*
 * constructor
 */
dir_cpi_impl::dir_cpi_impl (saga::impl::proxy* p, 
                            saga::impl::v1_0::cpi_info const & info,
                            saga::ini::ini const & glob_ini, 
                            saga::ini::ini const & adap_ini,
                            TR1::shared_ptr<saga::adaptor> adaptor)
    : base_type (p, info, glob_ini, adap_ini, adaptor, cpi::Noflags)
{
    namespace fs = boost::filesystem;

    instance_data data (this);
    saga::url dir_url(data->location_);

    try {
        #if BOOST_FILESYSTEM_VERSION == 3
        fs::path path (saga::url::unescape(dir_url.get_path()));
        #else
        fs::path path (saga::url::unescape(dir_url.get_path()), fs::native);
        #endif
        
        if ( ! ::detail::file_islocal (dir_url) )
        {
            SAGA_OSSTREAM strm;
            strm << "dir_cpi_impl::init: cannot handle file: " << dir_url.get_url();
            SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
        }

        // check if file exists AND is a dir (not a file)
        bool exists  = fs::exists(path);
        bool is_dir  = false;
        if(exists) is_dir = fs::is_directory(path);

        // check for openmode //
        saga::filesystem::flags OpenMode = (saga::filesystem::flags)data->mode_;
        if (exists) {
            if (!is_dir) {
                SAGA_ADAPTOR_THROW ("URL does not point to a directory: " +
                                    data->location_.get_url(), saga::BadParameter);
            }
            else {
                if ((OpenMode & saga::filesystem::Create) && (OpenMode & saga::filesystem::Exclusive))
                {
                    SAGA_ADAPTOR_THROW ("Directory " + data->location_.get_url() +
                                        " already exists.", saga::AlreadyExists); 
                }
            }
        }
        else {  // !exists
            if (!(OpenMode & saga::filesystem::Create)) {
                SAGA_ADAPTOR_THROW ("Directory does not exist and saga::filesystem::Create flag not given: " +
                                    data->location_.get_url(), saga::DoesNotExist);
            }
            else {
                try {
                    saga::detail::remove_trailing_dot(path);
                    if ((OpenMode & saga::filesystem::CreateParents)) {
                       if ( !fs::create_directories(path)) {
                           SAGA_ADAPTOR_THROW(path.string() + 
                               ": couldn't create the directory hierarchy", saga::DoesNotExist);
                       }
                    }
                    else if (!fs::create_directory (path)) {
                        SAGA_ADAPTOR_THROW(path.string() + 
                          ": couldn't create the directory", saga::DoesNotExist);
                    }
                }
                catch (boost::system::system_error const& e) {
                    SAGA_ADAPTOR_THROW(path.string() + 
                        ": couldn't create the directory (" + e.what() + ")", 
                        saga::DoesNotExist);
                }
            }
        }

        // make sure directory exists
        if ((data->mode_ & saga::filesystem::Create ||
             data->mode_ & saga::filesystem::CreateParents) && 
            data->mode_ & saga::filesystem::Exclusive)
        {
          // is_directory
          if (fs::exists(path)) {
            SAGA_ADAPTOR_THROW(path.string () + ": already exists", 
              saga::AlreadyExists);
          }
        }

        if (!fs::exists(path)) 
        {    
            try {
                // create directory, if needed
                saga::detail::remove_trailing_dot(path);
                if (OpenMode & saga::filesystem::CreateParents)
                {
                    if (!fs::create_directories (path)) {
                        SAGA_ADAPTOR_THROW(path.string () + 
                            ": couldn't create the directory hierarchy", saga::DoesNotExist);
                    }
                }
                else if (OpenMode & saga::filesystem::Create) 
                {
                    if (!fs::create_directory (path)) {
                        SAGA_ADAPTOR_THROW(path.string() + 
                            ": couldn't create the directory", saga::DoesNotExist);
                    }
                }
            }
            catch (boost::system::system_error const& e) {
                SAGA_ADAPTOR_THROW(path.string () + 
                  ": couldn't create the directory (" + e.what() + ")", 
                  saga::DoesNotExist);
            }
        }

        // we don't need to create the directory twice
        data->mode_ &= ~(saga::filesystem::Create | saga::filesystem::CreateParents);

        if (data->mode_ & saga::filesystem::Read || data->mode_ & saga::filesystem::Write ||
            data->mode_ & saga::filesystem::ReadWrite)
        {
          if (!fs::exists(path) || !fs::is_directory (path)) {
            SAGA_ADAPTOR_THROW(
              path.string () + ": doesn't refer to a directory object", 
                saga::BadParameter);
          }
        }

        if (data->mode_ & ~(
                saga::filesystem::Create | saga::filesystem::CreateParents |
                saga::filesystem::Exclusive | saga::filesystem::Overwrite |
                saga::filesystem::Read | saga::filesystem::Write | saga::filesystem::ReadWrite
              )) 
        {
          SAGA_ADAPTOR_THROW("Unknown openmode value: " + 
              boost::lexical_cast<std::string>(data->mode_), saga::BadParameter);
        }
    }
    catch (boost::system::system_error const& e) {
        SAGA_ADAPTOR_THROW(
            dir_url.get_string() + ": caught filesystem exception: " + e.what(),
            saga::NoSuccess);
    }
}

/*
 * destructor
 */
dir_cpi_impl::~dir_cpi_impl (void)
{
}

/*
 * SAGA API functions
 */

///////////////////////////////////////////////////////////////////////////////
void dir_cpi_impl::sync_get_size (saga::off_t& size, saga::url name_to_open, int flags)
{
  namespace fs = boost::filesystem;

  saga::url file_url;
  {
    instance_data data (this);
    this->check_if_open ("dir_cpi_impl::sync_get_size", data->location_);

    // verify the file can be handled
    if (!::detail::file_islocal(name_to_open))
    {
      SAGA_OSSTREAM strm;
      strm << "dir_cpi_impl::sync_get_size: "
        "cannot handle remote target directory: " << name_to_open.get_url();
      SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
    }

    file_url = data->location_;
    if (!::detail::file_islocal(file_url))
    {
      SAGA_OSSTREAM strm;
      strm << "dir_cpi_impl::sync_get_size: "
        "cannot handle remote current directory: " << file_url.get_url();
      SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
    }
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

      size = saga::off_t(fs::file_size(path));
    }
    catch (boost::system::system_error const& e) {
        SAGA_ADAPTOR_THROW(
            file_url.get_string() + ": caught filesystem exception: " + e.what(),
            saga::NoSuccess);
    }
}

///////////////////////////////////////////////////////////////////////////////
void dir_cpi_impl::sync_open (saga::filesystem::file  & entry, 
                              saga::url   name_to_open, 
                              int           openmode)
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
void dir_cpi_impl::sync_open_dir(saga::filesystem::directory & entry, 
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

saga::task
dir_cpi_impl::async_get_size (saga::url name, int flags)
{
    return saga::adaptors::task("dir_cpi_impl::sync_get_size", 
        shared_from_this(), &dir_cpi_impl::sync_get_size, name, flags); 
}

saga::task
dir_cpi_impl::async_open (saga::url name, int openmode)
{
    return saga::adaptors::task("dir_cpi_impl::sync_open", 
        shared_from_this(), &dir_cpi_impl::sync_open, name, openmode ); 
}

saga::task 
dir_cpi_impl::async_open_dir(saga::url name, int openmode)
{
    return saga::adaptors::task("dir_cpi_impl::sync_open_dir",
        shared_from_this(), &dir_cpi_impl::sync_open_dir, name, openmode);
}

#endif  // SAGA_DEFAULT_FILE_ADAPTOR_NO_ASYNCS
