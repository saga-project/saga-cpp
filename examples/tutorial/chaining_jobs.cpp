//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <saga/saga.hpp>

///////////////////////////////////////////////////////////////////////////////
// The chaining_jobs example tries to overcome one of the limitations of the 
// hello_world example: it introduces dependencies between 3 (possibly remotely)
// spawned childs. In this example the next child will be spawned only after 
// the previous one has finished its execution. To make it more interesting we 
// now use /usr/bin/bc to do some calculations, where the result of the previous
// calculation is used as the input for the next one.
//
// Try to make more complex calculations if you like!
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// The URLs names to spawn jobs to.  Please change the 3 macros below to the 
// urls you want the 3 childs to be spawned to.
///////////////////////////////////////////////////////////////////////////////
#define URL1 "fork://localhost"
#define URL2 "fork://localhost"
#define URL3 "fork://localhost"

///////////////////////////////////////////////////////////////////////////////
// the routine spawning the SAGA jobs and waiting for their results
std::string increment(saga::url url, std::string argument)
{
    try 
    {
        saga::job::service js (url);
        saga::job::ostream in;
        saga::job::istream out;
        saga::job::istream err;

        // run the job
        saga::job::job j = js.run_job("/usr/bin/bc -q", "", in, out, err);

        // if the job didn't start successfully, print error message
        if ( j.get_state () != saga::job::Running )
        {
            std::cerr << "SAGA job: " << j.get_job_id() << " failed" << std::endl;
            return argument;
        }

        // feed the remote process some input
        in << "1 + " << argument << std::endl;

        // receive result
        std::string line;
        std::getline(out, line);

        // quit remote process
        in << "quit\n";

        return line;
    }
    catch (saga::exception const& e) {
        std::cerr << "saga::exception caught: " << e.what () << std::endl;
    }
    catch (std::exception const& e) {
        std::cerr << "std::exception caught: " << e.what () << std::endl;
    }
    catch (...) {
        std::cerr << "unexpected exception caught" << std::endl;
    }
    return argument;    // by default just return argument
}

///////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
    std::string result;

    // run the incrementor 3 times (i.e. spawn 3 jobs to increment the counter)
    result = increment (URL1, "1");
    result = increment (URL2, result);
    result = increment (URL3, result);

    std::cout << "The overall result is: " << result << std::endl;

    return 0;
}
