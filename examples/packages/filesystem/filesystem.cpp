#include <saga/saga.hpp>

///////////////////////////////////////////////////////////////////////////////
int main (int argc, char* argv[])
{ 
  try 
  {
    saga::url u(argv[1]);
    
    // open a name file 
    saga::filesystem::file f(u); 

    // seek 200 bytes 
    f.seek(200, saga::filesystem::Current); 
   
    // read 100 byte 
    saga::mutable_buffer b; 
    f.read(b, 100); 
 
    // print the data 
    std::cout << b.get_data() << std::endl;
  }
  catch(saga::exception const & e) 
  {
    std::cout << "Ooops: " << e.what() << std::endl;
  }  

  return 0;
}

