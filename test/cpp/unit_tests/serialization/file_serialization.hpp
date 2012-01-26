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
namespace test_file_serialization {

    ///////////////////////////////////////////////////////////////////////////
    void serialize()
    {
        // create a small test file
        int mode = saga::filesystem::Create | saga::filesystem::ReadWrite;
        saga::filesystem::file f1 (saga::url("test.txt"), mode);
        std::string data("This is a test file!");
        f1.write(saga::buffer(data));
        std::string url1(f1.get_url().get_string());
        
        // serialize file
        std::string data1(saga::adaptors::serialize(f1));
        f1.close();
        
        // re-create a saga::file 
        saga::filesystem::file f2 (saga::adaptors::deserialize(data1));
        BOOST_CHECK_EQUAL(url1, f2.get_url());
        
        // check if position has been re-created correctly
        saga::off_t offset = f2.seek(0, saga::filesystem::Current);
        BOOST_CHECK_EQUAL(offset, (saga::off_t)data.size());
        
        std::string data2(saga::adaptors::serialize(f2));
        BOOST_CHECK_EQUAL(data1, data2);
        f2.close();
        
        boost::filesystem::remove("test.txt");
    }

    ///////////////////////////////////////////////////////////////////////////
    class suite : public boost::unit_test_framework::test_suite
    {
    public:
        suite () 
        : boost::unit_test_framework::test_suite ("file serialization test suite")
        {
            add (SAGA_TEST_CASE (&serialize, "filesystem entry serialization"));
        }
    };

///////////////////////////////////////////////////////////////////////////////
}   //  namespace test_file_serialization


