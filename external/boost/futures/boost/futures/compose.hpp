/*=============================================================================
    Copyright (c) 2001-2005 Joel de Guzman
    Copyright (c) 2004-2005 Hartmut Kaiser

    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at 
    http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(FUTURES_COMPOSE_HPP)
#define FUTURES_COMPOSE_HPP

#include <boost/futures/detail/as_future.hpp>
#include <boost/futures/detail/compose_impl.hpp>

namespace boost { namespace futures {

    template <typename Op, typename A, typename B>
    struct as_composite 
    :   detail::compose_composite<
            Op,
            typename detail::as_future<A>::type,
            typename detail::as_future<B>::type
        > 
    {};

    template <typename Op, typename A, typename B>
    inline typename as_composite<Op, A, B>::type
    compose(A const& a, B const& b)
    {
        return as_composite<Op, A, B>::eval(a, b);
    }
    
}}

#endif // !defined(FUTURES_COMPOSE_HPP)
