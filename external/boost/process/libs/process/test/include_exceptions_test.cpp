//
// Boost.Process
// Checks that exceptions.hpp can be included without errors.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt.)
//

#include <boost/process/exceptions.hpp>

namespace bp = ::boost::process;

// ------------------------------------------------------------------------

void*
test_it(void)
{
    return new bp::system_error("", "", 0);
}
