
#include <saga/saga.hpp>

int main ()
{
  try
  {
    saga::session s = saga::get_default_session ();
    saga::job::service js (s, "fork://localhost");

    std::string cmd (SAGA_TEST_BIN_TRUE);
    saga::job::job j = js.run_job (cmd, "localhost");

    if ( j.get_state () != saga::job::Running )
    {
      return -2;
    }

    j.wait ();

    if ( j.get_state () != saga::job::Done )
    {
      return -3;
    }
  }
  catch ( const saga::exception & e )
  {
    std::cerr << "Exception: " << e.what () << std::endl;
    return -1;
  }

  return 0;
}

