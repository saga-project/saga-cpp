//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>

#include <saga/saga.hpp>
#include <saga/saga/adaptors/serialization.hpp>

#include "../macros.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace test_advert_serialization {

    ///////////////////////////////////////////////////////////////////////////
    void serialize()
    {
        // create a small test file
        int mode = saga::advert::Create | saga::advert::ReadWrite;
        saga::advert::entry adv1 (saga::url("/test.adv"), mode);
        std::string url1(adv1.get_url().get_string());
        
        // serialize logical_file
        std::string data1(saga::adaptors::serialize(adv1));
        adv1.close();
        
        // re-create a saga::logical_file 
        saga::advert::entry adv2 (saga::adaptors::deserialize(data1));
        BOOST_CHECK_EQUAL(url1, adv2.get_url().get_string());
        
        std::string data2(saga::adaptors::serialize(adv2));
        BOOST_CHECK_EQUAL(data1, data2);
        adv2.close();
        
        saga::advert::directory adv(saga::url("/"), mode);
        adv.remove("test.adv");
    }

    ///////////////////////////////////////////////////////////////////////////
    class suite : public boost::unit_test_framework::test_suite
    {
    public:
        suite () 
        : boost::unit_test_framework::test_suite("advert serialization test suite")
        {
            add (SAGA_TEST_CASE (&serialize, "advert entry serialization"));
        }
    };

///////////////////////////////////////////////////////////////////////////////
}   //  namespace test_advert_serialization


