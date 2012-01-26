//
// Boost.Process
// Regression tests for the environment class.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt.)
//

#include <boost/process/config.hpp>

#if defined(BOOST_PROCESS_POSIX_API)
#   include <cstdlib>
#elif defined(BOOST_PROCESS_WIN32_API)
extern "C" {
#   include <tchar.h>
}
#   include <cstring>
#else
#   error "Unsupported platform."
#endif

#include <boost/process/detail/environment.hpp>
#include <boost/test/unit_test.hpp>

namespace bp = ::boost::process;
namespace bpd = ::boost::process::detail;
namespace but = ::boost::unit_test;

// ------------------------------------------------------------------------

static
void
test_init(void)
{
    bpd::environment env1;
    BOOST_CHECK(env1.find("THIS_SHOULD_NOT_BE_DEFINED") == env1.end());

#if defined(BOOST_PROCESS_POSIX_API)
    BOOST_REQUIRE(::setenv("THIS_SHOULD_BE_DEFINED", "some-value", 1) == 0);
#elif defined(BOOST_PROCESS_WIN32_API)
    BOOST_REQUIRE(::SetEnvironmentVariable(TEXT("THIS_SHOULD_BE_DEFINED"),
                                           TEXT("some-value")) != 0);
#endif

    bpd::environment env2;
    bpd::environment::const_iterator iter =
        env2.find("THIS_SHOULD_BE_DEFINED");
    BOOST_CHECK(iter != env2.end());
    BOOST_CHECK_EQUAL((*iter).second, "some-value");
}

// ------------------------------------------------------------------------

static
void
test_set(void)
{
    bpd::environment env;
    bpd::environment::size_type initialsize = env.size();
    env.set("SOME_VARIABLE", "content");
    BOOST_REQUIRE_EQUAL(env.size(), initialsize + 1);
    bpd::environment::const_iterator iter = env.find("SOME_VARIABLE");
    BOOST_REQUIRE(iter != env.end());
    BOOST_REQUIRE_EQUAL((*iter).first, "SOME_VARIABLE");
    BOOST_REQUIRE_EQUAL((*iter).second, "content");
}

// ------------------------------------------------------------------------

static
void
test_unset(void)
{
    bpd::environment env;
    env.set("SOME_VARIABLE", "content");
    bpd::environment::size_type initialsize = env.size();
    env.unset("SOME_VARIABLE");
    BOOST_REQUIRE_EQUAL(env.size(), initialsize - 1);
    bpd::environment::const_iterator iter = env.find("SOME_VARIABLE");
    BOOST_REQUIRE(iter == env.end());
}

// ------------------------------------------------------------------------

static
void
test_envp(void)
{
    bpd::environment env;
    env.clear();
    env.set("VAR1", "value1");
    env.set("VAR2", "value2");
    env.set("VAR3", "value3");

    char** ep = env.envp();

    BOOST_REQUIRE(ep[0] != NULL);
    BOOST_REQUIRE_EQUAL(std::string(ep[0]), "VAR1=value1");
    delete [] ep[0];

    BOOST_REQUIRE(ep[1] != NULL);
    BOOST_REQUIRE_EQUAL(std::string(ep[1]), "VAR2=value2");
    delete [] ep[1];

    BOOST_REQUIRE(ep[2] != NULL);
    BOOST_REQUIRE_EQUAL(std::string(ep[2]), "VAR3=value3");
    delete [] ep[2];

    BOOST_REQUIRE(ep[3] == NULL);
    delete [] ep;
}

// ------------------------------------------------------------------------

static
void
test_envp_unsorted(void)
{
    bpd::environment env;
    env.clear();
    env.set("VAR2", "value2");
    env.set("VAR1", "value1");

    char** ep = env.envp();

    BOOST_REQUIRE(ep[0] != NULL);
    BOOST_REQUIRE_EQUAL(std::string(ep[0]), "VAR1=value1");
    delete [] ep[0];

    BOOST_REQUIRE(ep[1] != NULL);
    BOOST_REQUIRE_EQUAL(std::string(ep[1]), "VAR2=value2");
    delete [] ep[1];

    BOOST_REQUIRE(ep[2] == NULL);
    delete [] ep;
}

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_WIN32_API)
static
void
test_strings(void)
{
    bpd::environment env;
    env.clear();
    env.set("VAR1", "value1");
    env.set("VAR2", "value2");
    env.set("VAR3", "value3");

    boost::shared_array< TCHAR > strs = env.win32_strings();
    BOOST_REQUIRE(strs.get() != NULL);

    TCHAR* expected = TEXT("VAR1=value1\0VAR2=value2\0VAR3=value3\0\0");
    BOOST_REQUIRE(std::memcmp(strs.get(), expected,
                              37 * sizeof(TCHAR)) == 0);
}
#endif

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_WIN32_API)
static
void
test_strings_unsorted(void)
{
    bpd::environment env;
    env.clear();
    env.set("VAR2", "value2");
    env.set("VAR1", "value1");

    boost::shared_array< TCHAR > strs = env.win32_strings();
    BOOST_REQUIRE(strs.get() != NULL);

    TCHAR* expected = TEXT("VAR1=value1\0VAR2=value2\0\0");
    BOOST_REQUIRE(std::memcmp(strs.get(), expected,
                              25 * sizeof(TCHAR)) == 0);
}
#endif

// ------------------------------------------------------------------------

but::test_suite *
init_unit_test_suite(int argc, char* argv[])
{
    but::test_suite* test = BOOST_TEST_SUITE("environment test suite");

    test->add(BOOST_TEST_CASE(&test_init));
    test->add(BOOST_TEST_CASE(&test_set));
    test->add(BOOST_TEST_CASE(&test_unset));
    test->add(BOOST_TEST_CASE(&test_envp));
    test->add(BOOST_TEST_CASE(&test_envp_unsorted));
#if defined(BOOST_PROCESS_WIN32_API)
    test->add(BOOST_TEST_CASE(&test_strings));
    test->add(BOOST_TEST_CASE(&test_strings_unsorted));
#endif

    return test;
}
