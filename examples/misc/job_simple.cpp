
#include <saga/saga.hpp>
#include <boost/date_time.hpp>

using namespace boost::posix_time;
using namespace boost::gregorian;

int main ()
{
  try 
  {
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

    saga::job::job j = js.create_job (jd);
    j.run  ();
    j.wait ();
  }
  catch ( const saga::exception & e )
  {
    std::cout << " exception: " << e.what () << std::endl;
    return -1;
  }

  return 0;
}

