//  Copyright (c) 2005-2006 Chris Miceli (cmicel1@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test_job.hpp"

std::string test_job::run_job::name("job::run_job");
std::string test_job::create_job::name("job::create_job");
std::string test_job::list::name("job::list");
std::string test_job::suspend_resume::name("job::suspend_resume");
std::string test_job::get_job::name("job::get_job");

void test_job::instantiate_tests(
    boost::unit_test_framework::test_suite *test_suite_,
    std::string test_helper, std::string scheme, std::string host,
    std::vector<std::string> const& arguments)
{
    test_suite_->add(new test_job::all::suite(test_helper, scheme, host, arguments));
}
