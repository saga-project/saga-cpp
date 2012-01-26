
#include <saga/saga.hpp>
#include <iostream>

int main (int argc, char** argv)
{

  // allocate a 'large' buffer statically
  char mem[1024];
  saga::mutable_buffer buf (mem, 512);

  // open a file
  saga::url u ("/etc/passwd");
  saga::filesystem::file f (u);

  // read data into buffer - the first 512 bytes get fill
  f.read (buf);

  // seek file and buffer
  buf.set_data (mem + 512, 512);
  f.seek (123, saga::filesystem::Current);

  // read again
  f.read (buf);

  // print what we got
  std::cout << std::string (mem, 1024) << std::endl;

  return (0);
}

