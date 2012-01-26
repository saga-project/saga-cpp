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

#include <boost/filesystem/operations.hpp>
#include <boost/process/child.hpp>
#include <boost/process/launcher.hpp>
#include <boost/test/unit_test.hpp>

#include "launcher_base_test.hpp"

namespace bfs = ::boost::filesystem;
namespace bp = ::boost::process;
namespace but = ::boost::unit_test;

// ------------------------------------------------------------------------

class start
{
public:
    bp::child
    operator()(bp::launcher& l, const bp::command_line& cl,
               bool usein = false)
        const
    {
        return l.start(cl);
    }
};

// ------------------------------------------------------------------------

but::test_suite *
init_unit_test_suite(int argc, char* argv[])
{
    bfs::initial_path();

    but::test_suite* test = BOOST_TEST_SUITE("launcher test suite");

    add_tests_launcher_base< bp::launcher, bp::child, start >(test);

    return test;
}
