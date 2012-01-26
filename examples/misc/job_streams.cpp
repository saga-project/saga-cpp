#include <saga/saga.hpp>
#include <iostream>

int main(int argc, char ** argv)
{
    saga::job::service js(argv[1]);

    saga::job::istream out, err;
    saga::job::ostream in;

    {
        saga::job::job job = js.run_job (argv[2], argv[3], in, out, err);

        std::string jobid = job.get_job_id();
        std::cout << "Started job: " << jobid << std::endl;

        job.wait(-1.0);

        in << argv[4] << std::endl;
    }

    std::string output;
    std::string input;

    while ( getline (out, output) )
    {
      std::cout << output << std::endl;
      std::cin >> input;
      in << input << std::endl;
    }

    std::cout << "done" << std::endl;
}


