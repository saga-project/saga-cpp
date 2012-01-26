
#include <saga/saga.hpp>

int main ()
{
  try 
  {
    saga::filesystem::directory d ("file://localhost/tmp/");

    std::cout << " ~~~ " << d.get_url () << std::endl;
    
    d.change_dir ("file://localhost/tmp/");
    
    std::cout << " ~~~ " << d.get_url () << std::endl;

    d.change_dir ("file://localhost/Users/");

    std::cout << " ~~~ " << d.get_url () << std::endl;

    d.change_dir ("file://localhost/etc/xgrid");

    std::cout << " ~~~ " << d.get_url () << std::endl;
    
    d.change_dir ("..");

    std::cout << " ~~~ " << d.get_url () << std::endl;

    saga::filesystem::file f = d.open ("passwd");
    
    std::cout << " ~~~ " << f.get_url () 
              << ": " << f.get_size () << std::endl;

    std::cout << " ~~~ is_file 1: " << d.is_file ("syslog.conf") << std::endl;
    std::cout << " ~~~ is_file 2: " << d.is_file ("syslog.cfg")  << std::endl;
    std::cout << " ~~~ is_file 3: " << d.is_file ("file://localhost/etc/syslog.conf") << std::endl;
    std::cout << " ~~~ is_file 4: " << d.is_file ("file://localhost/etc/syslog.cfg")  << std::endl;

    d.change_dir ("../tmp/");

    std::cout << " ~~~ " << d.get_url () << std::endl;

    d.copy ("t1", "t2", saga::filesystem::Overwrite);

    std::cout << " ~~~ copy t1 t2" << std::endl;

    saga::filesystem::file f2 ("file://localhost/tmp/t1", saga::filesystem::Create);

    f2.copy ("t3", saga::filesystem::Overwrite);

    std::cout << " ~~~ copy t1 t3" << std::endl;

    std::cout << " ~~~ " << d.get_url () << std::endl;

    saga::filesystem::file f3 ("ssh://localhost/does/not/exist", 
                               saga::filesystem::Read);

    std::cout << " ~~~ " << f3.get_url () << ": " << f3.get_size () << std::endl;
  }
  catch ( const saga::exception & e )
  {
    std::cerr << " --- what ----------------------" << std::endl;
    std::cerr << e.what ();
    std::cerr << " -------------------------------" << std::endl;
    std::cerr << " --- get_message ---------------" << std::endl;
    std::cerr << e.get_message ();
    std::cerr << " -------------------------------" << std::endl;
  }
}

