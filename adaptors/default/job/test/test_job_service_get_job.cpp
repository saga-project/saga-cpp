
#include <saga/saga.hpp>

int main ()
{
  int err = 0;

  try
  {
    /////////////////////////////////////////////////////////////////////////
    // for get_job, we create a sleep job in a session, and try to 
    // reattach with its job id in the same, or a different session.
    /////////////////////////////////////////////////////////////////////////

    std::string jobid;
    
    /////////////////////////////////////////////////////////////////////////
    // create the job, and reconnect in the same session
    {
      saga::session      s;
      saga::job::service js (s);
      saga::job::job     j1 = js.run_job ("sleep 3");
      saga::job::job     j2 = js.get_job (j1.get_job_id ());

      j2.wait ();

      if ( j2.get_state () != saga::job::Done )
      {
        err++;
      }
    }


    /////////////////////////////////////////////////////////////////////////
    // start a new job for the next test
    {
      saga::session      s;
      saga::job::service js (s);
      saga::job::job     j1 = js.run_job ("sleep 3");

      jobid = j1.get_job_id ();
    }

    // try to get a job handle to the jobid, in a new session
    {
      saga::session      s;
      saga::job::service js (s, "fork://localhost");
      saga::job::job     j2 = js.get_job (jobid);
    
      j2.wait ();

      if ( j2.get_state () != saga::job::Done )
      {
        err++;
      }
    }


    /////////////////////////////////////////////////////////////////////////
    // now we try the same for a job which has been Done
    {
      saga::session      s;
      saga::job::service js (s);
      saga::job::job     j1 = js.run_job ("true");
      
      j1.wait ();

      saga::job::job     j2 = js.get_job (j1.get_job_id ());
      
      j2.wait ();

      if ( j2.get_state () != saga::job::Done )
      {
        err++;
      }
    }


    /////////////////////////////////////////////////////////////////////////
    // start a new job for the next test
    {
      saga::session      s;
      saga::job::service js (s);
      saga::job::job     j1 = js.run_job ("true");

      jobid = j1.get_job_id ();

      // make sure job is gone
      j1.wait ();
    }

    // try to get a job handle to the jobid, in a new session
    try 
    {
      saga::session      s;
      saga::job::service js (s, "fork://localhost");
      saga::job::job     j2 = js.get_job (jobid);
    
      j2.wait ();

      if ( j2.get_state () != saga::job::Done )
      {
        std::cerr << "Expected Done reconnected job" << std::endl;
        err++;
      }
    }
    catch ( const saga::does_not_exist & e )
    {
      // local adaptor cannot reconnect to finished jobs in a new session - so
      // this exception is expected
      std::cout << "got expected DoesNotExist exception" << std::endl;
    }
    catch ( const saga::exception & e )
    {
      // local adaptor cannot reconnect to finished jobs in a new session - so
      // this exception is expected
      std::cout << "appearently, ticket #561 is still open :-/" << std::endl;
      std::cout << "error: " << e.get_error ()     << std::endl;
      std::cout << "     : " << saga::DoesNotExist << std::endl;
    }



    /////////////////////////////////////////////////////////////////////////
    //
    // the same for a suspended job
    //
    // start a new job for the next test
    {
      saga::session      s;
      saga::job::service js (s);
      saga::job::job     j1 = js.run_job ("sleep 10");

      jobid = j1.get_job_id ();
      j1.suspend ();
    }

    // try to get a job handle to the jobid, in a new session
    {
      saga::session      s;
      saga::job::service js (s, "fork://localhost");
      saga::job::job     j2 = js.get_job (jobid);
    
#ifdef SAGA_APPLE
      // alas, apple does not correctly report job_state in ps, thus we cannot
      // do much with the reconnected job, apart from killing it
      j2.cancel ();

      if ( j2.get_state () != saga::job::Canceled )
      {
        std::cerr << "Expected Canceled reconnected job" << std::endl;
        err++;
      }
#else
      // will fail
      if ( j2.get_state () != saga::job::Suspended )
      {
        std::cerr << "Expected Suspended reconnected job" << std::endl;
        err++;
      }

      j2.resume ();

      j2.wait ();

      if ( j2.get_state () != saga::job::Done )
      {
        err++;
      }
#endif
    }
  }
  catch ( const saga::exception & e )
  {
    std::cerr << "Exception: " << e.what () << std::endl;
    err++;
  }

  return err; 
}

