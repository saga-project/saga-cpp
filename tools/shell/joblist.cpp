//  Copyright (c) 2005-2008 Andre Merzky
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "saga-shell.hpp"

#include <iostream>

// helper class which maintains our process table

// c'tor
joblist::joblist (void)
  : last_pid_ (1)
{
}

// find smallest free pid, and store the jobid
int joblist::add (std::string jobid, 
                  std::string command, 
                  saga::job::job   job)
{
  entry_t e;

  int pid   = last_pid_++;

  e.pid     = pid;
  e.jobid   = jobid;
  e.command = command;
  e.job     = job;

  list_.push_back (e);

  return (pid);
}

// remove the entry for this pid
void joblist::del (int pid)
{
  list_t newlist;

  list_it_t it = list_.begin ();
  
  for ( ; it != list_.end (); ++it )
  {
    if ( pid != (*it).pid )
    {
      newlist.push_back (*it);
    }
  }

  list_ = newlist;
}

// return the jobid for that pid
std::string joblist::get_jobid (int pid)
{
  list_it_t it = list_.begin ();
  
  for ( ; it != list_.end (); ++it )
  {
    if ( pid == (*it).pid )
    {
      return ((*it).jobid);
    }
  }

  return "";
}

// return the command for that pid
std::string joblist::get_command (int pid)
{
  list_it_t it = list_.begin ();
  
  for ( ; it != list_.end (); ++it )
  {
    if ( pid == (*it).pid )
    {
      return ((*it).command);
    }
  }

  return "";
}

// return the job for that pid
saga::job::job joblist::get_job (int pid)
{
  list_it_t it = list_.begin ();
  
  for ( ; it != list_.end (); ++it )
  {
    if ( pid == (*it).pid )
    {
      return ((*it).job);
    }
  }

//   saga::job::job newjob;
  throw saga::exception ("no such job", saga::NoSuccess);
}

std::vector <int> joblist::list (void)
{
  std::vector <int> ret;

  list_it_t it = list_.begin ();
  
  for ( ; it != list_.end (); ++it )
  {
    ret.push_back ((*it).pid);
  }

  return (ret);
}

