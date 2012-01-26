//  Copyright (c) 2005-2008 Andre Merzky
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "saga-shell.hpp"

// exec tries to find the first argument in the path.  The host
// of the resulting URL is then used as target host for running
// that executable.  All other arguments are passed on.  IO
// streaming is enabled by default.

int shell::c_exec (std::vector <std::string> & args, 
                   props & p)
{
#if !defined(SAGA_HAVE_PACKAGE_JOB)
    carp("saga has been compiled without the job package, unable to use this command");
    return FAILURE;
#else
  // sanity checks
  if ( args.empty () )
  {
    return SUCCESS;
  }

  bool bg = false;

  // check if we run in backround
  if ( args.back () == "&" )
  {
    bg = true;
    args.erase ((++args.rbegin()).base());
  }

  // FIXME: find the command in PATH

  // create io streams for job io
  saga::job::ostream in;
  saga::job::istream out;
  saga::job::istream err;

  saga::job::job job;

  std::string command;

  std::vector <std::string> :: iterator begin = args.begin ();
  std::vector <std::string> :: iterator end   = args.end ();
  std::vector <std::string> :: iterator iter;

  for ( iter = begin; iter != end; ++iter )
  {
    command += *iter;
    command += " ";
  }

  try {
    job = js_.run_job (command, host_, in, out, err);
  }
  catch (std::exception const & e)
  {
    p.err += "run failed: ";
    p.err += e.what();
    p.err += "\n";
    return FAILURE;
  }

  // get job state
  saga::job::state state = job.get_state ();
  
  // check if that worked
  if ( state != saga::job::Running &&
       state != saga::job::Done    )
  {
    carp ("run failed: " + command, p);
    return FAILURE;
  }
  
  if ( bg )
  {
    // store background jobs in process table
    std::string jobid (job.get_job_id());
    int pid = jobs_.add (jobid, command, job);

    // output
    p.out += "  ["; 
    p.out += pid;
    p.out += "] ";
    p.out += command;
    p.out += "\n";
    p.out += "  " ; 
    p.out += jobid;
    p.out += "\n";
  }
  else
  {
    while ( true ) 
    {
      char buffer[255];

      // get stdout
      out.read (buffer, sizeof (buffer));
      err.read (buffer, sizeof (buffer));
      
      if ( out.gcount () > 0 )
      {
        p.out += std::string (buffer, out.gcount ());
      }
      
      if ( err.gcount () > 0 )
      {
        p.out += std::string (buffer, err.gcount ());
      }

      if ( out.fail () &&
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


