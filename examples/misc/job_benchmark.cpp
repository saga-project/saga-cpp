
#include <sys/time.h>

#include <saga/saga.hpp>

int main ()
{
  try 
  {
    saga::job::service js ("fork://localhost");

    saga::job::description jd;
    jd.set_attribute (saga::job::attributes::description_executable,
                      "/usr/bin/true");
    jd.set_attribute (saga::job::attributes::description_interactive,
                      saga::attributes::common_false);

    struct timeval start, end;
    ::gettimeofday (&start, NULL);

    for ( int i = 0; i < 1000; i++ )
    {
      saga::job::job j = js.create_job (jd);
      j.run ();
      j.wait ();
      // ::system ("/usr/bin/true");
      std::cout << i << " " << std::flush;
    }

    ::gettimeofday (&end, NULL);
    
    std::cout << "\n time: " << end.tv_sec - start.tv_sec << std::endl;
  }
  catch ( const saga::exception & e )
  {
    std::cout << " exception: " << e.what () << std::endl;
    return -1;
  }

  return 0;
}

