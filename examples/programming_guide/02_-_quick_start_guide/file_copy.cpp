
#include <saga/saga.hpp>

int main (int argc, char** argv)
{

  // do a file copy
  saga::url u(argv[1]);
  saga::filesystem::file f (u);
  f.copy (saga::url (argv[2]));
 
  // saga::url u ("/tmp/saga_tutorial/test");
  // saga::filesystem::file f (u);
  // f.copy (saga::url ("/tmp/saga_tutorial/test1"));
}

