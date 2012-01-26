
#include <saga/saga.hpp>
#include <iostream>
#include <vector>

int main (int argc, char** argv)
{
  saga::job::description jobdef;

  std::vector <std::string> args;
  args.push_back ("2");
  
  jobdef.set_attribute        ("Executable", "/bin/sleep");
  jobdef.set_vector_attribute ("Arguments",     args);

  saga::job::service js (saga::url ("fork://localhost"));
  saga::job::job     job = js.create_job (jobdef);

  job.run ();

  job.wait ();

  return 0;
}

