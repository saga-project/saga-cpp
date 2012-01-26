
#include <netdb.h>

#include <saga/saga.hpp>
#include <saga/saga/adaptors/utils.hpp>

int main (int argc, char** argv)
{
  int err = 0;

  try
  {
    /////////////////////////////////////////////////////////////////////////
    // for get_job_description, we try to get the job description for self, and
    // check if ARGV and execution host are set correctly.  The we create a jd,
    // run a job with it, and compare with the jd delivered by the job.
    /////////////////////////////////////////////////////////////////////////

    namespace sja = saga::job::attributes;
    namespace sa  = saga::attributes;

    /////////////////////////////////////////////////////////////////////////
    // get self, and my own jd
    {
      saga::session          s;
      saga::job::service     js (s);
      saga::job::self        self = js.get_self ();
      saga::job::description jd   = self.get_description ();

      if ( ! jd.attribute_exists (sja::description_executable) )
      {
        std::cerr << "Expected Executable in job description" << std::endl;
        err++;
      }
      else
      {
        if ( jd.get_attribute ("Executable") != argv[0] )
        {
          std::cerr << "Expected " << argv[0] << " as Executable in job description" << std::endl;
          err++;
        }
      }

      // can we compare args?
      if ( argc > 0 )
      {
        if ( ! jd.attribute_exists (sja::description_arguments) )
        {
          std::cerr << "Expected Arguments in job description" << std::endl;
          err++;
        }
        else
        {

          std::vector <std::string> jd_args = jd.get_vector_attribute (sja::description_arguments);

          if ( jd_args.size () != (unsigned int) argc - 1)
          {
            std::cerr << "Expected " << argc - 1 << " args, not " << jd_args.size () << std::endl;
            err++;
          }

          for ( unsigned int i = 0; i < jd_args.size (); i++ )
          {
            if ( jd_args[i] != argv[i + 1] )
            {
              std::cerr << "Expected " << argv[i+1] << " as " << i+1 << "'th args, not " 
                        << jd_args[i] << std::endl;
              err++;
            }
          }
        }
      } // argc > 0


      // check candidate host, which should be localhost
      if ( ! jd.attribute_exists (sja::description_candidate_hosts) )
      {
        std::cerr << "Expected CandidateHosts in job description" << std::endl;
        err++;
      }
      else
      {
        std::vector <std::string> jd_hosts = jd.get_vector_attribute (sja::description_candidate_hosts);

        if ( jd_hosts.size () != 1 )
        {
          std::cerr << "Expected 1 candidate host, not " << jd_hosts.size () << std::endl;
          err++;
        }


        if ( ! saga::adaptors::utils::is_local_address (jd_hosts[0]) )
        {
          std::cerr << "Expected 'localhost' (or a valid alias) as candidate host, not " 
                    << jd_hosts[0] << std::endl;
          err++;
        }
      } // candidate_hosts
    } // test for self


    /////////////////////////////////////////////////////////////////////////
    // create a jd, create a job with it, and check the returned new jd
    {
      saga::session          s;
      saga::job::service     js (s);
      saga::job::description old_jd;

      old_jd.set_attribute (sja::description_executable, "/bin/sleep");
      old_jd.set_attribute (sja::description_interactive, sa::common_false);

      std::vector <std::string> args;
      args.push_back ("10");
      old_jd.set_vector_attribute (sja::description_arguments, args);

      std::vector <std::string> hosts;
      hosts.push_back ("localhost");
      old_jd.set_vector_attribute (sja::description_candidate_hosts, hosts);

      saga::job::job j = js.create_job (old_jd);

      j.run ();

      saga::job::description new_jd = j.get_description ();


      if ( ! new_jd.attribute_exists (sja::description_executable) )
      {
        std::cerr << "Expected Executable in job description" << std::endl;
        err++;
      }
      else
      {
        if ( new_jd.get_attribute ("Executable") != "/bin/sleep" )
        {
          std::cerr << "Expected /bin/sleep as Executable in job description, not " 
                    << new_jd.get_attribute ("Executable") << std::endl;
          err++;
        }
      }

      // can we compare args?
      if ( ! new_jd.attribute_exists (sja::description_arguments) )
      {
        std::cerr << "Expected Arguments in job description" << std::endl;
        err++;
      }
      else
      {
        std::vector <std::string> jd_args = new_jd.get_vector_attribute (sja::description_arguments);

        if ( jd_args.size () != 1 )
        {
          std::cerr << "Expected 1 argument, not " << jd_args.size () << std::endl;
          err++;
        }

        if ( jd_args[0] != "10" )
        {
          std::cerr << "Expected '10' as argument, not " 
                    << jd_args[0] << std::endl;
          err++;
        }
      }
      // argc > 0


      // check candidate host, which should be localhost
      if ( ! new_jd.attribute_exists (sja::description_candidate_hosts) )
      {
        std::cerr << "Expected CandidateHosts in job description" << std::endl;
        err++;
      }
      else
      {
        std::vector <std::string> jd_hosts = new_jd.get_vector_attribute (sja::description_candidate_hosts);

        if ( jd_hosts.size () != 1 )
        {
          std::cerr << "Expected 1 candidate host, not " << jd_hosts.size () << std::endl;
          err++;
        }


        bool match = false;

        struct hostent * he = ::gethostbyname ("localhost");
        char** aliases = he->h_aliases;

        while ( *aliases != NULL )
        {
          if ( jd_hosts[0] == *aliases )
          {
            match = true;
          }

          aliases++;
        }

        if ( ! match && jd_hosts[0] != "localhost" )
        {
          std::cerr << "Expected 'localhost' (or a valid alias) as candidate host, not " 
            << jd_hosts[0] << std::endl;
          err++;
        }
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

