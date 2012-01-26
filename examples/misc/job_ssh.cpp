
//  Copyright (c) 2005-2007 Andre Merzky <andre@merzky.net>
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE file or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga.hpp>

int main (int argc, char* argv[])
{
  try
  {
    saga::context c ("ssh");
    saga::session s;
    s.add_context (c);

    saga::job::description jd;

    std::vector <std::string> args;
    std::vector <std::string> env;
    std::vector <std::string> hosts;

    std::string exe ("sleep");

    args.push_back  ("10");
    env.push_back   ("SAGA_VERBOSE=0");
    hosts.push_back ("jonas.local");

    jd.set_attribute        (saga::job::attributes::description_executable,      exe  );
    jd.set_vector_attribute (saga::job::attributes::description_arguments,       args );
    jd.set_vector_attribute (saga::job::attributes::description_environment,     env  );
    jd.set_vector_attribute (saga::job::attributes::description_candidate_hosts, hosts);
    jd.set_attribute        (saga::job::attributes::description_interactive, 
                                                         saga::attributes::common_true);

    saga::job::service js (s, "ssh://");
    saga::job::job j = js.create_job (jd);

    saga::job::istream out = j.get_stdout ();

    j.run ();

    std::string id = j.get_job_id ();

    std::cout << " ====== jobid: " << id << std::endl;

    size_t n = id.find ("qb.loni.org");

    if ( n != std::string::npos )
    {
      id.replace (n, 11, "localhost");
    }

    std::cout << " ====== jobid: " << id << std::endl;

    saga::job::job j2  = js.get_job (id);
    std::string    id2 = j2.get_job_id ();

    std::cout << " ====== newid: " << id2 << std::endl;

    while ( j.get_state () == saga::job::Running )
    {
      std::cout << "Running" << std::endl;
      ::sleep (1);

      j2.cancel ();
    }

    if ( j.get_state () == saga::job::Canceled )
    {
      std::cout << "Canceled" << std::endl;
    }
  }
  catch ( saga::exception const & e )
  {
    std::cerr << e.what ();
  }

  return 0;
}

