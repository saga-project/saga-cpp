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
// This example program demonstrates multiple ways to construct a command
// line and why such cases can be useful.
//

// quickbook:begin(code)
#include <cstdlib>
#include <iostream>

#include <boost/process.hpp>

namespace bp = ::boost::process;

//
// Helper function to print a command line's contents on a stream.
// We can see here the interface exported by the command_line class and
// how the user can use it to inspect a command_line's contents.
//
static
std::ostream&
operator<<(std::ostream& os, const bp::command_line& cl)
{
    os << "===> Binary:       " << cl.get_executable() << std::endl;

    os << "===> Command line: ";
    const bp::command_line::arguments_vector& args = cl.get_arguments();
    for (bp::command_line::arguments_vector::size_type i = 0;
         i < args.size(); i++) {
        os << args[i];
        if (i < args.size() - 1)
            os << ' ';
    }
    os << std::endl;

    return os;
}

//
// Helper function that executes the given command line.
//
template< class Command_Line >
static
void
run_it(const Command_Line& cl)
{
    std::cout << cl;

    bp::launcher l;
    l.set_stdout_behavior(bp::inherit_stream);
    l.set_stderr_behavior(bp::inherit_stream);
    bp::status s = l.start(cl).wait();
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
    // Constructs a simple command line that executes the 'ls' binary to
    // list the contents of the current directory.
    //
    bp::command_line cl1("/bin/ls");
    run_it(cl1);

    //
    // Constructs a command line similar to the previous one but passes
    // an argument to the 'ls' binary telling it which directory to list.
    //
    bp::command_line cl2("/bin/ls");
    cl2.argument("/tmp");
    run_it(cl2);

    //
    // Yet another command line that calls a system binary, but this time
    // with three arguments.  Note that when calling '/bin/test' with its
    // default program name 'test', the closing square bracket is not
    // allowed.
    //
    bp::command_line cl3("/bin/test");
    cl3.argument("foo").argument("=").argument("foo");
    run_it(cl3);

    //
    // Exactly the same command as the previous one but this one changes
    // the program name at execution.  Here, '/bin/test' is executed with
    // a program name of '[' (i.e. argv[0]'s value is "[").  Hence, it
    // requires a closing square bracket as an extra argument to execute
    // properly.
    //
    bp::command_line cl4("/bin/test", "[");
    cl4.argument("foo").argument("=").argument("foo").argument("]");
    run_it(cl4);

    //
    // Constructs a command line that is executed through the system
    // shell, much like the ::system(3) function does.  As shown here,
    // these calls are much simpler to write and read because the command
    // line is provided as a single string.  However, they may be subject
    // to quoting problems because they need to be run through the shell.
    // Also note that there is no need to give the full path to the binary
    // because the shell will resolve it for us.
    //
    bp::command_line cl5 = bp::command_line::shell("ls -l /var");
    run_it(cl5);

    //
    // Another shell command line construction used to demonstrate that
    // the command is really executed through the system shell.  Otherwise
    // the stdout redirection could not work.
    //
    bp::command_line cl6 = bp::command_line::shell("echo foo > /dev/null");
    run_it(cl6);

    return EXIT_SUCCESS;
}
// quickbook:end(code)
