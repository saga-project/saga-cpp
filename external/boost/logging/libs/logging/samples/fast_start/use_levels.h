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

// my_app_log.h - DECLARE your loggers & filters here
#ifndef my_app_LOG_H_header
#define my_app_LOG_H_header

#include <boost/logging/format/named_write_fwd.hpp>
// #include <boost/logging/writer/on_dedicated_thread.hpp> // uncomment if you want to do logging on a dedicated thread

namespace bl = boost::logging;
typedef bl::named_logger<>::type logger_type;
typedef bl::level::holder filter_type;

BOOST_DECLARE_LOG_FILTER(g_l_level, filter_type)
BOOST_DECLARE_LOG(g_l, logger_type)

#define L_(lvl) BOOST_LOG_USE_LOG_IF_LEVEL(g_l(), g_l_level(), lvl )

// initialize thy logs..
void init_logs();

#endif
