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

  // constructor
  job_cpi_impl::job_cpi_impl (proxy                           * p, 
                              cpi_info const                  & info,
                              saga::ini::ini const            & glob_ini, 
                              saga::ini::ini const            & adap_ini,
                              TR1::shared_ptr <saga::adaptor>   adaptor)
    : base_cpi  (p, info, adaptor, cpi::Noflags)
  {
    SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
  }


  // destructor
  job_cpi_impl::~job_cpi_impl (void)
  {
  }


//   //  SAGA API functions
//   void job_cpi_impl::sync_get_state (saga::job::state & ret)
//   {
//     SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
//   }
// 
//   void job_cpi_impl::sync_get_description (saga::job::description & ret)
//   {
//     SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
//   }
// 
//   void job_cpi_impl::sync_get_job_id (std::string & ret)
//   {
//     SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
//   }
// 
//   // access streams for communication with the child
//   void job_cpi_impl::sync_get_stdin (saga::job::ostream & ret)
//   {
//     SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
//   }
// 
//   void job_cpi_impl::sync_get_stdout (saga::job::istream & ret)
//   {
//     SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
//   }
// 
//   void job_cpi_impl::sync_get_stderr (saga::job::istream & ret)
//   {
//     SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
//   }
// 
//   void job_cpi_impl::sync_checkpoint (saga::impl::void_t & ret)
//   {
//     SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
//   }
// 
//   void job_cpi_impl::sync_migrate (saga::impl::void_t           & ret, 
//                                    saga::job::description   jd)
//   {
//     SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
//   }
// 
//   void job_cpi_impl::sync_signal (saga::impl::void_t & ret, 
//                                   int            signal)
//   {
//     SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
//   }
// 
// 
//   //  suspend the child process 
//   void job_cpi_impl::sync_suspend (saga::impl::void_t & ret)
//   {
//     SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
//   }
// 
//   //  suspend the child process 
//   void job_cpi_impl::sync_resume (saga::impl::void_t & ret)
//   {
//     SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
//   }
// 
// 
//   //////////////////////////////////////////////////////////////////////
//   // inherited from the task interface
//   void job_cpi_impl::sync_run (saga::impl::void_t & ret)
//   {
//     SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
//   }
// 
//   void job_cpi_impl::sync_cancel (saga::impl::void_t & ret, 
//                                   double timeout)
//   {
//     SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
//   }
// 
//   //  wait for the child process to terminate
//   void job_cpi_impl::sync_wait (bool   & ret, 
//                                 double   timeout)
//   {
//     SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
//   }

} // namespace ###suite###_###type###
////////////////////////////////////////////////////////////////////////

