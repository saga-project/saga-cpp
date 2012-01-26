//  Copyright (c) 2005-2006 Chris Miceli (cmicel1@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#if !defined(SAGA_TEST_LOGICAL_FILE_HPP)
#define SAGA_TEST_LOGICAL_FILE_HPP

#include <boost/test/unit_test.hpp>
#include "test_api.hpp"
#include "../test_functions.hpp"

namespace test_logical_file
{
  void instantiate_tests(boost::unit_test_framework::test_suite *test_suite_,
                         saga::test::test_helper_utils *utils);
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  struct add_location
  {
      typedef boost::mpl::vector_c<int, saga::adaptors::Success
         > error_codes;
      static std::string name;
      template <typename Tag, typename Error>
      static int test(Tag tag, Error e, test_logical_file::api::equipment equipment)
      {
          saga::session s = saga::get_default_session();
          saga::task t;
          int mode = saga::replica::Create        |
                     saga::replica::CreateParents |
                     saga::replica::ReadWrite;
          saga::replica::logical_file lf(s, equipment.logical_file, mode);
          t = lf.add_location<Tag>(equipment.replica_file);
          verify_created_state<Tag>(t);
          run_check_state<Tag>(t);
          wait_check_state<Tag>(t);
          rethrow_on_error(t);

          std::vector<saga::url> locations(lf.list_locations());
          SAGA_CHECK(locations.end() != std::find(locations.begin(), locations.end(), equipment.replica_file));
          return 0;
      }
      template <typename Error>
      static int test(test_logical_file::api::PlainSync t, Error e, test_logical_file::api::equipment equipment)
      {
          saga::session s = saga::get_default_session();
          int mode = saga::replica::Create        |
                     saga::replica::CreateParents |
                     saga::replica::ReadWrite;
          saga::replica::logical_file lf(s, equipment.logical_file, mode);
          lf.add_location(equipment.replica_file);
          std::vector<saga::url> locations(lf.list_locations());
          SAGA_CHECK(locations.end() != std::find(locations.begin(), locations.end(), equipment.replica_file));
          return 0;
      }
  }; //add_location
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  struct remove_location
  {
      typedef boost::mpl::vector_c<int, saga::adaptors::Success
         > error_codes;
      static std::string name;
      template <typename Tag, typename Error>
      static int test(Tag tag, Error e, test_logical_file::api::equipment equipment)
      {
          saga::session s = saga::get_default_session();
          saga::task t;
          int mode = saga::replica::Create        |
                     saga::replica::CreateParents |
                     saga::replica::ReadWrite;
          saga::replica::logical_file lf(s, equipment.logical_file, mode);
          lf.add_location(equipment.replica_file);
          std::vector<saga::url> locations(lf.list_locations());
          //Verify in location list
          SAGA_CHECK(locations.end() != std::find(locations.begin(), locations.end(), equipment.replica_file));
          t = lf.remove_location<Tag>(equipment.replica_file);
          verify_created_state<Tag>(t);
          run_check_state<Tag>(t);
          wait_check_state<Tag>(t);
          rethrow_on_error(t);

          locations = lf.list_locations();
          SAGA_CHECK(locations.end() == std::find(locations.begin(), locations.end(), equipment.replica_file));
          return 0;
      }
      template <typename Error>
      static int test(test_logical_file::api::PlainSync t, Error e, test_logical_file::api::equipment equipment)
      {
          saga::session s = saga::get_default_session();
          int mode = saga::replica::Create        |
                     saga::replica::CreateParents |
                     saga::replica::ReadWrite;
          saga::replica::logical_file lf(s, equipment.logical_file, mode);
          lf.add_location(equipment.replica_file);
          std::vector<saga::url> locations(lf.list_locations());
          //Verify in location list
          SAGA_CHECK(locations.end() != std::find(locations.begin(), locations.end(), equipment.replica_file));
          lf.remove_location(equipment.replica_file);
          locations = lf.list_locations();
          SAGA_CHECK(locations.end() == std::find(locations.begin(), locations.end(), equipment.replica_file));
          return 0;
      }
  }; //remove_location
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  struct update_location
  {
      typedef boost::mpl::vector_c<int, saga::adaptors::Success
         > error_codes;
      static std::string name;
      template <typename Tag, typename Error>
      static int test(Tag tag, Error e, test_logical_file::api::equipment equipment)
      {
          saga::session s = saga::get_default_session();
          saga::task t;
          int mode = saga::replica::Create        |
                     saga::replica::CreateParents |
                     saga::replica::ReadWrite;
          saga::url urltemp(equipment.replica_file.get_path() + "/temp");
          saga::replica::logical_file lf(s, equipment.logical_file, mode);
          lf.add_location(equipment.replica_file);
          std::vector<saga::url> locations(lf.list_locations());
          //Verify in location list
          SAGA_CHECK(locations.end() != std::find(locations.begin(), locations.end(), equipment.replica_file));
          t = lf.update_location<Tag>(equipment.replica_file, urltemp);
          verify_created_state<Tag>(t);
          run_check_state<Tag>(t);
          wait_check_state<Tag>(t);
          rethrow_on_error(t);

          locations = lf.list_locations();
          //Verify changed location list
          SAGA_CHECK(locations.end() == std::find(locations.begin(), locations.end(), equipment.replica_file));
          SAGA_CHECK(locations.end() != std::find(locations.begin(), locations.end(), urltemp));
          return 0;
      }
      template <typename Error>
      static int test(test_logical_file::api::PlainSync t, Error e, test_logical_file::api::equipment equipment)
      {
          saga::session s = saga::get_default_session();
          int mode = saga::replica::Create        |
                     saga::replica::CreateParents |
                     saga::replica::ReadWrite;
          saga::url urltemp(equipment.replica_file.get_path() + "/temp");
          saga::replica::logical_file lf(s, equipment.logical_file, mode);
          lf.add_location(equipment.replica_file);
          std::vector<saga::url> locations(lf.list_locations());
          //Verify in location list
          SAGA_CHECK(locations.end() != std::find(locations.begin(), locations.end(), equipment.replica_file));
          lf.update_location(equipment.replica_file, urltemp);
          locations = lf.list_locations();
          //Verify changed location list
          SAGA_CHECK(locations.end() == std::find(locations.begin(), locations.end(), equipment.replica_file));
          SAGA_CHECK(locations.end() != std::find(locations.begin(), locations.end(), urltemp));
          return 0;
      }
  }; //update_location
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  struct list_locations
  {
      typedef boost::mpl::vector_c<int, saga::adaptors::Success
         > error_codes;
      static std::string name;
      template <typename Tag, typename Error>
      static int test(Tag tag, Error e, test_logical_file::api::equipment equipment)
      {
          saga::session s = saga::get_default_session();
          saga::task t;
          int mode = saga::replica::Create        |
                     saga::replica::CreateParents |
                     saga::replica::ReadWrite;
          saga::replica::logical_file lf(s, equipment.logical_file, mode);
          //Add some locations
          lf.add_location(saga::url(equipment.replica_file.get_string()+"0"));
          lf.add_location(saga::url(equipment.replica_file.get_string()+"1"));
          lf.add_location(saga::url(equipment.replica_file.get_string()+"2"));
          lf.add_location(saga::url(equipment.replica_file.get_string()+"3"));

          //Verify list_locations reports them
          std::vector<saga::url> locations;
          t = lf.list_locations<Tag>();
          verify_created_state<Tag>(t);
          run_check_state<Tag>(t);
          wait_check_state<Tag>(t);
          rethrow_on_error(t);

          locations = t.get_result<std::vector<saga::url> >();
          SAGA_CHECK(locations.end() != std::find(locations.begin(), locations.end(), equipment.replica_file.get_string()+"0"));
          SAGA_CHECK(locations.end() != std::find(locations.begin(), locations.end(), equipment.replica_file.get_string()+"1"));
          SAGA_CHECK(locations.end() != std::find(locations.begin(), locations.end(), equipment.replica_file.get_string()+"2"));
          SAGA_CHECK(locations.end() != std::find(locations.begin(), locations.end(), equipment.replica_file.get_string()+"3"));
          return 0;
      }
      template <typename Error>
      static int test(test_logical_file::api::PlainSync t, Error e, test_logical_file::api::equipment equipment)
      {
          saga::session s = saga::get_default_session();
          int mode = saga::replica::Create        |
                     saga::replica::CreateParents |
                     saga::replica::ReadWrite;
          saga::replica::logical_file lf(s, equipment.logical_file, mode);
          //Add some locations
          lf.add_location(saga::url(equipment.replica_file.get_string()+"0"));
          lf.add_location(saga::url(equipment.replica_file.get_string()+"1"));
          lf.add_location(saga::url(equipment.replica_file.get_string()+"2"));
          lf.add_location(saga::url(equipment.replica_file.get_string()+"3"));

          //Verify list_locations reports them
          std::vector<saga::url> locations(lf.list_locations());
          SAGA_CHECK(locations.end() != std::find(locations.begin(), locations.end(), equipment.replica_file.get_string()+"0"));
          SAGA_CHECK(locations.end() != std::find(locations.begin(), locations.end(), equipment.replica_file.get_string()+"1"));
          SAGA_CHECK(locations.end() != std::find(locations.begin(), locations.end(), equipment.replica_file.get_string()+"2"));
          SAGA_CHECK(locations.end() != std::find(locations.begin(), locations.end(), equipment.replica_file.get_string()+"3"));
          return 0;
      }
  }; //list_location
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  struct replicate
  {
      typedef boost::mpl::vector_c<int, saga::adaptors::Success
         > error_codes;
      static std::string name;
      template <typename Tag, typename Error>
      static int test(Tag tag, Error e, test_logical_file::api::equipment equipment)
      {
          saga::session s = saga::get_default_session();
          saga::task t;
          int mode = saga::replica::Create        |
                     saga::replica::CreateParents |
                     saga::replica::ReadWrite;
          saga::replica::logical_file lf(s, equipment.logical_file, mode);
//          verify_created_state<Tag>(t);
//          run_check_state<Tag>(t);
//          wait_check_state<Tag>(t);
          std::vector<saga::url> locations(lf.list_locations());
          return 0;
      }
      template <typename Error>
      static int test(test_logical_file::api::PlainSync t, Error e, test_logical_file::api::equipment equipment)
      {
          saga::session s = saga::get_default_session();
          int mode = saga::replica::Create        |
                     saga::replica::CreateParents |
                     saga::replica::ReadWrite;
          saga::replica::logical_file lf(s, equipment.logical_file, mode);
          std::vector<saga::url> locations(lf.list_locations());
          return 0;
      }
  }; //replicate
  ///////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////
  namespace all
  {
    class suite : public boost::unit_test_framework::test_suite
    {
    public:
       struct add_tests
       {
          add_tests(suite& outer, test_logical_file::api::equipment equipment) 
            : outer_(outer), equipment_(equipment) {}

           template<typename Test>
           void operator()(Test)
           {
              outer_.add(
                  SAGA_TEST_CASE(
                      boost::bind(&test_logical_file::api::run_test<Test>::execute, equipment_),
                      Test::name.c_str()
                  ), 0, 40); // timeout in sec.
           }
           suite& outer_;
           test_logical_file::api::equipment equipment_;
       };

       suite (saga::test::test_helper_utils *utils) 
        : test_suite ("saga::replica::logical_file test suite"), utils_(utils)
       {
           typedef boost::mpl::vector<
               add_location, remove_location,
               update_location, list_locations,
               replicate
              > apitests;
           test_logical_file::api::equipment equipment_;
           equipment_.utils = utils_;
           boost::mpl::for_each<apitests>(add_tests(*this, equipment_));
       }
       saga::test::test_helper_utils *utils_;
    }; //end suite for unit tests

  }//End namespace all
}//End namespace test_logical_file
#endif // !SAGA_TEST_LOGICAL_FILE_HPP
