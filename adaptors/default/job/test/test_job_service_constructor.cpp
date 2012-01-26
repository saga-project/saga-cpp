
#include <saga/saga.hpp>

int main ()
{
  int err = 0;

  try
  {
    // test ctor with default session
    {
      saga::session s = saga::get_default_session ();
      saga::job::service js (s, "fork://localhost");

      saga::job::job j = js.run_job (SAGA_TEST_BIN_TRUE, "localhost");
      j.wait ();
    }

    // test ctor with empty session
    {
      saga::session s;
      saga::job::service js (s, "fork://localhost");

      saga::job::job j = js.run_job (SAGA_TEST_BIN_TRUE, "localhost");
      j.wait ();
    }

    // test ctor with default rm
    {
      saga::job::service js;

      saga::job::job j = js.run_job (SAGA_TEST_BIN_TRUE, "localhost");
      j.wait ();
    }
  }
  catch ( const saga::exception & e )
  {
    std::cerr << "Exception: " << e.what () << std::endl;
    return -1;
  }



  // an invalid rm must cause a BadParameter exception
  try
  {
      saga::job::service js ("invalid://invalid.net/");
      saga::job::job j = js.run_job (SAGA_TEST_BIN_TRUE, "localhost");
      j.wait ();
  }
  catch ( const saga::bad_parameter & e )
  {
    // all is well
  }
  catch ( const saga::no_success & e )  
  {
    // this can always happen, mapped from AdaptorDeclined
    std::cout << "WARNING: expected BadParameter, got NoSuccess exception" << std::endl;
  }
  catch ( const saga::not_implemented & e )  
  {
    // this can always happen
    std::cout << "WARNING: expected BadParameter, got NotImplemented exception" << std::endl;
  }
  catch ( const saga::exception & e )
  {
    std::cerr << "incorrect exception (! BadParameter): " << e.what () << std::endl;
    err++;
  }


  return err;
}

