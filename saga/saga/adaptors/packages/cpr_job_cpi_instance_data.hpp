//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_SAGA_ADAPTORS_CPR_JOB_CPI_INSTANCE_DATA_HPP
#define SAGA_SAGA_ADAPTORS_CPR_JOB_CPI_INSTANCE_DATA_HPP

#include <saga/saga/adaptors/config.hpp>
#include <saga/saga/types.hpp>
#include <saga/saga/url.hpp>
#include <saga/saga/cpr.hpp>

#include <saga/saga/adaptors/packages/job_cpi_instance_data.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace adaptors { namespace v1_0 
{
    ///////////////////////////////////////////////////////////////////////////
    //  Instance data held by the SAGA engine/package
    struct cpr_job_cpi_instance_data 
      : public job_cpi_instance_data
    {
      cpr_job_cpi_instance_data (saga::url rm, 
                                 saga::cpr::description jd_start,
                                 saga::cpr::description jd_restart) 
        : job_cpi_instance_data (rm, jd_start),
          jd_start_   (jd_start),
          jd_restart_ (jd_restart)
      {}

      cpr_job_cpi_instance_data (saga::url rm, 
                                 saga::cpr::description jd)
        : job_cpi_instance_data (rm, jd),
          jd_start_   (jd),
          jd_restart_ (jd)
      {}

      cpr_job_cpi_instance_data (saga::url rm, std::string jobid) 
        : job_cpi_instance_data (rm, jobid)
      {}
        
      saga::cpr::description jd_start_;  // restart job description 
      saga::cpr::description jd_restart_;  // restart job description 
    };
  
///////////////////////////////////////////////////////////////////////////////
}}} // namespace saga::adaptors::v1_0

#endif // SAGA_SAGA_ADAPTORS_CPR_JOB_CPI_INSTANCE_DATA_HPP

