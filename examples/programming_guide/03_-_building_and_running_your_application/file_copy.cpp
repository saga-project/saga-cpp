
#include <saga/saga.hpp>

int main (int argc, char** argv)
{
  if ( argc < 3 ) { return -1; }
  
  // do a file copy
  saga::url u (argv[1]);
  saga::filesystem::file f (u);
  f.copy (saga::url (argv[2]));
}

