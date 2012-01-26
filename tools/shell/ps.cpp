//  Copyright (c) 2005-2008 Andre Merzky
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "saga-shell.hpp"

// list all running/suspended jobs
int shell::c_ps (std::vector <std::string> & args,
                 props & p)
{
#if !defined(SAGA_HAVE_PACKAGE_JOB)
    carp("saga has been compiled without the job package, unable to use this command");
    return FAILURE;
#else

  // -A / -e list all jobs known to the job service
  if ( args[0] == "-A" ||
       args[0] == "-e" )
  {
    std::vector <std::string> jobs = js_.list ();

    std::vector <std::string> :: iterator begin = jobs.begin ();
    std::vector <std::string> :: iterator end   = jobs.end ();
    std::vector <std::string> :: iterator iter;

    for ( iter = begin; iter != end; iter++ )
    {
      saga::job::job         j = js_.get_job (*iter);
      saga::job::description d = j.get_description ();
      std::string            e = "?";
      
      if ( d.attribute_exists (saga::job::attributes::description_executable) )
      { 
        e = d.get_attribute (saga::job::attributes::description_executable);
      }

      p.out += *iter;
      p.out += " \t ";
      p.out += e;
      p.out += "\n";
    }
    p.out += "\n";
  }
  else
  {
    // list only known shell jobs
    std::vector <int> ids;

    if ( args.empty () )
    {
      // get list of jobs
      ids = jobs_.list ();
    }
    else
    {
      // build list from args
      std::vector <std::string> :: iterator begin = args.begin ();
      std::vector <std::string> :: iterator end   = args.end ();
      std::vector <std::string> :: iterator iter;

      for ( iter = begin; iter != end; ++iter )
      {
        ids.push_back (atoi (iter->c_str ()));
      }
    }


    // iterate list
    for ( std::size_t i = 0; i < ids.size (); i++ )
    {
      int pid = ids[i];

      // get details from process table
      std::string jobid   = jobs_.get_jobid   (pid);
      std::string command = jobs_.get_command (pid);
      saga::job::job job  = jobs_.get_job     (pid);

      // get job state
      saga::job::state state = job.get_state     ();

      std::stringstream pid_ss;
      pid_ss << pid;

      // show what we found
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

      // purge old (== final) entries
      if ( state == saga::job::Done     ||
           state == saga::job::Failed   ||
           state == saga::job::Canceled ||
           state == saga::job::Unknown )
      {
        jobs_.del (pid);
      }
    }
  }

  return SUCCESS;
#endif
}

