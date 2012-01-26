//
// Boost.Process
// Regression tests for the pistream class.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt.)
//

#include <ostream>

#include <boost/process/detail/pipe.hpp>
#include <boost/process/detail/systembuf.hpp>
#include <boost/process/pistream.hpp>
#include <boost/test/unit_test.hpp>

namespace bp = ::boost::process;
namespace bpd = ::boost::process::detail;
namespace but = ::boost::unit_test;

// ------------------------------------------------------------------------

static
void
test_it(void)
{
    bpd::pipe p;
    bp::pistream rend(p.rend());
    bpd::systembuf wbuf(p.wend().get());
    std::ostream wend(&wbuf);

    // XXX This assumes that the pipe's buffer is big enough to accept
    // the data written without blocking!
    wend << "1Test 1message" << std::endl;
    std::string tmp;
    rend >> tmp;
    BOOST_CHECK_EQUAL(tmp, "1Test");
    rend >> tmp;
    BOOST_CHECK_EQUAL(tmp, "1message");
}

// ------------------------------------------------------------------------

but::test_suite *
init_unit_test_suite(int argc, char* argv[])
{
    but::test_suite* test = BOOST_TEST_SUITE("pistream test suite");

    test->add(BOOST_TEST_CASE(&test_it));

    return test;
}
