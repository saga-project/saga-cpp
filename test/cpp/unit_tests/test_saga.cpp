//  Copyright (c) 2005-2006 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2006 Chris Miceli (cmicel1@cct.lsu.edu)
//  Copyright (c) 2006 Ole Christian Weidner (oweidner@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>
#include <boost/test/framework.hpp>

#include "test_saga.hpp"
#include "test_init.hpp"

///////////////////////////////////////////////////////////////////////////////
//  global lock for SAGA_CHECK_... macros
boost::mutex test_lock;
///////////////////////////////////////////////////////////////////////////////

#if BOOST_VERSION >= 103400

///////////////////////////////////////////////////////////////////////////////
//  This is the actual test suite entry point
///////////////////////////////////////////////////////////////////////////////
bool init_function()
{
    using namespace boost::unit_test;
    test_suite * suite = BOOST_TEST_SUITE ("SAGA C++ Conformance Tests");
    framework::master_test_suite().add(suite);
   
   std::cout << "SAGA C++ Conformance Tests" << std::endl;
   if(framework::master_test_suite().argc > 1)
   {
      test_init init(suite, framework::master_test_suite().argv[1]);
   }
   else
   {
      // no ini given on command line, use exe name
      boost::filesystem::path p(framework::master_test_suite().argv[0]);
      std::string leaf (saga::detail::leaf(p));
      std::string::size_type pos = leaf.find_last_of(".");
      if (std::string::npos != pos) 
      {
         leaf.replace(pos+1, leaf.size()-pos, "ini");
         p = p.branch_path() / leaf;
      }
      else 
      {
       p = p.branch_path() / (saga::detail::leaf(p) + ".ini");
      }
      test_init init(suite, p.string());
   }
   return true;
}

int main(int argc, char* argv[])
{
    saga::session s = saga::get_default_session();
    return boost::unit_test::unit_test_main(&init_function, argc, argv);
}

#else

///////////////////////////////////////////////////////////////////////////////
//  This is the actual test suite entry point BOOST pre 1.34 
///////////////////////////////////////////////////////////////////////////////
boost::unit_test::test_suite *init_unit_test_suite (int argc, char* argv[])
{
   saga::session s = saga::get_default_session();
   using namespace boost::unit_test;
   test_suite * suite = BOOST_TEST_SUITE ("SAGA C++ Conformance Tests");

   std::cout << "SAGA C++ Conformance Tests" << std::endl;
   if(argc > 1)
   {
      test_init init(suite, argv[1]);
   }
   else
   {
      // no ini given on command line, use exe name
      boost::filesystem::path p(argv[0]);
      std::string leaf (saga::detail::leaf(p));
      std::string::size_type pos = leaf.find_last_of(".");
      if (std::string::npos != pos)
      {
         leaf.replace(pos+1, leaf.size()-pos, "ini");
         p = p.branch_path() / leaf;
      }
      else
      {
       p = p.branch_path() / (saga::detail::leaf(p) + ".ini");
      }
      test_init init(suite, p.string());
   }
   return suite;
}

#endif
