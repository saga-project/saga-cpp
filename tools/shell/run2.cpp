//  Copyright (c) 2005-2008 Andre Merzky
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "saga-shell.hpp"
#include <boost/lexical_cast.hpp>

int shell::c_run2 (std::vector <std::string> & args,
                   props & p)
{
#if !defined(SAGA_HAVE_PACKAGE_JOB)
    carp("saga has been compiled without the job package, unable to use this command");
    return FAILURE;
#else
  // sanity checks
  if ( args.empty () )
  {
    carp ("format: run <exe> [args] ...", p);
    return FAILURE;
  }

  std::string exe = args[0];
  args.erase (args.begin());


  // check if exe is a complete URL.  If so, use the scheme and host part to
  // create a js instance to submit to.
  saga::url u (exe);
  saga::job::service js = js_;

  if ( ! u.get_scheme ().empty () ||
       ! u.get_host   ().empty () )
  {
    // create a new js
    js  = saga::job::service (u.get_scheme () + "://" + u.get_authority ());
    exe = u.get_path ();
  }


  // check if we run in background
  bool bg = false;
  if ( ! args.empty () && 
         args.back  () == "&" )
  {
    bg = true;
    args.erase ((++args.rbegin()).base());    // erase last element
  }

  // create io streams for job io
  saga::job::ostream in;
  saga::job::istream out;
  saga::job::istream err;

  saga::job::job job;

  try {
    saga::job::description jd;

    jd.set_attribute (saga::job::attributes::description_executable, exe);

    if ( ! bg )
    {
        jd.set_attribute (saga::job::attributes::description_interactive, 
            saga::attributes::common_true);
    }

    jd.set_vector_attribute (saga::job::attributes::description_arguments, 
                             args);

    job = js.create_job (jd);

    if ( ! bg ) 
    {
        in  = job.get_stdin  ();
        out = job.get_stdout ();
        err = job.get_stderr ();
    }
    job.run ();
  }

  catch (std::exception const & e)
  {
    p.out += "run failed: ";
    p.out += e.what();
    p.out += "\n";
    return FAILURE;
  }

  // get job state
  saga::job::state state = job.get_state ();

  // check if that worked
  if ( state != saga::job::Running &&
       state != saga::job::Done    )
  {
    std::string exitcode;
    if (job.attribute_exists(saga::job::attributes::exitcode))
    {
      exitcode = job.get_attribute(saga::job::attributes::exitcode);
      carp ("run failed: " + exe + 
            ", state: " + saga::job::detail::get_state_name(state) + 
            ", exitcode: " + exitcode, p);
    }
    else
    {
      carp ("run failed: " + exe +
            ", state: " + saga::job::detail::get_state_name(state), p);
    }
    return FAILURE;
  }

  if ( bg )
  {
    // store background jobs in process table
    std::string jobid (job.get_job_id());
    int pid = jobs_.add (jobid, exe, job);

    // output
    p.out += "  [";
    p.out += boost::lexical_cast<std::string>(pid);
    p.out += "] ";
    p.out += exe;
    p.out += "\n";
    p.out += "  ";
    p.out += jobid;
    p.out += "\n";
  }
  else
  {
    // a foreground job is pulled for its stdout
    while ( true ) 
    {
      char buffer[255];

      // get stdout
      out.read (buffer, sizeof (buffer));
      if ( out.gcount () > 0 )
      {
        p.out += std::string (buffer, out.gcount ());
      }


      // get stderr
      err.read (buffer, sizeof (buffer));
      if ( err.gcount () > 0 )
      {
        p.out += std::string (buffer, err.gcount ());
      }


      if ( out.fail () ||
           err.fail () ) 
      {
        break;
      }
    }

    p.out += "\n";
  }

  return SUCCESS;
#endif
}

