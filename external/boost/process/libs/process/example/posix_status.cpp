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
// This example program demonstrates how to retrieve extended exit status
// under a POSIX system.  The program starts the command line passed to it
// as arguments and shows its termination information.
//

// Avoid building this example under non-POSIX systems.
#include <boost/process/config.hpp>
#if defined(BOOST_PROCESS_POSIX_API)

// quickbook:begin(code)
extern "C" {
#include <unistd.h>
}

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include <boost/process.hpp>

//
// Error out early if we are trying to build this non-portable example
// code under a platform that does not provide the required posix_*
// classes.
//
#if !defined(BOOST_PROCESS_POSIX_API)
#   error "Unsupported platform."
#endif

namespace bp = ::boost::process;

int
main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "Please provide a program name." << std::endl;
        return EXIT_FAILURE;
    }

    //
    // Constructs a command line based on the arguments provided to the
    // program.
    //
    bp::command_line cl(argv[1]);
    for (int i = 2; i < argc; i++)
        cl.argument(argv[i]);

    //
    // Sets up a launcher inheriting all the three standard streams.
    //
    bp::launcher l;
    l.set_stdin_behavior(bp::inherit_stream);
    l.set_stdout_behavior(bp::inherit_stream);
    l.set_stderr_behavior(bp::inherit_stream);

    //
    // Spawns the new child process.
    //
    bp::child c = l.start(cl);

    //
    // Waits until the process exits and parses its termination status.
    // Note that we receive a posix_status object even when the wait()
    // method returns a status one.
    //
    bp::posix_status s = c.wait();
    if (s.exited()) {
        std::cout << "Program returned exit code " << s.exit_status()
                  << std::endl;
    } else if (s.signaled()) {
        std::cout << "Program received signal " << s.term_signal()
                  << std::endl;
        if (s.dumped_core())
            std::cout << "Program also dumped core" << std::endl;
    } else if (s.stopped()) {
        std::cout << "Program stopped by signal" << s.stop_signal()
                  << std::endl;
    } else {
        std::cout << "Unknown termination reason" << std::endl;
    }

    return s.exited() ? s.exit_status() : EXIT_FAILURE;
}
// quickbook:end(code)

#else // !defined(BOOST_PROCESS_POSIX_API)

#include <cstdlib>
#include <iostream>

int
main(int argc, char* argv[])
{
    std::cerr << "This example program is not supported in this platform."
              << std::endl;
    return EXIT_FAILURE;
}

#endif // defined(BOOST_PROCESS_POSIX_API)
