
#include <saga/saga.hpp>

int main ()
{
  saga::session s;
  std::cout << " ================================ " << std::endl;
  std::cout << " ================================ " << std::endl;
  std::cout << " ================================ " << std::endl;
  std::cout << " ================================ " << std::endl;
  sleep (1);

  saga::url          u ("fork://localhost");
  saga::task         t  = saga::job::service::create <saga::task::Async> (u);
  saga::job::service js = t.get_result <saga::job::service> ();

  return 0;
}

