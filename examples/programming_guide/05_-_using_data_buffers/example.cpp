
#include <saga/saga.hpp>
#include <iostream>

int main (int argc, char** argv)
{
  // allocate a 'large' buffer statically
  char mem[1024];

  // create a saga buffer object for reading (mutable)
  saga::mutable_buffer buf (mem, 512);

  // open a file
  saga::url u ("/etc/passwd");
  saga::filesystem::file f (u);

  // read data into buffer - the first 512 bytes get fill
  f.read (buf);

  // seek the buffer, so that the next read goes into the
  // second half of the buffer
  buf.set_data (mem + 512, 512);

  // now read again
  f.read (buf);

  // the complete buffer should be filled now
  // print what we got
  std::cout << std::string (mem, 1024) << std::endl;

  return 0;
}

