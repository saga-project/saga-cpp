//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>

#include "file_serialization.hpp"
#include "directory_serialization.hpp"
#include "logical_file_serialization.hpp"
#include "logical_directory_serialization.hpp"
#include "advert_serialization.hpp"
#include "advert_directory_serialization.hpp"

using boost::unit_test_framework::test_suite;

///////////////////////////////////////////////////////////////////////////////
//  This is the actual test suite entry point
test_suite* init_unit_test_suite (int argc, char* argv[])
{
    test_suite* suite = BOOST_TEST_SUITE("saga serialization test suite");
    
    suite->add (new test_file_serialization::suite());
    suite->add (new test_directory_serialization::suite());
    suite->add (new test_logical_file_serialization::suite());
    suite->add (new test_logical_directory_serialization::suite());
    suite->add (new test_advert_serialization::suite());
    suite->add (new test_advert_directory_serialization::suite());
    
    return suite;
};

