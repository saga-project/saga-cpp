#include <saga/saga.hpp>

///////////////////////////////////////////////////////////////////////////////
int main (int argc, char* argv[])
{ 
  try 
  {
    saga::url u(argv[1]);

    std::cout << "host: " << u.get_host () << std::endl;

    u.set_host ("Macintosh.local");

    std::cout << "host: " << u.get_host () << std::endl;
    std::cout << "path: " << u.get_path () << std::endl;
  }
  catch(saga::exception const & e) 
  {
    std::cout << "Ooops: " << e.what() << std::endl;
  }  

  try 
  {
    for ( int i = 0; i < 100000; i ++ )
    {
      saga::url u ("./Makefile");
      saga::filesystem::file f (u, saga::filesystem::Read);

      char data[1];
      saga::mutable_buffer b (data, 1);

      f.seek (i % f.get_size (), saga::filesystem::Current);
      f.read (b, 1);

      std::cout << data;
    }
  }
  catch(saga::exception const & e) 
  {
    std::cout << "Ooops: " << e.what() << std::endl;
  }  

  return 0;
}

