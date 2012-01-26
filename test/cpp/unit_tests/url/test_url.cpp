//  Copyright (c) 2005-2006 Chris Miceli (cmicel1@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test_url.hpp"

///////////////////////////////////////////////////////////////////////////////
std::string test_url::valid_urls::name   ("url::valid_urls");
std::string test_url::invalid_urls::name ("url::invalid_urls");
///////////////////////////////////////////////////////////////////////////////
void test_url::instantiate_tests(boost::unit_test_framework::test_suite *test_suite_)
{
    test_suite_->add(new test_url::all::suite());
}

