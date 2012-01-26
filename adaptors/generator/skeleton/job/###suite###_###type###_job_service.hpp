//  Copyright (c) 2005-2007 Hartmut Kaiser 
//  Copyright (c) 2005-2007 Andre Merzky   (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. 
//  (See accompanying file LICENSE or copy at 
//   http://www.boost.org/LICENSE_1_0.txt)

#ifndef ADAPTORS_###SUITE###_###TYPE###_JOB__SERVICE_HPP
#define ADAPTORS_###SUITE###_###TYPE###_JOB__SERVICE_HPP

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
#include <saga/impl/packages/job/job_service_cpi.hpp>

// adaptor includes
#include "###suite###_###type###_adaptor.hpp"


////////////////////////////////////////////////////////////////////////
namespace ###suite###_###type###
{
  class job_service_cpi_impl 
    : public saga::adaptors::v1_0::job_service_cpi <job_service_cpi_impl>
  {
    private:
      typedef saga::adaptors::v1_0::job_service_cpi <job_service_cpi_impl> 
              base_cpi;

      // adaptor data
      typedef saga::adaptors::adaptor_data <adaptor> adaptor_data_type;

    public:
      // constructor of the job_service cpi
      job_service_cpi_impl  (proxy                           * p, 
                             cpi_info const                  & info,
                             saga::ini::ini const            & glob_ini, 
                             saga::ini::ini const            & adap_ini,
                             TR1::shared_ptr <saga::adaptor>   adaptor);

      // destructor of the job_service cpi
      ~job_service_cpi_impl (void);

//       // CPI functions
//       void sync_create_job (saga::job::job            & ret, 
//                             saga::job::description      jd);
//       void sync_run_job    (saga::job::job            & ret, 
//                             std::string                 cmd, 
//                             std::string                 host, 
//                             saga::job::ostream        & in, 
//                             saga::job::istream        & out, 
//                             saga::job::istream        & err);
//       void sync_list       (std::vector <std::string> & ret);
//       void sync_get_job    (saga::job::job            & ret,
//                             std::string                 jobid);
//       void sync_get_self   (saga::job::self           & ret);
// 
//       // This adaptor implements the async functions 
//       // based on its own synchronous functions.
//       saga::task async_create_job (saga::job::description      jd);
//       saga::task async_run_job    (std::string                 cmd, 
//                                    std::string                 host, 
//                                    saga::job::ostream        & in, 
//                                    saga::job::istream        & out, 
//                                    saga::job::istream        & err);
//       saga::task async_list       (void);
//       saga::task async_get_job    (std::string                 jobid);
//       saga::task async_get_self   (void);

  };  // class job_service_cpi_impl

} // namespace ###suite###_###type###
////////////////////////////////////////////////////////////////////////

#endif // ADAPTORS_###SUITE###_###TYPE###_JOB_SERVICE_HPP

