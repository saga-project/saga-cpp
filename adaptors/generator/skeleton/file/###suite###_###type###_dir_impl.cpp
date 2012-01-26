//  Copyright (c) 2008 Andre Merzky <andre@merzky.net>
// 
//  Distributed under the Boost Software License, 
//  Version 1.0. (See accompanying LICENSE file 
//  or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga/exception.hpp>

#include "###suite###_###type###_dir.hpp"
#include "###suite###_###type###_file.hpp"

namespace ###suite###_###type###_adaptor
{

  ///////////////////////////////////////////////////////////////////////////////
  //
  dir_cpi_impl::dir_cpi_impl (proxy                * p, 
                              cpi_info       const & info,
                              saga::ini::ini const & glob_ini,
                              saga::ini::ini const & adap_ini,
                              boost::shared_ptr<saga::adaptor> adaptor)

      : directory_cpi (p, info, adaptor, cpi::Noflags)
  {
    adaptor_data_t            adata (this);
    directory_instance_data_t idata (this);

    SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
  }

  ///////////////////////////////////////////////////////////////////////////////
  //
  dir_cpi_impl::~dir_cpi_impl (void)
  {
  }

//   void dir_cpi_impl::sync_get_size (saga::off_t & size_out, 
//                                     saga::url      name)
//   {
//     SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
//   }
// 
//   ///////////////////////////////////////////////////////////////////////////////
//   //
//   void dir_cpi_impl::sync_open (saga::filesystem::file & ret, 
//                                 saga::url                name, 
//                                 int                      openmode)
//   {
//     SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
//   }
// 
//   ///////////////////////////////////////////////////////////////////////////////
//   //
//   void dir_cpi_impl::sync_open_dir (saga::filesystem::directory & ret, 
//                                     saga::url                     name, 
//                                     int                           openmode)
//   {
//     SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
//   }
// 
//   ///////////////////////////////////////////////////////////////////////////////
//   //
//   void dir_cpi_impl::sync_is_file (bool    & is_file, 
//                                    saga::url name)
//   {
//     SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
//   }

} // namespace

