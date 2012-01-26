
#include <saga/saga.hpp>

int main ()
{
  try 
  {
    saga::session session;

    saga::filesystem::directory d (session, "any://localhost/");

    saga::task t2 = d.get_url <saga::task::Async> ();

    std::cout << " === fake task created: " 
              << t2.get_id () << " - " 
              << t2.get_state () << std::endl;

    saga::job::service js (session, "fork://localhost/");

    saga::task t1 = js.get_url <saga::task::Async> ();

    std::cout << " === fake task created: " 
              << t1.get_id () << " - " 
              << t1.get_state () << std::endl;
  }
  catch ( const saga::exception & e )
  {
    std::cerr << " error: " << e.what () << std::endl;
  }

  return 0;
}

