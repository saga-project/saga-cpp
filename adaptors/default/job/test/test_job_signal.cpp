
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

#include <saga/saga.hpp>

void sighandler (int sig)
{
  if ( sig != SIGUSR1 )
  {
    // oops - some other signal!  Ignore
    std::cout << "wrong signal?" << std::endl;
    return;
  }

  // all seems fine, sighandler got triggered
  if ( 0 != ::system ("touch test_job_signal_flag_ok") )
  {
    std::cerr << "Could not touch ok flag" << std::endl;
    // oops - we can't tell the parent!
    ::exit (-1);
  }

  // we are done
  ::exit (0);
}


int main (int argc, char** argv)
{
  int err = 0;

  try
  {
    /////////////////////////////////////////////////////////////////////////
    // the signal test works just like the checkpoint test, only we explicitely
    // send SIGUSR1.
    //
    // So, first spawn test job
    pid_t child = ::fork ();


    ////////////////////////////////////////////////////////////////////////////
    if ( -1 == child )
    {
        std::cerr << "fork failed" << std::endl;
        return -1;
    }
    ////////////////////////////////////////////////////////////////////////////


    ////////////////////////////////////////////////////////////////////////////
    else if ( 0 == child )
    {
      // this is the child which is getting signalled.  So, install the
      // signal handler
      if ( SIG_ERR == ::signal (SIGUSR1, sighandler) )
      {
        std::cerr << "Could not register signal handler" << std::endl;
        // oops - not much we can do
        ::exit (-2);
      }

      // allow signal to arrive
      ::sleep (3);

      // nothing happened?  flag error
      if ( 0 != ::system ("touch test_job_signal_flag_nok") )
      {
        // oops - we can't tell the parent!
        std::cerr << "Could not touch nok flag" << std::endl;
        ::exit (-3);
      }
    }
    ////////////////////////////////////////////////////////////////////////////


    ////////////////////////////////////////////////////////////////////////////
    else 
    {
      // this is the parent who does the SAGA test.  So, first reconnect to the
      // job
      std::stringstream ss;
      ss << "[fork://localhost]-[" << child << "]";

      saga::session      s;
      saga::job::service js (s, "fork://localhost");
      saga::job::job     j = js.get_job (ss.str ());

      if ( j.get_state () != saga::job::Running )
      {
        std::cerr << "state != Running" << std::endl;
        err++;
      }

      j.signal (SIGUSR1);

      // allow child to finish
      int status = 0;
      if ( -1 == ::waitpid (child, &status, 0) )
      {
        std::cerr << "waitpid failed" << std::endl;
        ::exit (-4);
      }

      if ( j.get_state () != saga::job::Done )
      {
        std::cerr << "state != Done" << std::endl;
        err++;
      }

      if ( 0 == ::system ("test -f test_job_signal_flag_ok") )
      {
        // all is fine - just remove the tag file
        ::system ("rm test_job_signal_flag_ok");
      }
      else
      {
        // oops - lets see if the child said anything
        if ( 0 == ::system ("test -f test_job_signal_flag_nok") )
        {
          std::cerr << "child did not receive signal" << std::endl;
          err++;
          
          // remove tag file
          ::system ("rm test_job_signal_flag_nok");
        }
        else
        {
          std::cerr << "child disappeared" << std::endl;
          err++;
        }
      }
    }
    ////////////////////////////////////////////////////////////////////////////
  }
  catch ( const saga::exception & e )
  {
    std::cerr << "Exception: " << e.what () << std::endl;
    err++;
  }

  return err; 
}

