//
// Boost.Process
// Regression tests for the child class.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt.)
//

#include <boost/process/child.hpp>
#include <boost/process/detail/factories.hpp>
#include <boost/test/unit_test.hpp>

#include "child_base_test.hpp"

namespace bp = ::boost::process;
namespace bpd = ::boost::process::detail;
namespace but = ::boost::unit_test;

// ------------------------------------------------------------------------

struct factory
{
    bp::child
    operator()(bp::child::handle_type h, bpd::file_handle fhstdin,
               bpd::file_handle fhstdout, bpd::file_handle fhstderr)
    {
        return bpd::factories::create_child(h, fhstdin, fhstdout, fhstderr);
    }
};

// ------------------------------------------------------------------------

but::test_suite *
init_unit_test_suite(int argc, char* argv[])
{
    but::test_suite* test = BOOST_TEST_SUITE("child test suite");

    add_tests_child_base< bp::child, factory >(test);

    return test;
}
