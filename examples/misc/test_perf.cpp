
#include <saga/saga.hpp>
#include <iostream>

int main ()
{
//saga::job::service js ("fork://localhost/");
  saga::job::service js ("ssh://localhost/");

  system ("/bin/date");

  for ( int i = 0; i < 100; i++ )
  {
    std::cout << "." << std::flush;
    saga::job::job j = js.run_job ("/bin/date");
    j.wait ();
  }

  std::cout << std::endl;
  system ("/bin/date");

  return 0;
}

