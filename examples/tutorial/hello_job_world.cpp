//  Copyright (c) 2011 Ole Weidner, Louisiana State University
// 
//  This is part of the code examples on the SAGA website:
//  http://saga-project.org/documentation/tutorials/job-api

#include <saga/saga.hpp>

int main(int argc, char* argv[])
{
  try 
  {
    // create an "echo 'hello, world' job"
    saga::job::description jd;
    jd.set_attribute("Interactive", "True");
    jd.set_attribute("Executable", "/bin/echo");
    std::vector<std::string> args;
    args.push_back("Hello, World!");
    jd.set_vector_attribute("Arguments", args);

    // connect to the local job service
    saga::job::service js("fork://localhost");

    // submit the job
    saga::job::job job = js.create_job(jd);
    job.run();

    //wait for the job to complete
    job.wait(-1);
  
    // print the job's output
    saga::job::istream output = job.get_stdout();
    std::string line;
    while ( ! std::getline (output, line).eof () )
    {
      std::cout << line << std::endl;
    }
  } 
  catch(saga::exception const & e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}

