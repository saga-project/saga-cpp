
#include <saga/saga.hpp>

int main ()
{
  int iteration = 0;

  while ( true )
  {

    iteration++;
    std::cout << "Iteration: " << iteration << std::endl;

    saga::job::service js;
    saga::job::description jd;

    jd.set_attribute ("Executable", "/bin/hostname");
    jd.set_attribute ("Output",     "/tmp/foobar1");

    saga::job::job j = js.create_job (jd);
    j.run();

    std::cout << j.get_state () << std::endl;
    j.wait (5);
    std::cout << j.get_state () << std::endl;

  }

  return 0;
}


