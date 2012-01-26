
#include <saga/saga.hpp>

int main ()
{
  try
  {
    saga::session s;
    saga::context c ("fgeuca");

    s.add_context (c);
  }
  catch ( const saga::exception & e )
  {
    std::cerr << "Exception: " << e.what () << std::endl;
    return -1;
  }

  return 0;
}

