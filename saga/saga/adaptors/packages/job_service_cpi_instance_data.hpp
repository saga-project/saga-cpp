//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_SAGA_ADAPTORS_JOB_SERVICE_CPI_INSTANCE_DATA_HPP
#define SAGA_SAGA_ADAPTORS_JOB_SERVICE_CPI_INSTANCE_DATA_HPP

#include <string>

#include <saga/saga/packages/job/job_service.hpp>
#include <saga/saga/adaptors/config.hpp>
#include <saga/saga/types.hpp>
#include <saga/saga/adaptors/instance_data_base.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace adaptors { namespace v1_0 
{
  ///////////////////////////////////////////////////////////////////////////
  //  Instance data held by the SAGA engine/package
  //  This should be used as a base class when adding additional instance 
  //  data for an adaptor.
  struct job_service_cpi_instance_data 
      : public adaptors::instance_data_base
  {
    saga::url rm_;
    
    job_service_cpi_instance_data (saga::url rm)
        : rm_(rm)
    {
    }
  };
  
///////////////////////////////////////////////////////////////////////////////
}}} // namespace saga::adaptors::v1_0

#endif // SAGA_SAGA_ADAPTORS_JOB_SERVICE_CPI_INSTANCE_DATA_HPP

