
#include <saga/saga.hpp>
#include <iostream>

int main (int argc, char** argv)
{
  saga::job::service js;
  saga::job::ostream in;
  saga::job::istream out;
  saga::job::istream err;
  
  saga::job::job job = js.run_job ("/bin/date",
                                 "localhost",
                                 in, out, err);

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

  job.wait ();

  if ( job.attribute_exists (saga::job::attributes::exitcode) )
  {
    std::cout << std::endl
              << "job finished with " 
              << job.get_attribute (saga::job::attributes::exitcode)
              << std::endl;
  }
  
  return (0);
}

