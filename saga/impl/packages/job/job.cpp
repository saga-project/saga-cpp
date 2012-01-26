#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/job.cpp")
#endif
//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 0, output: null)
#endif

#include <boost/config.hpp>

#include <saga/saga/adaptors/task.hpp>
#include <saga/saga/adaptors/metric.hpp>
#include <saga/impl/packages/job/job.hpp>
#include <saga/saga/adaptors/instance_data.hpp>
#include <saga/saga/adaptors/packages/job_cpi_instance_data.hpp>
#include <saga/saga/adaptors/attribute.hpp>
#ifdef SAGA_DEBUG
#include <saga/impl/packages/data/job/preprocessed/job.cpp>
#else

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, output: "preprocessed/job.cpp")
#endif

///////////////////////////////////////////////////////////////////////////////
namespace saga 
{ 
  namespace impl 
  {
    job::job (saga::url rm, saga::job::description jd, 
              saga::session const & s, saga::object::type t)
      : proxy(t, s)
    {
        // initialize the instance data
        typedef adaptors::v1_0::job_cpi_instance_data instance_data_type;
        typedef adaptors::instance_data <instance_data_type> job_instance_data;

        {
            job_instance_data data;
            data.init_data (this, TR1::shared_ptr <instance_data_type> (
                new instance_data_type(rm, jd)));
        } // lock goes out of scope
    }
    
    job::job (saga::url rm, std::string jobid, saga::session const & s, 
              saga::object::type t)
      : proxy(t, s)
    {
        // initialize the instance data
        typedef adaptors::v1_0::job_cpi_instance_data instance_data_type;
        typedef adaptors::instance_data <instance_data_type> job_instance_data;

        // initialize cpi instance
        {
            job_instance_data data;
            data.init_data (this, TR1::shared_ptr <instance_data_type> (
                new instance_data_type(rm, jobid)));
        } // lock goes out of scope
    }
    
    void job::init()
    {
        try {
            // initialize a CPI instance and execute required functions
            this->initcpi(saga::adaptors::job_cpi);
        }
        catch (saga::exception const& e) {
            // map special exceptions
            switch (e.get_error()) {
            case saga::adaptors::AdaptorDeclined:
            case saga::adaptors::NoAdaptor:
            case saga::adaptors::NoAdaptorInfo:
            case saga::adaptors::Unexpected:
                SAGA_THROW_PLAIN(e.get_object(), e.what(), saga::NoSuccess);
            // just rethrow otherwise
            default:
                throw;
            }
        }
    }
    
    job::~job (void) 
    {
        // release all attached adaptors now
        saga::impl::proxy::release_cpis();
        
        // free the instance data
        typedef adaptors::v1_0::job_cpi_instance_data instance_data_type;
        typedef adaptors::instance_data <instance_data_type> job_instance_data;

        job_instance_data data;
        data.release_data (this);
    }

    // task interface
    SAGA_CALL_IMPL_IMPL_0_EX (job, job_cpi, job_run, run)
    SAGA_CALL_IMPL_IMPL_1_EX (job, job_cpi, job_cancel, cancel, double)
    SAGA_CALL_IMPL_IMPL_1_EX (job, job_cpi, job_wait, wait, double)

    // job inspection
    SAGA_CALL_IMPL_IMPL_0 (job, job_cpi, get_job_id)
    SAGA_CALL_IMPL_IMPL_0 (job, job_cpi, get_state)
    SAGA_CALL_IMPL_IMPL_0 (job, job_cpi, get_description)
    SAGA_CALL_IMPL_IMPL_0 (job, job_cpi, get_stdin)
    SAGA_CALL_IMPL_IMPL_0 (job, job_cpi, get_stdout)
    SAGA_CALL_IMPL_IMPL_0 (job, job_cpi, get_stderr)

    // job management
    SAGA_CALL_IMPL_IMPL_0 (job, job_cpi, suspend)
    SAGA_CALL_IMPL_IMPL_0 (job, job_cpi, resume)
    SAGA_CALL_IMPL_IMPL_0 (job, job_cpi, checkpoint)
    SAGA_CALL_IMPL_IMPL_1 (job, job_cpi, migrate, saga::job::description)
    SAGA_CALL_IMPL_IMPL_1 (job, job_cpi, signal, int)

///////////////////////////////////////////////////////////////////////////////
}} // namespace saga::impl

#endif

