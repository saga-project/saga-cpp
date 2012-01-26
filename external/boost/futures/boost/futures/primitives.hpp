/*=============================================================================
    Copyright (c) 2005 Hartmut Kaiser

    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at 
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(FUTURES_PRIMITIVES_HPP)
#define FUTURES_PRIMITIVES_HPP

#include <boost/futures/simple_future.hpp>

namespace boost { namespace futures {

    ////////////////////////////////////////////////////////////////////////////
    struct future_int
    {
        template <typename Functor>
        simple_future<int> 
        operator[] (Functor const& f) const
        {
            return simple_future<int>(f);
        }
    };
    future_int const int_ = future_int();
    
    ////////////////////////////////////////////////////////////////////////////
    struct future_long
    {
        template <typename Functor>
        simple_future<long> 
        operator[] (Functor const& f) const
        {
            return simple_future<long>(f);
        }
    };
    future_long const long_ = future_long();

    ////////////////////////////////////////////////////////////////////////////
    struct future_double
    {
        template <typename Functor>
        simple_future<double> 
        operator[] (Functor const& f) const
        {
            return simple_future<double>(f);
        }
    };
    future_double const double_ = future_double();

}}  // namespace boost::futures

#endif // !defined(FUTURES_PRIMITIVES_HPP)


