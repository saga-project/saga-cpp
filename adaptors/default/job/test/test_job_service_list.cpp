
#include <saga/saga.hpp>

int main ()
{
  int err = 0;
  try
  {
    // for list, we create a sleep job in a different session, and try to find
    // it again in the list of the tested job service
    //
    // create the sleeper
    std::string sleeper;
    {
      saga::session      s;
      saga::job::service js (s);
      saga::job::job     j = js.run_job ("sleep 10");

      sleeper = j.get_job_id ();
    }


    // try to find the sleeper
    {
      saga::session      s;
      saga::job::service js (s, "fork://localhost");

      std::vector <std::string> ids = js.list ();

      bool found = false;

      for ( unsigned int i = 0; i < ids.size (); i++ )
      {
        if ( ids[i] == sleeper )
        {
          found = true;
        }
      }

      if ( ! found )
      {
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

