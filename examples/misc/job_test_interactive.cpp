
#include <saga/saga.hpp>

#define SAGA_CHECK_EQUAL(x,y) std::cout << "check: " << x << " == " << y << std::endl; 


int main (int argc, char** argv)
{
  try
  {
    saga::job::service js ("fork://localhost");

    saga::job::description jd;
    jd.set_attribute (saga::job::attributes::description_executable,  "/usr/bin/bc");
    jd.set_attribute (saga::job::attributes::description_interactive, saga::attributes::common_true);

    saga::job::job job = js.create_job (jd);

    job.run ();

    SAGA_CHECK_EQUAL (job.get_state (), saga::job::Running);

    saga::job::ostream in  = job.get_stdin  ();
    saga::job::istream out = job.get_stdout ();
    saga::job::istream err = job.get_stderr ();
  
    job.wait (1.0);
    SAGA_CHECK_EQUAL (job.get_state (), saga::job::Running);
  
    in << "6 * 7\n" << std::endl;
    int result; out >> result;
    SAGA_CHECK_EQUAL (result, 6*7);
  
    in << "quit\n" << std::endl;
    job.wait ();
    SAGA_CHECK_EQUAL (job.get_state (), saga::job::Done);
  }
  catch ( const saga::exception & e )
  {
    std::cerr << "exception: " << e.what () << std::endl;
  }

  return 0;

}

