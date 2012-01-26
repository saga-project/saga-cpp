//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005 Michel Zandstra [michelzandstra@gmail.com]
// 
//  Use, modification and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_SAGA_ADAPTORS_STREAMSERVICE_CPI_ENTRY_INSTANCE_DATA_HPP
#define SAGA_SAGA_ADAPTORS_STREAMSERVICE_CPI_ENTRY_INSTANCE_DATA_HPP

#include <saga/saga/packages/stream/stream_server.hpp>
#include <saga/saga/adaptors/config.hpp>
#include <saga/saga/types.hpp>
#include <saga/saga/adaptors/instance_data_base.hpp>
#include <saga/saga/url.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace adaptors { namespace v1_0  {

  ///////////////////////////////////////////////////////////////////////////
  //  Instance data held by the SAGA engine/package
  //  This should be used as a base class when adding additional instance 
  //  data for an adaptor.
  struct stream_server_cpi_instance_data 
      : public adaptors::instance_data_base
  {
    saga::url location_; // location of the server endpoint

    stream_server_cpi_instance_data (saga::url location)
        : location_ (location)
    {
    }
  };
  
///////////////////////////////////////////////////////////////////////////////
}}}   // namespace saga::adaptors::v1_0

#endif // SAGA_SAGA_ADAPTORS_STREAMSERVICE_CPI_ENTRY_INSTANCE_DATA_HPP

