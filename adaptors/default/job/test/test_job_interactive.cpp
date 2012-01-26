
#include <saga/saga.hpp>

int main ()
{
  int err = 0;
  try
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
    else
    {
      io_in << "6*7" << std::endl;

      char a[1024];
      io_out.getline (a, sizeof (a));

      std::cout <<  "question: 6*7"   << std::endl;
      std::cout <<  "answer  : " << a << std::endl;

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
  }
  catch ( const saga::exception & e )
  {
    std::cerr << "Exception: " << e.what () << std::endl;
    err++;
  }


  // test for that strange condition where an interactive job.run() does not
  // return until the job actually finishes
  try
  {
    for ( int i = 0; i < 10; i++ )
    {
      saga::session s = saga::get_default_session ();
      saga::job::service js (s, "fork://localhost");

      saga::job::ostream io_in;
      saga::job::istream io_out;
      saga::job::istream io_err;

      // sleep 5 seconds
      std::string cmd ("/bin/sleep 2");

      // run_job calls internally job.run, which should trigger the problem if
      // it still persists
      saga::job::job j = js.run_job (cmd, "localhost", io_in, io_out, io_err);

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
  }
  catch ( const saga::exception & e )
  {
    std::cerr << "Exception: " << e.what () << std::endl;
    err++;
  }


  return err;
}

