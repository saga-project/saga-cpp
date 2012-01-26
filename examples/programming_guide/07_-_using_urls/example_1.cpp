
#include <saga/saga.hpp>
#include <iostream>

int main (int argc, char** argv)
{
  saga::url u  ("ftp://remote.host.net:1234/data/old.dat");

  std::cout << "url: " << u.get_string () << std::endl;

  try 
  {
    u.set_host   ("local.host/net");
  }
  catch ( const saga::bad_parameter & e ) 
  {
    std::cout << e.what () << std::endl;
  }

  u.set_path   ("/data/new.dat");

  std::cout << "url: " << u.get_string () << std::endl;

  return 0;
}

