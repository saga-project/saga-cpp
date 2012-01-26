
#include <saga/saga.hpp>

int main (int argc, char** argv)
{
  saga::job::service js;
  saga::job::job     j = js.run_job ("/bin/sleep 3",
                                     "localhost");
  j.wait ();

  return (0);
}

