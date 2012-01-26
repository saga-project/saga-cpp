
#include <saga/saga.hpp>

int main ()
{
  /////////////////////////////////////////////////////////////////////////
  // start a new sleeper for the next test
  std::string jobid;
  {
    saga::session      s;
    saga::job::service js (s);
    saga::job::job     j1 = js.run_job ("true");

    jobid = j1.get_job_id ();

    std::cout << jobid << std::endl;

    // make sure job is gone
    j1.wait ();
  }

  // try to get a job handle to the sleeper, in a new session
  try 
  {
    saga::session      s;
    saga::job::service js (s, "fork://localhost");
    saga::job::job     j2 = js.get_job (jobid);

    j2.wait ();

    if ( j2.get_state () != saga::job::Done )
    {
      std::cout << "unexpected state"
                << std::endl;
    }
  }
  catch ( const saga::does_not_exist & e )
  {
    // local adaptor cannot reconnect to finished 
    // jobs in a new session - this exception is 
    // expected
    std::cout << "got expected DoesNotExist exception"
              << std::endl;
  }
  catch ( const saga::exception & e )
  {
    // something different than DoesNotExist
    std::cerr << "Other Exception: " << e.what      ()
              << std::endl;
    std::cerr << "                 " << e.get_error () 
              << std::endl;

    std::vector <std::string> msgs = e.get_all_messages ();

    for ( unsigned int i = 0; i < msgs.size (); i++ )
    {
      std::cout << i << " : " << msgs[i]
                << std::endl;
    }
  }

  return 0;

}

