
#include <saga/saga.hpp>
#include <saga/saga/adaptors/utils.hpp>
#include <iostream>

int main (int argc, char ** argv)
{

  try
  {
    std::string exe ("mProjectPP");

    std::vector <std::string> env;
    env.push_back ("PATH=/tmp/0/bin/");

  
    saga::job::service js ("fork://localhost");
  
    saga::job::description jd;
  
    jd.set_attribute        (saga::job::attributes::description_executable,  exe);
    jd.set_vector_attribute (saga::job::attributes::description_environment, env);
  
    saga::job::job j = js.create_job (jd);
  
    std::cout << j.get_state () << std::endl;
    j.run  ();
    std::cout << j.get_state () << std::endl;
    j.wait ();
    std::cout << j.get_state () << std::endl;
  }
  catch ( const saga::exception & e )
  {
    std::cerr << "saga error: " << e.what () << std::endl;
    return -1;
  }

  std::cout << "done" << std::endl;

  return 0;
}

