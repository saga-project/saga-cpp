
#include <saga/saga.hpp>

///////////////////////////////////////////////////////////////////////////////
int main (int argc, char* argv[])
{
  try
  {
    // set up security
    saga::context c ("ec2");

    saga::session s;
    s.add_context (c);

    // start job service: this creates a VM
    saga::job::service js (s, "ec2://");

    // job description to run
    saga::job::description jd;

    jd.set_attribute (saga::job::attributes::description_executable, "/bin/uname");
    jd.set_attribute (saga::job::attributes::description_interactive, "True");

    std::vector <std::string> args;

    args.push_back ("-a");

    jd.set_vector_attribute (saga::job::attributes::description_arguments, args);

    // run job
    saga::job::job j = js.create_job (jd);
    j.run ();

    // catch stdout
    saga::job::istream out = j.get_stdout ();
    while ( true )
    {
      char buffer[255];
      out.read (buffer, sizeof (buffer));

      if ( out.gcount () > 0 )
        std::cout << std::string (buffer, out.gcount ());

      if ( out.fail () )
        break;
    }
  }
  catch ( saga::exception const & e )
  {
    std::cerr << e.what ();
  }

  return 0;
}

