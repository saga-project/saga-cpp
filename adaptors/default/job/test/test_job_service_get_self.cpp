
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

#include <saga/saga.hpp>

volatile sig_atomic_t signal_received = 0;

void
sigusr1_handler (int signum)
{
  // std::cout << "received signal SIGUSR1" << std::endl;
  signal_received = true;
}

int main ()
{
  int err = 0;

  try
  {
    /////////////////////////////////////////////////////////////////////////
    // for get_self, we simply check if the job_id of self makes sense, 
    // and if the job is in Running state.
    /////////////////////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////////////////////
    // create the job, and reconnect in the same session
    {
      saga::session      s;
      saga::job::service js (s);
      saga::job::self    self = js.get_self ();

      std::stringstream ss;

      ss << "[fork://localhost]-[" << ::getpid () << "]";


      // sensible job id?
      if ( ss.str () != self.get_job_id () )
      {
        std::cerr << "expected jobid "  << ss.str () << ", not " 
                  << self.get_job_id () << "!" << std::endl;
        err++;
      }


      // in running state?
      if ( self.get_state () != saga::job::Running)
      {
        std::cerr << "expected job_self to be in running state!" << std::endl;
        err++;
      }

      // set a signal handler for SIGUSR1, and check if that receives a signal
      // to self
      if ( SIG_ERR == ::signal (SIGUSR1, sigusr1_handler) )
      {
        std::cout << "Could not test signal handling: " << ::strerror (errno) << std::endl;
      }

      signal_received = false;
      self.signal (SIGUSR1);

      ::sleep (1);

      if ( signal_received != 1 )
      {
        std::cerr << "Expected to receive signal SIGUSR1!" << std::endl;
        err++;
      }
    }
  }
  catch ( const saga::exception & e )
  {
    std::cerr << "Exception: " << e.what () << std::endl;
    err++;
  }

  if ( err )
  {
    std::cerr << "Note that the globus adaptor will cause this test to fail, "
              << "as it installes a global signal handler *curse*" << std::endl;
  }

  return err; 
}

