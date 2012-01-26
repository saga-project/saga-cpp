//
// Boost.Process
// Regression tests for the launcher class.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt.)
//

#include <cstring>

#include <boost/filesystem/operations.hpp>
#include <boost/process/children.hpp>
#include <boost/process/pipeline.hpp>
#include <boost/test/unit_test.hpp>

#include "launcher_base_test.hpp"

namespace bfs = ::boost::filesystem;
namespace bp = ::boost::process;
namespace but = ::boost::unit_test;

// ------------------------------------------------------------------------

class start
{
public:
    bp::children
    operator()(bp::pipeline& p, const bp::command_line& cl,
               bool usein = false)
        const
    {
        bp::pipeline p2 = p;
        bp::command_line dummy(get_helpers_path());
        dummy.argument("stdin-to-stdout");
        if (usein)
            p2.add(cl).add(dummy);
        else
            p2.add(dummy).add(cl);
        return p2.start();
    }
};

// ------------------------------------------------------------------------

static
void
test_exit(const std::string& middle, int value)
{
    bp::command_line cl1(get_helpers_path());
    cl1.argument("exit-success");

    bp::command_line cl2(get_helpers_path());
    cl2.argument(middle);

    bp::command_line cl3(get_helpers_path());
    cl3.argument("exit-success");

    bp::pipeline p;
    p.add(cl1).add(cl2).add(cl3);
    bp::children cs = p.start();

    BOOST_REQUIRE(cs.size() == 3);

    bp::status s = cs.wait();
    BOOST_REQUIRE(s.exited());
    BOOST_CHECK_EQUAL(s.exit_status(), value);
}

// ------------------------------------------------------------------------

static
void
test_exit_failure(void)
{
    test_exit("exit-failure", EXIT_FAILURE);
}

// ------------------------------------------------------------------------

static
void
test_exit_success(void)
{
    test_exit("exit-success", EXIT_SUCCESS);
}

// ------------------------------------------------------------------------

static
void
test_simple(void)
{
    bp::command_line cl1(get_helpers_path());
    cl1.argument("prefix").argument("proc1-");

    bp::command_line cl2(get_helpers_path());
    cl2.argument("prefix").argument("proc2-");

    bp::pipeline p;
    p.set_stdin_behavior(bp::redirect_stream);
    p.set_stdout_behavior(bp::redirect_stream);
    p.add(cl1).add(cl2);
    bp::children cs = p.start();

    BOOST_REQUIRE(cs.size() == 2);

    cs.get_stdin() << "message" << std::endl;
    cs.get_stdin().close();

    std::string word;
    cs.get_stdout() >> word;
    BOOST_CHECK_EQUAL(word, "proc2-proc1-message");

    bp::status s = cs.wait();
    BOOST_REQUIRE(s.exited());
    BOOST_CHECK_EQUAL(s.exit_status(), EXIT_SUCCESS);
}

// ------------------------------------------------------------------------

static
void
test_merge_first(void)
{
    bp::command_line cl1(get_helpers_path());
    cl1.argument("echo-stdout-stderr").argument("message");

    bp::command_line cl2(get_helpers_path());
    cl2.argument("prefix").argument("second:");

    bp::pipeline p;
    p.set_stdout_behavior(bp::redirect_stream);
    p.add(cl1, true).add(cl2);
    bp::children cs = p.start();

    BOOST_REQUIRE(cs.size() == 2);

    std::string line;
    portable_getline(cs.get_stdout(), line);
    BOOST_CHECK_EQUAL(line, "second:stdout message");
    portable_getline(cs.get_stdout(), line);
    BOOST_CHECK_EQUAL(line, "second:stderr message");

    bp::status s = cs.wait();
    BOOST_REQUIRE(s.exited());
    BOOST_CHECK_EQUAL(s.exit_status(), EXIT_SUCCESS);
}

// ------------------------------------------------------------------------

but::test_suite *
init_unit_test_suite(int argc, char* argv[])
{
    bfs::initial_path();

    but::test_suite* test = BOOST_TEST_SUITE("pipeline test suite");

    add_tests_launcher_base< bp::pipeline, bp::children, start >(test);

    test->add(BOOST_TEST_CASE(&test_exit_success), 0, 10);
    test->add(BOOST_TEST_CASE(&test_exit_failure), 0, 10);
    test->add(BOOST_TEST_CASE(&test_simple), 0, 10);
    test->add(BOOST_TEST_CASE(&test_merge_first), 0, 10);

    return test;
}
