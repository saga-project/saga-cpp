//  Copyright (c) 2006 Ole Christian Weidner (oweidner@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_TEST_INIT_HPP
#define SAGA_TEST_INIT_HPP

#include <saga/saga/adaptors/utils/ini/ini.hpp>

#include "test_cleanup.hpp"
#include "handle_ini.hpp"

/**
  * This class is used for test initialization based on a given config file.
  *
  */
class test_init
{
  private:
    saga::ini::ini    config_file_;
    saga::ini::ini    pref_file_;
    handle_ini        handler_;
    boost::unit_test::test_suite* test_suite_;
    cleanup::suite*  cleanup_handler_;
    
    /**
      * the main config file processing function. Parses/validates the config file
      * and adds the test cases to the test suite. 
      *
      */  
    void add_adaptor_tests_();

    /**
      * the url file processing function. Parses/validates the config file
      * and adds the url test cases to the test suite. 
      *
      */  
    void add_url_tests_();

    /**
      * checks if the provided library (adaptor) has a factory method for
      * creating an instance of test_helper_utils.
      *
      * @param lib_path
      *      path to the library we want to test
      * 
      * @return
      *      TRUE if the factory exists, FALSE otherwise
      *
      */
    bool lib_has_test_helper_factory_(std::string& lib_path, 
        std::string adaptor_name, bool retry = true);
    
    /**
      * COMMENT ME !
      */
    saga::test::test_helper_utils * init_helper_object_(std::string lib_path);
  
  public:
    /**
      * constructor
      *
      * @param suite
      *      a pointer to the boost test suite we want to initialize
      *
      * @param config_file_location
      *      a string containing the path of the config file we want to use
      */  
    test_init(boost::unit_test::test_suite* suite, std::string config_file_location);

    /**
      * destructor
      */
    ~test_init() {}
};

#endif //SAGA_TEST_INIT_HPP
