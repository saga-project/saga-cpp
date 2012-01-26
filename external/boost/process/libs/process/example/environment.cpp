//
// Boost.Process
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt.)
//

//
// This example program demonstrates multiple ways to modify a new child's
// environment variables.
//

// quickbook:begin(code)
#include <cstdlib>
#include <iostream>
#include <string>

#include <boost/process.hpp>

namespace bp = ::boost::process;

//
// Helper function that forks a new process that shows the contents of
// the environment configured by the given launcher.
//
static
void
run_it(const std::string& msg, const bp::launcher& l)
{
#if defined(BOOST_PROCESS_POSIX_API)
    bp::command_line cl("env");
#elif defined(BOOST_PROCESS_WIN32_API)
    bp::command_line cl = bp::command_line::shell("set");
#endif

    bp::launcher l2(l);
    l2.set_stdout_behavior(bp::inherit_stream);

    std::cout << "===> " << msg << std::endl;
    bp::status s = l2.start(cl).wait();
    if (s.exited() && s.exit_status() == EXIT_SUCCESS)
        std::cout << "     *** SUCCESS ***" << std::endl;
    else
        std::cout << "     *** FAILURE ***" << std::endl;
    std::cout << std::endl;
}

int
main(int argc, char* argv[])
{
    //
    // This first launcher does not touch the environment so the child
    // will receive a snapshot of our current table.
    //
    bp::launcher l1;
    run_it("Inherited environment", l1);

    //
    // This second example clears the child's environment prior
    // execution so it will not receive any variable.
    //
    bp::launcher l2;
    l2.clear_environment();
    run_it("Clean environment", l2);

    //
    // This example adds an extra variable to the environment.
    //
    bp::launcher l3;
    l3.set_environment("NEW_VARIABLE", "Hello, world!");
    run_it("Environment with the NEW_VARIABLE extra variable", l3);

    //
    // This example removes a standard variable from the environment.
    //
    bp::launcher l4;
    l4.unset_environment("PATH");
    run_it("Environment without the standard PATH variable", l4);


    //
    // This last example illustrates how to start up a new child process
    // with a completely controlled environment table that is not subject
    // to existing variables at all.
    //
    bp::launcher l5;
    l5.clear_environment();
    l5.set_environment("HOME", "Known value for HOME");
    l5.set_environment("PATH", "Known value for PATH");
    run_it("Completely controlled environment", l5);

    return EXIT_SUCCESS;
}
// quickbook:end(code)
