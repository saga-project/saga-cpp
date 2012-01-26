//  Copyright (c) 2005-2008 Andre Merzky
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "saga-shell.hpp"
#include <boost/lexical_cast.hpp>

#include <errno.h>
#include <fcntl.h>

int shell::c_run (std::vector <std::string> & args,
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

  std::string command = exe + " ";

  std::vector <std::string> :: iterator begin = args.begin ();
  std::vector <std::string> :: iterator end   = args.end ();
  std::vector <std::string> :: iterator iter;

  for ( iter = begin; iter != end; ++iter )
  {
    command += *iter;
    command += " ";
  }


  // create io streams for job io
  saga::job::ostream in;
  saga::job::istream out;
  saga::job::istream err;

  saga::job::job job;

  try {
    job = js.run_job (command, host_, in, out, err);
  }
  catch (std::exception const & e)
  {
    p.err += "run failed: ";
    p.err += e.what();
    p.err += "\n";
    return FAILURE;
  }
  catch (...) {
    std::cerr << "ARGHH... very bad." << std::endl;
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
      carp ("run failed: " + command + 
            ", state: " + saga::job::detail::get_state_name(state) + 
            ", exitcode: " + exitcode, p);
    }
    else
    {
      carp ("run failed: " + command +
            ", state: " + saga::job::detail::get_state_name(state), p);
    }
    return FAILURE;
  }
  
  if ( bg )
  {
    // store background jobs in process table
    std::string jobid (job.get_job_id());
    int pid = jobs_.add (jobid, command, job);
    saga::job::state state = job.get_state ();

    // output
    p.out += "  [";
    p.out += boost::lexical_cast<std::string> (pid);
    p.out += "] \t ";
    p.out += jobid;
    p.out += "  \t ";
    p.out += " [";
    p.out += state_to_string (state);
    p.out += "] \t ";
    p.out += command;
    p.out += "\n";
  }
  else
  {
    while ( true ) 
    {
      bool is_new = false;

      if ( is_new )
      {
        sleep (1);
        // handle stdin
        ::fcntl (STDIN_FILENO,  F_SETFL, O_NONBLOCK);

        std::string inbuf;
        std::getline (std::cin, inbuf);

        if ( std::cin.fail () )
        {
          std::cin.clear ();
          usleep (100000);
        }
        else
        {
          std::cout << "(" << inbuf << ")" << std::endl;
          in << inbuf << std::endl;
        }


        std::string outbuf;
        std::string errbuf;
        
        std::getline (out, outbuf);
        std::getline (err, errbuf);
        
        if ( out.fail () )
        {
          out.clear ();
        }
        else
        {
          p.out += outbuf;
        }
        
        if ( err.fail () )
        {
          err.clear ();
        }
        else
        {
          p.out += errbuf;
        }
      }
      else // if is_new
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
    }
    p.out += "\n";
  }

  return SUCCESS;
#endif
}


