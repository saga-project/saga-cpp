
#include <saga/saga.hpp>

int main ()
{
  try 
  {
    saga::filesystem::directory d ("file://localhost//etc/");
    d.copy ("file://localhost/tmp/p1", saga::filesystem::Recursive);
  }
  catch ( const saga::exception & e )
  {
    std::cerr << "Exception: \n" << e.what () << std::endl;
  }

  return 0;
}

