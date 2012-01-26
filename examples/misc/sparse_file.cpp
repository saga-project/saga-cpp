
#include <saga/saga.hpp>

#define TMPFILE "file://localhost/tmp/test.dat"

int main ()
{
  try
  {
    saga::filesystem::file f (TMPFILE, saga::filesystem::Create);
    saga::const_buffer     b ("test\n", 6);

    f.seek  (0, saga::filesystem::Start);
    f.write (b);

    f.seek  (1024, saga::filesystem::Current);
    f.write (b);

    f.seek  (1024 * 1024, saga::filesystem::Current);
    f.write (b);

    f.seek  (1024 * 1024 * 1024, saga::filesystem::Current);
    f.write (b);
  }
  catch ( const saga::exception & e)
  {
    std::cerr << " saga exception: " << e.what () << std::endl;
    return -1;
  }

  return 0;
}

