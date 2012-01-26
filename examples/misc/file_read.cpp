
#include <saga/saga.hpp>

int main ()
{
  try 
  {
    saga::filesystem::file f ("file://localhost/etc/passwd",
                              saga::filesystem::Read);

    saga::mutable_buffer buf (10);

    while ( true )
    {
      int ret = f.read (buf);

      if ( ret > 0 )
      {
        std::cout << " " << (char*) (buf.get_data ()) ;
      } 
      else
      {
        std::cout << "read failed" << std::endl;
        break;
      }
    }
  }
  catch ( const saga::exception & e )
  {
    std::cerr << "cannot create directory\n";
  }

  return 0;
}

