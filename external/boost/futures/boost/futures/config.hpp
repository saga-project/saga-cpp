/*=============================================================================
    Copyright (c) 2005 Hartmut Kaiser

    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at 
    http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(FUTURES_CONFIG_HPP)
#define FUTURES_CONFIG_HPP 

#include <boost/futures/version.hpp>

///////////////////////////////////////////////////////////////////////////////
//
//  The BOOST_FUTURES_COMPOSITE_LIMIT defines the upper limit of elements
//  in a composite generator (such as the '||' or '&&' operators).
//  Please note, that this value must not be greater than the value of
//  FUSION_MAX_TUPLE_SIZE.
//
///////////////////////////////////////////////////////////////////////////////
#if !defined(BOOST_FUTURES_COMPOSITE_LIMIT)
#define BOOST_FUTURES_COMPOSITE_LIMIT 10
#endif

///////////////////////////////////////////////////////////////////////////////
//
//  Ensure a proper value for FUSION_MAX_TUPLE_SIZE is defined.
//
///////////////////////////////////////////////////////////////////////////////
#if !defined(FUSION_MAX_TUPLE_SIZE)
#define FUSION_MAX_TUPLE_SIZE BOOST_FUTURES_COMPOSITE_LIMIT
#elif (FUSION_MAX_TUPLE_SIZE < BOOST_FUTURES_COMPOSITE_LIMIT)
#error "FUSION_MAX_TUPLE_SIZE is set too low"
#endif

#endif // !defined(FUTURES_CONFIG_HPP)
