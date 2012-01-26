//  Copyright (c) 2005-2006 Chris Miceli (cmicel1@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(SAGA_TEST_FILE_HPP)
#define SAGA_TEST_FILE_HPP

#include <cctype>

#include <boost/config.hpp>
#include <boost/test/unit_test.hpp>

#include "test_api.hpp"
#include "../test_functions.hpp"

namespace test_file
{
void instantiate_tests(boost::unit_test_framework::test_suite *test_suite_,
                       saga::test::test_helper_utils *utils_,
                       std::string scheme, std::string host);
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
struct get_url
{
   typedef boost::mpl::vector_c<int, saga::adaptors::Success,
      saga::IncorrectState
      > error_codes;
   static std::string name;
   template <typename Tag, typename Error>
   static int test(Tag tag, Error, test_file::api::equipment equipment)
   {
      saga::session s = saga::get_default_session();
      saga::url url;
      saga::task t;
      saga::filesystem::file file(s, equipment.file);
      if(saga::IncorrectState == Error::value)
         file.close();
      t = file.get_url<Tag>();
      verify_created_state<Tag>(t);
      run_check_state<Tag>(t);
      wait_check_state<Tag>(t);
      rethrow_on_error(t);
      url = t.get_result<saga::url>();
      SAGA_CHECK_EQUAL(url.get_string(), equipment.file.get_string());
      return 0;
   }
   template <typename Error>
   static int test(test_file::api::PlainSync t, Error, test_file::api::equipment equipment)
   {
      saga::session s = saga::get_default_session();
      saga::url url;
      saga::filesystem::file file(s, equipment.file);
      if(saga::IncorrectState == Error::value)
         file.close();
      url = file.get_url();
      SAGA_CHECK_EQUAL(url.get_string(), equipment.file.get_string());
      return 0;
   }
}; //get_url
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
struct get_cwd
{
   typedef boost::mpl::vector_c<int, 
          saga::adaptors::Success, 
          saga::IncorrectState
      > error_codes;
   static std::string name;
   template <typename Tag, typename Error>
   static int test(Tag tag, Error, test_file::api::equipment equipment)
   {
      saga::session s = saga::get_default_session();
      saga::url cwd;
      saga::task t;
      saga::url url(equipment.file);

      saga::filesystem::file file(s, url);
      if(saga::IncorrectState == Error::value)
         file.close();
      t = file.get_cwd<Tag>();
      verify_created_state<Tag>(t);
      run_check_state<Tag>(t);
      wait_check_state<Tag>(t);
      rethrow_on_error(t);
      cwd = t.get_result<saga::url>();

      std::string p((boost::filesystem::path(saga::url::unescape(url.get_path()))).branch_path().string());
#if defined(BOOST_WINDOWS)
      // if path starts with a drive letter we must insert a '/'
      if (p.empty() || 
          (p.size() >= 2 && std::isalpha(p[0]) && p[1] == ':'))
      {
          p = "/" + p;
      }
#endif
      std::string path(url.get_scheme() + "://" + url.get_host() + p);
      SAGA_CHECK_EQUAL(saga::url::unescape(cwd.get_string()), path);
      return 0;
   }
   template <typename Error>
   static int test(test_file::api::PlainSync t, Error, test_file::api::equipment equipment)
   {
      saga::session s = saga::get_default_session();
      saga::url cwd;
      saga::url url(equipment.file);
      
      saga::filesystem::file file(s, url);
      if(saga::IncorrectState == Error::value)
         file.close();
      cwd = file.get_cwd();
      std::string p((boost::filesystem::path(saga::url::unescape(url.get_path()))).branch_path().string());
#if defined(BOOST_WINDOWS)
      // if path starts with a drive letter we must insert a '/'
      if (p.empty() || 
          (p.size() >= 2 && std::isalpha(p[0]) && p[1] == ':'))
      {
          p = "/" + p;
      }
#endif
      std::string path(url.get_scheme() + "://" + url.get_host() + p);
      SAGA_CHECK_EQUAL(saga::url::unescape(cwd.get_string()), path);
      return 0;
   }
}; //get_cwd
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
struct get_name
{
   typedef boost::mpl::vector_c<int, 
          saga::adaptors::Success,
          saga::IncorrectState
      > error_codes;
   static std::string name;
   template <typename Tag, typename Error>
   static int test(Tag tag, Error, test_file::api::equipment equipment) 
   {
      saga::session s = saga::get_default_session();
      saga::url name;
      saga::url url(equipment.file);
      saga::task t;

      saga::filesystem::file file(s, url);
      if(saga::IncorrectState == Error::value)
         file.close();
      t = file.get_name<Tag>();
      verify_created_state<Tag>(t);
      run_check_state<Tag>(t);
      wait_check_state<Tag>(t);
      rethrow_on_error(t);
      name = t.get_result<saga::url>();
      SAGA_CHECK_EQUAL(saga::url::unescape(name.get_string()), 
          saga::detail::leaf(boost::filesystem::path(saga::url::unescape(url.get_path()))));
      return 0;
   }
   template <typename Error>
   static int test(test_file::api::PlainSync t, Error, test_file::api::equipment equipment)
   {
      saga::session s = saga::get_default_session();
      saga::url name;
      saga::url url(equipment.file);

      saga::filesystem::file file(s, url);
      if(saga::IncorrectState == Error::value)
         file.close();
      name = file.get_name();
      SAGA_CHECK_EQUAL(saga::url::unescape(name.get_string()), 
          saga::detail::leaf(boost::filesystem::path(saga::url::unescape(url.get_path()))));
      return 0;
   }
}; //get_name
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
struct is_dir
{
   typedef boost::mpl::vector_c<int, 
          saga::adaptors::Success,
          saga::IncorrectState
      > error_codes;
   static std::string name;
   template <typename Tag, typename Error>
   static int test(Tag tag, Error, test_file::api::equipment equipment)
   {  
      saga::session s = saga::get_default_session();
      bool is_dir = true;
      saga::task t;
      saga::filesystem::file file(s, equipment.file);
      if(saga::IncorrectState == Error::value)
         file.close();
      t = file.is_dir<Tag>();
      verify_created_state<Tag>(t);
      run_check_state<Tag>(t);
      wait_check_state<Tag>(t);
      rethrow_on_error(t);
      is_dir = t.get_result<bool>();
      SAGA_CHECK(!is_dir);
      return 0;
   }
   template <typename Error>
   static int test(test_file::api::PlainSync t, Error, test_file::api::equipment equipment)
   {
      saga::session s = saga::get_default_session();
      bool is_dir = false;
      saga::filesystem::file file(s, equipment.file);
      if(saga::IncorrectState == Error::value)
         file.close();
      is_dir = file.is_dir();
      SAGA_CHECK(!is_dir);
      return 0;
   }
}; //is_dir
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
struct is_entry 
{
   typedef boost::mpl::vector_c<int, 
          saga::adaptors::Success,
          saga::IncorrectState
      > error_codes;
   static std::string name;
   template <typename Tag, typename Error>
   static int test(Tag tag, Error, test_file::api::equipment equipment)
   {
      saga::session s = saga::get_default_session();
      bool is_entry = false;
      saga::task t;
      saga::filesystem::file file(s, equipment.file);
      if(saga::IncorrectState == Error::value)
          file.close();
      t = file.is_entry<Tag>();
      verify_created_state<Tag>(t);
      run_check_state<Tag>(t);
      wait_check_state<Tag>(t);
      rethrow_on_error(t);
      is_entry = t.get_result<bool>();
      SAGA_CHECK(is_entry);
      return 0;
   }
   template <typename Error>
   static int test(test_file::api::PlainSync t, Error, test_file::api::equipment equipment)
   {
      saga::session s = saga::get_default_session();
      bool is_entry = false;
      saga::filesystem::file file(s, equipment.file);
      if(saga::IncorrectState == Error::value)
          file.close();
      is_entry = file.is_entry();
      SAGA_CHECK(is_entry);
      return 0;
   }
}; //is_entry
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
#if BOOST_VERSION >= 103400
struct is_link //original takes parameter
{
   typedef boost::mpl::vector_c<int, 
          saga::adaptors::Success,
          saga::IncorrectState
      > error_codes;
   static std::string name;
   template <typename Tag, typename Error>
   static int test(Tag tag, Error, test_file::api::equipment equipment)
   {
      saga::session s = saga::get_default_session();
      bool is_link = true;
      saga::task t;
      saga::filesystem::file file(s, equipment.file);
      if(saga::IncorrectState == Error::value)
          file.close();
      t = file.is_link<Tag>();
      verify_created_state<Tag>(t);
      run_check_state<Tag>(t);
      wait_check_state<Tag>(t);
      rethrow_on_error(t);
      is_link = t.get_result<bool>();
      SAGA_CHECK(!is_link);
      return 0;
   }
   template <typename Error>
   static int test(test_file::api::PlainSync t, Error, test_file::api::equipment equipment)
   {
      saga::session s = saga::get_default_session();
      bool is_link = true;
      saga::filesystem::file file(s, equipment.file);
      if(saga::IncorrectState == Error::value)
          file.close();
      is_link = file.is_link();
      SAGA_CHECK(!is_link);
      return 0;
   }
}; //is_link
#endif //  BOOST_VERSION >= 103400
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
struct read_write
{
   typedef boost::mpl::vector_c<int, 
        saga::adaptors::Success,
//         saga::PermissionDenied,
        saga::IncorrectState
      > error_codes;
   static std::string name;
   template <typename Tag, typename Error>
   static int test(Tag tag, Error, test_file::api::equipment equipment)
   {
      saga::session s = saga::get_default_session();
      char txt[] = "Some test string";
      char inbuff[64];
      saga::ssize_t writtenbytes = 0;
      saga::off_t   offset = 0;
      saga::ssize_t readbytes = 0;
      saga::off_t file_size = 0;
      int mode = saga::filesystem::ReadWrite;

      if(saga::PermissionDenied == Error::value)
          mode = saga::filesystem::Read;
      saga::filesystem::file file(s, equipment.file, mode);
      if(saga::IncorrectState == Error::value)
         file.close();
      saga::task t1=file.write<Tag>(saga::buffer(txt, sizeof(txt)));
      verify_created_state<Tag>(t1);
      run_check_state<Tag>(t1);
      wait_check_state<Tag>(t1);
      rethrow_on_error(t1);
      writtenbytes = t1.get_result<saga::ssize_t>();
      
      saga::task t2=file.seek<Tag>(0, saga::filesystem::Start);
      verify_created_state<Tag>(t2);
      run_check_state<Tag>(t2);
      wait_check_state<Tag>(t2);
      offset = t2.get_result<saga::off_t>();

      saga::task t3=file.read<Tag>(saga::buffer(inbuff, sizeof(inbuff)));
      verify_created_state<Tag>(t3);
      run_check_state<Tag>(t3);
      wait_check_state<Tag>(t3);
      readbytes = t3.get_result<saga::ssize_t>();
      
      saga::task t4=file.get_size<Tag>();
      verify_created_state<Tag>(t4);
      run_check_state<Tag>(t4);
      wait_check_state<Tag>(t4);
      file_size = t4.get_result<saga::off_t>();
      
      SAGA_CHECK_EQUAL(ssize_t(sizeof(txt)), writtenbytes);
      SAGA_CHECK_EQUAL(0, offset);
      SAGA_CHECK_EQUAL(ssize_t(sizeof(txt)), readbytes);
//      SAGA_CHECK(!memcmp(inbuff, txt, sizeof(txt)));
      SAGA_CHECK_EQUAL(off_t(sizeof(txt)), file_size);
      return 0;
   }
   template <typename Error>
   static int test(test_file::api::PlainSync t, Error, test_file::api::equipment equipment)
   {
      saga::session s = saga::get_default_session();
      char txt[] = "Some test string";
      char inbuff[64];
      int mode = saga::filesystem::ReadWrite | saga::filesystem::Create;

      if(saga::PermissionDenied == Error::value)
          mode = saga::filesystem::Read;
      saga::filesystem::file file(s, equipment.file, mode);
      if(saga::IncorrectState == Error::value)
         file.close();
      SAGA_CHECK_EQUAL(ssize_t(sizeof(txt)), file.write(saga::buffer(txt)));
      SAGA_CHECK_EQUAL(off_t(0), file.seek(0, saga::filesystem::Start));
      SAGA_CHECK_EQUAL(ssize_t(sizeof(txt)), file.read(saga::buffer(inbuff)));
//      SAGA_CHECK(!memcmp(inbuff, txt, sizeof(txt)));
      SAGA_CHECK_EQUAL(off_t(sizeof(txt)), file.get_size());
      return 0;
   }
}; //read_write
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
namespace all
{
    class suite : public boost::unit_test_framework::test_suite
    {
    public:
        struct add_tests
        {
            add_tests(suite& outer, test_file::api::equipment equipment) 
                : outer_(outer), equipment_(equipment) {}

            template<typename Test>
            void operator()(Test)
            {
                outer_.add(
                    SAGA_TEST_CASE(
                        boost::bind(&test_file::api::run_test<Test>::execute, equipment_),
                        Test::name.c_str()
                    ), 0, 40); // timeout in sec.
            }
            suite& outer_;
            test_file::api::equipment equipment_;
        };
        
        suite (saga::test::test_helper_utils * utils,
               std::string scheme, std::string host)
          : test_suite ("saga::filesystem::file test suite")
        {
            typedef boost::mpl::vector<
                get_url,
                get_cwd,
                get_name,
                is_dir, 
                is_entry,
#if BOOST_VERSION >= 103400 
                is_link,
#endif //BOOST_VERSION >= 103400
                read_write
            > apitests;
            
            test_file::api::equipment equipment_;
            equipment_.utils  = utils;
            equipment_.scheme = scheme;
            equipment_.host   = host;
            boost::mpl::for_each<apitests>(add_tests(*this, equipment_));
        }
   }; //end suite for unit tests

}}//End namespace test_file::all

#endif // !SAGA_TEST_FILE_HPP
