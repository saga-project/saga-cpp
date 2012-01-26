//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2006 Stephan Hirmer (shirmer@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ADAPTORS_DEFAULT_FILE_DEFAULT_DIR_HPP
#define ADAPTORS_DEFAULT_FILE_DEFAULT_DIR_HPP

#include <string>

#include <saga/saga/adaptors/adaptor.hpp>
#include <saga/saga/adaptors/instance_data.hpp>
#include <saga/saga/adaptors/packages/directory_cpi_instance_data.hpp>

#include <saga/impl/engine/proxy.hpp>
#include <saga/impl/packages/filesystem/directory_cpi.hpp>

#include "default_namespace_dir.hpp"

///////////////////////////////////////////////////////////////////////////////
class dir_cpi_impl 
    : public namespace_dir_cpi_impl<saga::adaptors::v1_0::directory_cpi<dir_cpi_impl> >
{
  private:
    typedef saga::adaptors::v1_0::directory_cpi<dir_cpi_impl> base_cpi;
    typedef namespace_dir_cpi_impl<base_cpi> base_type;

  public:
    /*! constructor of the directory cpi */
    dir_cpi_impl  (saga::impl::proxy* p, 
                   saga::impl::v1_0::cpi_info const & info,
                   saga::ini::ini const & glob_ini, 
                   saga::ini::ini const & adap_ini,
                   TR1::shared_ptr<saga::adaptor> adaptor);
    
    /*! destructor of the directory cpi */
    ~dir_cpi_impl();

    // CPI functions
    void sync_get_size (saga::off_t&, saga::url name, int flags);
    void sync_open (saga::filesystem::file& entry, saga::url name_to_open, int openmode);
    void sync_open_dir(saga::filesystem::directory & entry, saga::url name_to_open, int openmode);

#if ! defined (SAGA_DEFAULT_FILE_ADAPTOR_NO_ASYNCS)
    // This adaptor implements the async functions based on its own synchronous
    // functions for testing purposes only. SInce there is no principal need
    // to do so, we allow these to be 'switched off'.
    saga::task async_get_size (saga::url name, int flags);
    saga::task async_open(saga::url name, int openmode); 
    saga::task async_open_dir(saga::url name, int openmode); 
#endif

};  // class dir_cpi_impl

#endif // ADAPTORS_DEFAULT_FILE_DEFAULT_DIR_HPP

