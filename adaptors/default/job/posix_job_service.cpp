//  Copyright (c) 2005-2007 Hartmut Kaiser 
//  Copyright (c) 2005-2007 Andre Merzky   (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. 
//  (See accompanying file LICENSE or copy at 
//   http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga-defs.hpp>

#ifndef SAGA_WINDOWS

// stl includes
#include <vector>

// stdlib includes
#include <ctype.h>

// saga includes
#include <saga/saga.hpp>

// saga engine includes
#include <saga/impl/config.hpp>

// saga adaptor includes
#include <saga/saga/adaptors/task.hpp>
#include <saga/saga/adaptors/attribute.hpp>

// saga package includes
#include <saga/saga/packages/job/adaptors/job.hpp>
#include <saga/saga/packages/job/adaptors/job_self.hpp>

// adaptor includes
#include "posix_job_service.hpp"


// constructor
job_service_cpi_impl::job_service_cpi_impl (proxy                * p, 
                                            cpi_info const       & info,
                                            saga::ini::ini const & glob_ini, 
                                            saga::ini::ini const & adap_ini,
                                            TR1::shared_ptr <saga::adaptor> adaptor)
  : base_cpi   (p, info, adaptor, cpi::Noflags),
    session_   (p->get_session ()),
    have_self_ (false)
{
  instance_data idata (this);
  adaptor_data_type adata (this);
  
  // check if we have a rm url.  If yes, check if we are asked for.
  if ( ! idata->rm_.get_string ().empty () )
  {
    // initialize our service URL
    url_  = idata->rm_;
    host_ = url_.get_host ();
    
    SAGA_LOG_INFO(url_.get_string ().c_str ());
    SAGA_LOG_INFO(url_.get_host ().c_str ());
    SAGA_LOG_INFO(url_.get_scheme ().c_str ());
    
    
    // check if URL is usable
    if ( ! url_.get_host ().empty ()                                 &&
         ! saga::adaptors::utils::is_local_address (url_.get_host ()) )
    {
      SAGA_OSSTREAM strm;
      strm << "Could not initialize job service for [" << idata->rm_ << "]. " 
      << "Only 'localhost' and " << adata->localhost () << " are supported.";
      
      SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), 
                         saga::adaptors::AdaptorDeclined);
    }
    
    if ( ! url_.get_scheme ().empty ()  &&
           url_.get_scheme () != "fork" && 
           url_.get_scheme () != "any"  )
    {
      SAGA_OSSTREAM strm;
      strm << "Could not initialize job service for [" << idata->rm_ << "]. " 
           << "Only these schemas are supported: any://, fork://, none.";
      
      SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), 
                         saga::adaptors::AdaptorDeclined);
    }
    
    // std::cout << " == port     : " << url_.get_port      () << " == -1 \n";
    // std::cout << " == fragment : " << url_.get_fragment  () << " == '' \n";
    // std::cout << " == path     : " << url_.get_path      () << " == '' \n";
    // std::cout << " == userinfo : " << url_.get_userinfo  () << " == '' \n";
    // std::cout << " == query    : " << url_.get_query     () << " == '' \n";
    // std::cout << " == username : " << url_.get_username  () << " == '' \n";
    // std::cout << " == password : " << url_.get_password  () << " == '' \n";
    
    if ( ! url_.get_port      () == -1         ||
         ! url_.get_fragment  ().empty ()      ||
         ! ( url_.get_path    ().empty ()  ||
             url_.get_path    () == "/"    )   ||
         ! url_.get_userinfo  ().empty ()      ||
         ! url_.get_query     ().empty ()      ||
         ! url_.get_username  ().empty ()      ||
         ! url_.get_password  ().empty ()      )
    {
      SAGA_OSSTREAM strm;
      strm << "Could not initialize job service for [" << idata->rm_ << "]. " 
           << "Only these schemas are supported: any://, fork://, none. " 
           << "Other URL components are invalid";
      
      SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), 
                         saga::adaptors::AdaptorDeclined);
    }

    // don't use the real name, just localhost
    host_ = "localhost";
  }
  else
  {
    // SAGA_LOG_DEBUG("nuthin?");
    // if we don't have an URL at all, we accept
    host_ = "localhost";
  }
  
  // all is well: if we did not throw before now, we are done.
}


// destructor
job_service_cpi_impl::~job_service_cpi_impl (void)
{
}

//////////////////////////////////////////////////////////////////////
// SAGA API functions
void job_service_cpi_impl::sync_create_job (saga::job::job         & ret, 
                                            saga::job::description   jd)
{
  saga::job::job job = saga::adaptors::job (url_, jd, 
                                            proxy_->get_session ());
  ret = job;
}


void job_service_cpi_impl::sync_run_job (saga::job::job     & ret, 
                                         std::string          exe, 
                                         std::string          host, 
                                         saga::job::ostream & in, 
                                         saga::job::istream & out, 
                                         saga::job::istream & err)
{
  // rely on fallback adaptor to kick in
  SAGA_ADAPTOR_THROW ("run_job is not implemented", saga::NotImplemented);
}


void job_service_cpi_impl::sync_list (std::vector <std::string> & ret)
{
  adaptor_data_type adata (this);
  
  impl::tools::known_job_map_t  known_jobs     = impl::tools::get_known_jobs ();

  impl::tools::known_job_map_t::iterator begin = known_jobs.begin ();
  impl::tools::known_job_map_t::iterator end   = known_jobs.end   ();
  impl::tools::known_job_map_t::iterator iter;
  
  std::vector <std::string> ids;
  for ( iter = begin; iter != end; iter ++ )
  {
    ids.push_back (adata->pid2jobid (iter->first, host_));
  }
  
  ret = ids;
}


void job_service_cpi_impl::sync_get_job (saga::job::job & ret, 
                                         std::string      jobid)
{
  // parse the jobid, check the host, and attach to the process with the
  // rewspective pid (if it exists)
  pid_t       pid    = -1;
  saga::url   url;
  std::string host;
  std::string scheme;

  try 
  {
    adaptor_data_type adata (this);
    
    pid    = adata->jobid2pid (jobid);
    url    = adata->jobid2url (jobid);
    
    host   = url.get_host   ();
    scheme = url.get_scheme ();
    
    if ( host.empty () )
    {
      host = "localhost";
    }
  }
  catch ( ... )
  {
    SAGA_OSSTREAM strm;
    strm << "Could not parse id " << jobid << ". ";     
    SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), 
                       saga::BadParameter);
  }
  
  {
    adaptor_data_type adata (this);
    
    if ( ! url.get_host ().empty ()       &&
        url.get_host () != "localhost" && 
        url.get_host () !=  adata->localhost () )
    {
      SAGA_OSSTREAM strm;
      strm << "Could not get job for id " << jobid << ". " 
      << "Only 'localhost' and '" << adata->localhost () << "' are valid local hostnames.";
      
      SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), 
                         saga::BadParameter);
    }
    
    if ( ! url_.get_scheme ().empty ()  &&
        url.get_scheme () != "fork" && 
        url.get_scheme () != "any"  )
    {
      SAGA_OSSTREAM strm;
      strm << "Could not get job for id " << jobid << "]. " 
           << "Only these schemas are supported: any://, fork://, none.";
      
      SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), 
                         saga::BadParameter);
    }
  }
  
  
  {
    instance_data idata (this);

    // the job c'tor tries to find the job via ps
    ret = saga::adaptors::job (idata->rm_,
                               jobid, 
                               proxy_->get_session ());

    // verify job state
    if ( ret.get_state () == saga::job::Unknown )
    {
      SAGA_OSSTREAM strm;
      strm << "Could not find job info for jobid " << jobid << "]. ";

      SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), 
                         saga::DoesNotExist);
    }
    
    return;
  }
  
  // job not found
  SAGA_OSSTREAM strm;
  strm << "Could not find job for jobid " << jobid << "]. ";
  
  SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), 
                     saga::DoesNotExist);
}


void job_service_cpi_impl::sync_get_self (saga::job::self & ret)
{
  if ( ! have_self_ )
  {
    instance_data     idata (this);
    adaptor_data_type adata (this);
    
    // create the job_self instance
    self_ = saga::adaptors::self (idata->rm_,
                                  adata->pid2jobid (getpid (), host_), 
                                  proxy_->get_session ());
    have_self_ = true;
  }
  
  ret = self_;
}


#endif // SAGA_WINDOWS

