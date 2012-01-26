
#include <saga/saga.hpp>

int main (int argc, char** argv)
{
  // run a file copy asynchronously
  saga::url u ("/tmp/saga_tutorial/test1");
  saga::filesystem::file f (u);
  saga::task t = f.copy <saga::task::Async> (saga::url ("/tmp/saga_tutorial/test2"));

  // do something else

  // wait for the copy task to finish
  t.wait ();

}

