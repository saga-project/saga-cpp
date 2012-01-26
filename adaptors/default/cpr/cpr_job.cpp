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

// saga package includes
#include <saga/saga/packages/cpr/adaptors/cpr_job_self.hpp>
#include <saga/saga/packages/cpr/cpr_job_description.hpp>

// adaptor includes
#include "cpr_job.hpp"

// constructor
cpr_job_cpi_impl::cpr_job_cpi_impl (proxy                           * p, 
                                    cpi_info const                  & info,
                                    saga::ini::ini const            & glob_ini, 
                                    saga::ini::ini const            & adap_ini,
                                    TR1::shared_ptr <saga::adaptor>   adaptor)
    : base_cpi  (p, info, adaptor, cpi::Noflags)
{
  SAGA_LOG_BLURB ("saga default cpr adaptor: cpr_job c'tor start");
  // check if we can handle the request
  instance_data idata (this);

  SAGA_LOG_BLURB ("saga default cpr adaptor: cpr_job c'tor end");
}


// destructor
cpr_job_cpi_impl::~cpr_job_cpi_impl (void)
{
}


//  SAGA API functions
void cpr_job_cpi_impl::sync_get_state (saga::job::state & ret)
{
  ret = job_.get_state ();
}

void cpr_job_cpi_impl::sync_get_description (saga::job::description & ret)
{
  ret = job_.get_description ();
}

void cpr_job_cpi_impl::sync_get_job_id (std::string & ret)
{
  ret = job_.get_job_id ();
}

// access streams for communication with the child
void cpr_job_cpi_impl::sync_get_stdin (saga::job::ostream & ret)
{
  ret = job_.get_stdin ();
}

void cpr_job_cpi_impl::sync_get_stdout (saga::job::istream & ret)
{
  ret = job_.get_stdout ();
}

void cpr_job_cpi_impl::sync_get_stderr (saga::job::istream & ret)
{
  ret = job_.get_stderr ();
}

void cpr_job_cpi_impl::sync_checkpoint (saga::impl::void_t & ret)
{
  job_.checkpoint ();
}

void cpr_job_cpi_impl::sync_migrate (saga::impl::void_t     & ret, 
                                     saga::job::description   jd)
{
  job_.migrate (jd);
}

void cpr_job_cpi_impl::sync_signal (saga::impl::void_t & ret, 
                                    int                  signal)
{
  job_.signal (signal);
}


//  suspend the child process 
void cpr_job_cpi_impl::sync_suspend (saga::impl::void_t & ret)
{
  job_.suspend ();
}

//  resume the child process 
void cpr_job_cpi_impl::sync_resume (saga::impl::void_t & ret)
{
  job_.resume ();
}


//////////////////////////////////////////////////////////////////////
// inherited from the task interface
void cpr_job_cpi_impl::sync_run (saga::impl::void_t & ret)
{
  SAGA_LOG_BLURB ("run job");
  // job_.run ();
}

void cpr_job_cpi_impl::sync_cancel (saga::impl::void_t & ret, double timeout)
{
  job_.cancel (timeout);
}

void cpr_job_cpi_impl::sync_wait (bool   & ret, 
                                  double   timeout)
{
  ret = job_.wait (timeout);
}


