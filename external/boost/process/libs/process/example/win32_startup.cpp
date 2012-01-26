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
// This example program demonstrates how to modify the startup properties
// of a process under a Win32 system.  It asks the child process to show
// its main window on the top-left corner and with a predefined size.
//

// Avoid building this example under non-Win32 systems.
#include <boost/process/config.hpp>
#if defined(BOOST_PROCESS_WIN32_API)

// quickbook:begin(code)
extern "C" {
#include <windows.h>
}

#include <cstdlib>
#include <iostream>

#include <boost/process.hpp>

//
// Error out early if we are trying to build this non-portable example
// code under a platform that does not provide the required win32_*
// classes.
//
#if !defined(BOOST_PROCESS_WIN32_API)
#   error "Unsupported platform."
#endif

namespace bp = ::boost::process;

int
main(int argc, char* argv[])
{
    //
    // Creates a new STARTUPINFO object, specific to the Win32 platform,
    // that specifies the position and size of the child process' main
    // window.  Note that this is just a hint to the process, which may
    // choose to ignore our settings.
    //
    STARTUPINFO si;
    ::ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags |= STARTF_USEPOSITION | STARTF_USESIZE;
    si.dwX = 0;
    si.dwY = 0;
    si.dwXSize = 640;
    si.dwYSize = 480;

    //
    // The application we have to launch.  We default to Notepad because,
    // depending on its version, it is possible that it honors the window
    // settings described above.
    //
    std::string app = argc < 2 ? "notepad" : argv[1];

    //
    // The value returned by the example program.
    //
    int exitstatus = EXIT_FAILURE;

    try {
        //
        // Constructs a command line to launch the application chosen
        // above.  If the name does not contain any directory component,
        // it will be searched in the PATH.
        //
        bp::command_line cl(app);

        //
        // Constructs a Win32-specific launcher with the start settings
        // we configured.
        //
        bp::win32_launcher l(&si);

        //
        // Starts the application and waits for its termination, reporting
        // the results to the user.
        //
        bp::status s = l.start(cl).wait();
        if (s.exited()) {
            std::cout << "Application exited successfully" << std::endl;
            exitstatus = EXIT_SUCCESS;
        } else {
            std::cout << "The application returned an error" << std::endl;
        }
    } catch (bp::not_found_error< std::string > e) {
        std::cout << "Could not find " << app << " in path." << std::endl;
    }

    return exitstatus;
}
// quickbook:end(code)

#else // !defined(BOOST_PROCESS_WIN32_API)

#include <cstdlib>
#include <iostream>

int
main(int argc, char* argv[])
{
    std::cerr << "This example program is not supported in this platform."
              << std::endl;
    return EXIT_FAILURE;
}

#endif // defined(BOOST_PROCESS_WIN32_API)
