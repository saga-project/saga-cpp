
#include <saga/saga.hpp>

int main  ()
{
  try
  {
    saga::filesystem::directory dir ("file://localhost/tmp/source/");

    dir.copy ("file://localhost/tmp/target/test", saga::name_space::Recursive | saga::filesystem::Create | saga::filesystem::CreateParents) ;
  }
  catch ( const saga::exception & e )
  {
    std::cout << "exception: " << e.what () << std::endl;
  }
  
}

