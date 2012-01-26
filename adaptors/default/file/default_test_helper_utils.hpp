//  Copyright (c) 2006-2007 Ole Christian Weidner (oweidner@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_ADAPTORS_DEFAULT_TEST_HELPER_UTILS_HPP
#define SAGA_ADAPTORS_DEFAULT_TEST_HELPER_UTILS_HPP

#include <test/cpp/unit_tests/interface/test_helper_utils.hpp>
#include <boost/thread.hpp>

/** 
  * Implementation of the test_helper_utils interface.
  * This class is used by the SAGA Conformance Tests to generate files and 
  * directories without using the SAGA API (since the API is subject to the test)
  */

class file_test_helper_utils_impl : public saga::test::test_helper_utils
{
  typedef boost::recursive_mutex mutex_type;
  mutable mutex_type mtx_;

public:   
  file_test_helper_utils_impl() {}

  ~file_test_helper_utils_impl() {} 

  saga::url create_temp_file_name (bool create = true);
  saga::url create_temp_dir_name  (bool create = true);

  saga::url create_temp_file_for_exception( const saga::error & e );
  saga::url create_temp_dir_for_exception( const saga::error & e );

  void delete_temp_file (saga::url path);
  void delete_temp_dir  (saga::url path);
};

#endif //SAGA_ADAPTORS_DEFAULT_TEST_HELPER_UTILS_HPP

