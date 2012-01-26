//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/test/unit_test.hpp>

#include <saga/saga.hpp>
#include <saga/saga/adaptors/serialization.hpp>

#include "../macros.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace test_directory_serialization {

    ///////////////////////////////////////////////////////////////////////////
    void serialize()
    {
        // create a small test file
        int mode = saga::filesystem::Create | saga::filesystem::ReadWrite;
        saga::filesystem::directory d1 (saga::url("test"), mode);
        std::string url1(d1.get_url().get_string());
        
        // serialize directory
        std::string data1(saga::adaptors::serialize(d1));
        d1.close();
        
        // re-create a saga::directory 
        saga::filesystem::directory d2 (saga::adaptors::deserialize(data1));
        BOOST_CHECK_EQUAL(url1, d2.get_url());
        
        std::string data2(saga::adaptors::serialize(d2));
        BOOST_CHECK_EQUAL(data1, data2);
        d2.close();
        
        boost::filesystem::remove_all("test");
    }

    ///////////////////////////////////////////////////////////////////////////
    class suite : public boost::unit_test_framework::test_suite
    {
    public:
        suite () 
        : boost::unit_test_framework::test_suite("directory serialization test suite")
        {
            add (SAGA_TEST_CASE (&serialize, "filesystem directory serialization"));
        }
    };

///////////////////////////////////////////////////////////////////////////////
}   //  namespace test_directory_serialization


