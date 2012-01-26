//  Copyright (c) 2005-2007 Hartmut Kaiser 
//  Copyright (c) 2005-2007 Andre Merzky   (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. 
//  (See accompanying file LICENSE or copy at 
//   http://www.boost.org/LICENSE_1_0.txt)

#ifndef ADAPTORS_###SUITE###_###TYPE###_HPP
#define ADAPTORS_###SUITE###_###TYPE###_HPP

// stl includes
#include <string>

// saga includes
#include <saga/saga.hpp>
#include <saga/saga/adaptors/task.hpp>

// saga engine includes
#include <saga/impl/engine/proxy.hpp>

// saga adaptor includes
#include <saga/saga/adaptors/adaptor.hpp>
#include <saga/saga/adaptors/task.hpp>
#include <saga/saga/adaptors/adaptor_data.hpp>

// job package includes
#include <saga/impl/packages/job/job_cpi.hpp>

// adaptor includes
#include "###suite###_###type###_adaptor.hpp"

////////////////////////////////////////////////////////////////////////
namespace ###suite###_###type###
{
  class job_cpi_impl 
    : public saga::adaptors::v1_0::job_cpi <job_cpi_impl>
  {
    private:
      typedef saga::adaptors::v1_0::job_cpi <job_cpi_impl> base_cpi;

      // adaptor data
      typedef saga::adaptors::adaptor_data <adaptor> adaptor_data_type;


    public:
      // constructor of the job adaptor
      job_cpi_impl  (proxy                           * p, 
                     cpi_info const                  & info,
                     saga::ini::ini const            & glob_ini, 
                     saga::ini::ini const            & adap_ini,
                     TR1::shared_ptr <saga::adaptor>   adaptor);

      // destructor of the job adaptor
      ~job_cpi_impl (void);

//       // job functions
//       void sync_get_state       (saga::job::state       & ret);
//       void sync_get_description (saga::job::description & ret);
//       void sync_get_job_id      (std::string            & ret);
// 
//       void sync_get_stdin       (saga::job::ostream     & ret);
//       void sync_get_stdout      (saga::job::istream     & ret);
//       void sync_get_stderr      (saga::job::istream     & ret);
// 
//       void sync_checkpoint      (saga::impl::void_t     & ret);
//       void sync_migrate         (saga::impl::void_t     & ret,
//                                  saga::job::description   jd);
//       void sync_signal          (saga::impl::void_t     & ret, 
//                                  int                      signal);
// 
//       // inherited from saga::task
//       void sync_run     (saga::impl::void_t & ret);
//       void sync_cancel  (saga::impl::void_t & ret, 
//                          double               timeout);
//       void sync_suspend (saga::impl::void_t & ret);
//       void sync_resume  (saga::impl::void_t & ret);
// 
//       void sync_wait    (bool         & ret, 
//                          double         timeout);
// 
//       // This adaptor implements the async functions 
//       // based on its own synchronous functions.
//       saga::task async_get_state       (void);
//       saga::task async_get_description (void);
//       saga::task async_get_job_id      (void);
// 
//       saga::task async_get_stdin       (void);
//       saga::task async_get_stdout      (void);
//       saga::task async_get_stderr      (void);
// 
//       saga::task async_checkpoint      (void);
//       saga::task async_migrate         (saga::job::description   jd);
//       saga::task async_signal          (int                      signal);
// 
//       // inherited from the task interface
//       saga::task async_run             (void);
//       saga::task async_cancel          (double timeout);
//       saga::task async_suspend         (void);
//       saga::task async_resume          (void);
//       saga::task async_wait            (double  timeout);

  };  // class job_cpi_impl

} // namespace ###suite###_###type###
////////////////////////////////////////////////////////////////////////

#endif // ADAPTORS_###SUITE###_###TYPE###_HPP

