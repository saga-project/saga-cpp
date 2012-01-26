/*=============================================================================
    Copyright (c) 2001-2005 Joel de Guzman
    Copyright (c) 2004-2005 Hartmut Kaiser

    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at 
    http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(FUTURE_DETAIL_AS_FUTURE_HPP)
#define FUTURE_DETAIL_AS_FUTURE_HPP

#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>

#include <boost/futures/future.hpp>

namespace boost { namespace futures { 

    namespace detail
    {
        ///////////////////////////////////////////////////////////////////////
        //
        //  The as_future meta functions may be used to return a future
        //  type given an functor or future type, thus ensuring proper
        //  wrapping of functors into the appropriate future.
        //
        ///////////////////////////////////////////////////////////////////////
        template <typename T>
        struct as_future
        {
            // unchecked static pre-condition: is_future<T>
            // This should be checked on each use of as_future.
            // The check is not done here to allow error reporting
            // at the point of use.

            typedef T type;
            
            static type const&
            eval(type const& p)
            {
                return p;
            }
        };
    
        template <typename RT>
        struct as_future<RT ()>
        {
            typedef future<RT> type;

            static type
            eval(RT f())
            {
                return type(f);
            }
        };
    
        ///////////////////////////////////////////////////////////////////////
        //
        //  The can_convert metafunction may be used to test, whether the
        //  as_future metafunction is applicable for a given type.
        //
        ///////////////////////////////////////////////////////////////////////
        template <typename T>
        struct can_convert : is_future<T> {};
    
        template <typename RT>
        struct can_convert<RT ()> : mpl::true_ {};
        
    }
}}

#endif // !defined(FUTURE_DETAIL_AS_FUTURE_HPP)

