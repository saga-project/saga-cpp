//  Copyright (c) 2008 Ole Weidner
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <vector>
#include <string>

#include <saga/saga.hpp>
#include "saga-job.hpp"

#include <cstdlib>
#include <cstring>

///////////////////////////////////////////////////////////////////////////////

int main (int argc, char* argv[])
{
#if !defined(SAGA_HAVE_PACKAGE_JOB)

  std::cerr << "saga-job: saga has been compiled without the job "
               "package, bailing out" << std::endl;
  return -1;

#else

  saga_tools::common c;

  c.usage ().map ()["run"    ] = "<rm url> <command> <arguments>";
  c.usage ().map ()["submit" ] = "<rm url> <command> <arguments>";
  c.usage ().map ()["list"   ] = "<rm url>";
  c.usage ().map ()["state"  ] = "<rm url> <jobid> [...]";
  c.usage ().map ()["suspend"] = "<rm url> <jobid> [...]";
  c.usage ().map ()["resume" ] = "<rm url> <jobid> [...]";

  c.init (argc, argv);
  c.need_args_min (1);

  std::vector <std::string> args = c.args ();


  try
  {
    std::string cmd = args[0];

    if ( cmd == "run" )
    {
      c.need_args_min (2);
      
      std::vector <std::string> argvec;

      for ( int i = 3; i <= c.nargs (); ++i )
      {
        argvec.push_back (args[i]);
      }
          
      return job_run (c, args[1], args[2], argvec);
    }  


    else if ( cmd == "submit" )
    {
      c.need_args_min (2);
      
      std::vector <std::string> argvec;

      for ( int i = 3; i <= c.nargs (); ++i )
      {
        argvec.push_back (args[i]);
      }
          
      job_submit (c, args[1], args[2], argvec);
    }  


    else if ( cmd == "list" )
    {
      c.need_args (1);
      
      job_list (c, args[1]);
    }  


    else if ( cmd == "state" )
    {
      c.need_args_min (2);

      for ( int i = 2; i <= c.nargs (); i++ )
      {
        job_state (c, args[1], args[i]);
      }
    }


    else if ( cmd == "suspend" )
    {
      c.need_args_min (2);

      for ( int i = 2; i <= c.nargs (); i++ )
      {
        job_suspend (c, args[1], args[i]);
      }
    }


    else if ( cmd == "resume" )
    {
      c.need_args_min (2);

      for ( int i = 2; i <= c.nargs (); i++ )
      {
        job_resume (c, args[1], args[i]);
      }
    }


    else if ( cmd == "run_tests" )
    {
      c.need_args (1);

      job_run_test (c, args[1]);
    }  


    else
    {
      c.print_usage ("Unknown command");
    }
  }
  catch ( const saga::exception & e )
  {
    c.fail (e);
  }

  return 0;

#endif

}

