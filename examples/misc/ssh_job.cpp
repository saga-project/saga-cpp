
#include <saga/saga.hpp>

int main ()
{
  try 
  {
    {
      saga::context c;
      c.set_attribute ("Type",   "ssh");
      c.set_attribute ("UserID", "merzky");

      saga::session s;
      s.add_context (c);

      saga::job::description jd;
      jd.set_attribute ("Executable", "/bin/ls");
      jd.set_attribute ("Interactive", "True");
      jd.set_attribute ("WorkingDirectory", "/tmp/");

      std::vector <std::string> hosts;
      hosts.push_back ("abrakadabra");
      hosts.push_back ("hokuspokus");
      hosts.push_back ("localhost");
      jd.set_vector_attribute (saga::job::attributes::description_candidate_hosts,
                               hosts);


      saga::job::service js ("ssh://");

      //  saga::job::ostream in ;
      saga::job::istream out;
      saga::job::istream err;

      saga::job::job job = js.create_job (jd);

      job.run ();

      //  in  = job.get_stdin  ();
      out = job.get_stdout ();
      err = job.get_stderr ();


      // get job state
      saga::job::state state = job.get_state ();

      if ( state != saga::job::Running && state != saga::job::Done    )
      {
        std::cerr << "run unexpectedly failed!" << std::endl;
      }

      char buffer[1024*64];
      while ( out.good () ) 
      {
        // get stdout
        out.read (buffer, sizeof (buffer));
        if ( out.gcount () > 0 )
        {
          std::cout <<  std::string (buffer, out.gcount ()) << std::flush;
        }
      }

      std::cout << "end stdout" << std::endl;

      while ( err.good () ) 
      {
        // get stderr
        err.read (buffer, sizeof (buffer));
        if ( err.gcount () > 0 )
        {
          std::cerr <<  std::string (buffer, err.gcount ()) << std::flush;
        }
      }

      std::cout << "end stderr" << std::endl;

      job.wait ();

      if ( job.get_state () == saga::job::Done )
      {
        std::cout << "job done\n";
      }
      else
      {
        std::cout << "job failed\n";
      }
    }
  }
  catch ( const saga::exception & e )
  {
    std::cerr << e.what () << std::endl;
  }

  return 0;
}

