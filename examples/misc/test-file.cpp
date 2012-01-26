#include <saga/saga.hpp>

///////////////////////////////////////////////////////////////////////////////
int main (int argc, char* argv[])
{ 
  std::string in;

  try 
  {
    saga::url u1  ("file://localhost/etc/group" );
    saga::url u2  ("file://localhost/etc/groups");
    saga::url u3  ("file://localhost/etc/shell" );
    saga::url u4  ("file://localhost/etc/passwd");
    saga::url tgt ("file://localhost/tmp/test");

    saga::filesystem::file f1 (u1);
    std::cout <<  " --- 1 " << std::endl;
    f1.copy (tgt);
    std::cout <<  " --- 2 " << std::endl;
    f1.copy (tgt);
    std::cout <<  " --- 3 " << std::endl;
  }
  catch ( saga::exception const & e ) 
  {
    std::cout << "Ooops: " << e.what () << std::endl;
  }  

  return 0;
}

