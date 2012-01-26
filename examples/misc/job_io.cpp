
#include <saga/saga.hpp>
#include <iostream>

int main(int argc, char ** argv)
{

  try
  {
    saga::job::istream err;
    saga::job::istream out;

    saga::job::job j;

    {
      saga::job::service     js ("fork://localhost/");
      saga::job::ostream     in;
      saga::job::description jd;

      jd.set_attribute (saga::job::attributes::description_executable, "/bin/cat");
      jd.set_attribute (saga::job::attributes::description_input,      "/tmp/t.in");
      jd.set_attribute (saga::job::attributes::description_output,     "/tmp/t.out");
      jd.set_attribute (saga::job::attributes::description_output,     "/tmp/t.err");
      jd.set_attribute (saga::job::attributes::description_interactive,
                        saga::attributes::common_true);


      j = js.create_job (jd);

   // in  = j.get_stdin  ();
      out = j.get_stdout ();
      err = j.get_stderr ();

      j.run  ();

   // in << "test\n";
    }

    // in is out of scope here, so stdin should be closed for the spawned
    // process.

    while ( out.good () )
    {
      std::cout << "out is good" << std::endl;

      char c;

      out.read (&c, 1);

      if ( out.fail () )
      {
        std::cout << "out is ! good" << std::endl;
        break;
      }

      std::cout << c << std::flush;
    }


    j.wait ();

  }
  catch ( const saga::exception & e )
  {
    std::cerr << "saga error: " << e.what () << std::endl;
    return -1;
  }


  // try
  // {
  //   saga::job::service js ("fork://localhost/");
  //   saga::job::description jd;
  // 
  //   jd.set_attribute (saga::job::attributes::description_executable, "/usr/bin/wc");
  //   jd.set_attribute (saga::job::attributes::description_interactive,
  //                     saga::attributes::common_false);
  //   jd.set_attribute (saga::job::attributes::description_input,  "/tmp/t.in");
  //   jd.set_attribute (saga::job::attributes::description_output, "/tmp/t.out");
  //   jd.set_attribute (saga::job::attributes::description_error,  "/tmp/t.err");
  // 
  //   saga::job::job j = js.create_job (jd);
  // 
  //   j.run  ();
  //   j.wait ();
  // 
  // }
  // catch ( const saga::exception & e )
  // {
  //   std::cerr << "saga error: " << e.what () << std::endl;
  //   return -1;
  // }


  // try 
  // {
  //   saga::job::service js ("fork://localhost");
  // 
  //   saga::job::ostream in;
  //   saga::job::istream out;
  //   saga::job::istream err;
  // 
  // 
  //   // test-helper.pl works, bc does not.
  //   saga::job::job job = js.run_job
  //   // ("/Users/merzky/links/saga/trunk/test/cpp/test_helper/test_helper.pl -i -o -e -l",
  //      ("/usr/bin/bc",
  //       "localhost", in, out, err);
  // 
  //   in << "3*3"  << std::endl;
  // 
  //   std::string line;
  //   std::getline (out, line);
  //   std::cout << "'" << line << "'" << std::endl;
  //   
  //   in << "quit" << std::endl;
  // 
  //   job.wait();
  // }
  // catch ( const saga::exception & e )
  // {
  //   std::cerr << "saga error: " << e.what () << std::endl;
  //   return -1;
  // }

  // try
  // {
  //   saga::job::service js ("ssh://localhost");
  //
  //   saga::job::ostream in;
  //   saga::job::istream out;
  //   saga::job::istream err;
  //
  //   saga::job::job job = js.run_job
  //   // ("/Users/merzky/links/saga/trunk/test/cpp/test_helper/test_helper.pl -i -o -e -l",
  //      ("/usr/bin/bc",
  //   // ("/usr/bin/tee -a /bin/t",
  //   // ("perl -wlne'$|=1; open F,\\\">>/tmp/t\\\"; print F \\\"oops\n\\\"; print F eval; close F;'",
  //       "localhost", in, out, err);
  //
  //   in << "3*3"  << std::endl;
  //
  //   std::string line;
  //   std::getline (out, line);
  //   std::cout << line << std::endl;
  //   
  //   in << "exit" << std::endl;
  //   in << "quit" << std::endl;
  //
  //   job.wait ();
  //
  //   return 0;
  // }
  // catch ( const saga::exception & e )
  // {
  //   std::cerr << "saga error: " << e.what () << std::endl;
  //   return -1;
  // }


  // try
  // {
  //   std::string exe ("/bin/ls");
  //   std::vector <std::string> args;
  //   args.push_back ("/tmp/");
  //   args.push_back ("123456");
  // 
  //   saga::job::service js ("fork://localhost");
  // 
  //   saga::job::description jd;
  // 
  //   jd.set_attribute (saga::job::attributes::description_executable,        exe);
  //   jd.set_vector_attribute (saga::job::attributes::description_arguments,  args);
  //   jd.set_attribute (saga::job::attributes::description_output,            "stdout.txt"); 
  //   jd.set_attribute (saga::job::attributes::description_error,             "stderr.txt"); 
  //   jd.set_attribute (saga::job::attributes::description_working_directory, "/tmp/"); 
  // 
  //   saga::job::job j = js.create_job (jd);
  // 
  //   std::cout << j.get_state () << std::endl;
  //   j.run  ();
  //   std::cout << j.get_state () << std::endl;
  //   j.wait ();
  //   std::cout << j.get_state () << std::endl;
  // }
  // catch ( const saga::exception & e )
  // {
  //   std::cerr << "saga error: " << e.what () << std::endl;
  //   return -1;
  // }


  // try
  // {
  //   std::string exe ("/bin/date");
  // 
  //   if ( argc == 2 )
  //   {
  //     exe = argv[1];
  //   }
  // 
  //   saga::job::service js("fork://localhost");
  // 
  //   saga::job::description jd;
  // 
  //   jd.set_attribute (saga::job::attributes::description_executable,        exe);
  //   jd.set_attribute (saga::job::attributes::description_output,            "out"); 
  //   jd.set_attribute (saga::job::attributes::description_working_directory, "/tmp/test"); 
  // 
  //   saga::job::job j = js.create_job (jd);
  // 
  //   j.run  ();
  //   j.wait ();
  // 
  //   std::cout << j.get_state () << std::endl;
  // }
  // catch ( const saga::exception & e )
  // {
  //   std::cerr << "saga error: " << e.what () << std::endl;
  //   return -1;
  // }

  return 0;
}

