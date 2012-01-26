//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_SAGA_ADAPTORS_JOB_CPI_INSTANCE_DATA_HPP
#define SAGA_SAGA_ADAPTORS_JOB_CPI_INSTANCE_DATA_HPP

#include <saga/saga/packages/job/job.hpp>
#include <saga/saga/adaptors/config.hpp>
#include <saga/saga/types.hpp>
#include <saga/saga/url.hpp>
#include <saga/saga/adaptors/instance_data_base.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace adaptors { namespace v1_0 
{
    ///////////////////////////////////////////////////////////////////////////
    //  Instance data held by the SAGA engine/package
    struct job_cpi_instance_data 
        : public adaptors::instance_data_base
    {
        job_cpi_instance_data(saga::url rm, saga::job::description jd) 
        :   init_from_jobid_(false), jd_is_valid_(true), rm_(rm), jd_(jd.clone()) 
        {}
        job_cpi_instance_data(saga::url rm, std::string jobid) 
        :   init_from_jobid_(true), jd_is_valid_(false), rm_(rm), jobid_(jobid)
        {}
        
        bool init_from_jobid_;  // job+description must be initialized from jobid
        bool jd_is_valid_;      // this is true if jd has been initialized
        saga::url rm_;          // resource manager to use
        std::string jobid_;     // jobid from which the job is to be initialized from
        saga::job::description jd_;  // job description 
    };
  
///////////////////////////////////////////////////////////////////////////////
}}} // namespace saga::adaptors::v1_0

#endif // SAGA_SAGA_ADAPTORS_JOB_CPI_INSTANCE_DATA_HPP

