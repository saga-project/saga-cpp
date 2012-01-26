//
// Boost.Process
// Regression tests for the detail::pipe class.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt.)
//

#include <boost/process/config.hpp>

#if defined(BOOST_PROCESS_POSIX_API)
extern "C" {
#   include <unistd.h>
}
#elif defined(BOOST_PROCESS_WIN32_API)
#else
#   error "Unsupported platform."
#endif

#include <iostream>

#include <boost/process/detail/pipe.hpp>
#include <boost/process/detail/systembuf.hpp>
#include <boost/test/unit_test.hpp>

namespace bp = ::boost::process;
namespace bpd = ::boost::process::detail;
namespace but = ::boost::unit_test;

// ------------------------------------------------------------------------

static
void
test_read_and_write(void)
{
    bpd::pipe p;
    bpd::systembuf rbuf(p.rend().get());
    bpd::systembuf wbuf(p.wend().get());
    std::istream rend(&rbuf);
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

#if defined(BOOST_PROCESS_POSIX_API)
static
void
test_remap_read(void)
{
    bpd::pipe p;
    bpd::systembuf wbuf(p.wend().get());
    std::ostream wend(&wbuf);
    p.rend().posix_remap(STDIN_FILENO);

    // XXX This assumes that the pipe's buffer is big enough to accept
    // the data written without blocking!
    wend << "2Test 2message" << std::endl;
    std::string tmp;
    std::cin >> tmp;
    BOOST_CHECK_EQUAL(tmp, "2Test");
    std::cin >> tmp;
    BOOST_CHECK_EQUAL(tmp, "2message");
}
#endif

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
static
void
test_remap_write(void)
{
    bpd::pipe p;
    bpd::systembuf rbuf(p.rend().get());
    std::istream rend(&rbuf);
    p.wend().posix_remap(STDOUT_FILENO);

    // XXX This assumes that the pipe's buffer is big enough to accept
    // the data written without blocking!
    std::cout << "3Test 3message" << std::endl;
    std::string tmp;
    rend >> tmp;
    BOOST_CHECK_EQUAL(tmp, "3Test");
    rend >> tmp;
    BOOST_CHECK_EQUAL(tmp, "3message");
}
#endif

// ------------------------------------------------------------------------

but::test_suite *
init_unit_test_suite(int argc, char* argv[])
{
    but::test_suite* test = BOOST_TEST_SUITE("detail::pipe test suite");

    test->add(BOOST_TEST_CASE(&test_read_and_write));

#if defined(BOOST_PROCESS_POSIX_API)
    test->add(BOOST_TEST_CASE(&test_remap_read));
    test->add(BOOST_TEST_CASE(&test_remap_write));
#endif

    return test;
}
