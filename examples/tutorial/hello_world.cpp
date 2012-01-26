//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga.hpp>

///////////////////////////////////////////////////////////////////////////////
// The hello_world example is meant to be a very simple and first example to 
// try when it comes to SAGA. It's purpose is to spawn 3 (possibly remote) 
// identical jobs (/bin/echo) while passing the 3 words "Hello", "distributed", 
// and "world!" on their command lines. The result is that the jobs will print
// the respective command line arguments (hey, it's /bin/echo we're 
// launching...). The master job (this one) waits for the 3 child jobs to 
// finish. It intercepts the generated output and prints it to the user.
//
// Depending on which child jobs finish first the overall printed message might
// be some combination of the 3 arguments we passed. But most of the time you
// will see "Hello distributed world!", which is our way of saying hello and
// welcome to the world of SAGA.
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// The URLs to spawn jobs to.  Please change the 3 macros below to the URLs 
// you want the 3 childs to be spawned to.
///////////////////////////////////////////////////////////////////////////////
#define URL0 "fork://localhost"
#define URL1 "any://amerzky@cyder.cct.lsu.edu/"
#define URL2 "fork://localhost"

#define NJOBS 3

///////////////////////////////////////////////////////////////////////////////
int main ()
{
  try 
  {
    // we need I/O channels to mark jobs as interactive
    saga::job::ostream in [NJOBS];
    saga::job::istream out[NJOBS];
    saga::job::istream err[NJOBS];

    // create 3 job services, one per URL
    saga::job::service js[NJOBS];

    js[0] = saga::job::service (URL0);
    js[1] = saga::job::service (URL1);
    js[2] = saga::job::service (URL2);

    std::cout << " -- job services created" << std::endl;

    // for each job service, we create run job
    saga::job::job jobs[NJOBS];

    jobs[0] = js[0].run_job ("/bin/echo Hello",       "", in[0], out[0], err[0]);
    jobs[1] = js[1].run_job ("/bin/echo distributed", "", in[1], out[1], err[1]);
    jobs[2] = js[2].run_job ("/bin/echo world",       "", in[2], out[2], err[2]);

    std::cout << " -- jobs started" << std::endl;

    // wait for all jhbs to finish.  
    for ( unsigned int i = 0; i < NJOBS; i++ )
    {
      jobs[i].wait ();

      // Once a job finishes w/o error, print the output
      if (jobs[i].get_state () == saga::job::Done) 
      {
        std::string line;

        while ( ! std::getline (out[i], line).eof () )
        {
          std::cout << line << '\n';
        }
      }
      else 
      {
        std::cerr << "SAGA job: " << jobs[i].get_job_id() << " failed\n";
      }
    }

    std::cout << " -- jobs done" << std::endl;
  }

  catch ( saga::exception const & e )
  {
    std::cerr << "saga::exception caught: \n" << e.what () << std::endl;
  }

  return 0;
}

