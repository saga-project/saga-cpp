
#include <saga/saga.hpp>

int main ()
{
  try
  {
    saga::session s = saga::get_default_session();
    saga::context c("ssh");
    s.add_context(c);

    saga::filesystem::directory d("ssh://localhost/tmp/");
  }
  catch ( const saga::exception & e )
  {
    std::cerr << "exception: " << e.what () << std::endl;
    return -1;
  }


  return 0;
}

