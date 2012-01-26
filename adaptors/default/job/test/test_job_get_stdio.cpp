
#include <saga/saga.hpp>

int main (int argc, char** argv)
{
  int err = 0;

  try
  {
    /////////////////////////////////////////////////////////////////////////
    // get_stdin/get_stdou/get_stderr are best tested in combination, so that
    // we can see if what is entered on stdin arrives at stdout/stderr.
    /////////////////////////////////////////////////////////////////////////

    namespace sja = saga::job::attributes;
    namespace sa  = saga::attributes;

    /////////////////////////////////////////////////////////////////////////
    // create cat job, write to i, and check output
    {
      saga::session          s;
      saga::job::service     js (s, "fork://localhost");
      saga::job::description jd;

      jd.set_attribute (sja::description_executable, "/bin/sh");
      jd.set_attribute (sja::description_interactive, sa::common_true);

      std::vector <std::string> args;
      args.push_back ("-c");
      args.push_back ("/usr/bin/bc -ql");
      jd.set_vector_attribute (sja::description_arguments, args);

      saga::job::job j = js.create_job (jd);

      saga::job::ostream my_in  = j.get_stdin  ();
      saga::job::istream my_out = j.get_stdout ();
      saga::job::istream my_err = j.get_stderr ();

      j.run ();

      my_in << "6*7\n"  << std::flush;
      my_in << "quit\n" << std::flush;

      std::string output = "";
      char        buffer[1024*64];

      while ( my_out.good () )
      {
        my_out.read (buffer, sizeof (buffer));

        if ( my_out.gcount () > 0 )
        {
          output += buffer;
        }
      }

      if ( output != "42\n" )
      {
        std::cerr << "Expected Output to be '42\\n', not " << output << std::endl;
        err++;
      }

      j.wait ();
    }


    /////////////////////////////////////////////////////////////////////////
    // do just the same, but switch stdout and stderr in the shell, using
    // 
    {
      saga::session          s;
      saga::job::service     js (s, "fork://localhost");
      saga::job::description jd;

      jd.set_attribute (sja::description_executable, "/bin/sh");
      jd.set_attribute (sja::description_interactive, sa::common_true);

      std::vector <std::string> args;
      args.push_back ("-c");
      args.push_back ("/usr/bin/bc -ql 3>&1 1>&2 2>&3");
      jd.set_vector_attribute (sja::description_arguments, args);

      saga::job::job j = js.create_job (jd);

      saga::job::ostream my_in  = j.get_stdin  ();
      saga::job::istream my_out = j.get_stdout ();
      saga::job::istream my_err = j.get_stderr ();

      j.run ();

      my_in << "6*7\n"  << std::flush;
      my_in << "quit\n" << std::flush;

      std::string output = "";
      char        buffer[1024*64];

      while ( my_err.good () )
      {
        my_err.read (buffer, sizeof (buffer));

        if ( my_err.gcount () > 0 )
        {
          output += buffer;
        }
      }

      if ( output != "42\n" )
      {
        std::cerr << "Expected Output to be '42\\n', not " << output << std::endl;
        err++;
      }

      j.wait ();
    }
  }
  catch ( const saga::exception & e )
  {
    std::cerr << "Exception: " << e.what () << std::endl;
    err++;
  }

  return err; 
}

