//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005 Michel Zandstra (michelzandstra@gmail.com)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ADAPTORS_DEFAULT_FILE_DEFAULT_FILE_HPP
#define ADAPTORS_DEFAULT_FILE_DEFAULT_FILE_HPP

#include <fstream>
#include <string>

#include <saga/saga/util.hpp>
#include <saga/saga/types.hpp>

#include <saga/saga/adaptors/adaptor.hpp>
#include <saga/saga/adaptors/packages/file_cpi_instance_data.hpp>

#include <saga/impl/engine/proxy.hpp>
#include <saga/impl/packages/filesystem/file_cpi.hpp>

#include "default_namespace_entry.hpp"

///////////////////////////////////////////////////////////////////////////////
/**
  * This adaptor implements the functionality of the Saga API "file".
  * It defines the functions declared in its base class, file_cpi.
  *
  * @note This adaptor is written for test purposes only; it is possible that
  * it does not function exactly like the Saga API dictates.
  * @see The documentation of the Saga API 
  */
class file_cpi_impl 
:   public namespace_entry_cpi_impl<saga::adaptors::v1_0::file_cpi<file_cpi_impl> >
{
  private:
    typedef saga::adaptors::v1_0::file_cpi<file_cpi_impl> base_cpi;
    typedef namespace_entry_cpi_impl<base_cpi> base_type;

    std::fstream file_;

  public:    
    /*! constructor of the file adaptor */
    file_cpi_impl (proxy                * p, 
                   cpi_info       const & info,
                   saga::ini::ini const & glob_ini, 
                   saga::ini::ini const & adap_ini,
                   TR1::shared_ptr<saga::adaptor> adaptor);

    /*! destructor of the file adaptor */
    ~file_cpi_impl  (void);

    /*! overload from namespace_entry */
    void sync_move (saga::impl::void_t & ret, saga::url source, int flags);
    void sync_remove (saga::impl::void_t & ret, int flags);
    void sync_close (saga::impl::void_t & ret, double);

    /*! implementation of SAGA::file::get_size */
    void sync_get_size (saga::off_t& size_out);

    /*! implementation of SAGA::file::read */
    void sync_read  (saga::ssize_t & len_out, saga::mutable_buffer data, saga::ssize_t len_in);

    /*! implementation of SAGA::file::write */
    void sync_write (saga::ssize_t& len_out, saga::const_buffer data, saga::ssize_t len_in);

    /*! implementation of SAGA::file::seek */
    void sync_seek  (saga::off_t& out, saga::off_t offset, 
        saga::filesystem::seek_mode whence);

#if ! defined(SAGA_DEFAULT_FILE_ADAPTOR_NO_ASYNCS)
    // This adaptor implements the async functions based on its own synchronous
    // functions for testing purposes only. Since there is no real need
    // to do so, we allow these to be 'switched off'.

    saga::task async_remove (int flags);
    saga::task async_close (double);
    saga::task async_get_size();
    saga::task async_read (saga::mutable_buffer data, saga::ssize_t len_in);
    saga::task async_write (saga::const_buffer data, saga::ssize_t len_in);
    saga::task async_seek(saga::off_t offset, saga::filesystem::seek_mode whence);

#endif // !SAGA_DEFAULT_FILE_ADAPTOR_NO_ASYNCS

};  // class file_cpi_impl

#endif // ADAPTORS_DEFAULT_FILE_DEFAULT_FILE_HPP

