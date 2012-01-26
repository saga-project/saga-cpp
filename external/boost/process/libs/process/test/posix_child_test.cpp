//
// Boost.Process
// Regression tests for the posix_child class.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt.)
//

#if defined(BOOST_PROCESS_POSIX_API)
#   include <boost/process/detail/factories.hpp>
#   include <boost/process/posix_child.hpp>

#   include "child_base_test.hpp"

namespace bp = ::boost::process;
namespace bpd = ::boost::process::detail;
#endif

#include <boost/test/unit_test.hpp>

namespace but = ::boost::unit_test;

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
struct factory
{
    bp::posix_child
    operator()(bp::posix_child::handle_type h, bpd::file_handle fhstdin,
               bpd::file_handle fhstdout, bpd::file_handle fhstderr)
    {
        bpd::info_map infoin, infoout;

        if (fhstdin.is_valid()) {
            bpd::stream_info si;
            si.m_type = bpd::stream_info::usepipe;
            si.m_pipe = bpd::pipe();
            si.m_pipe->rend().close();
            si.m_pipe->wend() = fhstdin;
            infoin.insert(bpd::info_map::value_type(STDIN_FILENO, si));
        }

        if (fhstdout.is_valid()) {
            bpd::stream_info si;
            si.m_type = bpd::stream_info::usepipe;
            si.m_pipe = bpd::pipe();
            si.m_pipe->wend().close();
            si.m_pipe->rend() = fhstdout;
            infoout.insert(bpd::info_map::value_type(STDOUT_FILENO, si));
        }

        if (fhstderr.is_valid()) {
            bpd::stream_info si;
            si.m_type = bpd::stream_info::usepipe;
            si.m_pipe = bpd::pipe();
            si.m_pipe->wend().close();
            si.m_pipe->rend() = fhstderr;
            infoout.insert(bpd::info_map::value_type(STDERR_FILENO, si));
        }

        return bpd::factories::create_posix_child(h, infoin, infoout);
    }
};
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
    but::test_suite* test = BOOST_TEST_SUITE("posix_child test suite");

#if defined(BOOST_PROCESS_POSIX_API)
    add_tests_child_base< bp::posix_child, factory >(test);
#else
    test->add(BOOST_TEST_CASE(&test_dummy));
#endif

    return test;
}
