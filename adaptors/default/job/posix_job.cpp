//  Copyright (c) 2005-2007 Hartmut Kaiser 
//  Copyright (c) 2005-2007 Andre Merzky   (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. 
//  (See accompanying file LICENSE or copy at 
//   http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga-defs.hpp>

#ifndef SAGA_SKIP_CONFIG_HPP
# include "saga/saga-defs.hpp"
#endif

#ifndef SAGA_WINDOWS

// C includes 
#include <signal.h>
#include <errno.h>
#include <limits.h>
#include <unistd.h>
#include <sys/stat.h>

#ifdef SAGA_APPLE
#include <crt_externs.h>
#define environ (*_NSGetEnviron())
#elif !defined(BOOST_WINDOWS)
extern "C" {
  extern char **environ;
};
#endif

#ifndef  MAX_PATH
# define MAX_PATH _POSIX_PATH_MAX
#endif

// saga includes
#include <saga/saga.hpp>

// saga adaptor icnludes
#include <saga/saga/adaptors/task.hpp>
#include <saga/saga/adaptors/attribute.hpp>
#include <saga/saga/adaptors/file_transfer_spec.hpp>

// saga engine includes
#include <saga/impl/config.hpp>
#include <saga/impl/exception_list.hpp>

// saga package includes
// saga package includes
#include <saga/saga/packages/job/adaptors/job.hpp>
#include <saga/saga/packages/job/adaptors/job_self.hpp>
#include <saga/saga/packages/job/job_description.hpp>

// adaptor includes
#include "default_job_adaptor.hpp"
#include "posix_job.hpp"
#include "posix_job_iostream.hpp"

// constructor
job_cpi_impl::job_cpi_impl (proxy                           * p, 
                            cpi_info const                  & info,
                            saga::ini::ini const            & glob_ini, 
                            saga::ini::ini const            & adap_ini,
                            TR1::shared_ptr <saga::adaptor>   adaptor)
  : base_cpi  (p, info, adaptor, cpi::Noflags),
    session_  (p->get_session ())
{
  log ("posix job c'tor");

  instance_data     idata (this);
  adaptor_data_type adata (this);

  // check if we have a rm url.  If yes, check if we are asked for.
  if ( ! idata->rm_.get_string ().empty () )
  {
    // initialize our service URL
    url_ = idata->rm_;


    if ( ! url_.get_scheme ().empty ()      && 
           url_.get_scheme () != "fork"     && 
           url_.get_scheme () != "any"      )
    {
      SAGA_OSSTREAM strm;
      strm << "Could not initialize job for [" << idata->rm_ << "]. " 
           << "Only these schemas are supported: any://, fork://, none.";

      SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), 
                         saga::adaptors::AdaptorDeclined);
    }


    // check if URL is usable
    if ( ! url_.get_host ().empty ()                                 &&
         ! saga::adaptors::utils::is_local_address (url_.get_host ()) )
    {
      SAGA_OSSTREAM strm;
      strm << "Could not initialize job for [" << idata->rm_ << "]. " 
           << "Only 'localhost' and " << adata->localhost ()<< " are supported.";

      if ( url_.get_scheme () == "fork" )
      {
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), 
                           saga::BadParameter);
      }
      else
      {
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), 
                           saga::adaptors::AdaptorDeclined);
      }
    }

    if ( ! url_.get_port      () == -1         ||
         ! url_.get_fragment  ().empty ()      ||
         ! ( url_.get_path      ().empty () ||
             url_.get_path      () == "/"   )  ||
         ! url_.get_userinfo  ().empty ()      ||
         ! url_.get_query     ().empty ()      ||
         ! url_.get_username  ().empty ()      ||
         ! url_.get_password  ().empty ()      )
    {
      SAGA_OSSTREAM strm;
      strm << "Could not initialize job service for [" << idata->rm_ << "]. " 
           << "Only these schemas are supported: any://, fork://, none. " 
           << "Other URL components are invalid";
      
      if ( url_.get_scheme () == "fork" )
      {
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), 
                           saga::BadParameter);
      }
      else
      {
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), 
                           saga::adaptors::AdaptorDeclined);
      }
    }
  }
  else
  {
    // if we don't have an URL at all, we accept
  }

  log ("posix job ctor 1");

  SAGA_LOG_INFO("posix job ctor");



  if ( idata->init_from_jobid_ )
  {
    log ("posix job ctor from jobid");

    jobid_ = idata->jobid_;
    pid_   = jobid2pid (jobid_); 

    log ("init from jobid", jobid_);

    // try to find the job to connect to
    impl::tools::known_job kj = adata->known_job_find (pid_);

    log ("posix job ctor from jobid ok");
    // fill job description
    std::vector <std::string> chosts;
    chosts.push_back (adata->localhost ());

    jd_.set_attribute (saga::job::attributes::description_executable,  kj.exe);
    jd_.set_attribute (saga::job::attributes::description_interactive, saga::attributes::common_false);

    jd_.set_vector_attribute (saga::job::attributes::description_arguments,       kj.args);
    jd_.set_vector_attribute (saga::job::attributes::description_candidate_hosts, chosts);

    // recreate job (impl is not kept in known job kj
    impl_ = impl_t (new impl::posix_job (this->proxy_, session_, jd_, pid_, kj.state));


    if ( pid_ != getpid () )
    { 
      log ("posix job ctor ! self");
      is_self_ = false; 
    }
    else
    {
      log ("posix job ctor self");
      // we are a job::self - fill some more into the job description
      is_self_ = true;


      // get cwd
      char a_cwd[MAX_PATH];
      if ( NULL != getcwd (a_cwd, MAX_PATH) )
      {
        jd_.set_attribute (saga::job::attributes::description_working_directory, a_cwd);
      }


      // get env
      std::vector <std::string> a_env;

      char ** ep;

      for ( ep = environ; *ep != NULL; ++ep )
      {
        std::string tmp (*ep);
        a_env.push_back (tmp);
      }

      jd_.set_vector_attribute (saga::job::attributes::description_environment, a_env);


      // we don't support interactive self's, as we very likely did not spawn
      // ourself ;-)
      jd_.set_attribute (saga::job::attributes::description_interactive,
                         saga::attributes::common_false); 
      
      log ("posix job ctor self ok");
    }

    // we inited from job id, and created job description - store it
    idata->jd_ = jd_;


    // set some default jd attribs
    interactive_ = jd_is_interactive (jd_);
  }
  else
  {
    log ("posix job ctor ! from jobid");
    // we don't init from jobid, this we init from job description

    // check if we can run on the candidate hosts
    if ( ! jd_is_local (jd_) )
    {
      log ("no localhost");
      SAGA_ADAPTOR_THROW ("Cannot submit to CandidateHosts.", 
                          saga::BadParameter);
    }

    // save the job description
    jd_ = idata->jd_;

    // set some default jd attribs
    interactive_ = jd_is_interactive (jd_);

    // create new job
    try 
    {
      // create a new impl job.  Note that this job may have io channels, but they
      // are not connected as of yet.  No jobid is available before run().  
      impl_ = impl_t (new impl::posix_job (this->proxy_, session_, jd_));

      log ("posix job ctor from jd ok");
    }
    catch ( const saga::exception & e )
    {
      log ("oops", e.what ());
      throw e;
    }
    catch ( ... )
    {
      log ("OOPS");
      SAGA_ADAPTOR_THROW ("could not instantiate posix job impl",
                          saga::NoSuccess);
    }
    log ("ctor done");
  }


  // FIXME: register metrics etc.
  log ("posix job ctor done");
}


// destructor
job_cpi_impl::~job_cpi_impl (void)
{
  log ("posix job d'tor");
}


//  SAGA API functions
// FIXME: how is an attribute_get on state triggereing a get_state() 
//        and an respective state update?
void job_cpi_impl::sync_get_state (saga::job::state & ret)
{
  ret = impl_->get_state ();
}

void job_cpi_impl::sync_get_description (saga::job::description & ret)
{
  log ("getting description");
  ret = jd_;
}

void job_cpi_impl::sync_get_job_id (std::string & ret)
{
  ret = jobid_;
}

// access streams for communication with the child
void job_cpi_impl::sync_get_stdin (saga::job::ostream & ret)
{
  log (" = stdin", impl_->get_stdin ());

  if ( ! interactive_ )
  {
    SAGA_ADAPTOR_THROW ("job is not interactive", saga::IncorrectState);
  }

  posix_job_adaptor_ostream tmp (this, impl_->get_stdin ());

  // write (tmp.get_handle (), "GIVING_AWAY_STDIN\n", 18);
  // tmp << "giving_away_stdin\n";

  ret = tmp;
}


void job_cpi_impl::sync_get_stdout (saga::job::istream & ret)
{
  log (" = stdout", impl_->get_stdout ());

  if ( ! interactive_ )
  {
    SAGA_ADAPTOR_THROW ("job is not interactive", saga::IncorrectState);
  }

  posix_job_adaptor_istream tmp (this, impl_->get_stdout ());
  ret = tmp;
}


void job_cpi_impl::sync_get_stderr (saga::job::istream & ret)
{
  log (" = stderr", impl_->get_stderr ());

  if ( ! interactive_ )
  {
    SAGA_ADAPTOR_THROW ("job is not interactive", saga::IncorrectState);
  }

  posix_job_adaptor_istream tmp (this, impl_->get_stderr ());
  ret = tmp;
}


// we assume that sending a SIGUSR1 triggers an application level 
// checkpoint.  The signal num should be an configurable adaptor 
// option (FIXME).
void job_cpi_impl::sync_checkpoint (saga::impl::void_t & ret)
{
  impl_->signal (SIGUSR1);
}


void job_cpi_impl::sync_migrate (saga::impl::void_t     & ret, 
                                 saga::job::description   jd)
{
  // migrate just forks a new version of myself...  

  // can only migrate to localhost
  if ( ! jd_is_local (jd) )
  {
    SAGA_ADAPTOR_THROW ("Cannot submit to CandidateHosts.", 
                        saga::BadParameter);
  }

  // migrated jobs are NEVER interactive
  if ( jd_is_interactive (jd) )
  {
    SAGA_ADAPTOR_THROW ("Cannot migrate interactive jobs.", 
                        saga::BadParameter);
  }

  // first checkpoint the job
  impl_->signal (SIGUSR1);

  // allow for some time to complete the checkpoint
  ::sleep (5);

  // restart my clone
  saga::job::job job = saga::adaptors::job ("fork://localhost",
                                            jd, 
                                            session_);
  job.run ();

  // this is the original process - the migrated one is up, so we have
  // actually two instances at the moment.  We should be able to safely shut
  // off the old one now.

#ifdef MIGRATE_KILLS
  log ("killing self after migrate");
  impl_->cancel ();

  // oops, suicide failed, still alive?  Hmm, thats a bug!
  SAGA_ADAPTOR_THROW ("could not finish self after spawning new instance.", 
                      saga::NoSuccess);
#else
  // FIXME: for now, dayinlife relies on the migrate not to kill, but to
  // simply return...
  log ("! killing self after migrate");
#endif


  // FIXME: note that the job id has changed now - we need to reflect this in
  // our new state.
  
  // FIXME: Also, we should now actually reconnect to that new job


  return;
}


void job_cpi_impl::sync_signal (saga::impl::void_t & ret, 
                                int            signal)
{
  impl_->signal (signal);
}


//  suspend the child process 
void job_cpi_impl::sync_suspend (saga::impl::void_t & ret)
{
  impl_->suspend ();
}


//  resume the child process 
void job_cpi_impl::sync_resume (saga::impl::void_t & ret)
{
  impl_->resume ();
}


//////////////////////////////////////////////////////////////////////
// inherited from the task interface
void job_cpi_impl::sync_run (saga::impl::void_t & ret)
{
  // retrieve jd, check it, and run.
  instance_data     idata (this);
  adaptor_data_type adata (this);

  if ( ! idata->jd_is_valid_ )
  {
    SAGA_ADAPTOR_THROW ("Job description cannot be retrieved.", 
                        saga::NotImplemented);
  }

  SAGA_LOG_INFO("posix job run");

  // retrieve and check saga job description for this host
  jd_ = idata->jd_;

  // set jd for impl_
  impl_->set_jd (jd_);


  std::string log_s (" >> ");

  if ( jd_.attribute_exists (saga::job::attributes::description_executable) )
  { 
    std::string exe = jd_.get_attribute (saga::job::attributes::description_executable);

    SAGA_LOG_INFO(exe.c_str ());

    log_s += exe;
  }

  if ( jd_.attribute_exists (saga::job::attributes::description_arguments) )
  {
    std::vector <std::string> args = jd_.get_vector_attribute (saga::job::attributes::description_arguments);

    for ( unsigned int i = 0; i < args.size (); i++ )
    {
      SAGA_LOG_INFO (args[i].c_str ());
      log_s += " " + args[i];
    }
  }

  SAGA_LOG_DEBUG (log_s.c_str ());

  if ( impl_->get_state () != saga::job::New )
  {
    SAGA_ADAPTOR_THROW ("run can only be called on New jobs.", 
                        saga::IncorrectState);
  }


  // this try/catch is not really necessary, as the exception would fall
  // through - but it addes the information available via the context...
  try 
  {
    impl_->run ();
  }
  catch ( const saga::exception & e )
  {
    log ("run triggered an exception:", e.what ());
    SAGA_ADAPTOR_THROW (e.what (), e.get_error ());
  }


  if ( impl_->get_state () != saga::job::Running   &&
       impl_->get_state () != saga::job::Suspended &&
       impl_->get_state () != saga::job::Done      &&
       impl_->get_state () != saga::job::Failed    )
  {
    // running failed and did not throw?  Should not happen
    SAGA_ADAPTOR_THROW ("run failed.", saga::NoSuccess);
  }

  // run was successful it seems, so we can now set some job attributes 
  // (ctime and jobid)
  saga::adaptors::attribute jobattr (this);

  std::time_t current = 0;
  std::time (&current);

  jobattr.set_attribute (saga::job::attributes::created, ctime (&current));
  
  jobid_  = adata->pid2jobid (impl_->get_jobid (), "localhost");

  jobattr.set_attribute (saga::job::attributes::jobid, jobid_);


  // register job in registry
  adata->known_job_register (impl_);
}


void job_cpi_impl::sync_cancel (saga::impl::void_t & ret, double timeout)
{
  impl_->cancel ();
}


// wait for the child process to terminate
void job_cpi_impl::sync_wait (bool   & ret, 
                              double   timeout)
{
  ret = false;

  saga::job::state s = impl_->get_state ();

  if ( saga::job::Done     == s ||
       saga::job::Failed   == s ||
       saga::job::Canceled == s )
  {
    ret = true;

    saga::adaptors::attribute attr (this);
    attr.set_attribute (saga::job::attributes::exitcode,
                        boost::lexical_cast <std::string> (impl_->get_exit_code ()));
    
    // update state in registry
    adaptor_data_type adata (this);
    adata->known_job_register (impl_);
    
    return;
  }

  double time = 0.0;

  while ( timeout < 0.0 || time < timeout )
  {
    struct timespec delay;
    struct timespec remain;

    // delay for nanosleep in sec
    double const wait_delay = 0.1;

    delay.tv_sec  = 0;
    delay.tv_nsec = (long int) (wait_delay * 1000000000);  // < 10^9

    time += wait_delay;

    while ( -1 == ::nanosleep (&delay, &remain) )
    {
      log ("wait state:", impl_->get_state ());

      if ( EINTR == errno )
      {
        // interrupted by signal - sleep again, if remain is valid
        if ( remain.tv_sec  > 0          &&                               
             remain.tv_nsec > 0          && 
             remain.tv_nsec < 1000000000 )
        {
          delay = remain;
        }
        else
        {
          break;
        }
      }
      else
      {
        std::stringstream ss;
        ss << "nanosleep failed: " << ::strerror (errno);
        SAGA_ADAPTOR_THROW (ss.str ().c_str (), saga::NoSuccess);
      }
    }

    saga::job::state s = impl_->get_state ();

    if ( saga::job::Done     == s ||
         saga::job::Failed   == s ||
         saga::job::Canceled == s )
    {
      ret = true;

      saga::adaptors::attribute attr (this);
      attr.set_attribute (saga::job::attributes::exitcode,
                          boost::lexical_cast <std::string> (impl_->get_exit_code ()));

      break;
    }
    else if ( saga::job::Unknown == s )
    {
      SAGA_ADAPTOR_THROW ("Invalid state Unknown detected for job",
                          saga::NoSuccess);
    }
  }


  {
    adaptor_data_type adata (this);
    
    // update state in registry
    adata->known_job_register (impl_);
  }
    
  return;
}

//////////////////////////////////////////////////////////////////////
pid_t job_cpi_impl::jobid2pid (std::string jobid)
{
  size_t start = 0;
  size_t end   = 0;

  std::string pid_str;
  pid_t       pid;

  start = jobid.find_first_of ("[", 2);
  end   = jobid.find_first_of ("]", start);

  if ( end != std::string::npos)
  {
    pid_str = jobid.substr (start + 1, end - start - 1);
  }

  std::istringstream pid_strstream (pid_str);

  pid = atoi (pid_str.c_str ());

  return (pid);
}


bool job_cpi_impl::jd_is_local (saga::job::description & jd)
{
  bool can_run = false;

  if ( jd.attribute_exists (saga::job::attributes::description_candidate_hosts) )
  { 
    adaptor_data_type adata (this);

    std::vector <std::string> chosts = jd.get_vector_attribute 
                                           (saga::job::attributes::description_candidate_hosts);

    for ( unsigned int i = 0; i < chosts.size (); i++ )
    {
      if ( "localhost" == chosts[i] ||
           adata->localhost () == chosts[i] )
      {
        can_run = true;
      }
    }
  }
  else
  {
    can_run = true;
  }

  return can_run;
}


bool job_cpi_impl::jd_is_interactive (saga::job::description & jd)
{
  if ( jd.attribute_exists (saga::job::attributes::description_interactive) )
  { 
    if ( saga::attributes::common_false == jd.get_attribute
         (saga::job::attributes::description_interactive) )
    {
      return false;
    }
    else
    {
      return true;
    }
  }

  jd.set_attribute (saga::job::attributes::description_interactive, saga::attributes::common_false);

  return false;
}


#endif // SAGA_WINDOWS

