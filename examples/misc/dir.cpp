
#include <saga/saga.hpp>

int main ()
{
  try 
  {
    saga::url u ("file://localhost//Users/merzky/.saga/adaptors/ssh/ssh_file/mnt/");

    std::cout << " trying to create " << u << "\n";

    saga::filesystem::directory d (u, 
                                   saga::filesystem::Create |
                                   saga::filesystem::CreateParents);
  }
  catch ( const saga::exception & e )
  {
    std::cerr << "cannot create directory\n";
  }
}

