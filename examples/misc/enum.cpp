

#include <saga/saga.hpp>

int main ()
{
  saga::job::service js;
  saga::task t = js.get_url <saga::task_base::Task> ();
 
  return 0;
}

