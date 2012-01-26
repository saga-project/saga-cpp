
#include <saga/saga.hpp>
#include <saga/saga/adaptors/utils.hpp>

int main (int argc, char** argv)
{
  std::string host;

  if ( argc >= 2 )
  {
    host = argv[1];
  }
  else
  {
    std::cout << "no host given - using localhost\n";
    host = "localhost";
  }


  if ( saga::adaptors::utils::is_local_address (host) )
  {
    std::cout << host << " is localhost\n";
  }
  else
  {
    std::cout << host << " is not localhost\n";
  }

  return 0;
}

