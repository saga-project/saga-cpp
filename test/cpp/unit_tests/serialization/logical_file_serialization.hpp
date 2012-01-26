//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>

#include <saga/saga.hpp>
#include <saga/saga/adaptors/serialization.hpp>

#include "../macros.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace test_logical_file_serialization {

    ///////////////////////////////////////////////////////////////////////////
    void serialize()
    {
        // create a small test file
        int mode = saga::replica::Create | saga::replica::ReadWrite;
        saga::replica::logical_file lf1 (saga::url("/test.lfn"), mode);
        std::string url1(lf1.get_url().get_string());
        
        // serialize logical_file
        std::string data1(saga::adaptors::serialize(lf1));
        lf1.close();
        
        // re-create a saga::logical_file 
        saga::replica::logical_file lf2 (saga::adaptors::deserialize(data1));
        BOOST_CHECK_EQUAL(url1, lf2.get_url().get_string());
        
        std::string data2(saga::adaptors::serialize(lf2));
        BOOST_CHECK_EQUAL(data1, data2);
        lf2.close();
        
        saga::replica::logical_directory ld(saga::url("/"), mode);
        ld.remove("test.lfn");
    }

    ///////////////////////////////////////////////////////////////////////////
    class suite : public boost::unit_test_framework::test_suite
    {
    public:
        suite () 
        : boost::unit_test_framework::test_suite("logical_file serialization test suite")
        {
            add (SAGA_TEST_CASE (&serialize, "replica entry serialization"));
        }
    };

///////////////////////////////////////////////////////////////////////////////
}   //  namespace test_logical_file_serialization


