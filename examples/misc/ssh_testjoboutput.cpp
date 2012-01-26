
#include <saga/saga.hpp>
#include <vector>

int main ()
{
  try
  {
    saga::job::service js ("ssh://amerzky@cyder.cct.lsu.edu");

    saga::job::description d;

    {
      saga::job::description d1 = d;
      
      d1.set_attribute (saga::job::attributes::description_executable, "ls -la /etc/");
      d1.set_attribute (saga::job::attributes::description_output,     "/tmp/x.out");
      d1.set_attribute (saga::job::attributes::description_error,      "/tmp/x.err");
      d1.set_attribute (saga::job::attributes::description_input,      "/tmp/x.in");

      saga::job::job j1 = js.create_job (d1);

      j1.run  ();
      j1.wait ();
    }
  }
  catch ( const saga::exception & e )
  {
    std::cout << "exception: " << e.what () << std::endl;
    return -1;
  }

  return 0;
}

