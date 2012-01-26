//  Copyright (c) 2005-2007 Andre Merzky   (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. 
//  (See accompanying file LICENSE or copy at 
//   http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga-defs.hpp>

#ifndef SAGA_WINDOWS

// C++ headers
#include <iostream>

// C headers
#include <fcntl.h>
#include <sys/stat.h>

#include <saga/saga.hpp>
#include <saga/saga/adaptors/utils.hpp>
#include <saga/impl/exception.hpp>

// private headers
#include "impl_posix_job_tools.hpp"

// FIXME: should be something including user id and pid... 
#define SAGA_SEMAPHORE_NAME  "/tmp.saga.ipc"


////////////////////////////////////////////////////////////////////////
namespace impl
{
  namespace tools
  {
    std::vector <std::string> split (std::string src, char del)
    {
      std::vector <std::string> elems;
      std::string               elem;

      for ( unsigned int i = 0; i < src.length (); i++ )
      {
        char c = src[i];

        if ( c != del )
        {
          // save any char which is not the delimiter
          elem += c;
        }
        else if ( elem.length () > 0 )
        {
          // store element if there is any content
          elems.push_back (elem);
          elem = "";
        }
      }

      if ( elem.length () > 0 )
      {
        // save any remaining content in another element
        elems.push_back (elem);
      }

      return (elems);
    }
    
    ////////////////////////////////////////////////////////////////////
    //
    // read a single loine from a file descriptor.  Returns an empty line on
    // read failure
    //
    std::string readline (FILE* fd)
    {
      char c[1];
      std::string line = "";

      size_t n = ::fread (&c, sizeof (char), 1, fd);

      while ( 1 == n )
      {
        // on EOL, return
        if ( c[0] == '\n' )
        {
          return (line);
        }

        // append any other char to line
        line += c[0];

        n = ::fread (&c, sizeof (char), 1, fd);
      }

      // on read error, return what we have
      return (line);
    }
    //
    ////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////
    //
    // dump a given job description
    //
    void dump_jd (saga::job::description jd, 
                  std::string id)
    {
      std::cout << "--- " << id << " --------------------------------------------------------" << std::endl;
      std::vector <std::string> attr = jd.list_attributes ();
    
      for ( unsigned int i = 0; i < attr.size (); i++ )
      {
        std::cout << "  " << attr[i] << "\t : ";
    
        if ( ! jd.attribute_is_vector (attr[i]) )
        {
          std::cout << jd.get_attribute (attr[i]);
        }
        else
        {
          std::vector <std::string> vals = jd.get_vector_attribute (attr[i]);
    
          for ( unsigned int j = 0; j < vals.size (); j++ )
          {
            std::cout << vals[j] << " ";
          }
        }
    
        std::cout << std::endl;
      }
      std::cout << "-------------------------------------------------------------" << std::endl;
    }
    // 
    ////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////
    //
    // run 'ps' through popen, and pick up all possibl job information.  Return
    // the results a map pid->known_job_t.
    //
    known_job_map_t get_known_jobs (void)
    {
      known_job_map_t ret;

      // pipe ps and filter output.  ps and args are POSIX.1.  This does not
      // hinder MacOS to report nonsense on the job state though :-(
#ifdef SAGA_APPLE
      FILE* fd = ::popen ("ps -ewwo pid=,uid=,state=,command=", "r");
#else                                       
      FILE* fd = ::popen ("ps -ewwo pid=,uid=,state=,cmd=", "r");
#endif

      if ( NULL == fd )
      {
        SAGA_LOG_WARN ("Could not popen, and thus the local job adaptor will"
                       "not be able to attach to running jobs");
        return ret; // return empty list
      }

      std::string line = impl::tools::readline (fd);

      while ( ! line.empty () )
      {
        std::vector <std::string> elems = impl::tools::split (line);

        // std::cout << line << std::endl;

        if ( elems.size () >= 4 )
        {
          pid_t pid = atoi (elems[0].c_str ());
          uid_t uid = atoi (elems[1].c_str ());

          // FIXME: need adaptor config option to switch off this filtering
          if ( 0   == ::getuid () ||
               uid == ::getuid () )
          {
            struct impl::tools::known_job kj;

            kj.pid = pid;
            kj.uid = uid;
            kj.sta = elems[2];
            kj.exe = elems[3];

            // translate state info
            //
            // the state information from PS are peculiar, at least on MacOS.  For
            // example, 'S+' is a sleeping process which is actually active, so has
            // called 'sleep()'.   See the ps manpage for more details

            // check if the state has a '+' modifier
            bool mod_plus = false;

            if ( kj.sta.find ('+') != std::string::npos )   
              mod_plus = true; 

            if ( kj.sta[0] == 'T' ) 
            {
              kj.state = saga::job::Suspended;
            }
            else if ( ( kj.sta[0] == 'R' ) ||
                      ( kj.sta[0] == 'I' ) ||
                      ( kj.sta[0] == 'U' ) ||
                      ( kj.sta[0] == 'S' ) )
            {
              kj.state = saga::job::Running;
            }
            else if ( ( kj.sta[0] == 'Z' ) )
            {
              // Zombie. this could also mean that the process failed - but we have
              // no means to distinguish the cases here.
              kj.state = saga::job::Done;
            }
            else
            {
              // no idea...
              kj.state = saga::job::Unknown;
            }


            // remaining elems are args
            for ( unsigned int i = 4; i < elems.size (); i++ )
            {
              kj.args.push_back (elems[i]);
            }

            kj.psline = line;

            // store job
            ret[pid] = kj;
          }
        }

        line = impl::tools::readline (fd);
      }

      ::pclose (fd);

      return ret;
    }
    //
    ////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////
    //
    // obtain information about a running job, via get_known_jobs() (see above)
    //
    known_job get_known_job (pid_t pid)
    {
      // check the jobs we get from ps
      known_job_map_t known_jobs = get_known_jobs ();

      if ( known_jobs.find (pid) != known_jobs.end () )
      {
        // if found, return it
        // std::cout << " ---- found known job for " << pid << std::endl;
        // known_jobs [pid].dump ();
        return known_jobs [pid];
      }

      // otherwise return a default kj
      known_job ret;

      ret.state = saga::job::Unknown;
      ret.pid   = pid;
      ret.uid   = ::getuid ();
      ret.sta   = "";
      ret.exe   = "";
      // ret.arg has an empty vec by default anyway
      
      // std::cout << " ---- no known job for " << pid << std::endl;
      // ret.dump ();

      return ret;
    }
    //
    ////////////////////////////////////////////////////////////////////
  }
  
} // namespace impl
  
#endif // SAGA_WINDOWS

