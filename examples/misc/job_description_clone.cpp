
#include <saga/saga.hpp>
#include <boost/date_time.hpp>

using namespace boost::posix_time;
using namespace boost::gregorian;

int main ()
{
  try 
  {
    saga::job::description jd;
    jd.set_attribute (saga::job::attributes::description_executable,   "/bin/sleep");
    jd.set_attribute (saga::job::attributes::description_input,        "stdin");
    jd.set_attribute (saga::job::attributes::description_output,       "stdout");
    jd.set_attribute (saga::job::attributes::description_error,        "stderr");
    jd.set_attribute (saga::job::attributes::description_queue,        "queue");
    jd.set_attribute (saga::job::attributes::description_interactive, saga::attributes::common_false);

    std::vector <std::string> args;
    args.push_back (" 1");
    args.push_back (" 2");
    args.push_back (" 3");
    args.push_back (" 4");
    args.push_back (" 5");
    args.push_back (" 6");
    args.push_back (" 7");
    args.push_back (" 8");
    args.push_back (" 9");
    args.push_back ("10");
    jd.set_vector_attribute (saga::job::attributes::description_arguments, args);

    std::vector <std::string> hosts;
    hosts.push_back ("host_1");
    hosts.push_back ("host_2");
    hosts.push_back ("host_3");
    hosts.push_back ("host_4");
    hosts.push_back ("host_5");
    hosts.push_back ("host_6");
    hosts.push_back ("host_7");
    hosts.push_back ("host_8");
    hosts.push_back ("host_9");
    hosts.push_back ("host_10");
    jd.set_vector_attribute (saga::job::attributes::description_candidate_hosts, hosts);

    for ( int i = 0; i < 100000; i++ )
    {
      saga::job::description jd_2 = (saga::job::description) jd.clone ();

      // std::cout << i << " " << jd_2.get_attribute (saga::job::attributes::description_executable) << " ";
    }

    // std::cout << std::endl;

  }
  catch ( const saga::exception & e )
  {
    std::cout << " exception: " << e.what () << std::endl;
    return -1;
  }

  return 0;
}

