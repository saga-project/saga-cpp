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

#ifndef SETTINGS_log_HPP
#define SETTINGS_log_HPP

#include <boost/logging/format.hpp>
namespace bl = boost::logging;

namespace settings {
    typedef bl::logger_format_write<> logger_type;

#ifdef SETTINGS_USE_LOGGING
    // if doing logging, lets do it based on levels
    typedef bl::level::holder filter_type;
#else
    // john.torjo:
    // note: yes, it's my fault - the always_disabled filter from Boost Logging Lib needs to allow any no. of arguments
    //       I'll modify it after the review
    struct always_disabled {
        static bool is_enabled() { return false; }
        static bool is_enabled(int) { return false; }
    };
    typedef always_disabled filter_type;
#endif

    struct do_log {
        static BOOST_DEFINE_LOG(l_, logger_type)
        static BOOST_DEFINE_LOG_FILTER(l_filter_, filter_type)
    };

}

#define L_(lvl) BOOST_LOG_USE_LOG_IF_LEVEL( ::settings::do_log::l_(), ::settings::do_log::l_filter_(), lvl )


#endif
