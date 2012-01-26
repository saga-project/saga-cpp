
#include <saga/saga.hpp>

int main ()
{
  int err = 0;

  try
  {
    namespace sja = saga::job::attributes;
    namespace sa  = saga::attributes;

    // create a job description template
    saga::job::description templ;
    templ.set_attribute (sja::description_executable, SAGA_TEST_BIN_BC);

    std::vector <std::string> args;
    args.push_back ("-ql");
    templ.set_vector_attribute (sja::description_arguments, args);

    templ.set_attribute (sja::description_interactive, 
                         sa::common_true);


    // create_job valid interactive job
    {
      saga::job::service js (saga::get_default_session ());
      saga::job::description jd (templ);

      saga::job::job j = js.create_job (jd);
      j.run  ();

      saga::job::ostream io_in  = j.get_stdin  ();
      saga::job::istream io_out = j.get_stdout ();
      saga::job::istream io_err = j.get_stderr ();

      std::string q ("6*7");
      io_in << q << std::endl;

      char a[1024];
      io_out.getline (a, sizeof (a));

      std::cout <<  "question: " << q << std::endl;
      std::cout <<  "answer  : " << a << std::endl;

      if ( std::string (a) != "42" )
      {
        std::cerr << "unexpected answer: -" << a << "- != -42-" << std::endl;
        err++;
      }

      io_in << "quit\n" << std::endl;

      j.wait ();
    }

    // create_job *in*valid interactive job
    {
      saga::job::service js (saga::get_default_session ());
      saga::job::description jd (templ);

      jd.set_attribute (sja::description_executable, "/does/not/exist");

      // expecting a BadParameter exception
      try {
        saga::job::job j = js.create_job (jd);
      }
      catch ( const saga::bad_parameter & e )
      {
        // all is well
      }
      catch ( const saga::exception & e )
      {
        std::cerr << "incorrect exception (! BadParameter): " << e.what () << std::endl;
        std::cout << "error: " << e.get_error ()     << std::endl;
        std::cout << "     : " << saga::BadParameter << std::endl;
        err++;
      }
    }


    // create_job w/o executable?  BadParameter expected
    {
      saga::job::service js (saga::get_default_session ());
      saga::job::description jd;

      // expecting a BadParameter exception
      try 
      {
        saga::job::job j = js.create_job (jd);
      }
      catch ( const saga::bad_parameter & e )
      {
        // all is well
      }
      catch ( const saga::exception & e )
      {
        std::cerr << "incorrect exception (! BadParameter): " << e.what () << std::endl;
        std::cout << "error: " << e.get_error ()     << std::endl;
        std::cout << "     : " << saga::BadParameter << std::endl;
        err++;
      }
    }
  }
  catch ( const saga::not_implemented & e )  
  {
    // this can always happen
    std::cout << "WARNING: expected none, got NotImplemented exception" << std::endl;
  }
  catch ( const saga::exception & e )
  {
    std::cerr << "Exception: " << e.what () << std::endl;
    err++;
  }


  return err;
}

