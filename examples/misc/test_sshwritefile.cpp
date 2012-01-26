
#include <saga/saga.hpp>
#include <vector>

int main ()
{
  try 
  {
    // saga::url dir_url("ssh://qb4.loni.org/tmp");
    saga::url dir_url("ssh://qb4.loni.org/tmp");
    saga::filesystem::directory dir(dir_url);

    std::cout << "creating file" << std::endl;
    saga::filesystem::file foo = dir.open("foo", saga::filesystem::Create);

    if ( dir.exists ("foo") )
    {
      std::cout << "foo now exists" << std::endl;
    }
    else
    {
      std::cout << "foo does not exist" << std::endl;
      return -1;
    }

    std::cout << "writing 10 bytes" << std::endl;
    saga::mutable_buffer write_buf(10);
    foo.write(write_buf);

    std::cout << "closing file" << std::endl;
    foo.close();
    
  }
  catch ( const saga::exception & e )
  {
    std::cout << " SAGA exception: " << e.what () << std::endl;
    return -1;
  }

  return 0;
}

