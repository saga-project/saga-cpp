
#include <saga/saga.hpp>

int main ()
{
  try 
  {
    saga::session s = saga::get_default_session ();
    saga::context c ("ssh");
    
    s.add_context (c);

    saga::filesystem::file f (s, "ssh://amerzk@gg101.cct.lsu.edu//etc/passwd");

    std::cout << f.get_url () << ": " << f.get_size () << std::endl;
  }
  catch ( const saga::exception & e )
  {
    std::cerr << " --- what ----------------------" << std::endl;
    std::cerr << e.what ();
    std::cerr << " -------------------------------" << std::endl;
    std::cerr << " --- get_message ---------------" << std::endl;
    std::cerr << e.get_message ();
    std::cerr << " -------------------------------" << std::endl;

    return -1;
  }

  return 0;
}

