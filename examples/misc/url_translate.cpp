#include <saga/saga.hpp>

///////////////////////////////////////////////////////////////////////////////
int main (int argc, char* argv[])
{
  try 
  {
    saga::url in("http://data.silo.net/data/joe_doe/recent/abc.dat"); 
    saga::url out = in.translate ("ftp://"); 

    std::cout << in.get_string () << std::endl; 
    std::cout << out.get_string () << std::endl; 
  }
  catch(saga::exception const & e) 
  {
    std::cout << "Ooops: " << e.what() << std::endl;
  }  

  return 0;
}

