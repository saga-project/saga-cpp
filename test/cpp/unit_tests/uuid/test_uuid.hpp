//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(SAGA_TEST_UUID_HPP)
#define SAGA_TEST_UUID_HPP

#include "../test_saga.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace test_uuid {

    ///////////////////////////////////////////////////////////////////////////
    void uuid_tests();

    ///////////////////////////////////////////////////////////////////////////
    class suite : public test_suite
    {
    public:
        suite () : test_suite ("uuid test suite")
        {
          add (BOOST_TEST_CASE (&uuid_tests));
        }
    };

///////////////////////////////////////////////////////////////////////////////
} // namespace test_uuid

#endif // SAGA_TEST_UUID_HPP
