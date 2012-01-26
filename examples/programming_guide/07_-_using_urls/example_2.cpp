
#include <saga/saga.hpp>
#include <iostream>
#include <vector>

int main (int argc, char** argv)
{
  // Translating URLs
  saga::url in ("http://data.silo.net/data/joe_doe/recent/abc.dat");
  saga::url out = in.translate ("ftp://");

  std::cout <<  in.get_string () << std::endl;
  std::cout << out.get_string () << std::endl;
  
  return 0;
}

