//  Copyright (c) 2005-2007 Hartmut Kaiser 
//  Copyright (c) 2005-2007 Andre Merzky   (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. 
//  (See accompanying file LICENSE or copy at 
//   http://www.boost.org/LICENSE_1_0.txt)

// saga includes
#include <saga/saga.hpp>
#include <saga/saga/adaptors/task.hpp>

// saga adaptor icnludes
#include <saga/saga/adaptors/task.hpp>
#include <saga/saga/adaptors/attribute.hpp>
#include <saga/saga/adaptors/file_transfer_spec.hpp>

// saga engine includes
#include <saga/impl/config.hpp>
#include <saga/impl/exception_list.hpp>

// saga package includes
#include <saga/saga/packages/job/adaptors/job_self.hpp>
#include <saga/saga/packages/job/job_description.hpp>

// adaptor includes
#include "###suite###_###type###_job.hpp"
#include "###suite###_###type###_istream.hpp"
#include "###suite###_###type###_ostream.hpp"


////////////////////////////////////////////////////////////////////////
namespace ###suite###_###type###
{

//   //////////////////////////////////////////////////////////////////////
//   // This adaptor implements the async functions 
//   // based on its own synchronous functions.
// 
//   saga::task 
//     job_cpi_impl::async_get_state (void)
//   {
//     return saga::adaptors::task ("job_cpi_impl::async_get_state",
//                                  shared_from_this (), 
//                                  &job_cpi_impl::sync_get_state);
//   }
// 
//   saga::task 
//     job_cpi_impl::async_get_description (void)
//   {
//     return saga::adaptors::task ("job_cpi_impl::async_get_description",
//                                  shared_from_this (),
//                                  &job_cpi_impl::sync_get_description);
//   }
// 
//   saga::task 
//     job_cpi_impl::async_get_job_id (void)
//   {
//     return saga::adaptors::task ("job_cpi_impl::async_get_job_id",
//                                  shared_from_this (), 
//                                  &job_cpi_impl::sync_get_job_id);
//   }
// 
//   saga::task 
//     job_cpi_impl::async_run (void)
//   {
//     return saga::adaptors::task ("job_cpi_impl::async_run",
//                                  shared_from_this (), 
//                                  &job_cpi_impl::sync_run);
//   }
// 
//   saga::task 
//     job_cpi_impl::async_wait (double   timeout)
//   {
//     return saga::adaptors::task ("job_cpi_impl::async_wait",
//                                  shared_from_this (), 
//                                  &job_cpi_impl::sync_wait,
//                                  timeout);
//   }
// 
//   saga::task 
//     job_cpi_impl::async_cancel (double timeout)
//   {
//     return saga::adaptors::task ("job_cpi_impl::async_cancel",
//                                  shared_from_this (), 
//                                  &job_cpi_impl::sync_cancel,
//                                  timeout);
//   }
// 
//   saga::task 
//     job_cpi_impl::async_suspend (void)
//   {
//     return saga::adaptors::task ("job_cpi_impl::async_suspend",
//                                  shared_from_this (), 
//                                  &job_cpi_impl::sync_suspend);
//   }
// 
//   saga::task 
//     job_cpi_impl::async_resume (void)
//   {
//     return saga::adaptors::task ("job_cpi_impl::async_resume",
//                                  shared_from_this (), 
//                                  &job_cpi_impl::sync_resume);
//   }
// 
//   saga::task 
//     job_cpi_impl::async_get_stdin (void)
//   {
//     return saga::adaptors::task ("job_cpi_impl::async_get_stdin",
//                                  shared_from_this (), 
//                                  &job_cpi_impl::sync_get_stdin);
//   }
// 
//   saga::task  
//     job_cpi_impl::async_get_stdout (void)
//   {
//     return saga::adaptors::task ("job_cpi_impl::async_get_stdout",
//                                  shared_from_this (), 
//                                  &job_cpi_impl::sync_get_stdout);
//   }
// 
//   saga::task  
//     job_cpi_impl::async_get_stderr (void)
//   {
//     return saga::adaptors::task ("job_cpi_impl::async_get_stderr",
//                                  shared_from_this (), 
//                                  &job_cpi_impl::sync_get_stderr);
//   }
// 
//   saga::task 
//     job_cpi_impl::async_checkpoint (void)
//   {
//     return saga::adaptors::task ("job_cpi_impl::async_checkpoint",
//                                  shared_from_this (), 
//                                  &job_cpi_impl::sync_checkpoint);
//   }
// 
//   saga::task 
//     job_cpi_impl::async_migrate (saga::job::description   jd)
//   {
//     return saga::adaptors::task ("job_cpi_impl::async_migrate",
//                                  shared_from_this (), 
//                                  &job_cpi_impl::sync_migrate,
//                                  jd);
//   }
// 
//   saga::task 
//     job_cpi_impl::async_signal (int signal)
//   {
//     return saga::adaptors::task ("job_cpi_impl::async_signal",
//                                  shared_from_this (), 
//                                  &job_cpi_impl::sync_signal,
//                                  signal);
//   }

} // namespace ###suite###_###type###
////////////////////////////////////////////////////////////////////////

