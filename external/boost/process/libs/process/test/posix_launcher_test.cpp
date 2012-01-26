//
// Boost.Process
// Regression tests for the posix_launcher class.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt.)
//

#include <boost/process/config.hpp>

#if defined(BOOST_PROCESS_POSIX_API)

#   include <cstring>
#   include <string>

#   include <boost/process/command_line.hpp>
#   include <boost/process/posix_child.hpp>
#   include <boost/process/posix_launcher.hpp>

#   include "launcher_base_test.hpp"

namespace bp = ::boost::process;
#endif

#include <boost/filesystem/operations.hpp>
#include <boost/test/unit_test.hpp>

namespace bfs = ::boost::filesystem;
namespace but = ::boost::unit_test;

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
class start
{
public:
    bp::posix_child
    operator()(bp::posix_launcher& l, const bp::command_line& cl,
               bool usein = false)
        const
    {
        return l.start(cl);
    }
};
#endif

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
void
test_input(void)
{
    bp::command_line cl(get_helpers_path());
    cl.argument("stdin-to-stdout");

    bp::posix_launcher l;
    l.set_input_behavior(STDIN_FILENO, bp::redirect_stream);
    l.set_output_behavior(STDOUT_FILENO, bp::redirect_stream);
    bp::posix_child c = l.start(cl);

    bp::postream& os = c.get_input(STDIN_FILENO);
    bp::pistream& is = c.get_output(STDOUT_FILENO);

    os << "message-to-process" << std::endl;
    os.close();

    std::string word;
    is >> word;
    BOOST_CHECK_EQUAL(word, "message-to-process");

    bp::status s = c.wait();
    BOOST_REQUIRE(s.exited());
    BOOST_CHECK_EQUAL(s.exit_status(), EXIT_SUCCESS);
}
#endif

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
void
check_output(int desc, const std::string& msg)
{
    bp::command_line cl(get_helpers_path());
    cl.argument("posix-echo-one").argument(desc).argument(msg);

    bp::posix_launcher l;
    l.set_output_behavior(desc, bp::redirect_stream);
    bp::posix_child c = l.start(cl);

    bp::pistream& is = c.get_output(desc);
    std::string word;
    is >> word;
    BOOST_CHECK_EQUAL(word, msg);

    bp::status s = c.wait();
    BOOST_REQUIRE(s.exited());
    BOOST_CHECK_EQUAL(s.exit_status(), EXIT_SUCCESS);
}
#endif

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
static
void
test_output(void)
{
    check_output(STDOUT_FILENO, "message1-stdout");
    check_output(STDOUT_FILENO, "message2-stdout");
    check_output(STDERR_FILENO, "message1-stderr");
    check_output(STDERR_FILENO, "message2-stderr");
    check_output(10, "message1-10");
    check_output(10, "message2-10");
}
#endif

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
void
check_merge(int desc1, int desc2, const std::string& msg)
{
    bp::command_line cl(get_helpers_path());
    cl.argument("posix-echo-two").argument(desc1).argument(desc2);
    cl.argument(msg);

    bp::posix_launcher l;
    l.set_output_behavior(desc1, bp::redirect_stream);
    l.merge_outputs(desc2, desc1);
    bp::posix_child c = l.start(cl);

    bp::pistream& is = c.get_output(desc1);
    int dtmp;
    std::string word;
    is >> dtmp;
    BOOST_CHECK_EQUAL(dtmp, desc1);
    is >> word;
    BOOST_CHECK_EQUAL(word, msg);
    is >> dtmp;
    BOOST_CHECK_EQUAL(dtmp, desc2);
    is >> word;
    BOOST_CHECK_EQUAL(word, msg);

    bp::status s = c.wait();
    BOOST_REQUIRE(s.exited());
    BOOST_CHECK_EQUAL(s.exit_status(), EXIT_SUCCESS);
}
#endif

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
static
void
test_merge(void)
{
    check_merge(STDOUT_FILENO, STDERR_FILENO, "message");
    check_merge(STDERR_FILENO, STDOUT_FILENO, "message");
    check_merge(4, 5, "message");
    check_merge(10, 20, "message");
}
#endif

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
static
void
test_default_ids(void)
{
    bp::posix_launcher pl;
    BOOST_CHECK_EQUAL(pl.get_gid(), ::getgid());
    BOOST_CHECK_EQUAL(pl.get_egid(), ::getegid());
    BOOST_CHECK_EQUAL(pl.get_uid(), ::getuid());
    BOOST_CHECK_EQUAL(pl.get_euid(), ::geteuid());
}
#endif

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
static
void
test_setters(void)
{
    bp::posix_launcher pl1;
    gid_t gid = pl1.get_gid() + 1;
    pl1.set_gid(gid);
    BOOST_CHECK_EQUAL(pl1.get_gid(), gid);

    bp::posix_launcher pl2;
    gid_t egid = pl2.get_egid() + 1;
    pl2.set_egid(egid);
    BOOST_CHECK_EQUAL(pl2.get_egid(), egid);

    bp::posix_launcher pl3;
    uid_t uid = pl3.get_uid() + 1;
    pl3.set_uid(uid);
    BOOST_CHECK_EQUAL(pl3.get_uid(), uid);

    bp::posix_launcher pl4;
    uid_t euid = pl4.get_euid() + 1;
    pl4.set_euid(euid);
    BOOST_CHECK_EQUAL(pl4.get_euid(), euid);

    bp::posix_launcher pl5;
    pl5.set_chroot("/some/directory");
    BOOST_CHECK_EQUAL(pl5.get_chroot(), "/some/directory");
}
#endif

// ------------------------------------------------------------------------

#if !defined(BOOST_PROCESS_POSIX_API)
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

    but::test_suite* test = BOOST_TEST_SUITE("posix_launcher test suite");

#if defined(BOOST_PROCESS_POSIX_API)
    add_tests_launcher_base< bp::posix_launcher, bp::child, start >(test);
    add_tests_launcher_base< bp::posix_launcher, bp::posix_child, start >
        (test);

    test->add(BOOST_TEST_CASE(&test_output), 0, 10);
    test->add(BOOST_TEST_CASE(&test_merge), 0, 10);
    test->add(BOOST_TEST_CASE(&test_input), 0, 10);
    test->add(BOOST_TEST_CASE(&test_default_ids));
    test->add(BOOST_TEST_CASE(&test_setters));
#else
    test->add(BOOST_TEST_CASE(&test_dummy));
#endif

    return test;
}
