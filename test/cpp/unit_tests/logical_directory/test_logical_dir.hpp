//  Copyright (c) 2005-2006 Chris Miceli (cmicel1@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#if !defined(SAGA_TEST_LOGICAL_DIR_HPP)
#define SAGA_TEST_LOGICAL_DIR_HPP

#include <boost/test/unit_test.hpp>
#include "test_api.hpp"
#include "../test_functions.hpp"

namespace test_logical_dir
{
  void instantiate_tests(boost::unit_test_framework::test_suite *test_suite_,
                                  saga::test::test_helper_utils *utils_);
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  struct is_file
  {
      typedef boost::mpl::vector_c<int, saga::adaptors::Success
         > error_codes;
      static std::string name;
      template <typename Tag, typename Error>
      static int test(Tag tag, Error e, test_logical_dir::api::equipment equipment)
      {
          saga::session s = saga::get_default_session();
          bool is_file = true;
          int mode = saga::replica::Create |
                     saga::replica::CreateParents |
                     saga::replica::Read;
          saga::task t;
          saga::replica::logical_directory d(s, equipment.logical_dir, mode);
          t = d.is_file<Tag>(equipment.logical_dir);
          verify_created_state<Tag>(t);
          run_check_state<Tag>(t);
          wait_check_state<Tag>(t);
          rethrow_on_error(t);
          is_file = t.get_result<bool>();
          SAGA_CHECK(!is_file);
          return 0;
      }
      
      template <typename Error>
      static int test(test_logical_dir::api::PlainSync t, Error e, 
          test_logical_dir::api::equipment equipment)
      {
          saga::session s = saga::get_default_session();
          bool is_file = true;
          int mode = saga::replica::Create |
                     saga::replica::CreateParents |
                     saga::replica::Read;
          saga::replica::logical_directory d(s, equipment.logical_dir, mode);

          is_file = d.is_file(equipment.logical_dir);
          SAGA_CHECK(!is_file);
          return 0;
      }
  }; //is_file
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  struct open_dir
  {
      typedef boost::mpl::vector_c<int, saga::adaptors::Success
         > error_codes;
      static std::string name;
      template <typename Tag, typename Error>
      static int test(Tag tag, Error e, test_logical_dir::api::equipment equipment)
      {
          saga::session s = saga::get_default_session();
          saga::task t;
          int mode = saga::replica::Create |
                     saga::replica::CreateParents |
                     saga::replica::Read;
          saga::replica::logical_directory d(s, equipment.logical_dir, mode);

          saga::replica::logical_directory ld;
          t = d.open_dir<Tag>(saga::url("./temp"), mode);
          verify_created_state<Tag>(t);
          run_check_state<Tag>(t);
          wait_check_state<Tag>(t);
          rethrow_on_error(t);
          ld = t.get_result<saga::replica::logical_directory>();
          SAGA_CHECK(!d.is_file(ld.get_url()));
          SAGA_CHECK_EQUAL(saga::url::unescape(d.get_url().get_path()) + "temp/", 
              saga::url::unescape(ld.get_url().get_path()));
          return 0;
      }
      template <typename Error>
      static int test(test_logical_dir::api::PlainSync t, Error e, test_logical_dir::api::equipment equipment)
      {
          saga::session s = saga::get_default_session();
          int mode = saga::replica::Create |
                     saga::replica::CreateParents |
                     saga::replica::Read;
          saga::replica::logical_directory d(s, equipment.logical_dir, mode);

          saga::replica::logical_directory ld(d.open_dir(saga::url("./temp"), mode));
          SAGA_CHECK(!d.is_file(ld.get_url()));
          SAGA_CHECK_EQUAL(saga::url::unescape(d.get_url().get_string()) + "temp/", 
              saga::url::unescape(ld.get_url().get_string()));
          return 0;
      }
  }; //open_dir
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  struct open
  {
      typedef boost::mpl::vector_c<int, saga::adaptors::Success
         > error_codes;
      static std::string name;
      template <typename Tag, typename Error>
      static int test(Tag tag, Error e, test_logical_dir::api::equipment equipment)
      {
          saga::session s = saga::get_default_session();
          bool is_file = false;
          int mode = saga::replica::Create |
                     saga::replica::CreateParents |
                     saga::replica::Read;
          saga::task t;
          saga::replica::logical_directory d(s, equipment.logical_dir, mode);
          //add a logical_file
          t = d.open<Tag>(saga::url("./test"), mode);
          verify_created_state<Tag>(t);
          run_check_state<Tag>(t);
          wait_check_state<Tag>(t);
          rethrow_on_error(t);

          saga::replica::logical_file lf(t.get_result<saga::replica::logical_file>());
          is_file = d.is_file(lf.get_url());
          SAGA_CHECK(is_file);
          return 0;
      }
      
      template <typename Error>
      static int test(test_logical_dir::api::PlainSync t, Error e, 
          test_logical_dir::api::equipment equipment)
      {
          saga::session s = saga::get_default_session();
          bool is_file = false;
          int mode = saga::replica::Create |
                     saga::replica::CreateParents |
                     saga::replica::Read;
          saga::replica::logical_directory d(s, equipment.logical_dir, mode);
          //add a logical_file
          saga::replica::logical_file lf(d.open(saga::url("./test"), mode));
          is_file = d.is_file(lf.get_url());
          SAGA_CHECK(is_file);
          return 0;
      }
  }; //open
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  struct find
  {
      typedef boost::mpl::vector_c<int, saga::adaptors::Success
         > error_codes;
      static std::string name;
      template <typename Tag, typename Error>
      static int test(Tag tag, Error e, test_logical_dir::api::equipment equipment)
      {
          return 0;
      }
      
      template <typename Error>
      static int test(test_logical_dir::api::PlainSync t, Error e, 
          test_logical_dir::api::equipment equipment)
      {
          saga::session s = saga::get_default_session();
          std::vector<saga::url> results;
          std::vector<std::string> attr;
          int mode = saga::replica::Create |
                     saga::replica::CreateParents |
                     saga::replica::Read;

          saga::replica::logical_directory d(s, equipment.logical_dir, mode);
          //add a logical_file
          saga::replica::logical_file lf(d.open(saga::url("./test"), mode));
          results = d.find("test", attr);
          SAGA_CHECK(results.size() != 0);

          return 0;
      }
  }; //find
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  namespace all
  {
    class suite : public boost::unit_test_framework::test_suite
    {
      public:
       struct add_tests
       {
           add_tests(suite& outer, test_logical_dir::api::equipment equipment) 
            : outer_(outer), equipment_(equipment) {}

           template<typename Test>
           void operator()(Test)
           {
              outer_.add(
                  SAGA_TEST_CASE(
                      boost::bind(&api::run_test<Test>::execute, equipment_),
                      Test::name.c_str()
                  ), 0, 40); // timeout in sec.
           }
           suite& outer_;
           test_logical_dir::api::equipment equipment_;
       };
       
       suite (saga::test::test_helper_utils * utils) 
        : test_suite ("saga::logical_directory test suite") 
       {
           typedef boost::mpl::vector<is_file, open_dir, open, find
              > apitests;
           test_logical_dir::api::equipment equipment_;
           equipment_.utils = utils;
           boost::mpl::for_each<apitests>(add_tests(*this, equipment_));
       }
    };//end suite for unit tests
    
  }//End namespace all
}//End namespace test_logical_dir
#endif // !SAGA_TEST_LOGICAL_DIR_HPP
