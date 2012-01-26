
#include <saga/saga.hpp>
#include <vector>

int main ()
{
  try 
  {
    saga::filesystem::directory basedir("ssh://qb.loni.org/tmp/");
    basedir.make_dir ("testdir");
    basedir.close ();
  }
  catch ( const saga::exception & e )
  {
    std::cout << " SAGA exception: " << e.what () << std::endl;
    return -1;
  }

  return 0;
}

