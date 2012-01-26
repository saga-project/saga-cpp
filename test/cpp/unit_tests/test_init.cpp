//  Copyright (c) 2006 Ole Christian Weidner (oweidner@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <string>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

#include <boost/plugin/dll.hpp>
#include <boost/plugin/plugin_factory.hpp>

#include <boost/test/unit_test.hpp>
using boost::unit_test_framework::test_suite;

#include <saga/saga.hpp>
#include <saga/saga/adaptors/utils/utils.hpp>

#include "url/test_url.hpp"

#ifdef SAGA_CHECK_PACKAGE_FILESYSTEM
# include "file/test_file.hpp"
# include "directory/test_directory.hpp"
#endif

#ifdef SAGA_CHECK_PACKAGE_JOB
# include "job/test_job.hpp"
#endif

#ifdef SAGA_CHECK_PACKAGE_REPLICA
# include "logical_file/test_logical_file.hpp"
# include "logical_directory/test_logical_dir.hpp"
#endif

//#include "stream/test_stream.hpp"

#include "test_cleanup.hpp"
#include "test_init.hpp"
#include "test_helper_utils_fallback.hpp"

///////////////////////////////////////////////////////////////////////////////
//
bool test_init::lib_has_test_helper_factory_(std::string& lib_path, 
    std::string adaptor_name, bool retry)
{
  bool retval = false;
  
  SAGA_VERBOSE(SAGA_VERBOSE_LEVEL_INFO) {
    std::cerr << "[INIT] |--> Adaptor supplies test helper functions... "
              << std::flush;
  }
  
  namespace fs = boost::filesystem;
#if BOOST_FILESYSTEM_VERSION == 3
  fs::path lib(lib_path);
#else
  fs::path lib(lib_path, fs::native);
#endif
  std::string libname (SAGA_MANGLE_ADAPTOR_NAME_STR(adaptor_name) + SAGA_SHARED_LIB_EXTENSION);

#if BOOST_FILESYSTEM_VERSION == 3
  lib /= fs::path (libname);
#else
  lib /= fs::path (libname, fs::native);
#endif

  try {
    boost::plugin::dll d(lib.string());
    boost::plugin::plugin_factory <saga::test::test_helper_utils> the_factory (d);
    TR1::shared_ptr<saga::test::test_helper_utils> util(
        the_factory.create ("test_helper_utils")); 
    
    SAGA_VERBOSE(SAGA_VERBOSE_LEVEL_INFO) {
      std::cerr << " YES" << std::endl;
    }
    lib_path = lib.string();
    retval = true;
  }
  catch(std::exception const & e)  
  {
    {
      SAGA_VERBOSE(SAGA_VERBOSE_LEVEL_INFO) {
        std::cerr << " NO" << std::endl;
      }
      std::cerr << e.what() << std::endl << std::flush;
      return false;
    }
  }
  return retval;
}

///////////////////////////////////////////////////////////////////////////////
//
saga::test::test_helper_utils * test_init::init_helper_object_(std::string lib_path)
{
  boost::plugin::dll d (lib_path);
  cleanup_handler_->add_library_handle(d);
  
  boost::plugin::plugin_factory <saga::test::test_helper_utils> the_factory (d);
  
  saga::test::test_helper_utils * thu (the_factory.create ("test_helper_utils")); 
  cleanup_handler_->add_helper_object(thu);
  return thu;
}

///////////////////////////////////////////////////////////////////////////////
//
void test_init::add_url_tests_()
{
  saga::ini::section url_test_section;
  if( config_file_.has_section("url_test") )
  {
    url_test_section = config_file_.get_section("url_test");
    if(url_test_section.has_entry("enabled"))
    {
      SAGA_VERBOSE(SAGA_VERBOSE_LEVEL_INFO) {
         std::cerr << "[INIT]\n[INIT] Processing url_test section..." 
                   << std::endl;
      }
      if(handler_.is_enabled_section(url_test_section))
      {
         // Add URL tests
         cleanup_handler_->add_file(handler_.create_saga_ini_file_("."));
         test_url::instantiate_tests(test_suite_);
      }
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void test_init::add_adaptor_tests_()
{
  saga::ini::section adaptor_test_section;
  cleanup_handler_->add_file(handler_.create_saga_ini_file_("."));

#ifdef SAGA_CHECK_PACKAGE_FILESYSTEM
  /////////////////////////////
  // REGISTER THE FILE TESTS //
  /////////////////////////////
  saga::test::test_helper_utils *file_fallbackutils = NULL;
  saga::test::test_helper_utils *file_utils = NULL;
  if( config_file_.has_section("file_adaptor_test") )
  {
    adaptor_test_section = config_file_.get_section("file_adaptor_test");
    if(handler_.is_valid_section(adaptor_test_section) )
    {
      SAGA_VERBOSE(SAGA_VERBOSE_LEVEL_INFO) {
         std::cerr << "[INIT]\n[INIT] Processing file_adaptor_test section..." 
                   << std::endl;
      }        
      if(handler_.is_enabled_section(adaptor_test_section))
      {
        std::auto_ptr<saga::ini::section> prop (
           handler_.get_section_custom_properties_(adaptor_test_section));
       
        std::string adaptor_name(adaptor_test_section.get_entry("adaptor_name"));
        std::string adaptor_path(adaptor_test_section.get_entry("adaptor_path"));
        std::string enabled_file(adaptor_test_section.get_entry("enabled_file"));
        std::string enabled_dir (adaptor_test_section.get_entry("enabled_dir"));
        //std::string scheme = prop->has_entry("scheme") ? prop->get_entry("scheme") : "file";
        //std::string host   = prop->has_entry("host")   ? prop->get_entry("host")   : "localhost";
        std::string scheme = "file";
        std::string host = "localhost";
        saga::test::test_helper_utils *utils = NULL;
        if(lib_has_test_helper_factory_(adaptor_path, adaptor_name))
        {
          file_utils = init_helper_object_(adaptor_path);
          utils = file_utils;
        }
        else
        {
          std::cerr << "[INIT] Scheme for fallback = " << scheme << std::endl;
          std::cerr << "[INIT] host for fallback = " << host << std::endl;
          file_fallbackutils = new test_helper_utils_fallback(scheme, host);
          utils = file_fallbackutils;
        }
        cleanup_handler_->add_files(handler_.create_adaptor_ini_file_( ".", adaptor_test_section ));
        cleanup_handler_->add_file("saga_replica.db");
        if(enabled_file == "True" || enabled_file == "TRUE" || enabled_file == "true")
        {
          test_file::instantiate_tests(test_suite_, utils, scheme, host);
        }
        if(enabled_dir == "True" || enabled_dir == "TRUE" || enabled_dir == "true")
        {
          test_dir::instantiate_tests(test_suite_, utils);
        }
      }
    }
  }
#endif // SAGA_CHECK_PACKAGE_FILESYSTEM

#ifdef SAGA_CHECK_PACKAGE_REPLICA
  /////////////////////////////////////
  // REGISTER THE REPLICA TESTS      //
  /////////////////////////////////////
  if( config_file_.has_section("file_adaptor_test") )
  {
    saga::test::test_helper_utils *utils = NULL;
    if(config_file_.has_section("replica_adaptor_test"))
    {
      SAGA_VERBOSE(SAGA_VERBOSE_LEVEL_INFO) {
         std::cerr << "[INIT]\n[INIT] Processing replica_adaptor_test section..." 
                   << std::endl;
      }        
      adaptor_test_section = config_file_.get_section("replica_adaptor_test");
      if(handler_.is_valid_section(adaptor_test_section) &&
         handler_.is_enabled_section(adaptor_test_section))
      {
        std::string enabled_file(adaptor_test_section.get_entry("enabled_file"));
        std::string enabled_dir (adaptor_test_section.get_entry("enabled_dir"));
        if(file_fallbackutils == NULL) {
          // if this fires you need to enable the file section 
          BOOST_ASSERT(NULL != file_utils);
          utils = file_utils;
        }
        else {
          utils = file_fallbackutils;
        }
        cleanup_handler_->add_files(handler_.create_adaptor_ini_file_( ".", adaptor_test_section ));
        if(enabled_file == "True" || enabled_file == "TRUE" || enabled_file == "true")
        {
          test_logical_file::instantiate_tests(test_suite_, utils);
        }
        if(enabled_dir == "True" || enabled_dir == "TRUE" || enabled_dir == "true")
        {
          test_logical_dir::instantiate_tests(test_suite_, utils);
        }
      }
    }
  }
#endif // SAGA_CHECK_PACKAGE_REPLICA

#ifdef SAGA_CHECK_PACKAGE_JOB
  ////////////////////////////
  // REGISTER THE JOB TESTS //
  ////////////////////////////
  if(config_file_.has_section("job_adaptor_test"))
  {
    adaptor_test_section = config_file_.get_section("job_adaptor_test");
    if(handler_.is_valid_section(adaptor_test_section) 
       && handler_.is_enabled_section(adaptor_test_section))
    {
        SAGA_VERBOSE(SAGA_VERBOSE_LEVEL_INFO) {
          std::cerr << "[INIT]\n[INIT] Processing job_adaptor_test section..."
                    << std::endl;
        }
        std::auto_ptr<saga::ini::section> prop (
            handler_.get_section_custom_properties_(adaptor_test_section));

        std::string test_helper = prop.get() ? prop->get_entry("test_helper") : "test_helper -i";
        std::string scheme      = prop.get() ? prop->get_entry("scheme") : "any";
        std::string host        = prop.get() ? prop->get_entry("host") : "localhost";

        std::vector<std::string> args = saga::adaptors::utils::split_commandline(test_helper);
        test_helper = args[0];
        args.erase(args.begin());

        cleanup_handler_->add_files(handler_.create_adaptor_ini_file_(".", adaptor_test_section));
        test_job::instantiate_tests(test_suite_, test_helper, scheme, host, args);
    }
  }
#endif // SAGA_CHECK_PACKAGE_STREAM

#if SAGA_CHECK_PACKAGE_STREAM
  ////////////////////////////
  // REGISTER THE STREAM TESTS //
  ////////////////////////////
  if(config_file_.has_section("stream_adaptor_test"))
  {
    adaptor_test_section = config_file_.get_section("stream_adaptor_test");
    if(handler_.is_valid_section(adaptor_test_section) 
        && handler_.is_enabled_section(adaptor_test_section))
    {
        SAGA_VERBOSE(SAGA_VERBOSE_LEVEL_INFO) {
          std::cerr << "[INIT]\n[INIT] Processing stream_adaptor_test section..."
                    << std::endl;
        }
        std::auto_ptr<saga::ini::section> prop (
          handler_.get_section_custom_properties_(adaptor_test_section));
          
        std::string scheme      = prop.get() ? prop->get_entry("scheme") : "any";
        std::string host        = prop.get() ? prop->get_entry("host") : "localhost";
        std::string port        = prop.get() ? prop->get_entry("port") : "90";

        cleanup_handler_->add_files(handler_.create_adaptor_ini_file_( ".", adaptor_test_section ));
//        test_stream::instantiate_tests(test_suite_, scheme, host, port);
    }
  }
#endif // SAGA_CHECK_PACKAGE_STREAM

  ////////////////////////////////////
  ////////////////////////////////////
  // REGISTER THE CLEANUP "HANDLER" //
  ////////////////////////////////////
  SAGA_VERBOSE(SAGA_VERBOSE_LEVEL_INFO) {
     std::cerr << "[INIT] |--> Finished adding tests" << std::endl;
  }
  test_suite_->add (cleanup_handler_);
}

///////////////////////////////////////////////////////////////////////////////
//
test_init::test_init(test_suite* suite, std::string config_file_location)
: test_suite_(suite)
{
  try {
    SAGA_VERBOSE(SAGA_VERBOSE_LEVEL_INFO) {
      std::cerr << "[INIT] Reading config file: " << config_file_location
                << "... " << std::endl << std::flush;
    }
    config_file_.read(config_file_location);
  }
  catch( saga::exception const & e ) {
    std::cerr << "[INIT] Reading config file: ... " << config_file_location;
    std::cerr << " FAILED:" << std::endl << e.what() << std::endl;
    exit(-1);
  }

  if( !config_file_.has_section("test_preferences") )
  {
    std::cerr << "[INIT] Reading config file: ... " << config_file_location;
    std::cerr << "[INIT] section \"test_preferences\" missing" << std::endl;
  }
  else
  {
    saga::ini::section cfg_sec = config_file_.get_section("test_preferences");
    if (cfg_sec.has_entry("verbose"))
      setenv("SAGA_VERBOSE", cfg_sec.get_entry("verbose").c_str(), true);

    SAGA_VERBOSE(SAGA_VERBOSE_LEVEL_INFO) {
      std::cerr << "[INIT] Reading config file: " << config_file_location
                << " OK" << std::endl;
    }
  }

  // Create a cleanup_handler instance
  handler_ = handle_ini();
  pref_file_ = handler_.get_global_ini();
  cleanup_handler_ = new cleanup::suite();
//add_url_tests_();
  add_adaptor_tests_();
}
