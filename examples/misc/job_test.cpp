
#include <saga/saga.hpp>

#include <iostream>
#include <fstream>

void jd_dump (saga::job::description & jd);

int main (int argc, char ** argv)
{
  try 
  {
    saga::job::service js ("fork://localhost");
    saga::job::job     j;

    j = js.run_job ("/bin/false");
    j = js.run_job ("/bin/sleep 10");

    ::sleep (1);

    if ( j.get_state() != saga::job::Running)
      std::cerr << "not Running"<< std::endl;
    
    j.suspend ();

    if ( j.get_state() != saga::job::Suspended)
      std::cerr << "not Suspended"<< std::endl;
    
    j.resume  ();

    if ( j.get_state() != saga::job::Running)
      std::cerr << "not Running"<< std::endl;
    
    j.wait();
    
    if ( j.get_state() != saga::job::Done)
      std::cerr << "not Done"<< std::endl;
  }
  catch ( saga::exception e )
  {
    std::cout << e.what ();
  }

  std::cout << "test 1 ok" << std::endl; 
  return 0;
}

int test_2 () 
{
  try 
  {
    std::fstream l ("test.log", std::fstream::out | std::fstream::app);
    l << "--------------------------\n" << std::flush;
    l << getpid () << " -- " << " main    start\n" << std::flush;
    saga::job::service js (saga::url ("fork://localhost"));

    saga::job::self        s  = js.get_self ();
    saga::job::description jd = s.get_description ();

    jd.set_attribute (saga::job::attributes::description_interactive, 
                      saga::attributes::common_false);


    int count = ::atoi (::getenv("COUNTER"));
    count++;

    l << getpid () << " -- " << "count:   " << count << "\n" << std::flush;



    SAGA_OSSTREAM tmp;
    tmp << count;

    ::setenv ("COUNTER", SAGA_OSSTREAM_GETSTRING(tmp).c_str (), 1);
    l << getpid () << " -- " << " migrate start\n" << std::flush;
    s.migrate (jd);
    l << getpid () << " -- " << " migrate done\n" << std::flush;


    l << getpid () << " -- " << " main    done\n" << std::flush;
    l.close ();

    return (0);

  }
  catch ( saga::exception e )
  {
    std::cout << e.what ();
  }

  return 0;
}


int test_4 (int argc, char ** argv)
{
  using namespace std;
  using namespace saga;
  using namespace saga::job::attributes;

  if (argc < 2)
  {
    cerr << "Remote command was not specified." << endl;
    return 2;
  }

  job::service js("condor://localhost"); 

  job::description jd;
  jd.set_attribute(description_executable, argv[1]);

  if (argc > 2)
  {
    vector<string> args;
    for (int i = 2; i < argc; ++i)
      args.push_back(argv[i]);
    jd.set_vector_attribute(description_arguments, args);
  }

  job::job job = js.create_job(jd);
  job.run();

  std::cout << "jobid is: " << job.get_job_id() << std::endl;
}


int test_3 ()
{
  try 
  {
    saga::job::service js (saga::url ("fork://localhost"));

    for ( int i = 1; i < 2 ; i++ )
    {
      std::cout << "." << std::flush;

      saga::job::ostream in;
      saga::job::istream out;
      saga::job::istream err;

      SAGA_OSSTREAM cmd;
      cmd << "/usr/bin/touch /tmp/test-" << i;
      saga::job::job j = js.run_job (SAGA_OSSTREAM_GETSTRING (cmd), "localhost");

      j.wait ();

      if ( ! (i % 100) )
      {
        std::cout << " " << i <<  std::endl;
      }
    }


    // saga::job::self        s  = js.get_self ();
    // saga::job::description jd = s.get_description ();
    // jd_dump ();
  }
  catch ( saga::exception e )
  {
    std::cout << e.what ();
  }

  return 0;
}

void jd_dump (saga::job::description & jd)
{
  std::vector <std::string> attr = jd.list_attributes ();

  for ( int i = 0; i < attr.size (); i++ )
  {
    std::cout << "  " << attr[i] << "\t : ";

    if ( ! jd.attribute_is_vector (attr[i]) )
    {
      std::cout << jd.get_attribute (attr[i]);
    }
    else
    {
      std::vector <std::string> vals = jd.get_vector_attribute (attr[i]);

      for ( int j = 0; j < vals.size (); j++ )
      {
        std::cout << vals[j] << " ";
      }
    }

    std::cout << std::endl;
  }
}  

