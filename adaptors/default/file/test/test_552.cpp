#include <saga/saga.hpp>

using namespace std;

int main(int argc, char** argv)
{


try 
{
    std::cout << "If #553 is not fixed, the code below will produce at least an exception. " << std::endl;
    std::cout << "Please check: http://faust.cct.lsu.edu/trac/saga/ticket/553 for details." << std::endl;

    saga::job::service s("ssh://localhost");

    saga::job::description d;
    d.set_attribute(saga::job::attributes::description_executable, "/bin/sleep");

    vector<string> args;
    args.push_back("1");
    d.set_vector_attribute(saga::job::attributes::description_arguments, args);

    cerr << "creating job 1" << endl;
    saga::job::job j1 = s.create_job(d);
    cerr << "running job 1" << endl;
    j1.run();
    cerr << "waiting until job 1 is finished" << endl;
    j1.wait();

    cerr << "creating job 2" << endl;
    saga::job::job j2 = s.create_job(d);
    cerr << "running job 2" << endl;
    j2.run();
    cerr << "waiting until job 2 is finished" << endl;
    j2.wait();

    std::cout << "\nPASSED!" << std::endl;
}
catch(saga::exception const & e)
{
    std::cerr << "\nFAILED: " << e.what() << std::endl;
}

}
