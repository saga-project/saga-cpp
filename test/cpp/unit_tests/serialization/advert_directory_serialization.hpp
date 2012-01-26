//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>

#include <saga/saga.hpp>
#include <saga/saga/adaptors/serialization.hpp>

#include "../macros.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace test_advert_directory_serialization {

    ///////////////////////////////////////////////////////////////////////////
    void serialize()
    {
        // create a small test file
        int mode = saga::advert::CreateParents | saga::advert::ReadWrite;
        saga::advert::directory advd1 (saga::url("/test"), mode);
        std::string url1(advd1.get_url().get_string());
        
        // serialize logical_file
        std::string data1(saga::adaptors::serialize(advd1));
        advd1.close();
        
        // re-create a saga::logical_file 
        saga::advert::directory advd2 (saga::adaptors::deserialize(data1));
        BOOST_CHECK_EQUAL(url1, advd2.get_url().get_string());
        
        std::string data2(saga::adaptors::serialize(advd2));
        BOOST_CHECK_EQUAL(data1, data2);
        advd2.close();
        
        saga::advert::directory advd(saga::url("/"), saga::advert::ReadWrite);
        advd.remove("test", saga::advert::Recursive);
    }

    ///////////////////////////////////////////////////////////////////////////
    class suite : public boost::unit_test_framework::test_suite
    {
    public:
        suite () 
        : boost::unit_test_framework::test_suite("advert_directory serialization test suite")
        {
            add (SAGA_TEST_CASE (&serialize, "advert directory serialization"));
        }
    };

///////////////////////////////////////////////////////////////////////////////
}   //  namespace test_advert_directory_serialization


