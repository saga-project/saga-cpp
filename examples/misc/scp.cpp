
#include <saga/saga.hpp>

int main ()
{
  saga::job::description jd;

  std::vector <std::string> arguments;

  arguments.push_back ("qb3.loni.org:/tmp/test_1");
  arguments.push_back ("qb3.loni.org:/tmp/test_2");

  jd.set_attribute (saga::job::attributes::description_executable, "scp");

  jd.set_vector_attribute (saga::job::attributes::description_arguments, arguments);

  saga::job::service js;
  saga::job::job     j = js.create_job (jd);

  j.run  ();
  j.wait ();

  if ( j.get_state () != saga::job::Done )
  {
    std::cout << "Copy failed ..." << std::endl;
  }
  else
  {
    std::cout << "Copy success ..." << std::endl;
  }

  return 0;
}

