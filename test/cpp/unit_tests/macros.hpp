//  Copyright (c) 2005-2006 Chris Miceli (cmicel1@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(SAGA_MACROS_HPP)
#define SAGA_MACROS_HPP

#include <boost/thread.hpp>
#include <boost/test/test_tools.hpp>

extern boost::mutex test_lock;

///////////////////////////////////////////////////////////////////////////////
#define SAGA_CHECK_THROW(M, E) {                                              \
        BOOST_CHECK_THROW(M, E);                                              \
    }                                                                         \
    /**/

///////////////////////////////////////////////////////////////////////////////
#define SAGA_REQUIRE_THROW(M, E) {                                            \
        try { M; }                                                            \
        catch(saga::exception const &e) {                                     \
            if(e.get_error() != E) {                                          \
                E = e.get_error();                                            \
                std::cout << e.what() << std::endl;                           \
            }                                                                 \
        }                                                                     \
    }                                                                         \
    /**/

///////////////////////////////////////////////////////////////////////////////
#define SAGA_CHECK(A) {                                                       \
        boost::mutex::scoped_lock sl(test_lock);                              \
        BOOST_CHECK(A);                                                       \
    }                                                                         \
    /**/

///////////////////////////////////////////////////////////////////////////////
#define SAGA_CHECK_MESSAGE(A, msg) {                                          \
        boost::mutex::scoped_lock sl(test_lock);                              \
        BOOST_CHECK_MESSAGE(A, msg);                                          \
    }                                                                         \
    /**/

///////////////////////////////////////////////////////////////////////////////
#define SAGA_CHECK_EQUAL(A, B) {                                              \
        boost::mutex::scoped_lock sl(test_lock);                              \
        BOOST_CHECK_EQUAL(A, B);                                              \
    }                                                                         \
    /**/

///////////////////////////////////////////////////////////////////////////////
#define SAGA_TEST_CASE(test_function, name)                                   \
    boost::unit_test::make_test_case(                                         \
        boost::unit_test::callback0<>(test_function),                         \
        name)                                                                 \
    /**/

#endif //SAGA_MACROS_HPP
