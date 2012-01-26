//  Copyright (c) 2005-2010 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga/adaptors/register_workitem.hpp>
#include <saga/impl/runtime.hpp>
#include <saga/impl/session.hpp>
#include <saga/saga/version.hpp>

namespace saga { namespace adaptors
{
    ///////////////////////////////////////////////////////////////////////////
    workitem_cookie_handle register_workitem(workitem_function f, 
        time_type const& expire_at, saga::session const& s)
    {
// disable the monitoring thread for the V1.5 release
// #if SAGA_VERSION_FULL > 0x010600 
//         return impl::runtime::get_impl(s)->register_workitem(f, expire_at);
// #else
        return 1;
// #endif
    }

    ///////////////////////////////////////////////////////////////////////////
    workitem_cookie_handle register_workitem(workitem_function f, 
        duration_type const& expire_from_now, saga::session const& s)
    {
// disable the monitoring thread for the V1.5 release
// #if SAGA_VERSION_FULL > 0x010600 
//         return impl::runtime::get_impl(s)->register_workitem(f, expire_from_now);
// #else
        return 1;
// #endif
    }

    ///////////////////////////////////////////////////////////////////////////
    void unregister_workitem(workitem_cookie_handle h, saga::session const& s)
    {
// disable the monitoring thread for the V1.5 release
// #if SAGA_VERSION_FULL > 0x010600 
//         impl::runtime::get_impl(s)->unregister_workitem(h);
// #endif
    }
}}

