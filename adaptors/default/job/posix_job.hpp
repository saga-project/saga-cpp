//  Copyright (c) 2005-2007 Hartmut Kaiser 
//  Copyright (c) 2005-2007 Andre Merzky   (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. 
//  (See accompanying file LICENSE or copy at 
//   http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga-defs.hpp>

#ifndef SAGA_WINDOWS

#ifndef ADAPTORS_POSIX_JOB_HPP
#define ADAPTORS_POSIX_JOB_HPP

// stl includes
#include <string>

// boost includes
#include <boost/shared_ptr.hpp>

// saga includes
#include <saga/saga.hpp>

// saga engine includes
#include <saga/impl/engine/proxy.hpp>
#include <saga/impl/packages/job/job.hpp>

// saga adaptor includes
#include <saga/saga/adaptors/adaptor.hpp>
#include <saga/saga/adaptors/task.hpp>
#include <saga/saga/adaptors/adaptor_data.hpp>

// job package includes
#include <saga/impl/packages/job/job_cpi.hpp>

// adaptor includes
#include "default_job_adaptor.hpp"
#include "impl_posix_job.hpp"
#include "impl_posix_job_tools.hpp"

class job_cpi_impl 
  : public saga::adaptors::v1_0::job_cpi <job_cpi_impl>,
    private impl::tools::logger
{
  private:
    typedef saga::adaptors::v1_0::job_cpi <job_cpi_impl> base_cpi;

    // posix job implementation
    typedef boost::shared_ptr <impl::posix_job>          impl_t;

    // adaptor data
    typedef saga::adaptors::adaptor_data <default_job_adaptor> adaptor_data_type;

    // the implementation and private instance data
    impl_t impl_;

    saga::job::description      jd_;
    saga::url                   url_;
    std::string                 jobid_;
    pid_t                       pid_;
    bool                        interactive_;
    bool                        is_self_;
    saga::session               session_;

    // helper calls
    void sjd2pjd (void);
    void pjd2sjd (void);
    bool jd_is_local       (saga::job::description & jd);
    bool jd_is_interactive (saga::job::description & jd);


    // utilities
    pid_t                  jobid2pid (std::string pid);
    saga::job::description get_jd    (void) 
    {
      return jd_; 
    }

  public:
    // constructor of the job adaptor
    job_cpi_impl  (proxy                           * p, 
                   cpi_info const                  & info,
                   saga::ini::ini const            & glob_ini, 
                   saga::ini::ini const            & adap_ini,
                   TR1::shared_ptr <saga::adaptor>   adaptor);

    // destructor of the job adaptor
    ~job_cpi_impl (void);

    // job functions
    void sync_get_state       (saga::job::state       & ret);
    void sync_get_description (saga::job::description & ret);
    void sync_get_job_id      (std::string            & ret);

    void sync_get_stdin       (saga::job::ostream     & ret);
    void sync_get_stdout      (saga::job::istream     & ret);
    void sync_get_stderr      (saga::job::istream     & ret);

    void sync_checkpoint      (saga::impl::void_t     & ret);
    void sync_migrate         (saga::impl::void_t     & ret,
                               saga::job::description   jd);
    void sync_signal          (saga::impl::void_t     & ret, 
                               int                      signal);

    // inherited from saga::task
    void sync_run     (saga::impl::void_t & ret);
    void sync_cancel  (saga::impl::void_t & ret, double timeout = 0.0);
    void sync_suspend (saga::impl::void_t & ret);
    void sync_resume  (saga::impl::void_t & ret);

    void sync_wait    (bool               & ret, 
                       double               timeout);

};  // class job_cpi_impl

#endif // ADAPTORS_POSIX_JOB_HPP

#endif // SAGA_WINDOWS

