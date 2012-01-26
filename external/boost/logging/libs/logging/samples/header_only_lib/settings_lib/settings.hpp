/**
 Boost Logging library

 Author: John Torjo, www.torjo.com

 Copyright (C) 2007 John Torjo (see www.torjo.com for email)

 Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
          http://www.boost.org/LICENSE_1_0.txt)

 See http://www.boost.org for updates, documentation, and revision history.
 See http://www.torjo.com/log2/ for more details
*/

/* Assume you have a header-only library, 
   and you want to allow for logging.

   This example shows you just how to do that:
   - settings.hpp - part of your library, uses the Boost Logging Lib
   - settings_log.hpp - sets up using the Boost Logging Lib


   Pay attention to the SETTINGS_USE_LOGGING - if not defined, all logging is disabled automatically

*/

#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include "settings_lib/settings_log.hpp"
#include <iostream>
#include <fstream>
#include <string>

namespace settings {

    inline void read_first_line(const std::string & file_name) {
        L_(debug) << "opening file " << file_name;
        std::ifstream in( file_name.c_str() );
        std::string line;
        std::getline(in, line);
        L_(info) << "first line from " << file_name << " is " << line;
    }

    inline void read_first_line_or_error(const std::string & file_name) {
        L_(debug) << "opening file " << file_name;
        std::ifstream in( file_name.c_str() );
        std::string line;
        std::getline(in, line);
        if ( line.empty() )
            L_(error) << "file " << file_name << " - first line is empty";
    }

}

#endif
