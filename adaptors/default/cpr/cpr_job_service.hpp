//  Copyright (c) 2005-2007 Hartmut Kaiser 
//  Copyright (c) 2005-2007 Andre Merzky   (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. 
//  (See accompanying file LICENSE or copy at 
//   http://www.boost.org/LICENSE_1_0.txt)

#ifndef ADAPTORS_CPR_JOB_SERVICE_HPP
#define ADAPTORS_CPR_JOB_SERVICE_HPP

// stl includes
#include <string>
#include <iosfwd>

// saga includes
#include <saga/saga.hpp>
#include <saga/saga/adaptors/task.hpp>

// saga engine includes
#include <saga/impl/engine/proxy.hpp>

// saga adaptor includes
#include <saga/saga/adaptors/task.hpp>
#include <saga/saga/adaptors/adaptor.hpp>
#include <saga/saga/adaptors/adaptor_data.hpp>

// saga package includes
#include <saga/impl/packages/cpr/cpr_job_service_cpi.hpp>
#include <saga/impl/packages/cpr/cpr_job.hpp>

// adaptor includes
#include "default_cpr_adaptor.hpp"

class cpr_job_service_cpi_impl 
  : public saga::adaptors::v1_0::cpr_job_service_cpi <cpr_job_service_cpi_impl>
{
  private:

    typedef saga::adaptors::v1_0::cpr_job_service_cpi <cpr_job_service_cpi_impl> 
            base_cpi;


    // private instance data
    saga::job::service * js_;

    saga::cpr::description jd_to_cjd (const saga::job::description & jd);

  public:
    // constructor of the job_service cpi
    cpr_job_service_cpi_impl  (proxy                           * p, 
                           cpi_info const                  & info,
                           saga::ini::ini const            & glob_ini, 
                           saga::ini::ini const            & adap_ini,
                           TR1::shared_ptr <saga::adaptor>   adaptor);

    // destructor of the job_service cpi
    ~cpr_job_service_cpi_impl (void);

    // CPI functions
    void sync_create_job     (saga::cpr::job            & ret, 
                              saga::cpr::description      jd_run);
    void sync_create_job_cpr (saga::cpr::job            & ret, 
                              saga::cpr::description      jd_run,
                              saga::cpr::description      jd_restart);
    void sync_run_job        (saga::cpr::job            & ret, 
                              std::string                 cmd,
                              std::string                 host,
                              saga::job::ostream        & in, 
                              saga::job::istream        & out, 
                              saga::job::istream        & err);
    void sync_list           (std::vector <std::string> & ret);
    void sync_get_job        (saga::job::job            & ret,
                              std::string                 jobid);
    void sync_get_self       (saga::job::self           & ret);


};  // class cpr_job_service_cpi_impl

#endif // ADAPTORS_CPR_JOB_SERVICE_HPP

