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

// this is the client application - the one that uses the library

#include "settings_lib/settings.hpp"

// If you have a header-only library, you will still need to initialize its logs;
// Either the library provides a function like the one below, or you can provide it yourself

void init_library_logs() {
    // note: if the logging is disabled, anyway, nothing will be logged
    using namespace ::settings;
    using namespace bl;

    do_log::l_()->writer().add_formatter( formatter::idx(), "[%] "  );
    do_log::l_()->writer().add_formatter( formatter::time("$mm:$ss ") );
    do_log::l_()->writer().add_formatter( formatter::append_newline() );
    do_log::l_()->writer().add_destination( destination::file("out.txt") );
    do_log::l_()->writer().add_destination( destination::dbg_window() );
    do_log::l_()->writer().add_destination( destination::cout() );
    do_log::l_()->mark_as_initialized();
}

int main() {
    init_library_logs();

    // use some functions from the library
    settings::read_first_line("t.txt");
    settings::read_first_line_or_error("t.txt");
}

