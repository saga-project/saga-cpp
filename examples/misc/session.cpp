
#include <saga/saga.hpp>

int main ()
{
  try
  {
    for ( int i = 0; i < 10; i++ )
    {
      saga::session s = saga::get_default_session ();

      saga::filesystem::file f (s, "any://localhost/etc/passwd");
      f.copy ("/tmp/t", saga::filesystem::Overwrite);

      saga::job::service js (s, "ssh://localhost");
      saga::job::job j = js.run_job ("/bin/date");
    }
  }
  catch ( const saga::exception & e )
  {
    std::cerr << "Error: " << e.what () << std::endl;
  }
}

