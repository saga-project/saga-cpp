//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_SAGA_DETAIL_DISPATCH_PRIV_HPP
#define SAGA_SAGA_DETAIL_DISPATCH_PRIV_HPP

#include <saga/saga/task.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace detail
{
    /// @cond
    
    ///////////////////////////////////////////////////////////////////////////
    //  Helper templates dispatching the call to the required implementation.
    //  This avoids writing every function thrice.
    template <typename Tag>
    struct dispatch_priv;

    template <>
    struct dispatch_priv<saga::task_base::Sync>
    {
        template <typename F, typename This>
        static saga::task call(This& this_)
        {
            return F::call(this_, true);
        }

        template <typename F, typename This, typename T1>
        static saga::task call(This& this_, T1 const& t1)
        {
            return F::call(this_, t1, true);
        }

        template <typename F, typename This, typename T1, typename T2>
        static saga::task call(This& this_, T1 const& t1, T2 const& t2)
        {
            return F::call(this_, t1, t2, true);
        }
    };

    template <>
    struct dispatch_priv<saga::task_base::Async>
    {
        template <typename F, typename This>
        static saga::task call(This& this_)
        {
            return saga::detail::run(F::call(this_, false));
        }

        template <typename F, typename This, typename T1>
        static saga::task call(This& this_, T1 const& t1)
        {
            return saga::detail::run(F::call(this_, t1, false));
        }

        template <typename F, typename This, typename T1, typename T2>
        static saga::task call(This& this_, T1 const& t1, T2 const& t2)
        {
            return saga::detail::run(F::call(this_, t1, t2, false));
        }
    };
    
    template <>
    struct dispatch_priv<saga::task_base::Task>
    {
        template <typename F, typename This>
        static saga::task call(This& this_)
        {
            return F::call(this_, false);
        }

        template <typename F, typename This, typename T1>
        static saga::task call(This& this_, T1 const& t1)
        {
            return F::call(this_, t1, false);
        }

        template <typename F, typename This, typename T1, typename T2>
        static saga::task call(This& this_, T1 const& t1, T2 const& t2)
        {
            return F::call(this_, t1, t2, false);
        }
    };
    /// @endcond
  
///////////////////////////////////////////////////////////////////////////////
}}

#endif
