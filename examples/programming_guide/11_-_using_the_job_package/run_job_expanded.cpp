
#include <saga/saga.hpp>
#include <iostream>

int main (int argc, char** argv)
{
  saga::job::service js;
  saga::job::ostream in;
  saga::job::istream out;
  saga::job::istream err;

  // ----------------------------
  // begin of run_job expansion
  
  namespace sa = saga::attributes;
  namespace sja = saga::job::attributes;

  // create and fill a job::description
  saga::job::description jd;

  std::string exe ("/bin/date");

  std::vector <std::string> hosts;
  hosts.push_back ("localhost");

  jd.set_attribute (sja::description_interactive,
                    sa::common_true);

  jd.set_attribute (sja::description_executable, exe);

  jd.set_vector_attribute (sja::description_candidate_hosts, hosts);

  // create job from description
  saga::job::job j = js.create_job (jd);


  // get io channels
  in  = j.get_stdin  ();
  out = j.get_stdout ();
  err = j.get_stderr ();
  
  // job is still in 'New' state, we need to run it
  j.run ();

  // end of run_job expansion
  // ----------------------------

  char buffer[1024]; // read buffer

  // read stdout as long as its open
  while ( out.good () )
  {
    // get stdout
    out.read (buffer, sizeof (buffer));
    
    if ( out.gcount () > 0 )
    {
      std::cout << std::string (buffer, out.gcount ()) 
                << std::flush;
    }
  }

  while ( err.good () )
  {
    // get stderr
    err.read (buffer, sizeof (buffer));

    if ( err.gcount () > 0 )
    {
      std::cerr << std::string (buffer, err.gcount ()) 
                << std::flush;
    }
  }

  j.wait ();

  if ( j.attribute_exists (saga::job::attributes::exitcode) )
  {
    std::cout << std::endl
              << "job finished with " 
              << j.get_attribute (saga::job::attributes::exitcode)
              << std::endl;
  }
  
  return (0);
}

