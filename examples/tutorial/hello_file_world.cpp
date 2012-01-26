//  Copyright (c) 2011 Ole Weidner, Louisiana State University
// 
//  This is part of the code examples on the SAGA website:
//  http://saga-project.org/documentation/tutorials/file-api

#include <saga/saga.hpp>

int main(int argc, char* argv[])
{

  try 
  {
    // create a new empty file
    saga::filesystem::file f_orig("/tmp/hello.txt", saga::filesystem::Create);

    // write the string "Hello, World!" to the file
    char txt[] = "Hello, World!";
    f_orig.write(saga::buffer(txt, sizeof(txt)));
    
    // copy the file
    f_orig.copy("/tmp/hello_copy.txt");
     
    // open the copy and read the content
    saga::filesystem::file f_copy("/tmp/hello_copy.txt");
    
    saga::mutable_buffer buf (1024*64);
    while (true) 
    {
      // read a chunk into the buffer
      if ( f_copy.read (buf, 1024*64) ) {
        std::cout << (char*) (buf.get_data ());
      }
      else break;
    }
    
    // delete both files
    f_orig.remove();
    f_copy.remove();
  } 
  catch(saga::exception const & e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}

