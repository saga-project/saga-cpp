//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <errno.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unistd.h>

#include <saga/saga-defs.hpp>

#include <boost/version.hpp>
#include <boost/thread.hpp>
#include <boost/program_options.hpp>

#if BOOST_VERSION >= 103800
#include <boost/spirit/include/classic_core.hpp>
#else
#include <boost/spirit/core.hpp>
#endif

///////////////////////////////////////////////////////////////////////////////
//  This is a simple test helper application allowing to test the local SAGA
//  job adaptor. 
//
//  Possible command line options:
//  Usage: test_helper [options]:
//    -h [ --help ]         print out program usage (this message)
//    -i [ --interactive ]  run in interactive mode
//    -o [ --output ] arg   initial output stream: stdout or stderr
//    -e [ --error ] err    exit with error code 'err'
  
//  In interactive mode this application understands the following commands 
//  from std::cin (stdin):
//
//    <stdout>
//        from now on all output will be printed to std::cout (stdout)
//    <stderr>
//        from now on all output will be printed to std::cerr (stderr)
//    <exit> or <exit:code>
//        exit immediatly with the given exitcode (defaults to '-e err' or zero)
//    <delayedexit(delay)> or <delayedexit(delay):code>
//        exit after 'delay' seconds with the given exitcode (defaults to 
//        '-e err' or zero)
//
//    All other input will be echoed to the current output.

//  In non-interactive mode the application just prints its command line 
//  arguments to the requested output (see option -o).

///////////////////////////////////////////////////////////////////////////////

namespace po = boost::program_options;

bool parse_commandline(int argc, char *argv[], po::variables_map& vm)
{
    try {
    po::options_description desc_cmdline ("Usage: test_helper [options]");
        
        desc_cmdline.add_options()
            ("help,h", "print out program usage (this message)")
            ("interactive,i", "run in interactive mode")
            ("error,e", po::value<int>(), "return error code")
            ("output,o", po::value<std::string>(), 
                "initial output stream: stdout or stderr")
        ;

    po::options_description hidden;
    po::positional_options_description p;

        hidden.add_options()
            ("argument", po::value<std::vector<std::string> >()->composing(), 
                "arguments")
        ;
        hidden.add(desc_cmdline);
        p.add("argument", -1);

        po::store(po::command_line_parser(argc, argv).
            options(hidden).positional(p).run(), vm);
        po::notify(vm);

        // print help screen
        if (vm.count("help")) {
            std::cout << desc_cmdline;
            return false;
        }
    }
    catch (std::exception const& e) {
        std::cerr << "test_helper: exception caught: " << e.what() << std::endl;
        return false;
    }
    return true;
}

bool is_exit_command(std::string line, int& exitcode, int def = 0)
{
    using namespace boost::spirit;
    
    // recognize '<exit>' or '<exit:...>'
    int exit = def;

    parse_info<> pi = parse(line.c_str(),
        str_p("<exit") >> !( ch_p(':') >> int_p[assign_a(exit)] ) >> '>');

    if (!pi.full)
        return false;
        
    exitcode = exit;
    return true;
}

bool is_delayed_exit_command(std::string line, int& timeout, int& exitcode, int def = 0)
{
    using namespace boost::spirit;
    
    // recognize '<delayedexit(...)>' or '<delayedexit(...):...>'
    int exit = def;

    parse_info<> pi = parse(line.c_str(),
            str_p("<delayedexit(") >> int_p[assign_a(timeout)] >> ')'
        >> !( ch_p(':') >> int_p[assign_a(exit)] ) >> '>' );

    if (!pi.full)
        return false;
        
    exitcode = exit;
    return true;
}

///////////////////////////////////////////////////////////////////////////////
enum outsink {
    outsink_stdout,
    outsink_stderr
};

int main(int argc, char *argv[])
{
    // log pid for kill purposes as a safety guard
    std::ofstream out ("test_helper.pids", std::ios::out | std::ios::app);
    if (out.is_open()) {
        out << getpid() << std::endl;
        out.close();
    }

    // analyze the command line
    po::variables_map vm;
    if (!parse_commandline(argc, argv, vm))
        return -1;

    // do, what needs to be done :-P

    // use this applet in interactive mode    
    outsink sink = outsink_stdout;
    if (vm.count("interactive")) {
        std::string line;
        int exitcode = 0;
        int def_exit = 0;
        int delay    = 0;

        if (vm.count("error")) {
          def_exit = vm["error"].as<int>();
        }


        while ( true )
        {
            std::getline (std::cin, line);

            if ( std::cin.fail () )
            {
              std::cin.clear ();
              exit (-1);
            }
            else
            {
                if (line == "<stdout>") {
                    sink = outsink_stdout;
                }
                else if (line == "<stderr>") {
                    sink = outsink_stderr;
                }
                else if (is_exit_command(line, exitcode, def_exit)) {
                    break;
                }
                else if (is_delayed_exit_command(line, delay, exitcode, def_exit)) {
                    break;
                }
                else {
                    if (sink == outsink_stdout)
                        std::cout << line << std::endl;
                    else
                        std::cerr << line << std::endl;
                }
            }
        }

        
        // wait for the given amount of seconds, if required
        if (delay > 0) {
            time_t start_time = time (0);
            while ( (delay - (difftime (time(0), start_time))) > 0 )
            {
                boost::xtime xt;
                boost::xtime_get(&xt, boost::TIME_UTC);
                xt.nsec += 500 * 1000000;   // 500ms of sleeping is ok

                boost::thread::sleep(xt);
            }
        }
        
        return exitcode;
    }
    
    // non-interactive mode simply prints the arguments to the specified stream
    if (vm.count("output")) {
        if (vm["output"].as<std::string>() == "stderr")
            sink = outsink_stderr;
        else if (vm["output"].as<std::string>() != "stdout") {
            std::cerr << "test_helper: wrong value for option '--output/-o'" 
                << std::endl;
            return -1;
        }
    }
    
    // print out all positional arguments
    int number_of_arguments = 0;
    
    if (vm.count("argument")) {
    std::vector<std::string> arguments = 
        vm["argument"].as<std::vector<std::string> >();
    
        std::vector<std::string>::iterator end = arguments.end();
        for (std::vector<std::string>::iterator it = arguments.begin(); 
            it != end; ++it)
        {
            if (sink == outsink_stdout)
                std::cout << *it << std::endl;
            else
                std::cerr << *it << std::endl;
            ++number_of_arguments;
        }
    }
    
    if (vm.count("error")) {
      return (vm["error"].as<int>());
    }

    return number_of_arguments;
}
