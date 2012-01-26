
#include <iostream>

#include <saga/saga.hpp>

int main ()
{
  try
  {
    saga::filesystem::file f ("file://localhost/does/not/exist");
    std::cout << "Size: " << f.get_size () << std::endl;
  }
  catch ( const saga::does_not_exist & e )
  {
    std::cout << "DoesNotExist exception: " << e.what () << std::endl;
  }
  catch ( const saga::exception & e )
  {
    std::cout << "Other exception: " << e.what () << std::endl;
  }

  return 0;
}

