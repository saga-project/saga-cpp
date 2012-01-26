
#include <saga/saga.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace boost::posix_time;

int main (int argc, char** argv)
{
  try 
  {
    std::cout << "Please check: http://faust.cct.lsu.edu/trac/saga/ticket/559 for details."  << std::endl;

    saga::job::service js ("fork://localhost");

    saga::job::description jd;
    jd.set_attribute (saga::job::attributes::description_executable, "/bin/sleep");

    std::vector <std::string> args;
    args.push_back ("1");
    jd.set_vector_attribute(saga::job::attributes::description_arguments, args);

    for ( int i = 0; i < 1; i++ ) 
    {
      std::cout << "Run " << (i + 1) << ": " << std::flush;

      ptime start (microsec_clock::local_time ());

      for ( int j = 0; j < 10; j++ ) 
      {
        std::cout << "." << std::flush;

        saga::job::job j = js.create_job (jd);
        
        j.run  ();
        j.wait ();
      }

      ptime end (microsec_clock::local_time());

      time_duration td = end - start;
      double sec = (double) td.total_milliseconds() / 1000;
      std::cout << sec << " sec" << std::endl;

      if ( sec > 13 )
      {
        // thats too long!
        return -1;
      }

    }
  }
  catch ( saga::exception const & e )
  {
    std::cerr << "\nException: " << e.what () << std::endl;
    return -1;
  }

  return 0;
}

