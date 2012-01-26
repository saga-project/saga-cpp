//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <cassert>
#include <saga/saga.hpp>
#include <boost/lexical_cast.hpp>

///////////////////////////////////////////////////////////////////////////////
//
// Start this example by providing an arbitrary number of URLs on the command 
// line. It will re-spawn itself to each of the URLs. Each instance will 
// increment a number stored in a central counter store, using the advert service.
//
//
// example usage (slightly shortened):
//
//   merzky@boskop:~/saga/core/examples/tutorial$ saga-advert remove_entry /tutorial/depending_jobs/counter
//
//   merzky@boskop:~/saga/core/examples/tutorial$ saga-advert dump_directory /tutorial/depending_jobs/
//     /tutorial/depending_jobs/
//
//   merzky@boskop:~/saga/core/examples/tutorial$ make && ./depending_jobs fork://localhost fork://localhost
//   advert entry does not yet exist - initialize counter to 0
//
//   merzky@boskop:~/saga/core/examples/tutorial$ saga-advert dump_directory /tutorial/depending_jobs/
//     /tutorial/depending_jobs/
//         /tutorial/depending_jobs/counter
//           value     	: 2
//
//   merzky@boskop:~/saga/core/examples/tutorial$ make && ./depending_jobs fork://localhost fork://localhost
//
//   merzky@boskop:~/saga/core/examples/tutorial$ saga-advert dump_directory /tutorial/depending_jobs/
//     /tutorial/depending_jobs/
//         /tutorial/depending_jobs/counter
//           value     	: 4
//
///////////////////////////////////////////////////////////////////////////////

#define RESULT_STORE  "/tutorial/depending_jobs/counter"   // place in advert to store counter to
#define JOB_PATH      "./depending_jobs"                   // put the correct path here

///////////////////////////////////////////////////////////////////////////////
// retrieve the current value from the advert (counter store)
bool get_counter(int & counter)
{
  counter = 0;

  try 
  {
    saga::advert::entry e (RESULT_STORE, saga::advert::Read);
    counter = boost::lexical_cast <int> (e.get_attribute ("value"));
  }
  catch ( saga::does_not_exist const & e )
  {
    std::cout << "advert entry does not yet exist - initialize counter to 0" << std::endl;
    counter = 0;

    return true;
  }
  catch ( saga::exception const & e ) 
  {
    std::cerr << "saga::exception caught: " << e.what () << std::endl;
    return false;
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
// store the current value into the advert (counter store)
bool set_counter (int counter)
{
  try 
  {
    saga::advert::entry e (RESULT_STORE, 
                           saga::advert::CreateParents | 
                           saga::advert::Create        |
                           saga::advert::ReadWrite);

    e.set_attribute ("value", boost::lexical_cast <std::string> (counter));
  }
  catch ( saga::exception const & e )
  {
    std::cerr << "saga::exception caught: " << e.what () << std::endl;
    return false;
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
// the routine spawning the SAGA jobs and waiting for their counter
void respawn (int argc, char *argv[])
{
  assert (argc > 1); // we shouldn't end up here without any given URL

  try 
  {
    // start the johb server on the given URL   
    saga::url          url (argv[1]);
    saga::job::service js  (url);

    // compose the command line, skip first argument
    std::string commandline (JOB_PATH);

    for ( int i = 2; i < argc; ++i )
    {
      commandline += " ";
      commandline += argv[i];
    }

    // run the job 
    saga::job::job j = js.run_job (commandline);

    // wait for the job to start
    saga::job::state s = j.get_state ();

    while ( s != saga::job::Running && s != saga::job::Failed )
    {
      s = j.get_state ();
    }

    // if the job didn't start successfully, print error message
    if ( s == saga::job::Failed ) 
    {
      std::cerr << "SAGA job: " << j.get_job_id() << " failed" << std::endl;
    }

    // wait for the job to Finish
    s = j.get_state ();

    while (s == saga::job::Running)
    {
      s = j.get_state();
    }
  }
  catch ( saga::exception const & e )
  {
    std::cerr << "saga::exception caught: " << e.what () << std::endl;
  }
}

///////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
  int counter = 0;

  if ( argc > 1 ) 
  {
    // some target URLs given, so: get current value, increment it, and store new value
    get_counter (counter);   // will set counter to zero initially

    // re-spawn this job, increment counter
    // if set_counter fails, don't bother to respawn
    if ( set_counter (counter + 1) ) 
    {
      respawn (argc, argv);
    }
  }
  else
  {
    // more URLs are given - just print the counter value
    if ( get_counter (counter) )
    {
      std::cout << "The overall counter is: " << counter << std::endl;
    }
    else
    {
      std::cout << "no counter found" << std::endl;
    }
  }

  return 0;
}

