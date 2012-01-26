//  Copyright (c) 2005-2006 Chris Miceli (cmicel1@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test_file.hpp"

///////////////////////////////////////////////////////////////////////////////
std::string test_file::get_url::name   ("file::get_url");
std::string test_file::get_cwd::name   ("file::get_cwd");
std::string test_file::get_name::name  ("file::get_name");
std::string test_file::is_dir::name    ("file::is_dir");
std::string test_file::is_entry::name  ("file::is_entry");
#if BOOST_VERSION >= 103400
std::string test_file::is_link::name   ("file::is_link");
#endif
std::string test_file::read_write::name("file::read_write");

///////////////////////////////////////////////////////////////////////////////
void test_file::instantiate_tests(boost::unit_test_framework::test_suite *test_suite_,
                                  saga::test::test_helper_utils *utils_,
                                  std::string scheme, std::string host)
{
    test_suite_->add(new test_file::all::suite(utils_, scheme, host));
}

