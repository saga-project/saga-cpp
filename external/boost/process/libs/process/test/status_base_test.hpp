//
// Boost.Process
// Regression tests for Exit_Status implementations.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt.)
//

#include <string>

#include <boost/process/child.hpp>
#include <boost/process/command_line.hpp>
#include <boost/process/launcher.hpp>
#include <boost/test/unit_test.hpp>

#include "misc.hpp"

namespace bp = ::boost::process;

// ------------------------------------------------------------------------

namespace status_base_test {

// ------------------------------------------------------------------------

static
bp::child
launch_helper(const std::string& name)
{
    bp::command_line cl(get_helpers_path());
    cl.argument(name);
    return bp::launcher().start(cl);
}

// ------------------------------------------------------------------------

template< class Exit_Status >
void
test_exit_failure(void)
{
    Exit_Status s = launch_helper("exit-failure").wait();
    BOOST_REQUIRE(s.exited());
    BOOST_CHECK_EQUAL(s.exit_status(), EXIT_FAILURE);
}

// ------------------------------------------------------------------------

template< class Exit_Status >
static
void
test_exit_success(void)
{
    Exit_Status s = launch_helper("exit-success").wait();
    BOOST_REQUIRE(s.exited());
    BOOST_CHECK_EQUAL(s.exit_status(), EXIT_SUCCESS);
}

// ------------------------------------------------------------------------

} // namespace status_base_test

// ------------------------------------------------------------------------

template< class Exit_Status >
void
add_tests_status_base(boost::unit_test::test_suite* ts)
{
    using namespace status_base_test;

    ts->add(BOOST_TEST_CASE(&(test_exit_success< Exit_Status >)), 0, 10);
    ts->add(BOOST_TEST_CASE(&(test_exit_failure< Exit_Status >)), 0, 10);
}
