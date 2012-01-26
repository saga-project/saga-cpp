
#include <saga/saga.hpp>

int main ()
{
  saga::job::service js ("fork://localhost/");
  saga::job::description jd;

  std::vector <std::string> env;

  env.push_back ("SAGA_VERBOSE=10");

  jd.set_attribute        (saga::job::attributes::description_executable, "/bin/env");
  jd.set_attribute        (saga::job::attributes::description_interactive,
                           saga::attributes::common_true);
  jd.set_vector_attribute (saga::job::attributes::description_environment, env);

  saga::job::job j = js.create_job(jd);

  saga::job::ostream in  = j.get_stdin  ();
  saga::job::istream out = j.get_stdout ();
  saga::job::istream err = j.get_stderr ();

  j.run ();


  while ( out.good () ) 
  {
    char c;

    out.read (&c, 1);

    if ( out.fail () ) 
      break; 

    std::cout << c;
  }

  j.wait();

  std::cout << "state: " << j.get_state () << "\n";

  switch ( j.get_state () ) {
    case saga::job::Done :
      std::cout << "done\n";
      break;
    case saga::job::Running :
      std::cout << "Running\n";
      break;
    case saga::job::Failed :
      std::cout << "Failed\n";
      break;
    case saga::job::Canceled :
      std::cout << "Canceled\n";
      break;
    case saga::job::Suspended :
      std::cout << "Suspended\n";
      break;
    case saga::job::New :
      std::cout << "New\n";
      break;
    case saga::job::Unknown :
      std::cout << "Unknown\n";
      break;
    default :
      std::cout << "???\n";
      break;
  }

  return 0;

}

