//  Copyright (c) 2005-2006 Chris Miceli (cmicel1@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(SAGA_TEST_DIR_HPP)
#define SAGA_TEST_DIR_HPP

#include <boost/test/unit_test.hpp>

#include "test_api.hpp"
#include "../test_functions.hpp"

namespace test_dir
{
void instantiate_tests(boost::unit_test_framework::test_suite *test_suite_,
                       saga::test::test_helper_utils *utils_);
struct get_url
{
   typedef boost::mpl::vector_c<int, saga::adaptors::Success,
      saga::IncorrectState
      > error_codes;
   static std::string name;
   template <typename Tag, typename Error>
   static int test(Tag tag, Error, test_dir::api::equipment equipment)
   {
      saga::session s = saga::get_default_session();
      saga::url url;
      saga::task t;
      saga::filesystem::directory d(s, equipment.dir);
      if(saga::IncorrectState == Error::value)
         d.close();
      t = d.get_url<Tag>();
      verify_created_state<Tag>(t);
      run_check_state<Tag>(t);
      wait_check_state<Tag>(t);
      rethrow_on_error(t);
      url = t.get_result<saga::url>();
      SAGA_CHECK_EQUAL(url.get_string(), equipment.dir.get_string() + "/");
      return 0;
   }
   template <typename Error>
   static int test(test_dir::api::PlainSync t, Error, test_dir::api::equipment equipment)
   {
      saga::session s = saga::get_default_session();
      saga::url url;
      saga::filesystem::directory d(s, equipment.dir);
      if(saga::IncorrectState == Error::value)
         d.close();
      url = d.get_url();
      SAGA_CHECK_EQUAL(url.get_string(), equipment.dir.get_string() + "/");
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
   static int test(Tag tag, Error, test_dir::api::equipment equipment)
   {
      saga::session s = saga::get_default_session();
      saga::url cwd, name;
      saga::task t;
      saga::filesystem::directory d(s, equipment.dir);
      if(saga::IncorrectState == Error::value)
         d.close();
      t = d.get_cwd<Tag>();
      verify_created_state<Tag>(t);
      run_check_state<Tag>(t);
      wait_check_state<Tag>(t);
      rethrow_on_error(t);
      cwd  = t.get_result<saga::url>();
      name = d.get_name();
      SAGA_CHECK_EQUAL(cwd.get_string() + '/' + name.get_string(), equipment.dir.get_string());
      return 0;
   }
   template <typename Error>
   static int test(test_dir::api::PlainSync t, Error, test_dir::api::equipment equipment)
   {
      saga::session s = saga::get_default_session();
      saga::url cwd, name;
      saga::filesystem::directory d(s, equipment.dir);
      if(saga::IncorrectState == Error::value)
         d.close();
      cwd  = d.get_cwd();
      name = d.get_name();
      SAGA_CHECK_EQUAL(cwd.get_string() + '/' + name.get_string(), equipment.dir.get_string());
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
   static int test(Tag tag, Error, test_dir::api::equipment equipment)
   {
      saga::session s = saga::get_default_session();
      saga::url name;
      saga::url url(equipment.dir);
      saga::task t;

      saga::filesystem::directory d(s, equipment.dir);
      if(saga::IncorrectState == Error::value)
         d.close();
      t = d.get_name<Tag>();
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
   static int test(test_dir::api::PlainSync t, Error, test_dir::api::equipment equipment)
   {
      saga::session s = saga::get_default_session();
      saga::url name;
      saga::url url(equipment.dir);
      saga::filesystem::directory d(s, equipment.dir);
      if(saga::IncorrectState == Error::value)
         d.close();
      name = d.get_name();
      SAGA_CHECK_EQUAL(saga::url::unescape(name.get_string()), 
          saga::detail::leaf(boost::filesystem::path(saga::url::unescape(url.get_path()))));
      return 0;
   }
}; //get_name

template <class T>
struct exists
{
   typedef boost::mpl::vector_c<int, 
            saga::adaptors::Success,
//             saga::IncorrectURL,
            saga::BadParameter,
            saga::IncorrectState
      > error_codes;
   static std::string name;
   template <typename Tag, typename Error>
   static int test(Tag tag, Error, test_dir::api::equipment equipment)
   {
      saga::session s = saga::get_default_session();
      bool existence;
      saga::filesystem::directory d(s,  equipment.dir);
      saga::task t;
      if(T::value && saga::IncorrectURL != Error::value)
         d.make_dir(saga::url("./temp"), saga::name_space::CreateParents|saga::name_space::Overwrite);
      if(saga::IncorrectState == Error::value)
         d.close();
      t = d.exists<Tag>(saga::url("./temp"));
      verify_created_state<Tag>(t);
      run_check_state<Tag>(t);
      wait_check_state<Tag>(t);
      rethrow_on_error(t);
      existence = t.get_result<bool>();
      SAGA_CHECK_EQUAL(existence, T::value);
      return 0;
   }
   template <typename Error>
   static int test(test_dir::api::PlainSync t, Error, test_dir::api::equipment equipment)
   {
      saga::session s = saga::get_default_session();
      bool existence;
      saga::filesystem::directory d(s, equipment.dir);
      if(T::value && saga::IncorrectURL != Error::value)
         d.make_dir(saga::url("./temp"), saga::name_space::CreateParents|saga::name_space::Overwrite);
      if(saga::IncorrectState == Error::value)
         d.close();
      existence = d.exists(saga::url("./temp"));
      SAGA_CHECK_EQUAL(existence, T::value);
      return 0;
   }
}; //exists
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
struct make_dir
{
   typedef boost::mpl::vector_c<int, saga::adaptors::Success,
           /*saga::IncorrectURL,*/ 
           saga::BadParameter,
           saga::AlreadyExists,
           saga::DoesNotExist,
           saga::IncorrectState
      > error_codes;
   static std::string name;
   template <typename Tag, typename Error>
   static int test(Tag tag, Error, test_dir::api::equipment equipment)
   {
      saga::session s = saga::get_default_session();
      saga::filesystem::directory d(s, equipment.dir);
      saga::task t;
      
      if(saga::IncorrectState == Error::value)
         d.close();
      if(saga::AlreadyExists == Error::value) {
         d.make_dir(saga::url("./temp"));
         t = d.make_dir<Tag>(saga::url("./temp"), saga::name_space::Exclusive);}
      if(saga::BadParameter == Error::value)
         t = d.make_dir<Tag>(saga::url("./temp"), saga::name_space::Unknown);
      else 
         t = d.make_dir<Tag>(saga::url("./temp"));
      verify_created_state<Tag>(t);
      run_check_state<Tag>(t);
      wait_check_state<Tag>(t);
      rethrow_on_error(t);
      SAGA_CHECK(d.is_dir(saga::url("./temp")));
      t = d.make_dir<Tag>(saga::url(equipment.dir.get_string() + "/temp_abs"));
      verify_created_state<Tag>(t);
      run_check_state<Tag>(t);
      wait_check_state<Tag>(t);
      t.get_result();
      SAGA_CHECK(d.is_dir(saga::url("./temp_abs"))); 
      return 0;
   }
   template <typename Error>
   static int test(test_dir::api::PlainSync t, Error, test_dir::api::equipment equipment)
   {
      saga::session s = saga::get_default_session();
      saga::filesystem::directory d(s, equipment.dir);
      if(saga::IncorrectState == Error::value)
         d.close();
      if(saga::AlreadyExists == Error::value) {
         d.make_dir(saga::url("./temp"));
         d.make_dir(saga::url("./temp"), saga::name_space::Exclusive);}
      if(saga::BadParameter == Error::value)
         d.make_dir(saga::url("./temp"), saga::name_space::Unknown);
      else 
         d.make_dir(saga::url("./temp"));
      SAGA_CHECK(d.is_dir(saga::url("./temp")));
      d.make_dir(saga::url(equipment.dir.get_string() + "/temp_abs"));
      SAGA_CHECK(d.is_dir(saga::url("./temp_abs")));
      return 0;
   }
}; //make_dir
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
struct is_dir
{
   typedef boost::mpl::vector_c<int, saga::adaptors::Success,
           //saga::IncorrectURL,
           saga::BadParameter,
           saga::DoesNotExist,
           saga::IncorrectState
      > error_codes;
   static std::string name;
   template <typename Tag, typename Error>
   static int test(Tag tag, Error, test_dir::api::equipment equipment)
   {
      saga::session s = saga::get_default_session();
      bool is_dir;
      saga::filesystem::directory d(s, equipment.dir);
      saga::task t;
      d.make_dir(saga::url("./temp"));
      if(saga::IncorrectState == Error::value)
         d.close();
      t = d.is_dir<Tag>(saga::url("./temp"));
      verify_created_state<Tag>(t);
      run_check_state<Tag>(t);
      wait_check_state<Tag>(t);
      rethrow_on_error(t);
      is_dir = t.get_result<bool>();
      SAGA_CHECK(is_dir);
      return 0;
   }
   template <typename Error>
   static int test(test_dir::api::PlainSync t, Error, test_dir::api::equipment equipment)
   {
      saga::session s = saga::get_default_session();
      bool is_dir;
      saga::filesystem::directory d(s, equipment.dir);
      d.make_dir(saga::url("./temp"));
      if(saga::IncorrectState == Error::value)
          d.close();
      is_dir = d.is_dir(saga::url("./temp"));
      SAGA_CHECK(is_dir);
      return 0;
   }
}; //is_dir
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
template <typename T>
struct is_entry
{
   typedef boost::mpl::vector_c<int, 
          saga::adaptors::Success,
//           saga::IncorrectURL//,
          saga::BadParameter,
          saga::DoesNotExist,
          saga::IncorrectState
      > error_codes;
   static std::string name;
   template <typename Tag, typename Error>
   static int test(Tag tag, Error, test_dir::api::equipment equipment)
   {
      saga::session s = saga::get_default_session();
      bool is_entry;
      saga::filesystem::directory d(s, equipment.dir);
      saga::task t;
      
      if(T::value && saga::IncorrectURL != Error::value)
         d.make_dir(saga::url("./temp"));
      if(saga::IncorrectState == Error::value)
         d.close();
      t = d.is_entry<Tag>(saga::url("./temp"));
      verify_created_state<Tag>(t);
      run_check_state<Tag>(t);
      wait_check_state<Tag>(t);
      rethrow_on_error(t);
      is_entry = t.get_result<bool>();
      SAGA_CHECK_EQUAL(is_entry, false);
      return 0;
   }
   template <typename Error>
   static int test(test_dir::api::PlainSync t, Error, test_dir::api::equipment equipment)
   {
      saga::session s = saga::get_default_session();
      bool is_entry;
      saga::filesystem::directory d(s, equipment.dir);
      if(T::value && saga::IncorrectURL != Error::value)
         d.make_dir(saga::url("./temp"));
      if(saga::IncorrectState == Error::value)
         d.close();
      is_entry = d.is_entry(saga::url("./temp"));
      SAGA_CHECK_EQUAL(is_entry, false);
      return 0;
   }
}; //is_entry
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
 #if BOOST_VERSION >= 103400
 struct is_link
 {
    typedef boost::mpl::vector_c<int, saga::adaptors::Success,
        saga::IncorrectState, saga::BadParameter,
        saga::DoesNotExist,
        saga::IncorrectURL
       > error_codes;
    static std::string name;
    template <typename Tag, typename Error>
    static int test(Tag tag, Error, test_dir::api::equipment equipment)
    {
      saga::session s = saga::get_default_session();
       bool is_link;
       saga::task t;
       saga::filesystem::directory d(s, equipment.dir);
       d.make_dir(saga::url("./test"));
       t = d.is_link<Tag>(saga::url("./test"));
       verify_created_state<Tag>(t);
       run_check_state<Tag>(t);
       wait_check_state<Tag>(t);
       rethrow_on_error(t);
       is_link = t.get_result<bool>();
       SAGA_CHECK(is_link);
       return 0;
    }
    template <typename Error>
    static int test(test_dir::api::PlainSync t, Error, test_dir::api::equipment equipment)
    {
      saga::session s = saga::get_default_session();
       bool is_link;
       saga::filesystem::directory d(s, equipment.dir);
       d.make_dir(saga::url("./test"));
       is_link = d.is_link(saga::url("./test"));
       SAGA_CHECK(is_link);
       return 0;
    }
 }; //is_link
#endif
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
struct change_dir
{
   typedef boost::mpl::vector_c<int, saga::adaptors::Success,
       saga::IncorrectState, saga::BadParameter,
       saga::DoesNotExist//,
       /*saga::IncorrectURL*/
      > error_codes;
   static std::string name;
   template <typename Tag, typename Error>
   static int test(Tag tag, Error, test_dir::api::equipment equipment)
   {
      saga::session s = saga::get_default_session();
      saga::filesystem::directory d(s, equipment.dir);
      saga::task t;
      
      if(saga::IncorrectURL != Error::value)
         d.make_dir(saga::url("./temp/"), saga::name_space::CreateParents|saga::name_space::Overwrite);
      if(saga::IncorrectState == Error::value)
         d.close();
      //Relative test
      t = d.change_dir<Tag>(saga::url("./temp/"));
      verify_created_state<Tag>(t);
      run_check_state<Tag>(t);
      wait_check_state<Tag>(t);
      rethrow_on_error(t);
      SAGA_CHECK_EQUAL(d.get_url().get_string(), saga::url(equipment.dir.get_string()+"/temp/"));
      //Absolute test
      t = d.change_dir<Tag>(saga::url(equipment.dir.get_string()+"/temp/"));
      verify_created_state<Tag>(t);
      run_check_state<Tag>(t);
      wait_check_state<Tag>(t);
      SAGA_CHECK_EQUAL(d.get_url(), saga::url(equipment.dir.get_string()+"/temp/"));
      return 0;
   }
   template <typename Error>
   static int test(test_dir::api::PlainSync t, Error, test_dir::api::equipment equipment)
   {
      saga::session s = saga::get_default_session();
      saga::filesystem::directory d(s, equipment.dir);
      if(saga::IncorrectURL != Error::value)
         d.make_dir(saga::url("./temp"), saga::name_space::CreateParents|saga::name_space::Overwrite);
      if(saga::IncorrectState == Error::value)
         d.close();
      //Relative test
      d.change_dir(saga::url("./temp/"));
      SAGA_CHECK_EQUAL(d.get_url(), saga::url(equipment.dir.get_string()+"/temp/"));
      //Absolute test
      d.change_dir(equipment.dir.get_string()+"/temp/");
      SAGA_CHECK_EQUAL(d.get_url(), saga::url(equipment.dir.get_string()+"/temp/"));
      return 0;
   }
}; //change_dir
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
struct move
{
   typedef boost::mpl::vector_c<int, 
         saga::adaptors::Success,
         saga::AlreadyExists, 
         saga::BadParameter,
         saga::IncorrectState
         // saga::DoesNotExist,
         // saga::IncorrectURL
      > error_codes;

    static std::string name;

    template <typename Error>
    struct error_checker
    {
        error_checker(saga::session & session,
                saga::filesystem::directory & dir)
            : s(session)
            , d(dir)
        {
        }

        ~error_checker()
        {
            try {
                if (saga::adaptors::Success == Error::value)
                {
                    SAGA_CHECK(!d.exists(saga::url("./src")));
                    SAGA_CHECK( d.exists(saga::url("./dst")));

                    saga::filesystem::directory dst(s,
                            saga::url(d.get_url().get_string() + "/dst"));

                    SAGA_CHECK(dst.is_dir());
                }
                else if (saga::BadParameter != Error::value) {
                    SAGA_CHECK( d.exists(saga::url("./src")));

                    if (saga::AlreadyExists != Error::value)
                        SAGA_CHECK(!d.exists(saga::url("./dst")));
                }
            }
            catch (saga::exception const& e) {
                if (e.get_error() != Error::value) {
                    std::string msg("caught saga::exception in dir::move::error_checker: ");
                    msg += e.what();
                    SAGA_CHECK_MESSAGE(false, msg);
                }
            }
            catch (std::exception const& e) {
                std::string msg("caught std::exception in dir::move::error_checker: ");
                msg += e.what();
                SAGA_CHECK_MESSAGE(false, msg);
            }
            catch (...) {
                std::string msg("caught unexpected exception in dir::move::error_checker");
                SAGA_CHECK_MESSAGE(false, msg);
            }
        }

        saga::session & s;
        saga::filesystem::directory & d;
    };

    template <typename Tag, typename Error>
    static int test(Tag tag, Error, test_dir::api::equipment equipment)
    {
        saga::session s = saga::get_default_session();
        saga::task t;
        int mode = saga::filesystem::CreateParents |
                 saga::filesystem::Create;
        saga::filesystem::directory d(s, equipment.dir, mode);
        d.make_dir(saga::url("./src"), mode);

        if (saga::IncorrectState == Error::value)
            d.close();
        else if (saga::AlreadyExists == Error::value)
        {
            d.make_dir(saga::url("./dst"));
            mode |= saga::name_space::Exclusive;
        }
        else if (saga::BadParameter == Error::value)
            mode = saga::name_space::Unknown;

        error_checker<Error> on_return(s, d);

        t = d.move<Tag>(saga::url("./src"), saga::url("./dst"), mode);

        verify_created_state<Tag>(t);
        run_check_state<Tag>(t);
        wait_check_state<Tag>(t);
        rethrow_on_error(t);

        return 0;
    }

    template <typename Error>
    static int test(test_dir::api::PlainSync t, Error, test_dir::api::equipment equipment)
    {
        saga::session s = saga::get_default_session();
        int mode = saga::filesystem::CreateParents |
                   saga::filesystem::Create;

        saga::filesystem::directory d(s, equipment.dir, mode);
        d.make_dir(saga::url("./src"), mode);

        if (saga::IncorrectState == Error::value)
            d.close();
        else if (saga::AlreadyExists == Error::value)
        {
            d.make_dir(saga::url("./dst"));
            mode |= saga::name_space::Exclusive;
        }
        else if (saga::BadParameter == Error::value)
            mode = saga::name_space::Unknown;

        error_checker<Error> on_return(s, d);

        d.move(saga::url("./src"), saga::url("./dst"), mode);

        return 0;
    }
}; //move
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
struct copy
{
   typedef boost::mpl::vector_c<int, 
          saga::adaptors::Success,
          saga::AlreadyExists, 
          saga::BadParameter,
          saga::IncorrectState,
          saga::DoesNotExist//, saga::IncorrectURL
      > error_codes;
   static std::string name;
   template <typename Tag, typename Error>
   static int test(Tag tag, Error, test_dir::api::equipment equipment)
   {
      saga::session s = saga::get_default_session();
      saga::task t;
      int mode = saga::filesystem::CreateParents |
                 saga::filesystem::Overwrite     |
                 saga::filesystem::Create;
      
      saga::filesystem::directory d(s, equipment.dir, mode);
      d.make_dir(saga::url("./src"), mode);
      if(saga::IncorrectState == Error::value)
          d.close();
      if(saga::AlreadyExists == Error::value) {
          d.make_dir(saga::url("./dst"), mode);
          t = d.copy<Tag>(saga::url("./src"), saga::url("./dst"), mode | saga::name_space::Exclusive);}
      else if(saga::BadParameter == Error::value)
          t = d.copy<Tag>(saga::url("./src"), saga::url("./dst"), saga::name_space::Unknown);
      else
          t = d.copy<Tag>(saga::url("./src"), saga::url("./dst"), mode);
      verify_created_state<Tag>(t);
      run_check_state<Tag>(t);
      wait_check_state<Tag>(t);
      rethrow_on_error(t);

      saga::filesystem::directory dst(s, saga::url(d.get_url().get_string()+"/dst"));
      SAGA_CHECK(d.exists(saga::url("./src")));
      SAGA_CHECK(d.exists(saga::url("./dst")));
      SAGA_CHECK(dst.is_dir());

      return 0;
   }
   template <typename Error>
   static int test(test_dir::api::PlainSync t, Error, test_dir::api::equipment equipment)
   {
      saga::session s = saga::get_default_session();
      int mode = saga::filesystem::CreateParents |
                 saga::filesystem::Overwrite     |
                 saga::filesystem::Create;
      
      saga::filesystem::directory d(s, equipment.dir, mode);
      d.make_dir(saga::url("./src"), mode);
      if(saga::IncorrectState == Error::value)
          d.close();
      if(saga::AlreadyExists == Error::value) {
          d.make_dir(saga::url("./dst"), mode);
          d.copy(saga::url("./src"), saga::url("./dst"), mode | saga::name_space::Exclusive);}
      else if(saga::BadParameter == Error::value)
          d.copy(saga::url("./src"), saga::url("./dst"), saga::name_space::Unknown);
      else
          d.copy(saga::url("./src"), saga::url("./dst"), mode);
      saga::filesystem::directory dst(s, saga::url(d.get_url().get_string()+"/dst"));
      SAGA_CHECK(dst.is_dir());
      return 0;
   }
}; //copy
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
struct remove
{
   typedef boost::mpl::vector_c<int, 
          saga::adaptors::Success,
          saga::BadParameter, 
          saga::IncorrectState,
          saga::DoesNotExist
      > error_codes;
   static std::string name;
   template <typename Tag, typename Error>
   static int test(Tag tag, Error, test_dir::api::equipment equipment)
   {
      saga::session s = saga::get_default_session();
      saga::filesystem::directory d(s, equipment.dir);
      saga::task t;
      
      d.make_dir(saga::url("./temp"), saga::filesystem::Create);
      SAGA_CHECK(d.exists(saga::url("./temp")));
      if(saga::IncorrectState == Error::value)
         d.close();
      t = d.remove<Tag>(saga::url("./temp"));
      verify_created_state<Tag>(t);
      run_check_state<Tag>(t);
      wait_check_state<Tag>(t);
      rethrow_on_error(t);

      SAGA_CHECK(!d.exists(saga::url("./temp")));
      return 0;
   }
   template <typename Error>
   static int test(test_dir::api::PlainSync t, Error, test_dir::api::equipment equipment)
   {
      saga::session s = saga::get_default_session();
      saga::filesystem::directory d(s, equipment.dir);

      d.make_dir(saga::url("./temp"), saga::filesystem::Create);
      SAGA_CHECK(d.exists(saga::url("./temp")));
      if(saga::IncorrectState == Error::value)
         d.close();
      d.remove(saga::url("./temp"));
      SAGA_CHECK(!d.exists(saga::url("./temp")));
      return 0;
   }
}; //remove
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
struct open
{
   typedef boost::mpl::vector_c<int, saga::adaptors::Success,
       saga::BadParameter, saga::IncorrectState,
       saga::AlreadyExists,
       saga::DoesNotExist
      > error_codes;
   static std::string name;
   template <typename Tag, typename Error>
   static int test(Tag tag, Error, test_dir::api::equipment equipment)
   {
      saga::session s = saga::get_default_session();
      saga::filesystem::directory d(s, equipment.dir);
      saga::task t;
      saga::filesystem::file newfile;
      int mode = saga::filesystem::ReadWrite;
      
      if(saga::IncorrectState == Error::value)
         d.close();
      if(saga::DoesNotExist != Error::value)
         mode = mode | saga::filesystem::Create;


      if(saga::AlreadyExists == Error::value){
         saga::filesystem::file f(saga::url(equipment.dir.get_string() + "/file.txt"), mode);
         t = d.open<Tag>(saga::url("./file.txt"), mode|saga::filesystem::Exclusive);}
      else if(saga::BadParameter == Error::value)
         t = d.open<Tag>(saga::url("./file.txt"), saga::filesystem::Unknown);
      else
         t = d.open<Tag>(saga::url("./file.txt"), mode);
      verify_created_state<Tag>(t);
      run_check_state<Tag>(t);
      wait_check_state<Tag>(t);
      rethrow_on_error(t);
      newfile = t.get_result<saga::filesystem::file>();

      SAGA_CHECK(newfile.is_entry());
      SAGA_CHECK_EQUAL(newfile.get_url().get_string(), saga::url(equipment.dir.get_string()+"/file.txt"));
      return 0;
   }
   template <typename Error>
   static int test(test_dir::api::PlainSync t, Error, test_dir::api::equipment equipment)
   {
      saga::session s = saga::get_default_session();
      saga::filesystem::directory d(s, equipment.dir);
      saga::filesystem::file newfile;
      int mode = saga::filesystem::ReadWrite;
      
      if(saga::IncorrectState == Error::value)
         d.close();
      if(saga::DoesNotExist != Error::value)
         mode = mode | saga::filesystem::Create;
      if(saga::AlreadyExists == Error::value){
         saga::filesystem::file f(saga::url(equipment.dir.get_string() + "/file.txt"), mode);
         newfile = d.open(saga::url("./file.txt"), mode|saga::filesystem::Exclusive);}
      else if(saga::BadParameter == Error::value)
         newfile = d.open(saga::url("./file.txt"), saga::filesystem::Unknown);
      else
         newfile = d.open(saga::url("./file.txt"), mode);
      SAGA_CHECK(newfile.is_entry());
      SAGA_CHECK_EQUAL(newfile.get_url().get_string(), equipment.dir.get_string()+"/file.txt");
      return 0;

   }
}; //open
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
struct open_dir
{
    typedef boost::mpl::vector_c<int, 
        saga::adaptors::Success,
        saga::BadParameter, 
        saga::IncorrectState,
        saga::AlreadyExists,
        saga::DoesNotExist
    > error_codes;
   static std::string name;
   template <typename Tag, typename Error>
   static int test(Tag tag, Error, test_dir::api::equipment equipment)
   {
      saga::session s = saga::get_default_session();
      saga::filesystem::directory newdir;
      saga::task t;
      int mode =saga::filesystem::ReadWrite;
      
      if(saga::DoesNotExist != Error::value)
         mode = mode | saga::name_space::Create;
      saga::filesystem::directory d(s, equipment.dir, mode);

      if(saga::IncorrectState == Error::value)
         d.close();
      if(saga::AlreadyExists == Error::value) {
         d.make_dir(saga::url("./temp"), mode);
         t = d.open_dir<Tag>(saga::url("./temp"), mode | saga::name_space::Exclusive);
      }
      else if(saga::BadParameter == Error::value) {
         t = d.open_dir<Tag>(saga::url("./temp"), saga::name_space::Unknown);
      }
      else {
         t = d.open_dir<Tag>(saga::url("./temp"), mode);
      }
      verify_created_state<Tag>(t);
      run_check_state<Tag>(t);
      wait_check_state<Tag>(t);
      rethrow_on_error(t);
      newdir = t.get_result<saga::filesystem::directory>();

      SAGA_CHECK(newdir.is_dir());
      SAGA_CHECK_EQUAL(newdir.get_url().get_string(), d.get_url().get_string() + "temp/");
      return 0;
   }
   template <typename Error>
   static int test(test_dir::api::PlainSync t, Error, test_dir::api::equipment equipment)
   {
      saga::session s = saga::get_default_session();
      saga::filesystem::directory newdir;
      int mode = saga::filesystem::ReadWrite;
      
      if(saga::DoesNotExist != Error::value)
         mode = mode | saga::name_space::Create;
      saga::filesystem::directory d(s, equipment.dir, mode);
      if(saga::IncorrectState == Error::value)
         d.close();
      if(saga::AlreadyExists == Error::value) {
         d.make_dir(saga::url("./temp"), mode);
         newdir = d.open_dir(saga::url("./temp"), mode|saga::name_space::Exclusive);
      }
      else if(saga::BadParameter == Error::value) {
         newdir = d.open_dir(saga::url("./temp"), saga::name_space::Unknown);
      }
      else {
         newdir = d.open_dir(saga::url("./temp"), mode);
      }
      SAGA_CHECK(newdir.is_dir());
      SAGA_CHECK_EQUAL(newdir.get_url().get_string(), d.get_url().get_string() + "temp/");
      return 0;

   }
}; //open_dir
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
namespace all
{
   class suite : public boost::unit_test_framework::test_suite
   {
     public:
      struct add_tests
      {
         add_tests(suite& outer, test_dir::api::equipment &equipment)
            : outer_(outer) , equipment_(equipment) {}

         template<typename Test>
         void operator()(Test)
         {
            outer_.add(
                SAGA_TEST_CASE(
                    boost::bind(&test_dir::api::run_test<Test>::execute, equipment_),
                    Test::name.c_str()
                ), 0, 40); // timeout in sec.
         }
         suite& outer_;
         test_dir::api::equipment &equipment_;
      };
      suite (saga::test::test_helper_utils * utils)
        : test_suite ("saga::file directory test suite")
      {
         typedef boost::mpl::vector<
// #if BOOST_VERSION >= 103400
//             is_link,
// #endif
            get_url, 
            get_cwd, 
            get_name,
            exists<boost::mpl::true_>,
            exists<boost::mpl::false_>,
            make_dir, 
            is_dir,
            is_entry<boost::mpl::true_>,
            is_entry<boost::mpl::false_>,
            change_dir, 
            move, 
            copy,
            test_dir::remove, 
            open, 
            open_dir
         > apitests;
         test_dir::api::equipment equipment_;
         equipment_.utils = utils;
         boost::mpl::for_each<apitests>(add_tests(*this, equipment_));
      }
   };
}//End namespace all
}//End namespace test_dir

#endif
