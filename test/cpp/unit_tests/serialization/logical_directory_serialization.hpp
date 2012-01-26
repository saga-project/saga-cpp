//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>

#include <saga/saga.hpp>
#include <saga/saga/adaptors/serialization.hpp>

#include "../macros.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace test_logical_directory_serialization {

    ///////////////////////////////////////////////////////////////////////////
    void serialize()
    {
        // create a small test file
        int mode = saga::replica::CreateParents | saga::replica::ReadWrite;
        saga::replica::logical_directory ld1 (saga::url("/test"), mode);
        std::string url1(ld1.get_url().get_string());
        
        // serialize logical_file
        std::string data1(saga::adaptors::serialize(ld1));
        ld1.close();
        
        // re-create a saga::logical_file 
        saga::replica::logical_directory ld2 (saga::adaptors::deserialize(data1));
        BOOST_CHECK_EQUAL(url1, ld2.get_url().get_string());
        
        std::string data2(saga::adaptors::serialize(ld2));
        BOOST_CHECK_EQUAL(data1, data2);
        ld2.close();
        
        saga::replica::logical_directory ld(saga::url("/"), saga::replica::ReadWrite);
        ld.remove("test", saga::replica::Recursive);
    }

    ///////////////////////////////////////////////////////////////////////////
    class suite : public boost::unit_test_framework::test_suite
    {
    public:
        suite () 
        : boost::unit_test_framework::test_suite("logical_directory serialization test suite")
        {
            add (SAGA_TEST_CASE (&serialize, "replica directory serialization"));
        }
    };

///////////////////////////////////////////////////////////////////////////////
}   //  namespace test_logical_directory_serialization


