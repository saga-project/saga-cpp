/*=============================================================================
    Copyright (c) 2001-2005 Joel de Guzman
    Copyright (c) 2004-2005 Hartmut Kaiser

    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at 
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef BOOST_PP_IS_ITERATING

#if !defined(FUTURES_DETAIL_COMPOSITE_IMPL_HPP)
#define FUTURES_DETAIL_COMPOSITE_IMPL_HPP

#include <boost/preprocessor/iterate.hpp>

#define BOOST_PP_ITERATION_PARAMS_1                                           \
    (3, (3, BOOST_FUTURES_COMPOSITE_LIMIT,                                    \
    "boost/futures/detail/composite_impl.hpp"))                               \
    /**/

// generate the remaining constructors for the composite class
#include BOOST_PP_ITERATE()

#endif // !defined(FUTURES_DETAIL_COMPOSITE_IMPL_HPP)

///////////////////////////////////////////////////////////////////////////////
//
//  Preprocessor vertical repetition code
//
///////////////////////////////////////////////////////////////////////////////
#else // defined(BOOST_PP_IS_ITERATING)

#define N BOOST_PP_ITERATION()

    template <BOOST_PP_ENUM_PARAMS(N, typename U)>
    composite(BOOST_PP_ENUM_BINARY_PARAMS(N, U, & _))
    :   base_type(BOOST_PP_ENUM_PARAMS(N, _)), 
        internal(new evaluator_type(*this))  
    {}

#undef N
#endif // defined(BOOST_PP_IS_ITERATING)


