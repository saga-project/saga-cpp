
#include <saga/saga.hpp>
#include <iostream>

int main (int argc, char** argv)
{
  namespace sa  = saga::attributes;
  namespace sja = saga::job::attributes;

  std::string        jid;
  saga::job::service js;

  std::string exe ("/bin/sleep");

  std::vector <std::string> args;
  args.push_back ("5000");

  saga::job::description jd;

  jd.set_attribute        (sja::description_executable, exe);
  jd.set_vector_attribute (sja::description_arguments, args);


  std::vector <saga::job::job> jobs;

  for ( int i = 0; i < 1000; i++ )
  {
    saga::job::job j = js.create_job (jd);

    // job is in 'New' state here, we need to run it
    j.run ();
    jid = j.get_job_id ();

    std::cout << " == " << i << " - jobid: " << jid << "\n";
    jobs.push_back (j);
  }


  {
    std::string exe ("/bin/sleep");

    std::vector <std::string> args;
    args.push_back ("5");

    saga::job::description jd;

    jd.set_attribute        (sja::description_executable, exe);
    jd.set_vector_attribute (sja::description_arguments, args);


    saga::job::job j = js.create_job (jd);

    // job is in 'New' state here, we need to run it
    j.run ();
    jid = j.get_job_id ();

    std::cout << " == jobid: " << jid << "\n";
  }

  saga::job::job j1 = js.get_job (jid);

  saga::job::job j2 = js.get_job (jid);
    
  {
    saga::job::state state = j1.get_state ();

    while ( state != saga::job::Done &&
            state != saga::job::Failed &&
            state != saga::job::Canceled )
    {
      std::cout << "Running\n";
      ::sleep (1);
      state = j1.get_state ();
    }

    std::cout << "Finished\n";
  }

    
  {
    saga::job::state state = j2.get_state ();

    while ( state != saga::job::Done &&
            state != saga::job::Failed &&
            state != saga::job::Canceled )
    {
      std::cout << "Running\n";
      ::sleep (1);
      state = j2.get_state ();
    }

    std::cout << "Finished\n";
  }

  return (0);
}

