//  Copyright (c) 2005-2006 Chris Miceli (cmicel1@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test_logical_file.hpp"

///////////////////////////////////////////////////////////////////////////////
std::string test_logical_file::add_location::name("logical_file_add_location");
std::string test_logical_file::remove_location::name("logical_file_remove_location");
std::string test_logical_file::update_location::name("logical_file_update_location");
std::string test_logical_file::list_locations::name("logical_file_list_locations");
std::string test_logical_file::replicate::name("logical_file_replicate");

///////////////////////////////////////////////////////////////////////////////
void test_logical_file::instantiate_tests(boost::unit_test_framework::test_suite *test_suite_,
                                  saga::test::test_helper_utils *utils_)
{
    test_suite_->add(new test_logical_file::all::suite(utils_));
}

