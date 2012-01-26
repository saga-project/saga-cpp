#include <saga/saga.hpp>
#include <iostream>

int main(int argc, char ** argv)
{
  saga::task t;

  try
  {
    saga::task t (saga::task::Done);

    std::cout << "task id   : " << t.get_id () << std::endl;
    std::cout << "task state: " << t.get_state () << std::endl;

    t.cancel ();
  }
  catch ( const saga::exception & e )
  {
    std::cerr << "<None> saga error: " << e.what () << std::endl;
  }


//  try
//  {
//    saga::filesystem::file f ("c:/temp/passwd");
//
//    std::cout << "<None> file opened" << std::endl;
//
//    // this code should never throw, as we don't call get_result(), nor rethrow()
//    f.copy ("/does/not/exist");
//
//    std::cout << "<None> sync file copy done" << std::endl;
//  }
//  catch ( const saga::exception & e )
//  {
//    std::cerr << "<None> saga error: " << e.what () << std::endl;
//  }
//
//
//  try
//  {
//    saga::filesystem::file f ("c:/temp/passwd");
//
//    std::cout << "<Sync> file opened" << std::endl;
//
//    // this code should never throw, as we don't call get_result(), nor rethrow()
//    t = f.copy <saga::task::Sync> ("/does/not/exist");
//
//    std::cout << "<Sync> file copy started " << std::endl;
//
//    t.wait ();
//    t.get_result ();
//  }
//  catch ( const saga::exception & e )
//  {
//    std::cerr << "<Sync> saga error: " << e.what () << std::endl;
//  }
//
//
//
//  try
//  {
//    saga::filesystem::file f ("c:/temp/passwd");
//
//    std::cout << "<Async> file opened" << std::endl;
//
//    // this code should never throw, as we don't call get_result(), nor rethrow()
//    t = f.copy <saga::task::Async> ("/does/not/exist");
//
//    std::cout << "<Async> file copy started " << std::endl;
//
//    t.wait ();
//    t.get_result ();
//  }
//  catch ( const saga::exception & e )
//  {
//    std::cerr << "<Async> saga error: " << e.what () << std::endl;
//  }
//
//
//
//  try
//  {
//    saga::filesystem::file f ("/etc/passwd");
//
//    std::cout << "<Task> file opened" << std::endl;
//
//    // this code should never throw, as we don't call get_result(), nor rethrow()
//    t = f.copy <saga::task_base::Task> ("/does/not/exist");
//
//    std::cout << "<Task> file copy started " << std::endl;
//
//    t.run ();
//    t.wait ();
//    t.get_result ();
//  }
//  catch ( const saga::exception & e )
//  {
//    std::cerr << "<Task> saga error: " << e.what () << std::endl;
//  }



//   try
//   {
//     std::cout << "1 file copy wait" << std::endl;
//     t.wait ();
// 
//     std::cout << "1 file copy check" << std::endl;
// 
//     if ( t.get_state () == saga::task::Failed )
//     {
//       std::cout << "1 file copy failed" << std::endl;
//       t.rethrow ();
//     }
//     else
//     {
//       std::cout << "1 file copy ok" << std::endl;
//     }
//   }
//   catch ( const saga::exception & e )
//   {
//     std::cerr << "1 saga error: " << e.what () << std::endl;
//   }
// 

//  try
//  {
//    std::string exe ("/bin/date");
//
//    if ( argc == 2 )
//    {
//      exe = argv[1];
//    }
//
//    saga::job::service js("fork://localhost");
//
//    saga::job::description jd;
//
//    jd.set_attribute (saga::job::attributes::description_executable,        exe);
//    jd.set_attribute (saga::job::attributes::description_output,            "out"); 
//    jd.set_attribute (saga::job::attributes::description_working_directory, "/tmp/test"); 
//
//    t = js.create_job <saga::task::Async> (jd);
//    t.wait ();
//
//    saga::job::job     j   = t.get_result <saga::job::job> ();
//    saga::job::service js2 = t.get_result <saga::job::service> ();
//
//    j.run  ();
//    j.wait ();
//
//    std::cout << j.get_state () << std::endl;
//  }
//  catch ( const saga::exception & e )
//  {
//    std::cerr << "saga error: " << e.what () << std::endl;
//  }
//
//  std::cout << "done" << std::endl;

  return 0;
}

