//  Copyright (c) 2008 Ole Weidner
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga.hpp>
#include "saga-job.hpp"

///////////////////////////////////////////////////////////////////////////////
#if defined(SAGA_HAVE_PACKAGE_JOB)

int  job_run (saga_tools::common & c, 
              std::string          rm, 
              std::string          exe, 
              std::vector <std::string> args)
{
  saga::job::description jd;

  jd.set_attribute (saga::job::attributes::description_executable, exe);
  jd.set_attribute (saga::job::attributes::description_interactive, 
                    saga::attributes::common_true);
  jd.set_vector_attribute (saga::job::attributes::description_arguments, args);

  saga::job::service js (c.session (), saga::url (rm));
  saga::job::job     job = js.create_job (jd);

  // create io streams for job io
  //saga::job::ostream in;
  saga::job::istream out;
  saga::job::istream err;

  job.run ();

  //in  = job.get_stdin  ();
  out = job.get_stdout ();
  err = job.get_stderr ();

  // get job state
  saga::job::state state = job.get_state ();

  if ( state != saga::job::Running && state != saga::job::Done )
  {
    std::cerr << "run unexpectedly failed!" << std::endl;
  }

  char buffer[1024*64];

  while (out.good()) 
  {
    // get stdout
    out.read (buffer, sizeof (buffer));

    if ( out.gcount () > 0 )
    {
      std::cout << std::string (buffer, out.gcount ()) << std::flush;
    }
  }

  while (err.good()) 
  {
    // get stderr
    err.read (buffer, sizeof (buffer));

    if ( err.gcount () > 0 )
    {
      std::cerr << std::string (buffer, err.gcount ()) << std::flush;
    }
  }

  job.wait ();

  if ( job.attribute_exists (saga::job::attributes::exitcode) )
  {
    return boost::lexical_cast <int> (job.get_attribute (saga::job::attributes::exitcode));
  }

  return job.get_state () == saga::job::Done ? 0 : -1;
}

#endif

