//  Copyright (c) 2006 Ole Christian Weidner (oweidner@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_TEST_CLEANUP_HPP
#define SAGA_TEST_CLEANUP_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <boost/config.hpp>
#include <boost/plugin/dll.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/thread.hpp>
#include <boost/test/unit_test.hpp>
#include <sys/types.h>
#include <signal.h>
#include "test_saga.hpp"

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable: 4251 4231 4275 4660)
#endif

namespace cleanup
{
   class suite : public boost::unit_test_framework::test_suite
   {
   private:
      std::vector<boost::filesystem::path> files_;
      std::vector<boost::filesystem::path> directories_;
      std::vector<boost::plugin::dll> library_handles_; 
      boost::ptr_vector<saga::test::test_helper_utils> helper_objects_;

   public:
      suite ();
      ~suite();

      void add_file          (boost::filesystem::path config_file);
      void add_files         (std::vector<boost::filesystem::path> config_files);
      void add_directory     (boost::filesystem::path directory);
      void add_library_handle(boost::plugin::dll dll);
      void add_helper_object (saga::test::test_helper_utils* util);
   };
}

// re-enable warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

#endif //SAGA_TEST_CLEANUP_HPP

