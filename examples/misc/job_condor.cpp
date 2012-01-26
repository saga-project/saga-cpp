#include <saga/saga.hpp>
#include <iostream>

int main(int argc, char ** argv)
{
  try 
  {
    saga::job::service js ("condor://localhost");
    saga::job::job j = js.run_job (argv[1]);

    std::string jobid = j.get_job_id();

    std::cout << "Started job: " << jobid << std::endl;

    j.wait ();

    std::cout << j.get_state () << std::endl;
  }
  catch ( const std::exception & e )
  {
    std::cerr << "Exception: " << e.what () << std::endl;
  }
}


