//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005 Michel Zandstra [michelzandstra@gmail.com]
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 
#include <map>
#include <vector>
#include <cstdlib>

#include <boost/config.hpp>
#include <boost/assert.hpp>

#if defined(BOOST_HAS_UNISTD_H)
#include <unistd.h>
#endif

#include <saga/saga/util.hpp>
#include <saga/saga/exception.hpp>
#include <saga/saga/url.hpp>
#include <saga/saga/adaptors/task.hpp>

#include <saga/impl/config.hpp>

#include "default_file.hpp"
#include "default_namespace_entry_impl.hpp"
#include "common_helpers.hpp"

#ifndef  MAX_PATH
# define MAX_PATH _POSIX_PATH_MAX
#endif

/*
 * constructor
 */
file_cpi_impl::file_cpi_impl (proxy* p, cpi_info const& info, 
        saga::ini::ini const& glob_ini, saga::ini::ini const& adap_ini, 
        TR1::shared_ptr<saga::adaptor> adaptor)
:   base_type (p, info, glob_ini, adap_ini, adaptor, cpi::Noflags) 
{
  // gets properly initialized
  mutex_type::scoped_lock lock(mtx_);
  if (file_.is_open ()) 
      return;   // this file has been opened already

  // first usage of this adaptor
  instance_data data (this);
  saga::url file_url(data->location_);
  std::string url = saga::url::unescape(file_url.get_path());

  if ( ! ::detail::file_islocal (file_url) )
  {
    SAGA_OSSTREAM strm;
    strm << "Cannot handle remote file: " << data->location_ ;
    SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
  }

  saga::filesystem::flags mode = (saga::filesystem::flags)data->mode_;

  namespace fs = boost::filesystem;
  try {
#if BOOST_FILESYSTEM_VERSION == 3
    fs::path fpath (url);
#else
    fs::path fpath (url, fs::native);
#endif    
    if ( (mode & saga::filesystem::Create) && (mode & saga::filesystem::Exclusive) ) 
    {

      if ( fs::exists (fpath) ) 
      {
        SAGA_OSSTREAM strm;
        strm << "File already exists: " << data->location_;
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::AlreadyExists);
      }
    }

    // Create file if saga::filesystem::Create flag is given and file does not exist
    if ( mode & saga::filesystem::Create && !fs::exists(fpath) )
    {
      file_.open (url.c_str (), std::ios::out);
      if (!file_.is_open())
      {
        SAGA_OSSTREAM strm;
        strm << "Could not create file: " << data->location_;
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::DoesNotExist);
      }
      file_.close();

      data->mode_ &= ~saga::filesystem::Create;   // no need to create this file twice
    }

    file_.open (url.c_str (), ::detail::translate_open_mode (mode));
    if ( ! file_.is_open () )
    { 
      SAGA_OSSTREAM strm;
      strm << "Could not open file: " << data->location_;
      SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::DoesNotExist);
    }
    
    // position the pointer to the given position if appropriate
    if (0 != data->pointer_) 
    {
      file_.seekg (data->pointer_, std::ios::beg);
    }
    else if (mode & saga::filesystem::Append)
    {
    // position the file pointer to the end, if the file was opened in Append mode
      file_.seekg (0, std::ios::end);
      data->pointer_ = file_.tellg();
    }
  }
  catch (boost::system::system_error const& e) {
    SAGA_ADAPTOR_THROW(
      file_url.get_string() + ": caught filesystem exception: " + e.what(),
      saga::NoSuccess);
  }
}

/*
 * destructor
 */
file_cpi_impl::~file_cpi_impl (void)
{
    try {
        mutex_type::scoped_lock lock(mtx_);
        if (file_.is_open())
            file_.close();
    }
    catch (...) {
        ;
    }
}

/*
 * SAGA API functions
 */
void file_cpi_impl::sync_get_size (saga::off_t& size_out)
{
    {
        instance_data data(this);
        saga::url file_url(data->location_);
        this->check_if_open ("file_cpi_impl::sync_get_size", file_url);
        if (!::detail::file_islocal(file_url))
        {
            SAGA_OSSTREAM strm;
            strm << "file_cpi_impl::sync_get_size: "
                "cannot handle remote current file: " << file_url.get_url();
            SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
        }
        if (!file_.is_open())
        {
            SAGA_OSSTREAM strm;
            strm << "file_cpi_impl::sync_get_size: "
                "cannot handle remote file: " << file_url.get_url();
            SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
        }
    }

    // position to the end of the file
    {
        mutex_type::scoped_lock lock(mtx_);
        if (file_.eof())
            file_.clear(std::ios::eofbit);
        file_.seekg (0, std::ios::end);
        size_out = file_.tellg ();      // figure out the file size
    }

    // reposition file pointer
    instance_data data (this);
    file_.seekg (data->pointer_, std::ios::beg);
}

void file_cpi_impl::sync_close (saga::impl::void_t & ret, double)
{

    {
        instance_data data(this);
        saga::url file_url(data->location_);
        if (!::detail::file_islocal(file_url))
        {
            SAGA_OSSTREAM strm;
            strm << "file_cpi_impl::sync_close: "
                "cannot handle remote current file: " << file_url.get_url();
            SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
        }
    }

    mutex_type::scoped_lock lock(mtx_);

    if (file_.is_open()) 
      file_.close();

    is_open_ = false;

}

void file_cpi_impl::sync_move (saga::impl::void_t & ret, saga::url dest, int flags)
{
    {
        instance_data data (this);
        this->check_if_open ("file_cpi_impl::sync_move", data->location_);
    }

    // first make sure this file is closed 
    {
        mutex_type::scoped_lock lock(mtx_);
        if (file_.is_open())
            file_.close();
    }

    // move the file
    base_type::sync_move(ret, dest, flags);

    // reopen the moved file
    {
        mutex_type::scoped_lock lock(mtx_);
        instance_data data (this);
        this->check_if_open ("file_cpi_impl::sync_move", data->location_);

        std::string url = saga::url::unescape(dest.get_path());
        file_.open (url.c_str(), ::detail::translate_open_mode (data->mode_));
        if (!file_.is_open())
        { 
            SAGA_OSSTREAM strm;
            strm << "Could not open file: " << dest;
            SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
        }
        file_.seekg (data->pointer_, std::ios::beg);

        data->location_ = dest;
    }
}

void file_cpi_impl::sync_remove (saga::impl::void_t & ret, int flags)
{

    {
        instance_data data(this);
        saga::url file_url(data->location_);
        this->check_if_open ("file_cpi_impl::sync_remove", file_url);
        if (!::detail::file_islocal(file_url))
        {
            SAGA_OSSTREAM strm;
            strm << "file_cpi_impl::sync_remove: "
                "cannot handle remote current file: " << file_url.get_url();
            SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
        }
    }

    // first make sure this file is closed 
    {
        mutex_type::scoped_lock lock(mtx_);
        if (file_.is_open())
            file_.close();
    }
    
    // remove the file
    base_type::sync_remove(ret, flags);
}

void file_cpi_impl::sync_read (saga::ssize_t & len_out, 
    saga::mutable_buffer data, saga::ssize_t len_in)
{

    {
        instance_data data(this);
        saga::url file_url(data->location_);
        this->check_if_open ("file_cpi_impl::sync_read", file_url);
        if (!::detail::file_islocal(file_url))
        {
            SAGA_OSSTREAM strm;
            strm << "file_cpi_impl::sync_read: "
                "cannot handle remote current file: " << file_url.get_url();
            SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
        }
    }

    // validate parameters
    if (len_in < 0)
    {
        SAGA_ADAPTOR_THROW(
            "file_cpi_impl::sync_read: 'len_in' should not be negative", 
            saga::BadParameter);
    }
    if (data.get_size() != -1 && len_in > data.get_size())
    {
        SAGA_ADAPTOR_THROW(
            "file_cpi_impl::sync_read: buffer too small for data to read", 
            saga::BadParameter);
    }

    // calculate length of rest of the file
    mutex_type::scoped_lock lock(mtx_);

    file_.seekg (0, std::ios::end);

    std::fstream::pos_type length = file_.tellg ();
    {
        instance_data data (this);
        file_.seekg (data->pointer_, std::ios::beg);
    }

    length -= file_.tellg ();
    if (0 == len_in)
        len_in = length;

    // initialize buffer
    if (-1 == data.get_size())
    {
        // use implementation managed memory
        data.set_size(len_in);    
    }

    // set length to read 
    if ( (saga::ssize_t) length < data.get_size() )
    {
        len_out = length;
    }
    else
    {
        len_out = data.get_size();
    }

    // ensure, that the stream operates on bytes
    BOOST_ASSERT (sizeof (std::fstream::char_type) == sizeof (saga::char_t));

    /* read data */
    file_.read ((char *)data.get_data(), (std::streamsize)data.get_size());
    if (file_.rdstate () & (std::ios::badbit | std::ios::failbit | std::ios::eofbit))
    {
      // wasn't able to read the required amount of data
      if (file_.rdstate () & std::ios::eofbit)
      {
        len_out = file_.gcount ();
      }
      else
      {
        SAGA_OSSTREAM strm;
        instance_data data (this);
        strm << "file_cpi_impl::read: could not read from file: " 
             << data->location_.get_url();

        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), 
            saga::NoSuccess);
      }
    }

    /* set buffer and file pointer */
    {
      instance_data data (this);
      data->pointer_ += (saga::off_t) len_out;
    }
}

void file_cpi_impl::sync_write (saga::ssize_t& len_out, 
    saga::const_buffer data, saga::ssize_t len_in)
{
    {
        instance_data data(this);
        saga::url file_url(data->location_);
        this->check_if_open ("file_cpi_impl::sync_write", file_url);
        if (!::detail::file_islocal(file_url))
        {
            SAGA_OSSTREAM strm;
            strm << "file_cpi_impl::sync_write: "
                "cannot handle remote current file: " << file_url.get_url();
            SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
        }
    }

    // validate parameters
    if (len_in < 0)
    {
        SAGA_ADAPTOR_THROW(
            "file_cpi_impl::sync_write: 'len_in' should not be negative", 
            saga::BadParameter);
    }

    // write data 
    mutex_type::scoped_lock lock(mtx_);

    {
      instance_data data (this);
      file_.seekp (data->pointer_, std::ios::beg);
    }

    // ensure, that the stream operates on bytes
    BOOST_ASSERT (sizeof (std::fstream::char_type) == sizeof (saga::char_t));

    if (0 == len_in)
        len_in = data.get_size();

    file_.write ((char const*)data.get_data(), (std::streamsize)len_in);

    if ( file_.rdstate () & (std::ios::badbit | std::ios::failbit) )
    {
      // wasn't able to write the required amount of data
      SAGA_OSSTREAM strm;
      instance_data data (this);
      strm << "file_cpi_impl::write: could not write to file: " 
           << data->location_.get_url();

      SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), 
          saga::NoSuccess);
    }

    // always make sure things are flushed to the file
    file_.flush();

    {
      instance_data instdata (this);

      len_out = len_in;
      instdata->pointer_ += (saga::off_t)len_out;
    }
}

void file_cpi_impl::sync_seek (saga::off_t & out, 
                               saga::off_t   offset, 
                               saga::filesystem::seek_mode whence)
{
    {
        instance_data data(this);
        saga::url file_url(data->location_);
        this->check_if_open ("file_cpi_impl::sync_seek", file_url);
        if (!::detail::file_islocal(file_url))
        {
            SAGA_OSSTREAM strm;
            strm << "file_cpi_impl::sync_seek: "
                "cannot handle remote current file: " << file_url.get_url();
            SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
        }
    }

    /* set position of get pointer */
    mutex_type::scoped_lock lock(mtx_);

    switch ( (saga::filesystem::seek_mode) whence ) 
    {
        case saga::filesystem::Start:   file_.seekg (offset, std::ios::beg); break;
        case saga::filesystem::Current: file_.seekg (offset, std::ios::cur); break;
        case saga::filesystem::End:     file_.seekg (offset, std::ios::end); break;
        default:
        {
            // wasn't able to seek 
            SAGA_OSSTREAM strm;
            strm << "file_cpi_impl::seek: bogus seek mode: " << (int) whence;
            SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), 
              saga::BadParameter);
        }
        break;
    }

    {
        instance_data data (this);
        data->pointer_ = file_.tellg ();
        out = data->pointer_;
    }
}

#if ! defined (SAGA_DEFAULT_FILE_ADAPTOR_NO_ASYNCS)
///////////////////////////////////////////////////////////////////////////////
//  This adaptor implements the async functions based on its own synchronous
//  functions for testing purposes only. Since there is no real need
//  to do so, we allow these to be 'switched off'.

saga::task file_cpi_impl::async_get_size()
{
    return saga::adaptors::task("file_cpi_impl::sync_get_size",
        shared_from_this(), &file_cpi_impl::sync_get_size);
}

saga::task file_cpi_impl::async_close (double timeout)
{
    return saga::adaptors::task("file_cpi_impl::sync_close",
        shared_from_this(), &file_cpi_impl::sync_close, timeout);
}

saga::task file_cpi_impl::async_remove (int flags)
{
    return saga::adaptors::task("file_cpi_impl::sync_remove",
        shared_from_this(), &file_cpi_impl::sync_remove, flags);
}

saga::task 
file_cpi_impl::async_read (saga::mutable_buffer data, saga::ssize_t len_in)
{
    return saga::adaptors::task("file_cpi_impl::sync_read",
        shared_from_this(), &file_cpi_impl::sync_read, data, len_in);
}

saga::task 
file_cpi_impl::async_write (saga::const_buffer data, saga::ssize_t len_in)
{
    return saga::adaptors::task("file_cpi_impl::sync_write",
        shared_from_this(), &file_cpi_impl::sync_write, data, len_in);
}

saga::task file_cpi_impl::async_seek(saga::off_t offset, 
    saga::filesystem::seek_mode whence) 
{
    return saga::adaptors::task("file_cpi_impl::sync_seek",
        shared_from_this(), &file_cpi_impl::sync_seek, offset, whence);
}

#endif // SAGA_DEFAULT_FILE_ADAPTOR_NO_ASYNCS

