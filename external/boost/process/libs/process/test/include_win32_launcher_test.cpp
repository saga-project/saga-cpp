//
// Boost.Process
// Checks that win32_launcher.hpp can be included without errors.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt.)
//

#include <boost/process/config.hpp>

#if defined(BOOST_PROCESS_WIN32_API)

#   include <boost/process/win32_launcher.hpp>

namespace bp = ::boost::process;

// ------------------------------------------------------------------------

void*
test_it(void)
{
    return new bp::win32_launcher(NULL);
}

// ------------------------------------------------------------------------

#endif
