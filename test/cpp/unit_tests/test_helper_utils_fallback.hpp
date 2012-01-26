//  Copyright (c) 2008 Chris Miceli
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga.hpp>
#include <test/cpp/unit_tests/interface/test_helper_utils.hpp>
#include <iostream>


class test_helper_utils_fallback : public saga::test::test_helper_utils
{
 public:
   test_helper_utils_fallback(std::string scheme, std::string host);
   ~test_helper_utils_fallback();

   saga::url create_temp_file_name(bool create);
   saga::url create_temp_dir_name (bool create);

   saga::url create_temp_file_for_exception(const saga::error &e);
   saga::url create_temp_dir_for_exception (const saga::error &e);

   void delete_temp_file(saga::url url);
   void delete_temp_dir (saga::url url);
 private:
   std::string scheme_;
   std::string host_;
};
