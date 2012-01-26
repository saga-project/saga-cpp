
#include <saga/saga.hpp>

int main ()
{
  int err = 0;
  try
  {
    // run_job with io redirection
    {
      saga::session s = saga::get_default_session ();
      saga::job::service js (s, "fork://localhost");

      saga::job::ostream io_in;
      saga::job::istream io_out;
      saga::job::istream io_err;

      std::string cmd (SAGA_TEST_BIN_BC);
      cmd += " -ql";

      saga::job::job j = js.run_job (cmd, "localhost", io_in, io_out, io_err);

      if ( j.get_state () != saga::job::Running )
      {
        std::cerr << "state != Running" << std::endl;
        err++;
      }

      std::string q ("6*7");
      io_in << q << std::endl;

      char a[1024];
      io_out.getline (a, sizeof (a));

      // std::cout <<  "question: " << q << std::endl;
      // std::cout <<  "answer  : " << a << std::endl;

      if ( std::string (a) != "42" )
      {
        std::cerr << "unexpected answer: -" << a << "- != -42-" << std::endl;
        err++;
      }

      io_in << "quit\n" << std::endl;

      j.wait ();

      if ( j.get_state () != saga::job::Done )
      {
        std::cerr << "state != Done" << std::endl;
        err++;
      }
    }


    // run_job without io redirection
    {
      saga::session s = saga::get_default_session ();
      saga::job::service js (s, "fork://localhost");

      std::string cmd (SAGA_TEST_BIN_SLEEP);
      cmd += " 3";

      saga::job::job j = js.run_job (cmd, "localhost");

      if ( j.get_state () != saga::job::Running )
      {
        std::cerr << "state != Running" << std::endl;
        err++;
      }

      j.wait ();

      if ( j.get_state () != saga::job::Done )
      {
        std::cerr << "state != Done" << std::endl;
        err++;
      }
    }


    // run_job on default host
    {
      saga::session s = saga::get_default_session ();
      saga::job::service js (s, "fork://localhost");

      std::string cmd (SAGA_TEST_BIN_SLEEP);
      cmd += " 3";

      saga::job::job j = js.run_job (cmd);

      if ( j.get_state () != saga::job::Running )
      {
        std::cerr << "state != Running" << std::endl;
        err++;
      }

      j.wait ();

      if ( j.get_state () != saga::job::Done )
      {
        std::cerr << "state != Done" << std::endl;
        err++;
      }
    }


    // run_job with non-existing executable - expect BadParameter exception
    try
    {
      saga::session s = saga::get_default_session ();
      saga::job::service js (s, "fork://localhost");

      saga::job::job j = js.run_job ("/does/not/exist", "localhost");
    }
    catch ( const saga::bad_parameter & e )
    {
      // all is well
    }
  }
  catch ( const saga::exception & e )
  {
    std::cerr << "Exception: " << e.what () << std::endl;
    err++;
  }


  return err;
}

