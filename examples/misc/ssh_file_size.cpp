
#include <saga/saga.hpp>

int main ()
{
  try 
  {
    // std::cout << " ------------------------------ 1 " << std::endl;
    // {
    //   saga::url              u ("file://localhost//etc/passwd");
    //   saga::filesystem::file f (u);

    //   std::cout << "f: " << u
    //     << " ("  << f.get_size () 
    //     << ")"   << std::endl;

    //   std::cout << "f: " << u
    //     << " ("  << f.get_size () 
    //     << ")"   << std::endl;

    //   f.copy ("file://localhost/tmp/p1");
    //   f.copy ("ssh://qb.loni.org/tmp/p2");
    // }
       std::cout << " ------------------------------ 2 " << std::endl;
       {
         saga::url              u ("ssh://merzky@localhost/etc/passwd");
         saga::filesystem::file f (u);
       
         std::cout << "f: " << u
           << " ("  << f.get_size () 
           << ")"   << std::endl;
       
         // std::cout << "f: " << u
         //   << " ("  << f.get_size () 
         //   << ")"   << std::endl;
       
         // f.copy ("ssh://amerzky@gg101.cct.lsu.edu/tmp/p3");
         // f.copy ("file://localhost/tmp/p4");
         // f.copy ("ssh://qb.loni.org/tmp/p5");
       }
       std::cout << " ------------------------------ 3 " << std::endl;
    // {
    //   saga::url                   u ("file://localhost//etc/");
    //   saga::filesystem::directory d (u);
    //   saga::filesystem::file      f = d.open ("passwd");

    //   saga::task_container tc;

    //   for ( int i = 0; i < 100; i++ )
    //   {
    //     tc.add_task (f.get_size <saga::task_base::Task> ());
    //   }
    //   
    //   std::cout << " ------------------------------ 4 " << std::endl;
    //   
    //   tc.run ();

    //   std::cout << " ------------------------------ 5 " << std::endl;

    //   std::vector <saga::task> tasks = tc.wait (saga::task_container::All);

    //   for ( int i = 0; i < 100; i++ )
    //   {
    //     std::cout << "f: " << u
    //               << " ("  << tasks[i].get_result <int> () 
    //               << ")"   << std::endl;
    //   }

    // }
    // std::cout << " ------------------------------ 6 " << std::endl;
  }
  catch ( const saga::exception & e )
  {
    std::cerr << "Exception: \n" << e.what () << std::endl;
  }

  return 0;
}

