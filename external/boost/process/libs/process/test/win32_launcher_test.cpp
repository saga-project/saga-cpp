//
// Boost.Process
// Regression tests for the win32_launcher class.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt.)
//

#include <boost/process/config.hpp>

#if defined(BOOST_PROCESS_WIN32_API)

extern "C" {
#   include <windows.h>
}

#   include <sstream>
#   include <string>

#   include <boost/process/child.hpp>
#   include <boost/process/win32_child.hpp>
#   include <boost/process/win32_launcher.hpp>

#   include "launcher_base_test.hpp"

namespace bp = ::boost::process;
#endif

#include <boost/filesystem/operations.hpp>
#include <boost/test/unit_test.hpp>

namespace bfs = ::boost::filesystem;
namespace but = ::boost::unit_test;

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_WIN32_API)
class start
{
public:
    bp::win32_child
    operator()(bp::win32_launcher& l, const bp::command_line& cl,
               bool usein = false)
        const
    {
        return l.start(cl);
    }
};
#endif

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_WIN32_API)
template< class Child >
static
void
test_startupinfo(void)
{
    bp::command_line cl(get_helpers_path());
    cl.argument("win32-print-startupinfo");

    std::string line;
    std::ostringstream flags;

    bp::win32_launcher l1;
    l1.set_stdout_behavior(bp::redirect_stream);
    flags << STARTF_USESTDHANDLES;
    Child c1 = l1.start(cl);
    portable_getline(c1.get_stdout(), line);
    BOOST_CHECK_EQUAL(line, "dwFlags = " + flags.str());
    flags.str("");
    portable_getline(c1.get_stdout(), line);
    BOOST_CHECK_EQUAL(line, "dwX = 0");
    portable_getline(c1.get_stdout(), line);
    BOOST_CHECK_EQUAL(line, "dwY = 0");
    portable_getline(c1.get_stdout(), line);
    BOOST_CHECK_EQUAL(line, "dwXSize = 0");
    portable_getline(c1.get_stdout(), line);
    BOOST_CHECK_EQUAL(line, "dwYSize = 0");
    bp::status s1 = c1.wait();
    BOOST_REQUIRE(s1.exited());
    BOOST_CHECK_EQUAL(s1.exit_status(), EXIT_SUCCESS);

    STARTUPINFO si;
    ::ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USEPOSITION | STARTF_USESIZE;
    si.dwX = 100;
    si.dwY = 200;
    si.dwXSize = 300;
    si.dwYSize = 400;
    bp::win32_launcher l2(&si);
    l2.set_stdout_behavior(bp::redirect_stream);
    flags << (STARTF_USESTDHANDLES | STARTF_USEPOSITION | STARTF_USESIZE);
    Child c2 = l2.start(cl);
    portable_getline(c2.get_stdout(), line);
    BOOST_CHECK_EQUAL(line, "dwFlags = " + flags.str());
    flags.str("");
    portable_getline(c2.get_stdout(), line);
    BOOST_CHECK_EQUAL(line, "dwX = 100");
    portable_getline(c2.get_stdout(), line);
    BOOST_CHECK_EQUAL(line, "dwY = 200");
    portable_getline(c2.get_stdout(), line);
    BOOST_CHECK_EQUAL(line, "dwXSize = 300");
    portable_getline(c2.get_stdout(), line);
    BOOST_CHECK_EQUAL(line, "dwYSize = 400");
    bp::status s2 = c2.wait();
    BOOST_REQUIRE(s2.exited());
    BOOST_CHECK_EQUAL(s2.exit_status(), EXIT_SUCCESS);
}
#endif

// ------------------------------------------------------------------------

#if !defined(BOOST_PROCESS_WIN32_API)
static
void
test_dummy(void)
{
}
#endif

// ------------------------------------------------------------------------

but::test_suite *
init_unit_test_suite(int argc, char* argv[])
{
    bfs::initial_path();

    but::test_suite* test = BOOST_TEST_SUITE("win32_launcher test suite");

#if defined(BOOST_PROCESS_WIN32_API)
    add_tests_launcher_base< bp::win32_launcher, bp::child, start >(test);
    add_tests_launcher_base< bp::win32_launcher, bp::win32_child, start >
        (test);
    test->add(BOOST_TEST_CASE(&(test_startupinfo< bp::child >)));
    test->add(BOOST_TEST_CASE(&(test_startupinfo< bp::win32_child >)));
#else
    test->add(BOOST_TEST_CASE(&test_dummy));
#endif

    return test;
}
