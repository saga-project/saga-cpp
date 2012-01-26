//  Copyright (c) 2005-2006 Chris Miceli (cmicel1@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test_directory.hpp"

using boost::unit_test_framework::test_suite;


namespace test_dir
{
    ///////////////////////////////////////////////////////////////////////////
    std::string get_url::name = std::string("dir::get_url");
    std::string get_cwd::name = std::string("dir::get_cwd");
    std::string get_name::name = std::string("dir::get_name");
    std::string is_dir::name = std::string("dir::is_dir");
    template <typename T>
    std::string exists<T>::name = std::string("dir::exists");
    template <typename T>
    std::string is_entry<T>::name = std::string("dir::is_entry");
    std::string make_dir::name = std::string("dir::make_dir");
#if BOOST_VERSION >= 103400
   std::string is_link::name = std::string("dir::is_link");
#endif
    std::string change_dir::name = std::string("dir::change_dir");
    std::string move::name = std::string("dir::move");
    std::string copy::name = std::string("dir::copy");
    std::string remove::name = std::string("dir::remove");
    std::string open::name = std::string("dir::open");
    std::string open_dir::name = std::string("dir::open_dir");

    ///////////////////////////////////////////////////////////////////////////
    void 
    instantiate_tests(boost::unit_test_framework::test_suite *test_suite_,
        saga::test::test_helper_utils *utils_)
    {
        test_suite_->add (new test_dir::all::suite (utils_));
    }
}

