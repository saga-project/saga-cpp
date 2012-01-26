//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga-defs.hpp>

#ifdef SAGA_WINDOWS

#ifndef ADAPTORS_DEFAULT_JOB_DEFAULT_JOB_SERVICE_HPP
#define ADAPTORS_DEFAULT_JOB_DEFAULT_JOB_SERVICE_HPP

#include <string>
#include <iosfwd>

#include <saga/saga/adaptors/adaptor.hpp>
#include <saga/saga/adaptors/adaptor_data.hpp>

#include <saga/impl/engine/proxy.hpp>
#include <saga/impl/packages/job/job_service_cpi.hpp>

#include "default_job_adaptor.hpp"

///////////////////////////////////////////////////////////////////////////////
class job_service_cpi_impl 
    : public saga::adaptors::v1_0::job_service_cpi<job_service_cpi_impl>
{
  private:
    typedef saga::adaptors::v1_0::job_service_cpi<job_service_cpi_impl> 
        base_cpi;

    // adaptor data
    typedef saga::adaptors::adaptor_data<default_job_adaptor> adaptor_data_type;
        
  public:
    /*! constructor of the job_service cpi */
    job_service_cpi_impl  (proxy                * p, 
                           cpi_info       const & info,
                           saga::ini::ini const & glob_ini, 
                           saga::ini::ini const & adap_ini,
                           TR1::shared_ptr<saga::adaptor> adaptor);

    /*! destructor of the job_service cpi */
    ~job_service_cpi_impl (void);

    // CPI functions
    void sync_create_job(saga::job::job & ret, saga::job::description jd);
    void sync_run_job_noio(saga::job::job & ret, 
        std::string commandline, std::string host);
    void sync_run_job(saga::job::job & ret, 
        std::string commandline, std::string host,
        saga::job::ostream& in, saga::job::istream& out, 
        saga::job::istream& err);
    void sync_list(std::vector<std::string>& list_of_jobids);
    void sync_get_job(saga::job::job& job, std::string jobid);
    void sync_get_self(saga::job::self& self);
    
#if ! defined (SAGA_DEFAULT_JOB_ADAPTOR_NO_ASYNCS)
    // This adaptor implements the async functions based on its own synchronous
    // functions for testing purposes only. Since there is no principal need
    // to do so, we allow these to be 'switched off'.
    saga::task async_create_job(saga::job::description jd);
    saga::task async_run_job( std::string commandline, 
        std::string host, saga::job::ostream& in, saga::job::istream& out, 
        saga::job::istream& err);
    saga::task async_list();
    saga::task async_get_job(std::string jobid);
    saga::task async_get_self();
#endif

};  // class job_service_cpi_impl

#endif // ADAPTORS_DEFAULT_JOB_DEFAULT_JOB_SERVICE_HPP


#endif // SAGA_WINDOWS

