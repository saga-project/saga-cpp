//  Copyright (c) 2005-2007 Andre Merzky   (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. 
//  (See accompanying file LICENSE or copy at 
//   http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga-defs.hpp>

#ifndef SAGA_WINDOWS

#ifndef SAGA_POSIX_JOB_HPP
#define SAGA_POSIX_JOB_HPP

// C++ includes
#include <map>
#include <vector>
#include <string>
#include <fstream>

// C includes
#include <semaphore.h>

// SAGA includes
#include <saga/saga.hpp>

// local includes
#include "impl_posix_job_tools.hpp"


////////////////////////////////////////////////////////////////////////
namespace impl
{
  // this C functions help us to get the watcher threads started
  extern "C" 
  {
    void * thread_startup_io   (void * arg);
    void * thread_startup_wait (void * arg);
  }

  class posix_job ;
  class posix_job : private tools::logger
  {
    private:
      // our io channels to the child
      int                       channel_in[2];
      int                       channel_out[2];
      int                       channel_err[2];

      // convenience names for the parents ends of the channel
      int                       stdin_;
      int                       stdout_;
      int                       stderr_;
  
      std::string               stdio_in_;
      std::string               stdio_out_;
      std::string               stdio_err_;
  
      std::fstream              f_stdio_in_;   // stdin  is redirected from here
      std::fstream              f_stdio_out_;  // stdout is redirected to here
      std::fstream              f_stdio_err_;  // stderr is redirected to here
  
      std::string               stdin_log_;
      std::string               stdout_log_;
      std::string               stderr_log_;

      bool                      stdin_redirect_;
      bool                      stdout_redirect_;
      bool                      stderr_redirect_;

      saga::impl::proxy       * proxy_;
      saga::session             session_;
      saga::job::description    jd_;
  
      pid_t                     child_;
      saga::job::state          state_;
      bool                      final_;
      std::string               pwd_;
      std::string               exe_;
  
      // these threads watch the child for IO and state changes
      pthread_t                 thread_io_;
      pthread_t                 thread_wait_;
  
      // for thread synchronization
      std::string               sem_name_1_;
      std::string               sem_name_2_;
      // sem_t                   * sem_1_;
      // sem_t                   * sem_2_;
      pthread_mutex_t           * mtx_1_;
      pthread_mutex_t           * mtx_2_;
      bool                      pull_for_io_;
      bool                      pull_for_io_in_;
      bool                      pull_for_io_out_;
      bool                      pull_for_io_err_;
      bool                      pull_io_killed_;
      bool                      pull_io_;
      bool                      pull_wait_;
      bool                      threads_running_;
      bool                      interactive_;
      bool                      is_reconnected_;
      bool                      setup_done_;
      int                       exit_code_;
      bool                      dying_;

      // these functions help us to get the watcher threads started
      friend void * thread_startup_io   (void * arg);
      friend void * thread_startup_wait (void * arg);
  
      // helper
      void close          (void);
      void setup          (void);

    protected:
      // run_thread_xy are the mains for the threads watching the child
      void * run_thread_io   (void);
      void * run_thread_wait (void);
  
  
    public:
      posix_job (void);
      posix_job (saga::impl::proxy    * p, 
                 saga::session          s, 
                 saga::job::description jd);
                 
      posix_job (saga::impl::proxy    * p, 
                 saga::session          s, 
                 saga::job::description jd, 
                 pid_t                  pid, 
                 saga::job::state       state = saga::job::Unknown);

     ~posix_job (void);
  
      void  run (void);
  
      int         get_stdin        (void);
      int         get_stdout       (void);
      int         get_stderr       (void);
      void        set_jd           (saga::job::description jd);
      saga::job::description
                  get_jd           (void);
      void        set_state        (saga::job::state s);
      saga::job::state 
                  get_state        (void);
      void        set_jobid        (pid_t pid);
      pid_t       get_jobid        (void);
      pid_t       get_pid          (void);
  
      void        signal           (int signum);
      void        suspend          (void);
      void        resume           (void);
      void        cancel           (void);

      int         get_exit_code    (void) { return exit_code_; }
      std::string state2string     (saga::job::state s);
  };

} // namespace impl
////////////////////////////////////////////////////////////////////////

#endif // SAGA_POSIX_JOB_HPP

#endif // SAGA_WINDOWS

