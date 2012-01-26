
#include <saga/saga.hpp>

int main (int argc, char** argv)
{
  int err = 0;

  try
  {
    /////////////////////////////////////////////////////////////////////////
    // start a sleep(3) job, suspend it, and check if its still alive after,
    // say, 5 seconds.
    // create cat job, write to i, and check output
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

      // reconnect to the job in a second session, and see if job is indeed
      // considered suspended.
      // Alas, that won't  work on MacOS
#ifndef SAGA_APPLE
      std::string id = j.get_job_id ();
      {
        saga::session      s2;
        saga::job::service js2 (s2, "fork://localhost");
        saga::job::job     j2 = js2.get_job (id);

        if ( j2.get_state () != saga::job::Suspended )
        {
          std::cerr << "job state != Suspended" << std::endl;
          err++;
        }
      }
#endif

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
  }
  catch ( const saga::exception & e )
  {
    std::cerr << "Exception: " << e.what () << std::endl;
    err++;
  }

  return err; 
}

