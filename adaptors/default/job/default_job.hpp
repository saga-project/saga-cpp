//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga-defs.hpp>

#ifdef SAGA_WINDOWS

#ifndef ADAPTORS_DEFAULT_JOB_DEFAULT_JOB_HPP
#define ADAPTORS_DEFAULT_JOB_DEFAULT_JOB_HPP

#include <string>

//#include <saga/saga/util.hpp>
#include <saga/saga/adaptors/adaptor.hpp>
#include <saga/saga/adaptors/adaptor_data.hpp>

#include <saga/impl/engine/proxy.hpp>
#include <saga/impl/packages/job/job_cpi.hpp>

#include <boost/process.hpp>
#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>

#include "default_job_adaptor.hpp"

///////////////////////////////////////////////////////////////////////////////
// forward declaration only
class state_setter;

class job_cpi_impl 
    : public saga::adaptors::v1_0::job_cpi<job_cpi_impl>
{
  private:
    typedef saga::adaptors::v1_0::job_cpi<job_cpi_impl> base_cpi;

    std::string jobid_;
    boost::thread *thrd_;
    boost::condition *cond_;
    boost::condition cancel_thrd_;      // cancel thread from destructor
    bool thread_alive_;                 // thread is running
    bool isinitialized_;
    boost::process::child child_;
    
    // adaptor data
    typedef saga::adaptors::adaptor_data<default_job_adaptor> adaptor_data_type;

    friend class state_setter;
    
  public:
    typedef base_cpi::mutex_type mutex_type;
    
  public:
    /*! constructor of the job cpi */
    job_cpi_impl  (proxy                * p, 
                   cpi_info       const & info,
                   saga::ini::ini const & glob_ini, 
                   saga::ini::ini const & adap_ini,
                   TR1::shared_ptr<saga::adaptor> adaptor);

    /*! destructor of the job cpi */
    ~job_cpi_impl (void);

    void update_state(saga::job::state newstate);
    saga::job::state get_state();

    // CPI functions
    void sync_get_state (saga::job::state& state);
    void sync_get_description(saga::job::description& jd);
    void sync_get_job_id (std::string& jobid);

    // inherited from the task interface
    void sync_run (saga::impl::void_t&);
    void sync_cancel (saga::impl::void_t&, double);
    void sync_wait (bool&, double wait);
    void sync_suspend (saga::impl::void_t&);
    void sync_resume (saga::impl::void_t&);
    
    void sync_get_stdin(saga::job::ostream& ostrm);
    void sync_get_stdout(saga::job::istream& istrm);
    void sync_get_stderr(saga::job::istream& errstrm);
    
//     void sync_checkpoint(saga::impl::void_t&);
    void sync_migrate(saga::impl::void_t&, saga::job::description jd);
//     void sync_signal(saga::impl::void_t&, int signal_type);
        
#if ! defined (SAGA_DEFAULT_JOB_ADAPTOR_NO_ASYNCS)
    // This adaptor implements the async functions based on its own synchronous
    // functions for testing purposes only. Since there is no principal need
    // to do so, we allow these to be 'switched off'.
    saga::task async_get_state ();
    saga::task async_get_description();
    saga::task async_get_job_id ();
    
    // inherited from the task interface
    saga::task async_run ();
    saga::task async_cancel (double);
    saga::task async_wait (double wait);
    saga::task async_suspend ();
    saga::task async_resume ();
    
    saga::task async_get_stdin();
    saga::task async_get_stdout();
    saga::task async_get_stderr();
    
//     saga::task async_checkpoint();
    saga::task async_migrate(saga::job::description jd);
//     saga::task async_signal(int signal_type);
        
#endif

  private:
    // thread routine waiting for the child to terminate
    void wait_for_child();
    void setup_wait_thread();
    
    // initialize the job description from a given jobid
    bool fill_jobdescription(std::string jobid, saga::job::description& jd);
    bool fill_self_jobdescription(saga::job::description& jd);

    // (un)register our jobid with the adaptor data
    void register_jobid(bool mayfail = false);
    void unregister_jobid();
    
    // staging
    void do_pre_staging (saga::job::description jd);
    bool do_post_staging (saga::object, saga::metric, saga::context);
    void register_post_staging ();
    
    saga::monitorable::cookie_handle staging_cookie_;
    
};  // class job_cpi_impl

#endif // ADAPTORS_DEFAULT_JOB_DEFAULT_JOB_HPP


#endif // SAGA_WINDOWS

