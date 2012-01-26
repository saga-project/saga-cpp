//  Copyright (c) 2005-2006 Chris Miceli (cmicel1@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test_stream.hpp"

///////////////////////////////////////////////////////////////////////////////
std::string test_stream::get_url::name   ("stream::get_url");
///////////////////////////////////////////////////////////////////////////////
void test_stream::instantiate_tests(boost::unit_test_framework::test_suite *test_suite_,
                                  std::string scheme, std::string host, std::string port)
{
    test_suite_->add(new test_stream::all::suite(scheme, host, port));
}

