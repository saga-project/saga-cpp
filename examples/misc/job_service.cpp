#include <saga/saga.hpp>
#include <iostream>

int main(int argc, char ** argv)
{
    saga::job::service js(argv[1]);

    std::cout << "done" << std::endl;
}


