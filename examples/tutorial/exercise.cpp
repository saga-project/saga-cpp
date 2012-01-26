//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <cassert>
#include <vector>
#include <string>

#include <saga/saga.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/foreach.hpp>
#include <boost/version.hpp>

#if BOOST_VERSION < 103600
#error "This code is usable with Boost versions newer than V1.35.0 only"
#endif

///////////////////////////////////////////////////////////////////////////////
//
// Complement the code below in a way that it retrieves a list of URLs from a 
// given advert entry. The URls listed there should be used to spawn a child 
// process to each of them (for instance spawn /usr/bin/bc to fork://localhost
// and ssh://localhost).
//
// The entry in the advert service needs to be created first, before this 
// example may be executed. The simplest way of creating the entry is to use 
// the command line tool saga-advert:
//
//    saga-advert add_entry /tutorial/exercise/url_list
//    saga-advert store_string /tutorial/exercise/url_list "fork://localhost,fork://localhost"
//
// these commands will create an advert entry /tutorial/exercise/url_list and
// will store the list of two URLs into the new entry.

///////////////////////////////////////////////////////////////////////////////
// place in advert where URLs are stored
#define URL_ENTRY "/tutorial/exercise/url_list"

///////////////////////////////////////////////////////////////////////////////
// the routine spawning the SAGA jobs and waiting for their results
std::string increment(saga::url url, std::string argument)
{
    try {
        // construct command line
        std::string command ("/bin/sh -c \"/bin/echo");
        command += " $((" + argument + "+1))\"";

        // run the job
        saga::job::service js (url);
        saga::job::ostream in;
        saga::job::istream out;
        saga::job::istream err;

        saga::job::job j = js.run_job(command, "", in, out, err);

        // wait for the job to finish
        saga::job::state s = j.get_state();
        while (s != saga::job::Running && s != saga::job::Failed)
            s = j.get_state();

        // if the job didn't start successfully, print error message
        if (s == saga::job::Failed) {
            std::cerr << "SAGA job: " << j.get_job_id() << " failed (state: " 
                      << saga::job::detail::get_state_name(s) << ")\n";
            return argument;
        }

        // receive result
        std::string line;
        std::getline(out, line);

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
// retrieve the list of URLs from the advert entry
bool get_list_of_urls(std::vector <std::string> & url_strings)
{
    try {
        saga::advert::entry e (URL_ENTRY, saga::advert::Read);
        std::string tmp  =  e.get_attribute ("urls");

        boost::algorithm::split (url_strings, tmp, boost::algorithm::is_any_of(",; "));
    }
    catch (saga::exception const& e) {
        std::cerr << "saga::exception caught: " << e.what () << std::endl;
        return false;
    }
    catch (std::exception const& e) {
        std::cerr << "std::exception caught: " << e.what () << std::endl;
        return false;
    }
    catch (...) {
        std::cerr << "unexpected exception caught" << std::endl;
        return false;
    }
    return true;
}

///////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
    // try to read list of URLs from the advert service
    std::vector <std::string> url_strings;
    
    if ( ! get_list_of_urls (url_strings) ) {
        std::cerr << "List of urls is not available" << std::endl;
        return -1;
    }

    // spawn a child job to each of the URLs read from the URL list in 
    // the advert service

    std::string result ("1");    // start with value '1'

    BOOST_FOREACH (std::string const& url, url_strings)
    {
        // note that the string is converted to a saga::url here
        result = increment (url, result);
    }

    std::cout << "The overall result is: " << result << std::endl;

    return 0;
}

