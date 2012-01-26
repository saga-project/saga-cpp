//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005-2006 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2006 Chris Miceli (cmicel1@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#if !defined(SAGA_TEST_URL_API_HPP)
#define SAGA_TEST_URL_API_HPP

#include <boost/bind.hpp>
#include <boost/futures.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/for_each.hpp>
#include "../test_saga.hpp"
#include "../macros.hpp"

namespace test_url
{
 namespace api
 {
    template <typename TestFunctor>
    struct run_test
    {
        static void execute()
        {
            TestFunctor::test();
        }
    };
 } // api    
} // test_url

#endif // !SAGA_TEST_URL_API_HPP

