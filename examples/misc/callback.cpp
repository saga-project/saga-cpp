

#include <iostream>

#include <unistd.h>

#include <saga/saga.hpp>

bool my_cb (saga::monitorable o, 
            saga::metric      m, 
            saga::context     c)
{
  try
  {
    std::cout << "state changed: " 
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
    saga::job::service js;
    saga::job::job j = js.run_job ("/bin/sleep 3");

    std::cout << "job started" << std::endl;

    j.add_callback (saga::job::metrics::state, boost::bind (my_cb, _1, _2, _3));

    ::sleep (10);
  }
  catch ( const saga::exception & e )
  {
    std::cerr << "saga exception: " << e.what () << std::endl;
  }


  // try task state metric
  try
  {
    saga::filesystem::file f ("/tmp/in", saga::filesystem::Create);

    saga::task t = f.copy <saga::task_base::Task> ("/tmp/out");

    t.add_callback (saga::metrics::task_state, boost::bind (my_cb, _1, _2, _3));

    t.run ();

    ::sleep (10);
  }
  catch ( const saga::exception & e )
  {
    std::cerr << "saga exception: " << e.what () << std::endl;
  }

  return 0;
}

