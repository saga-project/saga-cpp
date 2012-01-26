
#include <unistd.h>
#include <iostream>

#include <saga/saga.hpp>

bool my_cb (saga::monitorable o, 
            saga::metric      m, 
            saga::context     c)
{
  try
  {
    std::cout << "state changed : " 
              << m.get_attribute (saga::attributes::metric_value) 
              << std::endl;
  }
  catch ( const saga::exception & e )
  {
    std::cerr << "saga exception in cb: " << e.what () << std::endl;
  }

  return true; // stay registered
}


int main ()
{
  // try job state metric
  try
  {
    saga::job::job            j;
    saga::job::service        js;
    saga::job::description    jd;
    std::vector <std::string> args;

    // FIXME: why can't I add a cb right here?  objecttype seems to be
    // incorrectly set to 'Task' at this point...
    // j.add_callback (saga::job::metrics::state, boost::bind (my_cb, _1, _2, _3));


    args.push_back ("3");

    jd.set_attribute (saga::job::attributes::description_executable, "/bin/sleep");
    jd.set_attribute (saga::job::attributes::description_interactive,
                      saga::attributes::common_false);

    jd.set_vector_attribute (saga::job::attributes::description_arguments, args);

    j = js.create_job (jd);

    j.add_callback (saga::job::metrics::state, boost::bind (my_cb, _1, _2, _3));
    saga::metric m = j.get_metric (saga::job::metrics::state);
    std::cout << "job created   : " << m.get_attribute (saga::attributes::metric_value) <<  std::endl;

    j.run ();

    std::cout << "job started   : " << m.get_attribute (saga::attributes::metric_value) <<  std::endl;


    j.wait ();

    std::cout << "job waited    : " << m.get_attribute (saga::attributes::metric_value) <<  std::endl;

    ::sleep (1);
  }
  catch ( const saga::exception & e )
  {
    std::cerr << "saga exception: " << e.what () << std::endl;
  }

  return 0;
}

