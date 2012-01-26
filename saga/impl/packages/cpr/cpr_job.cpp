#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/cpr_job.cpp")
#endif
//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 0, output: null)
#endif

#include <saga/saga/adaptors/task.hpp>
#include <saga/saga/adaptors/metric.hpp>
#include <saga/impl/packages/cpr/cpr_job.hpp>
#include <saga/saga/adaptors/instance_data.hpp>
#include <saga/saga/adaptors/packages/cpr_job_cpi_instance_data.hpp>
#include <saga/saga/adaptors/attribute.hpp>

#ifdef SAGA_DEBUG
#include <saga/impl/packages/data/cpr/preprocessed/cpr_job.cpp>
#else

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, output: "preprocessed/cpr_job.cpp")
#endif
///////////////////////////////////////////////////////////////////////////////
namespace saga 
{ 
  namespace impl 
  {
    cpr_job::cpr_job (saga::url              rm, 
                      saga::cpr::description jd_start, 
                      saga::cpr::description jd_restart, 
                      saga::session const &  s,
                      saga::object::type     t)
      : saga::impl::job (rm, jd_start, s, t)
    {
        // initialize the instance data
        typedef adaptors::v1_0::cpr_job_cpi_instance_data instance_data_type;
        typedef adaptors::instance_data <instance_data_type> cpr_job_instance_data;

        {
            cpr_job_instance_data data;
            data.init_data (this, TR1::shared_ptr <instance_data_type> (
                new instance_data_type(rm, jd_start, jd_restart)));
        } // lock goes out of scope
    }
    
    cpr_job::cpr_job (saga::url              rm, 
                      saga::cpr::description jd, 
                      saga::session const &  s,
                      saga::object::type     t)
      : saga::impl::job (rm, jd, s, t)
    {
        // initialize the instance data
        typedef adaptors::v1_0::cpr_job_cpi_instance_data instance_data_type;
        typedef adaptors::instance_data <instance_data_type> cpr_job_instance_data;

        {
            cpr_job_instance_data data;
            data.init_data (this, TR1::shared_ptr <instance_data_type> (
                new instance_data_type(rm, jd, jd)));
        } // lock goes out of scope
    }
    
    cpr_job::cpr_job (saga::url             rm, 
                      std::string           jobid, 
                      saga::session const & s,
                      saga::object::type    t)
      : saga::impl::job (rm, jobid, s, t)
    {
        // initialize the instance data
        typedef adaptors::v1_0::cpr_job_cpi_instance_data instance_data_type;
        typedef adaptors::instance_data <instance_data_type> cpr_job_instance_data;

        // initialize cpi instance
        {
            cpr_job_instance_data data;
            data.init_data (this, TR1::shared_ptr <instance_data_type> (
                new instance_data_type(rm, jobid)));
        } // lock goes out of scope
    }
    
    void cpr_job::init()
    {
        try {
            // initialize a CPI instance and execute required functions
            this->initcpi(saga::adaptors::cpr_job_cpi);
        }
        catch (saga::exception const& e) {
            // map special exceptions
            switch (e.get_error()) {
            case saga::adaptors::AdaptorDeclined:
            case saga::adaptors::NoAdaptor:
            case saga::adaptors::NoAdaptorInfo:
            case saga::adaptors::Unexpected:
                SAGA_THROW_PLAIN(e.get_object(), e.get_message(), saga::NoSuccess);
            // just rethrow otherwise
            default:
                throw;
            }
        }
    }
    
    cpr_job::~cpr_job (void) 
    {
        // release all attached adaptors now
        saga::impl::proxy::release_cpis();
        
        // free the instance data
        typedef adaptors::v1_0::cpr_job_cpi_instance_data instance_data_type;
        typedef adaptors::instance_data <instance_data_type> cpr_job_instance_data;

        cpr_job_instance_data data;
        data.release_data (this);
    }

    SAGA_CALL_IMPL_IMPL_1 (cpr_job, cpr_job_cpi, checkpoint,    saga::url)
    SAGA_CALL_IMPL_IMPL_1 (cpr_job, cpr_job_cpi, recover,       saga::url)

    SAGA_CALL_IMPL_IMPL_1 (cpr_job, cpr_job_cpi, cpr_stage_in,  saga::url)
    SAGA_CALL_IMPL_IMPL_1 (cpr_job, cpr_job_cpi, cpr_stage_out, saga::url)

    SAGA_CALL_IMPL_IMPL_0 (cpr_job, cpr_job_cpi, cpr_list)
    SAGA_CALL_IMPL_IMPL_0 (cpr_job, cpr_job_cpi, cpr_last)

///////////////////////////////////////////////////////////////////////////////
}} // namespace saga::impl

#endif

