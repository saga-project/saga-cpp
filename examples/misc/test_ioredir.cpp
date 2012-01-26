
#include <saga/saga.hpp>
#include <vector>

int main ()
{
  try 
  {
    saga::job::service s("ssh://localhost");

    saga::job::description d;
    d.set_attribute(saga::job::attributes::description_executable, "/bin/sleep");

    std::vector<std::string> args;
    args.push_back("1");
    d.set_vector_attribute(saga::job::attributes::description_arguments, args);
    // d.set_attribute(saga::job::attributes::description_output, "myjob.out");

    saga::job::job j = s.create_job(d);

    std::cout << "running job" << std::endl;
    j.run();

    std::cout << "waiting until job is finished" << std::endl;
    j.wait();

    std::cout << "done" << std::endl;

  }
  catch ( const saga::exception & e )
  {
    std::cout << " SAGA exception: " << e.what () << std::endl;
    return -1;
  }

  return 0;
}

