//  Copyright (c) 2005-2007 Hartmut Kaiser 
//  Copyright (c) 2005-2007 Andre Merzky   (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. 
//  (See accompanying file LICENSE or copy at 
//   http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga-defs.hpp>

#ifndef SAGA_WINDOWS

#ifndef ADAPTORS_POSIX_JOB_SERVICE_HPP
#define ADAPTORS_POSIX_JOB_SERVICE_HPP

// stl includes
#include <string>
#include <iosfwd>

// boost includes
#include <boost/shared_ptr.hpp>

// saga includes
#include <saga/saga.hpp>

// saga engine includes
#include <saga/impl/engine/proxy.hpp>

// saga adaptor includes
#include <saga/saga/adaptors/task.hpp>
#include <saga/saga/adaptors/adaptor.hpp>
#include <saga/saga/adaptors/adaptor_data.hpp>

// saga package includes
#include <saga/impl/packages/job/job_service_cpi.hpp>
#include <saga/impl/packages/job/job.hpp>

// adaptor includes
#include "impl_posix_job_tools.hpp"
#include "default_job_adaptor.hpp"

class job_service_cpi_impl 
    : public saga::adaptors::v1_0::job_service_cpi <job_service_cpi_impl>
{
  private:
    typedef saga::adaptors::v1_0::job_service_cpi <job_service_cpi_impl> 
            base_cpi;

    // adaptor data
    typedef saga::adaptors::adaptor_data <default_job_adaptor> adaptor_data_type;


    // the implementation and private instance data
    saga::session   session_;
    saga::url       url_;
    std::string     host_;
    saga::job::self self_;
    bool            have_self_;


  public:
    // constructor of the job_service cpi
    job_service_cpi_impl  (proxy                           * p, 
                           cpi_info const                  & info,
                           saga::ini::ini const            & glob_ini, 
                           saga::ini::ini const            & adap_ini,
                           TR1::shared_ptr <saga::adaptor>   adaptor);

    // destructor of the job_service cpi
    ~job_service_cpi_impl (void);

    // CPI functions
    void sync_create_job (saga::job::job            & ret, 
                          saga::job::description      jd);
    void sync_run_job    (saga::job::job            & ret, 
                          std::string                 exe, 
                          std::string                 host, 
                          saga::job::ostream        & in, 
                          saga::job::istream        & out, 
                          saga::job::istream        & err);
    void sync_list       (std::vector <std::string> & ret);
    void sync_get_job    (saga::job::job            & ret,
                          std::string                 jobid);
    void sync_get_self   (saga::job::self           & ret);

};  // class job_service_cpi_impl

#endif // ADAPTORS_POSIX_JOB_SERVICE_HPP

#endif // SAGA_WINDOWS

