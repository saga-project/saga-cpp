//  Copyright (c) 2005-2006 Chris Miceli (cmicel1@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(SAGA_TEST_STREAM_HPP)
#define SAGA_TEST_STREAM_HPP

#include <boost/test/unit_test.hpp>
#include "test_api.hpp"
#include "../test_functions.hpp"

namespace test_stream
{
void instantiate_tests(boost::unit_test_framework::test_suite *test_suite_,
                       std::string scheme, std::string host, std::string port);
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
struct get_url
{
   typedef boost::mpl::vector_c<int, saga::adaptors::Success,
      saga::IncorrectState/*, saga::Timeout*/
      > error_codes;
   static std::string name;
   template <typename Tag, typename Error>
   static int test(Tag tag, Error, test_stream::api::equipment equipment)
   {
      saga::session s = saga::get_default_session();
      saga::task t;
      saga::url url;

      saga::stream::stream stream(s, equipment.contact);
      t = stream.get_url<Tag>();
      verify_created_state<Tag>(t);
      run_check_state<Tag>(t);
      wait_check_state<Tag>(t);
      url = t.get_result<saga::url>();
      SAGA_CHECK_EQUAL(url.get_string(), equipment.contact.get_string());
      return 0;
   }
   template <typename Error>
   static int test(test_stream::api::PlainSync t, Error, test_stream::api::equipment equipment)
   {
      saga::session s = saga::get_default_session();
      saga::url url;

      saga::stream::stream stream(s, equipment.contact);
      url = stream.get_url();
      SAGA_CHECK_EQUAL(url.get_string(), equipment.contact.get_string());
      return 0;
   }
}; //get_url
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
namespace all
{
    class suite : public boost::unit_test_framework::test_suite
    {
    public:
        struct add_tests
        {
            add_tests(suite& outer, test_stream::api::equipment equipment) 
                : outer_(outer), equipment_(equipment) {}

            template<typename Test>
            void operator()(Test)
            {
                outer_.add(
                    SAGA_TEST_CASE(
                        boost::bind(&test_stream::api::run_test<Test>::execute, equipment_),
                        Test::name.c_str()
                    ), 0, 40); // timeout in sec.
            }
            suite& outer_;
            test_stream::api::equipment equipment_;
        };
        
        suite (std::string scheme, std::string host, std::string port)
          : test_suite ("saga::stream test suite")
        {
            typedef boost::mpl::vector<get_url
            > apitests;
            test_stream::api::equipment equipment_;
            saga::url contact;
            std::istringstream strin(host);
            int numeric_port;
            strin >> numeric_port;
            contact.set_scheme(scheme);
            contact.set_host(host);
            contact.set_port(numeric_port);
            equipment_.contact = contact;
            boost::mpl::for_each<apitests>(add_tests(*this, equipment_));
        }
   }; //end suite for unit tests

}}//End namespace test_stream::all

#endif // !SAGA_TEST_STREAM_HPP
