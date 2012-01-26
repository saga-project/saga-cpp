
#include <saga/saga.hpp>

class my_cb : public saga::callback
{
  bool cb (saga::object  o,
           saga::metric  m,
           saga::context c)
  {
    std::cout << "cb called" << std::endl;

    return true;
  }
};

int main ()
{
  try
  {
    saga::job::service js;

    saga::task_container tc;

    std::cout << " adding jobs " << std::endl;

    for ( int i = 1; i < 11; i++ )
    {
      std::vector <std::string> args;
      std::stringstream ss;
      ss << i;

      args.push_back (ss.str ());

      saga::job::description jd;

      jd.set_attribute        ("Executable", "/bin/sleep");
      jd.set_vector_attribute ("Arguments",  args);

      saga::job::job j = js.create_job (jd);

      std::cout << " - " << j.get_state ();

      my_cb cb;
      j.add_callback (saga::job::metrics::state, cb);

      tc.add_task (j);

      std::cout << "   job " << i << " added" << std::endl;
    }

    std::cout << " running jobs" << std::endl;

    tc.run ();


    std::cout << " collecting jobs" << std::endl;

    my_cb cb;
    tc.add_callback ("task.state", cb);

    int k = 0;
    while ( tc.list_tasks ().size () )
    {
      std::cout << " tc size: " << tc.list_tasks ().size () << std::endl;

      std::vector <saga::task> tasks = tc.wait (saga::task_container::Any);

      for ( unsigned int j = 0; j < tasks.size (); j++ )
      {
        k++;
        std::cout << k << std::endl;
        std::cout << k << " - " << j+1 << " - " << tasks[j].get_state () << std::endl;
        // saga::job::job j = dynamic_cast <saga::job::job> (tasks[j]);

        // std::cout << j << " done: " << j.get_job_id () << std::endl;
      }
    }
  }
  catch ( const saga::exception & e )
  {
    std::cerr << " Error: " << e.what () << std::endl; 
    return -1;
  }

  return 0;
}

