
#include <saga/saga.hpp>

int main (int argc, char** argv)
{
  int err = 0;

  try
  {
    /////////////////////////////////////////////////////////////////////////
    // we do the same as in the suspend test, but not only resumt the job
    // itself, but, on the second test, try to resume the reconnected job.
    {
      saga::session      s;
      saga::job::service js (s, "fork://localhost");
      saga::job::job     j = js.run_job ("sleep 3");

      if ( j.get_state () != saga::job::Running )
      {
        std::cerr << "state != Running" << std::endl;
        err++;
      }

      ::sleep (1);

      j.suspend ();

      if ( j.get_state () != saga::job::Suspended )
      {
        std::cerr << "state != Suspended" << std::endl;
        err++;
      }

      ::sleep (5);

      if ( j.get_state () != saga::job::Suspended )
      {
        std::cerr << "state != Suspended" << std::endl;
        err++;
      }

      //  resume the job, and see it moving to Done immediately
      j.resume ();

      // allow job to realize its awake, if needed
      if ( j.get_state () == saga::job::Running )
      {
        ::sleep (1);
      }

      if ( j.get_state () != saga::job::Done )
      {
        std::cerr << "state != Done" << std::endl;
        err++;
      }
    }


    /////////////////////////////////////////////////////////////////////////
    // now for the reconnected resume.  That will only works on MacOS though if
    // we reconnect before the suspend - hence the ifdef.
    {
      saga::session      s1;
      saga::job::service js1 (s1, "fork://localhost");
      saga::job::job     j1 = js1.run_job ("sleep 3");

      if ( j1.get_state () != saga::job::Running )
      {
        std::cerr << "state != Running" << std::endl;
        err++;
      }

      ::sleep (1);

#ifndef SAGA_APPLE
      // we can suspend already
      j1.suspend ();
#endif

      saga::session      s2;
      saga::job::service js2 (s2, "fork://localhost");
      saga::job::job     j2 = js2.get_job (j1.get_job_id ());

#ifdef SAGA_APPLE
      // suspend now
      j2.suspend ();
#endif

      if ( j2.get_state () != saga::job::Suspended )
      {
        std::cerr << "state != Suspended" << std::endl;
        err++;
      }


      ::sleep (5);

      //  resume the job, and see it moving to Done immediately
      j2.resume ();

      // allow job to realize its awake, if needed
      if ( j2.get_state () == saga::job::Running )
      {
        ::sleep (1);
      }

      if ( j2.get_state () != saga::job::Done )
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

