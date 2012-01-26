//
// Boost.Process
// Checks that detail/command_line_ops.hpp can be included without errors.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt.)
//

#include <boost/process/detail/command_line_ops.hpp>
#include <boost/process/command_line.hpp>

namespace bp = ::boost::process;
namespace bpd = ::boost::process::detail;

// ------------------------------------------------------------------------

void
test_it(void)
{
    bp::command_line cl("foo");
#if defined(BOOST_PROCESS_POSIX_API)
    bpd::command_line_to_posix_argv(cl);
#elif defined(BOOST_PROCESS_WIN32_API)
    bpd::command_line_to_win32_cmdline(cl);
#endif
}
