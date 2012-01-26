//  Copyright (c) 2005-2007 Andre Merzky   (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. 
//  (See accompanying file LICENSE or copy at 
//   http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga-defs.hpp>

#ifndef SAGA_WINDOWS

// C++ includes
#include <iostream>

// C includes
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>

#include <errno.h>


// saga includes
#include <saga/impl/exception.hpp>
#include <saga/saga/adaptors/utils.hpp>

// local includes
#include "impl_posix_job.hpp"

// should be "/tmp.saga.ipc" on Solaris
#define SAGA_SEMAPHORE_NAME  "/tmp.saga.ipc"

// define idle time when busy waiting
#define WAIT_DELAY 0.1

// saga includes
#include <saga/saga.hpp>

// FIXME: why do I need task.hpp to use metrics?
#include <saga/saga/adaptors/task.hpp>

////////////////////////////////////////////////////////////////////////
namespace impl
{
  // default c'tor
  posix_job::posix_job (void)
    : state_           (saga::job::New)
    , pwd_             (".")
    , threads_running_ (false)
    , setup_done_      (false)
    , exit_code_       (0)
    , dying_           (false)
  {
    // construct an empty job.  We do, however, setup io
    // channels so that we can answer to get_stdin() etc.
    setup ();
  }


  // c'tor to create a new instance from a job description
  posix_job::posix_job (saga::impl::proxy    * p, 
                        saga::session          s, 
                        saga::job::description jd)
    : proxy_      (p)
    , session_    (s)
    , jd_         (jd)
    , setup_done_ (false)
    , exit_code_  (0)
    , dying_      (false)
  {
    // debug output
    // impl::tools::dump_jd (jd_);

    // initial job state is new
    child_           = -1;
    set_state (saga::job::New);
    threads_running_ = false;
    pull_for_io_     = false;
    pull_io_         = false;
    interactive_     = false;
    pull_for_io_in_  = false;
    pull_for_io_out_ = false;
    pull_for_io_err_ = false;
    stdin_redirect_  = false;
    stdout_redirect_ = false;
    stderr_redirect_ = false;
    is_reconnected_  = false;
    pwd_             = ".";

    // get job's pwd
    if ( jd_.attribute_exists
         (saga::job::attributes::description_working_directory))
    {
      pwd_ = jd_.get_attribute (saga::job::attributes::description_working_directory);

      // check if dir exists
      struct stat sbuf;
      if ( ::stat (pwd_.c_str (), &sbuf) )
      {
        std::string cmd ("mkdir -p ");
        cmd += pwd_;
        if ( 0 != ::system (cmd.c_str ()) )
        {
          log ("mkdir on pwd failed", pwd_);
          std::string err ("could not create job's working directory: ");
          err += ::strerror (errno);
          SAGA_ADAPTOR_THROW_NO_CONTEXT (err.c_str (), saga::BadParameter);
        }
      }

      if ( ! (sbuf.st_mode & S_IFDIR) )
      {
        log ("pwd is not a dir", pwd_);
        SAGA_ADAPTOR_THROW_NO_CONTEXT ("job's working directory is in fact not a directory",
                                       saga::BadParameter);
      }
    }


    // get exe name
    if ( ! jd_.attribute_exists 
           (saga::job::attributes::description_executable) )
    {
      SAGA_ADAPTOR_THROW_NO_CONTEXT ("No executable specified in job description",
                                     saga::BadParameter);
    }

    exe_ = jd_.get_attribute (saga::job::attributes::description_executable);

    // if exe is an path, we don't search further, but just check
    // that exe's existence
    if ( exe_.find_first_of ("/") != std::string::npos )
    {
      struct stat sbuf;
      if ( 0 != ::stat (exe_.c_str (), &sbuf) )
      {
        SAGA_ADAPTOR_THROW_NO_CONTEXT (std::string ("Executable ") + exe_ + " not found", 
                                       saga::BadParameter);
      }
    }
    else
    {
      // we have to check the path
      // find path in environment
      std::string path;

      if ( jd_.attribute_exists 
           (saga::job::attributes::description_environment) )
      {
        std::vector <std::string> env = jd_.get_vector_attribute 
          (saga::job::attributes::description_environment);

        for ( unsigned int i = 0; i < env.size (); i++ )
        {
          std::vector <std::string> elems = saga::adaptors::utils::split (env[i], '=', 2);

          if ( elems[0] == "PATH" )
          {
            path = elems[1];
          }
        }
      }
      else
      {
        char * path_c = ::getenv ("PATH");
        path = path_c;
      }


      // if we have a path, split it, and also use pwd for search
      std::vector <std::string> path_v = saga::adaptors::utils::split (path, ':');
      
      if ( pwd_ != "" )
      {
        path_v.push_back (pwd_);
      }

      bool   exe_exists = false;
      struct stat sbuf;

      for ( unsigned int i = 0; 
            exe_exists == false && i < path_v.size (); 
            i++ )
      {
        std::string full_exe = path_v[i] + "/" + exe_;

        if ( 0 == ::stat (full_exe.c_str (), &sbuf) )
        {
          exe_exists = true;
        }
      }
      

      if ( ! exe_exists )
      {
        SAGA_ADAPTOR_THROW_NO_CONTEXT (std::string ("Executable ") + exe_ + " not found in path",
                                       saga::BadParameter);
      }

    } // check path


    // check if we are to redirect stdio
    if (   jd_.attribute_exists (saga::job::attributes::description_input)          &&
         ! jd_.get_attribute    (saga::job::attributes::description_input).empty () )
    {
      interactive_     = true;
      pull_for_io_     = true;
      pull_io_         = true;
      pull_for_io_in_  = true;
      stdin_redirect_  = true;
      stdio_in_        = jd_.get_attribute (saga::job::attributes::description_input);

      if ( stdio_in_[0] != '/' )
      {
        stdio_in_ = pwd_ + "/" + stdio_in_;
      }

      saga::url tmp (jd_.get_attribute (saga::job::attributes::description_input));

      if ( ( ! ( tmp.get_scheme ().empty () ) &&
             ! ( tmp.get_scheme () == "any" ) )  || 
             ! saga::adaptors::utils::is_local_address (tmp.get_host ()) )
      {
        SAGA_ADAPTOR_THROW_NO_CONTEXT ("cannot open specified stdin url",
                                       saga::BadParameter);
      }

      stdio_in_ = tmp.get_path ();

      if ( stdio_in_.empty () )
      {
        // FIXME: we could set a default, ey?
        SAGA_ADAPTOR_THROW_NO_CONTEXT ("cannot open specified unspecified stdin target",
                                       saga::BadParameter);
      }

      f_stdio_in_.open (stdio_in_.c_str (), std::fstream::in);

      if ( f_stdio_in_.fail () )
      {
        SAGA_ADAPTOR_THROW_NO_CONTEXT ("cannot open specified stdin source",
                                       saga::BadParameter);
      }
    }

    if (   jd_.attribute_exists (saga::job::attributes::description_output)          &&
         ! jd_.get_attribute    (saga::job::attributes::description_output).empty () )
    {
      interactive_     = true;
      pull_for_io_     = true;
      pull_io_         = true;
      pull_for_io_out_ = true;
      stdout_redirect_ = true;

      saga::url tmp (jd_.get_attribute (saga::job::attributes::description_output));

      log ("pwd_     ", pwd_);
      log ("stdio_out", tmp);

      if ( ( ! ( tmp.get_scheme ().empty () ) &&
             ! ( tmp.get_scheme () == "any" ) )  || 
             ! saga::adaptors::utils::is_local_address (tmp.get_host ()) )
      {
        SAGA_ADAPTOR_THROW_NO_CONTEXT ("cannot open specified stdout url",
                                       saga::BadParameter);
      }

      stdio_out_ = tmp.get_path ();

      if ( stdio_out_.empty () )
      {
        // FIXME: we could set a default, ey?
        SAGA_ADAPTOR_THROW_NO_CONTEXT ("cannot open specified unspecified stdout target",
                                       saga::BadParameter);
      }

      if ( stdio_out_[0] != '/' )
      {
        log ("fixing stdio_out", pwd_);
        stdio_out_ = pwd_ + "/" + stdio_out_;
      }
      log ("stdio_out", stdio_out_);

      f_stdio_out_.open (stdio_out_.c_str (), std::fstream::out);

      if ( f_stdio_out_.fail () )
      {
        SAGA_ADAPTOR_THROW_NO_CONTEXT ("cannot open specified stdout target",
                                       saga::BadParameter);
      }
    }

    if (   jd_.attribute_exists (saga::job::attributes::description_error)          &&
         ! jd_.get_attribute    (saga::job::attributes::description_error).empty () )
    {
      interactive_     = true;
      pull_for_io_     = true;
      pull_io_         = true;
      pull_for_io_err_ = true;
      stderr_redirect_ = true;

      saga::url tmp (jd_.get_attribute (saga::job::attributes::description_error));

      if ( ( ! ( tmp.get_scheme ().empty () ) &&
             ! ( tmp.get_scheme () == "any" ) )  || 
             ! saga::adaptors::utils::is_local_address (tmp.get_host ()) )
      {
        SAGA_ADAPTOR_THROW_NO_CONTEXT ("cannot open specified stderr url",
                                       saga::BadParameter);
      }

      stdio_err_ = tmp.get_path ();

      if ( stdio_err_.empty () )
      {
        // FIXME: we could set a default, ey?
        SAGA_ADAPTOR_THROW_NO_CONTEXT ("cannot open specified unspecified stderr target",
                                       saga::BadParameter);
      }

      if ( stdio_err_[0] != '/' )
      {
        stdio_err_ = pwd_ + "/" + stdio_err_;
      }

      f_stdio_err_.open (stdio_err_.c_str (), std::fstream::out);

      if ( f_stdio_err_.fail () )
      {
        SAGA_ADAPTOR_THROW_NO_CONTEXT ("cannot open specified stderr target",
                                       saga::NoSuccess);
      }
    }


    if ( jd_.attribute_exists (saga::job::attributes::description_interactive) )
    {
      std::string i = jd_.get_attribute (saga::job::attributes::description_interactive);

      if ( i == saga::attributes::common_true )
      {
        interactive_ = true;
      }
      else if ( i != saga::attributes::common_false )
      {
        SAGA_ADAPTOR_THROW_NO_CONTEXT ("invalid value for bool attribute 'Interactive' - can only be set to 'True' or 'False'",
                                       saga::BadParameter);
      }
    }

    setup ();
  }

  // c'tor to create a new instance from a pid.
  // jd is only given to be stored in jd_.
  posix_job::posix_job (saga::impl::proxy     * p, 
                        saga::session           s, 
                        saga::job::description  jd, 
                        pid_t                   pid, 
                        saga::job::state        state)
    : proxy_     (p)
    , session_   (s)
    , jd_        (jd)
    , exit_code_ (0)
    , dying_     (false)
  {
    // initial job state is RUNNING (TODO: check if pid exists)
    child_           = pid;
    set_state (state);
    pull_for_io_     = false;
    is_reconnected_  = true;

    // no io for reconnected jobs
    stdin_  = -1;
    stdout_ = -1;
    stderr_ = -1;

    // start the waiting thread, if the job is still active
    if ( state_ == saga::job::Unknown )
    {
      SAGA_ADAPTOR_THROW_NO_CONTEXT ("cannot reconnect to job in unknown state",
                                     saga::IncorrectState);
    }

    // for intermediate states, start a watcher thread which catches state
    // changes via waitpid
    else if ( state_ == saga::job::New       ||
              state_ == saga::job::Running   ||
              state_ == saga::job::Suspended )
    {

      // FIXME: we should only attempt to reconnect if we actually started this
      // job - there is no other guarantee that waitpid will
      // actually repotry anything for that job.
      
      impl::tools::known_job kj = impl::tools::get_known_job (child_);

      if ( kj.spawned ) // are we parent to that job?
      {
        pull_wait_ = true;

        if ( 0 != pthread_create (&thread_wait_, NULL, impl::thread_startup_wait, this) )
        {
          log ("ptherad create for wait thread failed", strerror (errno));
          SAGA_ADAPTOR_THROW_NO_CONTEXT (std::string ("pthread_create failed: ") + strerror (errno),
                                         saga::NoSuccess);
        }
        threads_running_ = true;
      }
      else
      {
        threads_running_ = false;
      }
    }

    // else we are in a final state already
    else
    {
      threads_running_ = false;
    }
  }

  posix_job::~posix_job (void)
  {
    dying_ = true;
    close ();
  }

  void posix_job::close (void)
  {
    // kill io pulling.  This is the d'tor, so its not like anybody still cares
    // about this jobs i/o...
    if ( pull_for_io_ )
    {
      pull_io_        = false;
      pull_io_killed_ = true; // we don't wait for pending stdio 
      
      if ( interactive_ )
      {
        // close may need to wait for pending stdio
        while ( ! pull_io_killed_ && pull_io_ )
        {
          // delay for nanosleep in sec
          static struct timespec delay;
          static struct timespec remain;

          delay.tv_sec  = 0;
          delay.tv_nsec = (long int) (WAIT_DELAY * 1000000000);  // < 10^9

          ::nanosleep (&delay, &remain);
        }
      }

      pull_for_io_ = false;
    }

    if ( threads_running_ )
    {
      // signal the helper threads to stop
      pull_wait_ = false;

      // wait for the helper threads to stop.
      if ( pull_for_io_ )
      {
        pull_io_ = false;
        pthread_join (thread_io_, NULL);
      }

      pthread_join (thread_wait_, NULL);

      threads_running_ = false;
    }

    // close all file descriptors, possibly again
    if ( f_stdio_in_ .is_open () ) f_stdio_in_ .close ();
    if ( f_stdio_out_.is_open () ) f_stdio_out_.close ();
    if ( f_stdio_err_.is_open () ) f_stdio_err_.close ();

    if ( interactive_ )
    {
      ::close (channel_in [1]);
      ::close (channel_out[0]);
      ::close (channel_err[0]);

      ::close (channel_in [0]);
      ::close (channel_out[1]);
      ::close (channel_err[1]);
    }
  }

  void posix_job::run (void)
  {
    if ( saga::job::New != state_ )
    {
      log ("IncorrectState");
      SAGA_ADAPTOR_THROW_NO_CONTEXT ("run() can only be called on New jobs", 
                                     saga::IncorrectState);
    }

    // after fork, we can't throw anymore.  Thus, we search the env PATH here
    // for the executable, to report if its missing.  execvp may later disagree
    // if we here think we found it, but thats live...
    
    // create the semaphore we need for later syncchronization
    // initially, the parent has the semaphore, before fork()
    char pid_name[20];
    snprintf (pid_name, 19, "%ld", (long) ::getpid ());

    try {
      mtx_1_ = new pthread_mutex_t ();
      mtx_2_ = new pthread_mutex_t ();
    }
    catch ( ... )
    {
      SAGA_ADAPTOR_THROW_NO_CONTEXT (std::string ("could not create mutex: ") + strerror (errno),
                                     saga::NoSuccess);
    }

    if ( 0 != pthread_mutex_init (mtx_1_, NULL) ||
         0 != pthread_mutex_init (mtx_2_, NULL) )
    {
      SAGA_ADAPTOR_THROW_NO_CONTEXT (std::string ("could not init mutex: ") + strerror (errno),
                                     saga::NoSuccess);
    }

    log ("forking");

    // get mini-me
    child_ = ::fork ();

    log ("forked");

    if ( -1 == child_ )
    {
      log ("fork failed", strerror (errno));
      SAGA_ADAPTOR_THROW_NO_CONTEXT (std::string ("fork failed: ") + strerror (errno),
                                     saga::NoSuccess);
    }
    else if ( 0 == child_ )
    {
      // me child

      // we need these for execve
      char** cargs = NULL;
      int    nargs = 0;

      // if the child dies before it can release the semaphores,
      // the main thread will hang.  Thus we need to do
      // try/catch, and release the semaphores in the catch.
      // Hope that works *fingers crossed*
      try
      {
        // child to close old stdio
        ::close (STDIN_FILENO );
        ::close (STDOUT_FILENO);
        ::close (STDERR_FILENO);

        if ( interactive_ )
        {
          // child to close unneeded pipe ends
          ::close (channel_in [1]);
          ::close (channel_out[0]);
          ::close (channel_err[0]);

          // dup other ends to stdio channels
          if ( -1 == ::dup2 (channel_in [0], STDIN_FILENO ) ||
               -1 == ::dup2 (channel_out[1], STDOUT_FILENO) ||
               -1 == ::dup2 (channel_err[1], STDERR_FILENO) )
          {
            log ("dup2 failed", strerror (errno));
            SAGA_ADAPTOR_THROW_NO_CONTEXT (std::string ("dup2 failed: ") + strerror (errno),
                                           saga::NoSuccess);
          }
        }

        // wait until the parent has set up the io channels
        if ( 0 != pthread_mutex_lock (mtx_1_) )
        {
          SAGA_ADAPTOR_THROW_NO_CONTEXT (std::string ("could not lock mutex: ") + strerror (errno),
                                         saga::NoSuccess);
        }

        // clean up mutex
        // log ("closing sem 1");
        if ( ! pthread_mutex_destroy (mtx_1_) )
        {
          // who cares? :-P
        }


        // chdir
        if ( pwd_ != "" )
        {
          if ( -1 == ::chdir (pwd_.c_str ()) )
          {
            log ("chdir failed", strerror (errno));
            SAGA_ADAPTOR_THROW_NO_CONTEXT (std::string ("chdir failed: ") + strerror (errno),
                                           saga::NoSuccess);
          }
        }

        std::vector <std::string> args;

        if ( jd_.attribute_exists 
                (saga::job::attributes::description_arguments ) )
        {
          args = jd_.get_vector_attribute 
                  (saga::job::attributes::description_arguments);

        }

        nargs = args.size () + 1;
        cargs = (char**) ::calloc (nargs + 1, sizeof (char*));

        if ( NULL == cargs )
        {
          log ("calloc failed", strerror (errno));
          SAGA_ADAPTOR_THROW_NO_CONTEXT (std::string ("calloc failed: ") + strerror (errno),
                                         saga::NoSuccess);
        }


        cargs[0] = ::strdup (exe_.c_str ());

        if ( NULL == cargs[0] )
        {
          log ("strdup failed", strerror (errno));
          SAGA_ADAPTOR_THROW_NO_CONTEXT (std::string ("strdup failed: ") + strerror (errno),
                                         saga::NoSuccess);
        }


        std::string cmd (exe_);

        for ( unsigned int i = 1; i <= args.size (); i++ )
        {
          cargs[i] = ::strdup (args[i-1].c_str ());

          if ( NULL == cargs[i] )
          {
            log ("strdup failed", strerror (errno));
            SAGA_ADAPTOR_THROW_NO_CONTEXT (std::string ("strdup failed: ") + strerror (errno),
                                           saga::NoSuccess);
          }
          cmd += " ";
          cmd += cargs[i];
        }

        cargs[args.size () + 1] = NULL;

        log (" == running ", cmd);
        log ("    pwd     ", pwd_);

        // set up environment
        std::vector <std::string> env;
        if ( jd_.attribute_exists 
                (saga::job::attributes::description_environment ) )
        {
          env = jd_.get_vector_attribute (saga::job::attributes::description_environment);
        }

        for ( unsigned int j = 0; j < env.size (); j++ )
        {
          char* charptr = ::strdup (env[j].c_str ());
          ::putenv (charptr);

          log ("    env ", charptr);
        }

        // log (" === release sem_2_");
        if ( 0 != pthread_mutex_unlock (mtx_2_) )
        {
          SAGA_ADAPTOR_THROW_NO_CONTEXT (std::string ("could not unlock mutex: ") + strerror (errno),
                                         saga::NoSuccess);
        }
        // log (" === release sem_2_ ok");
      }
      catch ( const saga::exception & e )
      {
        if ( 0 != pthread_mutex_unlock (mtx_2_) )
        {
          log ("could not unlock mutex", strerror (errno));
        }
        log ("bye");
        ::exit (121);
      }
      catch ( const std::exception & e )
      {
        if ( 0 != pthread_mutex_unlock (mtx_2_) )
        {
          log ("could not unlock mutex", strerror (errno));
        }
        log ("bye bye");
        ::exit(121);
      }
      catch ( ... )
      {
        if ( 0 != pthread_mutex_unlock (mtx_2_) )
        {
          log ("could not unlock mutex",  strerror (errno));
        }
        log ("something bad happened to the child before execv");
        ::exit(121);
      }

      // finally, run the child program
      ::execvp (exe_.c_str (), cargs);

      // Nononononooooo - thats wrong!
      log ("execvp failed", ::strerror (errno));

      // cargs not needed anymore
      for ( int i = 0; i < nargs; i++ )
      {
        free (cargs[i]);
      }
      free (cargs);
      
      // we cannot throw here.  Well, we *can*, but remember, this is another
      // process (we are after fork), so the parent process will not see the
      // exception.  Instead, we write the error into a temp file, and hope the
      // parent can use it.  We signal the error with exit value -1.
      // Note: the parent needs to clean up the tmp file.

      std::string msg ("execvp failed (");
      msg += exe_ + "): " + ::strerror (errno);

      std::stringstream name;
      name << "/tmp/saga_job_tmp_msg." << ::getpid ();

      std::fstream tmp;
      tmp.open (name.str ().c_str (), std::fstream::out);
      tmp << msg << std::endl;
      tmp.close ();

      std::cerr << msg << std::endl << std::flush;

      // SAGA_ADAPTOR_THROW_NO_CONTEXT(msg.c_str (), saga::NoSuccess);

      // secret exit code nobody else is using for sure!
      ::exit (121);
    }
    else
    {
      // me parent
      log (" === parent pid: ", (int) getpid ());
      log (" === child  pid: ", (int) child_);

      // ignore SIGPIPE
      if ( SIG_ERR == ::signal (SIGPIPE, SIG_IGN) )
      {
        log ("signal failed", strerror (errno));
        SAGA_ADAPTOR_THROW_NO_CONTEXT (std::string ("signal failed: ") + strerror (errno),
                                       saga::NoSuccess);
      }


      log ("interactive ", interactive_);
      log ("pull_for_io ", pull_for_io_);

      // parent to close unneeded pipe ends
      if ( interactive_ )
      {
        ::close (channel_in [0]);
        ::close (channel_out[1]);
        ::close (channel_err[1]);

        if ( pull_for_io_ )
        {
          log ("going to create io thread");
          if ( 0 != pthread_create (&thread_io_, NULL, impl::thread_startup_io, this) )
          {
            log ("ptherad create for io thread failed", strerror (errno));
            SAGA_ADAPTOR_THROW_NO_CONTEXT (std::string ("pthread_create failed: ") + strerror (errno),
                                           saga::NoSuccess);
          }
        }
      }

      log ("posix job impl: start waiting thread");
      pull_wait_ = true;

      if ( 0 != pthread_create (&thread_wait_, NULL, impl::thread_startup_wait, this) )
      {
        log ("ptherad create for wait thread failed", strerror (errno));
        SAGA_ADAPTOR_THROW_NO_CONTEXT (std::string ("pthread_create failed: ") + strerror (errno),
                                       saga::NoSuccess);
      }


      threads_running_ = true;

      // io channels and waitpid thread are set up, so we can
      // release the semaphore and allow the child to exec the
      // application
      if ( 0 != pthread_mutex_unlock (mtx_1_) )
      {
        SAGA_ADAPTOR_THROW_NO_CONTEXT (std::string ("could not unlock mutex: ") + strerror (errno),
                                       saga::NoSuccess);
      }

      // wait for child to run execve.  We have to set Running before the
      // sem_wait, because of another race condition which would occure
      // otherwise: the job may finish so fast that the state is not changed
      // here yet, causing trouble for waitpid.  But setting it before means
      // that the job may marked Running before execve occurred - that is bad if
      // execve fails, as the job itself never rans at all.  The falsely
      // reported runtime starts at setting state here, and finishes when
      // waitpid realizes that the child finishes (w/o a successfull execve).
      set_state (saga::job::Running);
      if ( 0 != pthread_mutex_lock (mtx_2_) )
      {
        SAGA_ADAPTOR_THROW_NO_CONTEXT (std::string ("could not lock mutex: ") + strerror (errno),
                                       saga::NoSuccess);
      }

      // clean up the semaphore
      if ( ! pthread_mutex_destroy (mtx_2_) )
      {
        // who cares? :-P
      }
    }

    return;
  }

  void * posix_job::run_thread_io (void)
  {
    log ("pulling for io?");
    if ( ! pull_for_io_ )
    {
      log ("pulling for io? no!");
      return NULL;
    }
    log ("pulling for io? yes!");

    // Now the parent collects the stdio from the child.
    // This thread runs until pull_io_ is set to false
    pull_io_killed_ = false;

    // init the io collecting strings
    stdin_log_  = "";
    stdout_log_ = "";
    stderr_log_ = "";

    // first, set channels to non blocking IO
    try 
    {
      int flags;

      log ("before fcntl");
      if ( 0 > (flags = fcntl (stdin_,  F_GETFL, 0)                  ) ) throw 1;
      if ( 0 > (        fcntl (stdin_,  F_SETFL, flags | O_NONBLOCK) ) ) throw 1;

      if ( 0 > (flags = fcntl (stdout_, F_GETFL, 0)                  ) ) throw 1;
      if ( 0 > (        fcntl (stdout_, F_SETFL, flags | O_NONBLOCK) ) ) throw 1;

      if ( 0 > (flags = fcntl (stderr_, F_GETFL, 0)                  ) ) throw 1;
      if ( 0 > (        fcntl (stderr_, F_SETFL, flags | O_NONBLOCK) ) ) throw 1;
      log ("after fcntl");
    }
    catch ( const std::exception & e )
    {
      log ("fcntl failed", strerror (errno));

      // we don't wait for pending stdio 
      pull_io_        = false;
      pull_io_killed_ = true;
      close ();

      SAGA_ADAPTOR_THROW_NO_CONTEXT (std::string ("fcntl failed: ") + strerror (errno),
                                     saga::NoSuccess);
    }

    int wmax = stdin_ + 1;
    int rmax = stdout_ > stderr_ ? stdout_ + 1 : stderr_ + 1;
    int amax = wmax    > rmax    ? wmax : rmax;

    // log ("pulling io");
    // log ("pull_for_io_in_ ", pull_for_io_in_ );
    // log ("pull_for_io_out_", pull_for_io_out_);
    // log ("pull_for_io_err_", pull_for_io_err_);


    // if ( pull_for_io_in_  ) log (" IN !"); else log ("!IN");
    // if ( pull_for_io_out_ ) log (" OUT!"); else log ("!OUT");
    // if ( pull_for_io_err_ ) log (" ERR!"); else log ("!ERR");

    if ( ! ( interactive_ || pull_for_io_in_ || pull_for_io_out_ || pull_for_io_err_ ) )
    {
      log ("pulling io - not!");
      pull_io_ = false;
    }

    // read stdio as long as
    //  - master thread says I should
    //  - stdout/stderr is available, and we did not get killed
    while ( pull_io_ || 
            ( ! pull_io_killed_ && (pull_for_io_in_ ||
                                    pull_for_io_out_ ||
                                    pull_for_io_err_ ) ) )
    {
      // log ("waiting for io");

      // log ("pull_for_io_in_ ", pull_for_io_in_ );
      // log ("pull_for_io_out_", pull_for_io_out_);
      // log ("pull_for_io_err_", pull_for_io_err_);

      // do a waiting select 
      struct timeval timeout;
      timeout.tv_sec  = 0;
      timeout.tv_usec = 100000; // 0.1 sec

      // select on all channels
      fd_set  wset; // read  fds
      fd_set  rset; // write fds
      fd_set  aset; // all   fds

      FD_ZERO (&wset);
      FD_ZERO (&rset);
      FD_ZERO (&aset);

      if ( pull_for_io_in_  ) FD_SET  (stdin_,  &wset);

      if ( pull_for_io_out_ ) FD_SET  (stdout_, &rset);
      if ( pull_for_io_err_ ) FD_SET  (stderr_, &rset);

      if ( pull_for_io_in_  ) FD_SET  (stdin_,  &aset);
      if ( pull_for_io_out_ ) FD_SET  (stdout_, &aset);
      if ( pull_for_io_err_ ) FD_SET  (stderr_, &aset);

      int ret = ::select (amax, &rset, &wset, &aset, &timeout);
  //  int ret = ::select (amax, &rset, &wset, &aset, NULL); // block

      if ( ret < 0 )
      {
        log ("select failed", strerror (errno));
        set_state (saga::job::Unknown);

        // we don't wait for pending stdio 
        pull_io_        = false;
        pull_io_killed_ = true;
        close ();
      }
      else if ( ret == 0 )
      {
        log ("select timed out");
      }
      else
      {
        log ("got io");

        if ( pull_for_io_in_ && 
             FD_ISSET (stdin_, &wset) )
        {
          log ("need stdin");
          std::string s;

          // real stdin is always available.  That means that we actually 
          // do a busy wait in this loop!  No good.  Thus we slow 
          // down stdin artificially, with a nanosleep.  
          //
          // Note that this may also slow dows stdin/stdout, as that is 
          // handled in the same loop!
          //
          // FIXME: the delay should be a configurable adaptor option.
          //
          // we either get stdin from file (redirect), or from in stream

          if ( stdin_redirect_ )
          {
            log ("redirect stdin");

            // read full lines, and rewrite to the process' stdin
            char   line[1026]; // 1024 + \n + \0
            size_t len, n;


            while ( f_stdio_in_.good () )
            {
              f_stdio_in_.getline (line, 1024);

              len = ::strlen (line);

              if ( len > 0 )
              {
                // readd line break
                line[len    ] = '\n';
                line[len + 1] = '\0';
                
                log ("write one stdin line to process", line);

                n = ::write (stdin_, line, len + 1);

                if ( n == 0 )
                {
                  // if we wait for trailing io, but read 0 bytes, 
                  // stop waiting
                  if ( n == 0 && pull_io_killed_ == false )
                  {
                    pull_io_killed_ = true;
                  }
                }
                else if ( n > 0 )
                {
                  // no need for sleep here - we simply read all and redirect as
                  // fast as we can.  The assumption here is that stdin and
                  // stdout/stderr will not block each other - but that seems
                  // unlikely in the case of redirection anyway.
                }
                else
                {
                  log ("write to stdin_ failed", strerror (errno));
                  pull_for_io_in_ = false;
                }
              }
            }

            // while (good) ended
            log ("found end of stdin - closing");
            pull_for_io_in_ = false;

            // we send EOF to the process, so it knows about stdin closing
            ::close (stdin_);
          }
          else // ! stdin_redirect_
          {
            if ( pull_for_io_in_ )
            {
              log ("write one stdin line to process", s);

              size_t n = ::write (stdin_, s.c_str (), s.length ());

              if ( n == 0 )
              {
                // if we wait for trailing io, but read 0 bytes, stop waiting
                if ( n == 0 && pull_io_killed_ == false )
                {
                  pull_io_killed_ = true;
                }
              }
              else if ( n > 0 )
              {
                stdin_log_ += s;

                // delay for nanosleep in sec
                static struct timespec delay;
                static struct timespec remain;

                delay.tv_sec  = 0;
                delay.tv_nsec = (long int) (WAIT_DELAY * 1000000000);  // < 10^9

                ::nanosleep (&delay, &remain);
              }
              else
              {
                log ("write to stdin_ failed", strerror (errno));
                pull_for_io_in_ = false;
              }
            }
          }

          // ::sleep (1);
        }

        if ( pull_for_io_out_ && 
             FD_ISSET (stdout_, &rset) )
        {
          log ("got stdout ----------------------------------------");
          char* buf = new char[256];
          size_t n   = ::read (stdout_, buf, 255);

          if ( n == 0 )
          {
            // if we wait for trailing io, but read 0 bytes, stop waiting
            if ( n == 0 && pull_io_killed_ == false )
            {
              pull_io_killed_ = true;
            }
          }
          else if ( n > 0 )
          {
            buf[n]       = '\0';
            stdout_log_  += buf;
            f_stdio_out_ << buf << std::flush;

            log ("buffer", buf);
            log ("bufsize", int (n));
          }
          else
          {
            log ("read from stdout_ failed", strerror (errno));
            pull_for_io_out_ = false;
          }

          delete (buf);
        }

        if ( pull_for_io_err_ && 
             FD_ISSET (stderr_, &rset) )
        {
          log ("got stderr");
          char* buf = new char[256];
          size_t n   = ::read (stderr_, buf, 255);

          if ( n == 0 )
          {
            // if we wait for trailing io, but read 0 bytes, stop waiting
            if ( n == 0 && pull_io_killed_ == false )
            {
              pull_io_killed_ = true;
            }
          }
          else if ( n > 0 )
          {
            buf[n]        = '\0';
            stderr_log_  += buf;
            f_stdio_err_ << buf << std::flush;

            log ("buffer", buf);
            log ("bufsize", int (n));
          }
          else
          {
            log ("read from stderr_ failed", strerror (errno));
            pull_for_io_err_ = false;
          }
          delete (buf);
        }

        if ( FD_ISSET (stdin_,  &aset) ) { log ("in  failed"); }
        if ( FD_ISSET (stdout_, &aset) ) { log ("out failed"); }
        if ( FD_ISSET (stderr_, &aset) ) { log ("err failed"); }
      }


      // re-check if we still need to do any i.o
      if ( pull_for_io_in_  ) log (" in !"); else log ("!in");
      if ( pull_for_io_out_ ) log (" out!"); else log ("!out");
      if ( pull_for_io_err_ ) log (" err!"); else log ("!err");

      if ( ! ( pull_for_io_in_ || pull_for_io_out_ || pull_for_io_err_ ) )
      {
        log ("shutting down io");
        pull_io_ = false;
      }
    }

    log ("pulling io done");

    // FIXME: dumping stdin_log seems to crash 
    //        when the string gets too large.
    log ("--------------------\nSTDIN");
    log (stdin_log_);
    log ("--------------------\nSTDOUT");
    log (stdout_log_);
    log ("--------------------\nSTDERR");
    log (stderr_log_);
    log ("--------------------");

    pthread_exit (NULL);
  }


  void * posix_job::run_thread_wait (void)
  {
    if ( -1 == child_ )
    {
      log ("not waiting for ", child_);
      pthread_exit (NULL);
    }

    while ( pull_wait_ )
    {
      // check if child has exited
      log ("waiting for ", child_);
      logn (".");

      int status = 0;
      int flags  = WNOHANG;

      // try to catch state changes
      if ( ! is_reconnected_ )
      {
        flags |= WUNTRACED;
      }

#ifdef WCONTINUED
      flags |= WCONTINUED;
#endif

      int retval = ::waitpid (child_, &status, flags);

      if ( -1 == retval )
      {
        if ( EINTR == errno )
        {
          // ignore and simply try again
        }
        else if ( ECHILD == errno )
        {
          // FIXME: can't distinguish between done and failed here :-(
          set_state (saga::job::Done);
          exit_code_ = 0;

          // tell io thread to stop listening for io soonish
          pull_wait_      = false;
          pull_io_killed_ = true;   // don't bother about pending stdio

          close ();

          log ("child disappeared - assume its done");
        }
        else
        {
          log ("waitpid failed", strerror (errno));

          // FIXME: can't distinguish between done and failed here :-(
          set_state (saga::job::Done);
          exit_code_ = 0;

          // don't bother listening further
          pull_wait_      = false;
          pull_io_killed_ = true; // don't bother about pending stdio

          close ();
        }
      }
      else if ( 0 == retval )
      {
     // log ("child unchanged (WNOHANG)");
        logn ("_");

        static struct timespec delay;
        static struct timespec remain;

        delay.tv_sec  = 0;
        delay.tv_nsec = (long int) (WAIT_DELAY * 1000000000);  // < 10^9

        ::nanosleep (&delay, &remain);
      }
      else
      {
        // was the child suspended?
        if ( WIFSTOPPED (status) )
        {
          set_state (saga::job::Suspended);
          log ("child stopped");
        }

#ifdef WCONTINUED
        // was the child continued?
        else if ( WIFCONTINUED (status) )
        {
          set_state (saga::job::Running);
          log ("child continued");
        }
#endif

        // so, got SIGCHLD?
        else if ( WIFEXITED (status) )
        {
          // done
          // FIXME: exit status should go into the jobs attrib (as should go
          // signum etc)
          exit_code_ = WEXITSTATUS (status);

          if ( 0 == exit_code_ )
          {
            set_state (saga::job::Done);
          }
          else
          {
            set_state (saga::job::Failed);
          }

          // tell io thread to stop listening for io soonish
          pull_wait_      = false;
          pull_io_killed_ = false;  // collect pending stdio

          log ("child exited", exit_code_);

          // it also happens that the child's execvp failed - catch that here
          if ( exit_code_ == 121 )
          {
            // try to give a sensible error message
            bool have_error = false;
            char msg[1024]  = "";
            try 
            {
              std::fstream      tmp;
              std::stringstream name;
              
              name << "/tmp/saga_job_tmp_msg." << child_;

              const char * name_s = name.str ().c_str ();

              tmp.open    (name_s, std::fstream::in);
              tmp.getline (msg, 1024);
              tmp.close   ();

              if ( ! ::unlink (name_s) )
              {
                SAGA_VERBOSE(SAGA_VERBOSE_LEVEL_WARNING)
                {
                  std::cerr << "unlink of tmpfile " << name_s 
                            << " failed: " << ::strerror (errno) 
                            << std::endl;
                }
              }

              have_error = true;
            }
            catch ( ... )
            {
              // hmm, status is 255, but no logfile? hmmm...
              log ("exit val is 255, but can't find error log");
            }

            if ( have_error )
            {
              // FIXME: where do we store the error??
              log ("job start error caught", msg);
            }
          } // exit == 121
        }

        // was the child signaled?
        else if ( WIFSIGNALED (status) )
        {
          // dead - stop listening
          set_state (saga::job::Canceled);
          pull_io_killed_ = true;  // won't find pending io
          pull_wait_      = false;

          close ();

          log ("child killed ", WTERMSIG (status));
        }

        // why were we called???
        else
        {
          log ("waitpid oops", status);

          // FIXME: can't distinguish between done and failed here :-(
          set_state (saga::job::Done);
          exit_code_ = 0;

          pull_wait_      = false;
          pull_io_        = false;
          pull_io_killed_ = true;  // won't find pending io

          close ();

          SAGA_ADAPTOR_THROW_NO_CONTEXT ("waitpid failed", saga::NoSuccess);
        }
      }

      // always check for cancelation
      pthread_testcancel ();
    }

    log ("stop waiting for ", child_);

    pthread_exit (NULL);
  }


  int posix_job::get_stdin  (void)
  {
    if ( ! interactive_ )
    {
      SAGA_ADAPTOR_THROW_NO_CONTEXT (std::string ("job is not interactive"), saga::IncorrectState);
    }

    return (stdin_);
  }

  int posix_job::get_stdout (void)
  {
    if ( ! interactive_ )
    {
      SAGA_ADAPTOR_THROW_NO_CONTEXT (std::string ("job is not interactive"), saga::IncorrectState);
    }

    return (stdout_);
  }

  int posix_job::get_stderr (void)
  {
    if ( ! interactive_ )
    {
      SAGA_ADAPTOR_THROW_NO_CONTEXT (std::string ("job is not interactive"), saga::IncorrectState);
    }

    return (stderr_);
  }

  void posix_job::set_jd (saga::job::description jd)
  {
    jd_ = jd;
  }

  saga::job::description posix_job::get_jd (void)
  {
    return (jd_);
  }

  saga::job::state posix_job::get_state (void)
  {
    // final states are simply returned
    if ( state_ == saga::job::Canceled ||
         state_ == saga::job::Done     ||
         state_ == saga::job::Failed   )
    {
      return state_;
    }


    // for reconnected jobs, we get state info etc from ps
    if ( is_reconnected_ )
    {
      // MacOS is reporting Suspended incorrectly for reconnected jobs - so rely
      // on our own state management here
#ifdef SAGA_APPLE
      if ( state_ == saga::job::Suspended )
      {
        return state_;
      }
#endif


      // otherwise refresh state
      impl::tools::known_job kj = impl::tools::get_known_job (child_);

      // refresh state if there are any valid state infos.  If new state is
      // Unknown, but old state is not, we assume the job is Done.
      if ( kj.state == saga::job::Unknown &&
           state_   != saga::job::Unknown )
      {
        set_state (saga::job::Done);
      }
      else
      {
        set_state (kj.state);
      }
    }

    return state_;
  }

  void posix_job::set_state (saga::job::state s)
  {
    if ( dying_ )
    {
      return;
    }

    state_ = s;

    // saga::monitorable monitor (proxy_);
    // saga::adaptors::metric m  (monitor.get_metric (saga::job::metrics::state));
    // m.set_attribute           (saga::attributes::metric_value, state2string (s));
    // m.fire();

  }    

  pid_t posix_job::get_jobid (void)
  {
    return (child_);
  }

  void posix_job::set_jobid (pid_t pid)
  {
    child_ = pid;
  }

  pid_t posix_job::get_pid (void)
  {
    return (child_);
  }

  void posix_job::setup (void)
  {
    if ( setup_done_ )
    {
      return;
    }

    setup_done_ = true;

    if ( ! interactive_ )
    {
      log ("not interactive - no setup");
      return;
    }

    log ("setup pipes");

    // create io pipes
    if ( -1 == ::pipe (channel_in)  ||
         -1 == ::pipe (channel_out) ||
         -1 == ::pipe (channel_err) )
    {
      log ("pipe failed", strerror (errno));
      SAGA_ADAPTOR_THROW_NO_CONTEXT (std::string ("pipe failed: ") + strerror (errno),
                                     saga::NoSuccess);
    }

    // keep parent ends as child i/o channels
    stdin_  = channel_in [1];
    stdout_ = channel_out[0];
    stderr_ = channel_err[0];

  }

  void posix_job::signal (int signum)
  {
    log ("signalling", signum);

    if ( -1 == child_ )
    {
      log ("not signalling for ", child_);
      return;
    }


    // only send signals to jobs which are not in a final state
    if ( saga::job::Done     == state_ ||
         saga::job::Failed   == state_ ||
         saga::job::Canceled == state_ )
    {
      // FIXME: relax condition for now
      return;

      log ("can't send signal, job is already in final state");
      SAGA_ADAPTOR_THROW_NO_CONTEXT ("signal cannot be send to jobs in final state",
                                     saga::IncorrectState);
    }

    if ( -1 == ::kill (child_, signum) )
    {
      log ("kill(signal) failed", strerror (errno));
      SAGA_ADAPTOR_THROW_NO_CONTEXT (std::string ("kill(signal) failed: ") + strerror (errno),
                                     saga::NoSuccess);
    }

    log ("signalling done\n");
    return;
  }

  void posix_job::suspend (void)
  {
    // only suspend running jobs
    if ( saga::job::Running != state_ )
    {
      log ("can't suspend non-running jobs");
      SAGA_ADAPTOR_THROW_NO_CONTEXT ("cannot suspend non-running jobs",
                                     saga::IncorrectState);
    }

    signal (SIGSTOP);


    // wait for a couple (10) seconds for the signal to arrive, and the job to
    // go into suspended state.  If the job is reconnected, or self, we may not
    // see that happening (see waitpid manpage on WUNTRACED), but according to
    // the spec, we MUST throw an error here.
    //
    // FIXME: alas, that won't work for MacOS -- ps is broken here.  So we
    // simply rely on the signal to arrive, and set the job state ourself
#ifdef SAGA_APPLE
    set_state (saga::job::Suspended);
#else
    struct timeval start, stop;
    ::gettimeofday (&start, NULL);

    struct timeval timeout;
    timeout.tv_sec  = 0;
    timeout.tv_usec = 10000; // 1/100 second

    while ( saga::job::Suspended != state_ )
    {
      get_state ();

      ::gettimeofday (&stop, NULL);

      // after 10 seconds, we assume an error.
      if ( stop.tv_sec - start.tv_sec > 10 )
      {
        log ("couldn't suspend job");
        SAGA_ADAPTOR_THROW_NO_CONTEXT ("job suspend failed",
                                       saga::NoSuccess);
      }

      // otherwise sleep a while, and try again
      ::select (0, NULL, NULL, NULL, &timeout);
    }
#endif
    
    log ("job suspended");
  }


  void posix_job::resume (void)
  {
    // only resume suspended jobs
    if ( saga::job::Suspended != state_ )
    {
      log ("can't resume non-suspended jobs");
      SAGA_ADAPTOR_THROW_NO_CONTEXT ("cannot resume non-suspended jobs",
                                     saga::IncorrectState);
    }

    signal (SIGCONT);


    // wait for a couple (10) seconds for the signal to arrive, and the job to
    // leave the suspended state.  If the job is reconnected, or self, we may
    // not see that happening (see waitpid manpage on WUNTRACED), but according
    // to the spec, we MUST throw an error here.
    //
    // FIXME: alas, that won't work for MacOS -- ps is broken here.  So we
    // simply rely on the signal to arrive, and set the job state ourself
#ifdef SAGA_APPLE
    set_state (saga::job::Running);
#else
    struct timeval start, stop;
    ::gettimeofday (&start, NULL);

    struct timeval timeout;
    timeout.tv_sec  = 0;
    timeout.tv_usec = 10000; // 1/100 second

    while ( saga::job::Suspended == state_ )
    {
      get_state ();

      ::gettimeofday (&stop, NULL);

      // after 10 seconds, we assume an error.
      if ( stop.tv_sec - start.tv_sec > 10 )
      {
        log ("couldn't suspend job");
        SAGA_ADAPTOR_THROW_NO_CONTEXT ("job resume failed",
                                       saga::NoSuccess);
      }

      // otherwise sleep a while, and try again
      ::select (0, NULL, NULL, NULL, &timeout);
    }
#endif
    
    log ("job resumed");
  }


  void posix_job::cancel (void)
  {
    // only cancel running or suspended jobs
    //
    // FIXME: relax that requirement for now, to allow to free resources
    // note to self: should we have a close?
    //
    // if ( saga::job::Running   != state_ &&
    //      saga::job::Suspended != state_ )
    // {
    //   log ("can't cancel non-suspended/non-running jobs");
    //   SAGA_ADAPTOR_THROW_NO_CONTEXT ("cannot cancel non-running/non-suspended jobs",
    //                                  saga::IncorrectState);
    // }

    signal (SIGKILL);

    // if we run the waitpid thread for the job, that thread will set the
    // canceled state.  For reconnected jobs we have to do it here
    if ( is_reconnected_ )
    {
      set_state (saga::job::Canceled);
    }

    close ();
  }

  std::string posix_job::state2string (saga::job::state s)
  {
    switch ( s )
    {
      case saga::job::New:       return "New"      ; break;
      case saga::job::Running:   return "Running"  ; break;
      case saga::job::Suspended: return "Suspended"; break;
      case saga::job::Done:      return "Done"     ; break;
      case saga::job::Failed:    return "Failed"   ; break;
      case saga::job::Canceled:  return "Canceled" ; break;
      default:                   return "Unknown"  ; break;
    }
  }

  void * thread_startup_io (void * arg)
  {
    posix_job * j = (posix_job*) arg;
    return j->run_thread_io ();
  }

  void * thread_startup_wait (void * arg)
  {
    posix_job * j = (posix_job*) arg;
    return j->run_thread_wait ();
  }
} // namespace impl
////////////////////////////////////////////////////////////////////////

#endif // SAGA_WINDOWS

