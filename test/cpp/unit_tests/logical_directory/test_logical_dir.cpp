//  Copyright (c) 2005-2006 Chris Miceli (cmicel1@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test_logical_dir.hpp"

///////////////////////////////////////////////////////////////////////////////
std::string test_logical_dir::is_file::name ("logical_dir::is_file");
std::string test_logical_dir::open_dir::name("logical_dir::open_dir");
std::string test_logical_dir::open::name    ("logical_dir::open");
std::string test_logical_dir::find::name    ("logical_dir::find");

///////////////////////////////////////////////////////////////////////////////
void test_logical_dir::instantiate_tests(boost::unit_test_framework::test_suite *test_suite_,
                                  saga::test::test_helper_utils *utils_)
{
    test_suite_->add(new test_logical_dir::all::suite(utils_));
}

