//  Copyright (c) 2005-2007 Hartmut Kaiser 
//  Copyright (c) 2005-2007 Andre Merzky   (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. 
//  (See accompanying file LICENSE or copy at 
//   http://www.boost.org/LICENSE_1_0.txt)

// saga includes
#include <saga/saga.hpp>
#include <saga/saga/adaptors/task.hpp>

// saga engine includes
#include <saga/impl/config.hpp>

// saga adaptor includes
#include <saga/saga/adaptors/task.hpp>
#include <saga/saga/adaptors/attribute.hpp>

// saga package includes
#include <saga/saga/packages/cpr/cpr_job_description.hpp>
#include <saga/saga/packages/cpr/adaptors/cpr_job.hpp>
#include <saga/saga/packages/cpr/adaptors/cpr_job_self.hpp>

// adaptor includes
#include "cpr_job_service.hpp"


// constructor
cpr_job_service_cpi_impl::cpr_job_service_cpi_impl (proxy                * p, 
                                                    cpi_info const       & info,
                                                    saga::ini::ini const & glob_ini, 
                                                    saga::ini::ini const & adap_ini,
                                                    TR1::shared_ptr <saga::adaptor> adaptor)
: base_cpi (p, info, adaptor, cpi::Noflags)
{
  SAGA_LOG_BLURB ("saga default cpr adaptor: cpr_job_service c'tor start");
  // check if we can handle this request
  instance_data data (this);
  
  if ( ! data->rm_.get_url ().empty () )
  {
    std::string scheme (data->rm_.get_scheme ());

    if ( ! ( scheme.empty () ) && 
         ! ( scheme == "cpr" ) && 
         ! ( scheme == "any" ) ) 
    {
      SAGA_LOG_BLURB ("saga default cpr adaptor: cpr_job_service c'tor refuse");
      SAGA_ADAPTOR_THROW ("Can't use schemes others from 'any' or 'cpr' " 
                          "for cpr job submission.", saga::NotImplemented);
    }
  }

  SAGA_LOG_BLURB("creating default job_service");
  // create job service with default job adaptor
  js_ = new saga::job::service (p->get_session (), saga::url ("fork://localhost"));

  if ( NULL == js_ )
  {
    SAGA_LOG_BLURB ("saga default cpr adaptor: cpr_job_service c'tor fail");
    SAGA_ADAPTOR_THROW ("Ooops!  Could not create new job_service.",
                        saga::NoSuccess);
  }

  SAGA_LOG_BLURB ("creating default job_service - done");
  SAGA_LOG_BLURB ("saga default cpr adaptor: cpr_job_service c'tor end");
}

// destructor
cpr_job_service_cpi_impl::~cpr_job_service_cpi_impl (void)
{
  if ( js_ ) delete (js_);
}

//////////////////////////////////////////////////////////////////////
// SAGA API functions
void cpr_job_service_cpi_impl::sync_create_job (saga::cpr::job         & ret, 
                                                saga::cpr::description   jd_start)
{
  SAGA_LOG_BLURB("create_job");
  std::cerr << " ------------- \n";
  ret = js_->create_job (jd_start);
  SAGA_LOG_BLURB("created job");
  
  // FIXME 
  // ret.set_restart_description (jd_restart);
}


saga::cpr::description cpr_job_service_cpi_impl::jd_to_cjd (const saga::job::description & jd)
{
  saga::cpr::description cjd;

  std::vector <std::string> attribs = jd.list_attributes ();

  for ( std::size_t i = 0; i < attribs.size (); i++ )
  {
    if ( jd.attribute_is_vector (attribs[i]) )
    {
      cjd.set_vector_attribute (attribs[i], jd.get_vector_attribute (attribs[i]));
    }
    else
    {
      cjd.set_attribute (attribs[i], jd.get_attribute (attribs[i]));
    }
  }

  return cjd;
}

void cpr_job_service_cpi_impl::sync_create_job_cpr (saga::cpr::job         & ret, 
                                                    saga::cpr::description   jd_start,
                                                    saga::cpr::description   jd_restart)
{
  SAGA_LOG_BLURB ("saga default cpr adaptor: cpr_job_service sync_create_job_cpr start");
  instance_data data (this);
  ret = saga::adaptors::cpr_job (data->rm_.get_url (), jd_start, jd_restart);
  SAGA_LOG_BLURB ("saga default cpr adaptor: cpr_job_service sync_create_job_cpr end");
  
  // FIXME 
  // ret.set_restart_description (jd_restart);
}


void cpr_job_service_cpi_impl::sync_run_job (saga::cpr::job     & ret, 
                                             std::string          cmd, 
                                             std::string          host, 
                                             saga::job::ostream & in, 
                                             saga::job::istream & out, 
                                             saga::job::istream & err)
{
  SAGA_LOG_BLURB("run_job");
  ret = js_->run_job (cmd, host, in, out, err);
  
  // FIXME 
  // ret.set_restart_description (jd_restart);
}


// FIXME: merge known_job_list_ with created_job_list
void cpr_job_service_cpi_impl::sync_list (std::vector <std::string> & ret)
{
  ret = js_->list ();
}

void cpr_job_service_cpi_impl::sync_get_job (saga::job::job & ret, 
                                             std::string      jobid)
{
  ret = js_->get_job (jobid);
}

void cpr_job_service_cpi_impl::sync_get_self (saga::job::self & ret)
{
  ret = js_->get_self ();
}


