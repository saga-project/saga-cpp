
#include <saga/saga.hpp>
#include <boost/date_time.hpp>

using namespace boost::posix_time;
using namespace boost::gregorian;

int main ()
{
  // try 
  // {
    saga::job::service js ("fork://localhost");

    saga::job::description jd;
    jd.set_attribute (saga::job::attributes::description_executable,
                      "/bin/sleep");
    jd.set_attribute (saga::job::attributes::description_interactive,
                      saga::attributes::common_false);

    std::vector <std::string> args;
    args.push_back ("1");
    jd.set_vector_attribute (saga::job::attributes::description_arguments,
                             args);

    for ( int i = 0; i < 3; i++ ) 
    {
      std::cout << "Run " << (i + 1) << ": " << std::flush;

      ptime start (microsec_clock::local_time ());

      for ( int j = 0; j < 60; j++ ) 
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
    }
  // }
  // catch ( const saga::exception & e )
  // {
  //   std::cout << " exception: " << e.what () << std::endl;
  //   return -1;
  // }

  return 0;
}

