//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>
using boost::unit_test_framework::test_suite;

#include "../test_saga.hpp"
#include "test_uuid.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace test_uuid {

    ///////////////////////////////////////////////////////////////////////////
    void uuid_tests()
    {
        saga::uuid u1;
        saga::uuid u2;
        
        BOOST_CHECK(!(u1 == u2));
        BOOST_CHECK(u1 != u2);
        
        BOOST_CHECK(u1.string() != u2.string());
        
        BOOST_CHECK(u1 < u2 || u1 > u2);
        BOOST_CHECK((u1 <= u2 || u1 >= u2) && u1 != u2);
    
        std::string u1_str (u1.string());
        saga::uuid u3 (u1_str.c_str());
        BOOST_CHECK_EQUAL(u1, u3);
        
        saga::uuid u4 (u2);
        BOOST_CHECK_EQUAL(u2, u4);

        u4 = u1;
        BOOST_CHECK_EQUAL(u1, u4);
        BOOST_CHECK_EQUAL(u1.string(), u4.string());
    }

///////////////////////////////////////////////////////////////////////////////
}

