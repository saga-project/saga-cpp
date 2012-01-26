//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <string>

#include <saga/saga/base.hpp>
#include <saga/saga/util.hpp>
#include <saga/saga/task.hpp>
#include <saga/saga/adaptors/task.hpp>

#include <saga/impl/call.hpp>
#include <saga/impl/engine/object.hpp>
#include <saga/impl/engine/cpi_info.hpp>
#include <saga/impl/engine/permissions.hpp>
#include <saga/impl/engine/proxy.hpp>
#include <saga/impl/engine/sync_async.hpp>
#include <saga/impl/engine/cpi_info.hpp>
#include <saga/impl/engine/cpi_list.hpp>
#include <saga/impl/engine/run_mode_wrapper.hpp>

#include <saga/saga/adaptors/permissions_cpi.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl
{
    ///////////////////////////////////////////////////////////////////////////
    saga::impl::proxy* permissions::get_proxy()
    {
        SAGA_THROW(
            "This object does not expose the SAGA proxy interface", 
            saga::NotImplemented);
        return NULL;
    }

    saga::impl::proxy const* permissions::get_proxy() const
    {
        SAGA_THROW(
            "This object does not expose the SAGA proxy interface", 
            saga::NotImplemented);
        return NULL;
    }

    ///////////////////////////////////////////////////////////////////////////
    saga::task 
    permissions::permissions_allow (std::string id, int perm, bool is_sync)
    {
        return saga::impl::execute_sync_async(get_proxy(),
            "permissions_cpi", "permissions_allow", 
            "permissions::permissions_allow", is_sync,
            &permissions_cpi::sync_permissions_allow, 
            &permissions_cpi::async_permissions_allow, 
            id, perm);
    }

    saga::task 
    permissions::permissions_deny (std::string id, int perm, bool is_sync)
    {
        return saga::impl::execute_sync_async(get_proxy(),
            "permissions_cpi", "permissions_deny", 
            "permissions::permissions_deny", is_sync,
            &permissions_cpi::sync_permissions_deny, 
            &permissions_cpi::async_permissions_deny, 
            id, perm);
    }

    saga::task 
    permissions::permissions_check (std::string id, int perm, bool is_sync)
    {
        return saga::impl::execute_sync_async(get_proxy(),
            "permissions_cpi", "permissions_check", 
            "permissions::permissions_check", is_sync,
            &permissions_cpi::sync_permissions_check, 
            &permissions_cpi::async_permissions_check, 
            id, perm);
    }

    saga::task 
    permissions::get_owner (bool is_sync)
    {
        return saga::impl::execute_sync_async(get_proxy(),
            "permissions_cpi", "get_owner", 
            "permissions::get_owner", is_sync,
            &permissions_cpi::sync_get_owner, 
            &permissions_cpi::async_get_owner);
    }

    saga::task 
    permissions::get_group (bool is_sync)
    {
        return saga::impl::execute_sync_async(get_proxy(),
            "permissions_cpi", "get_group", 
            "permissions::get_group", is_sync,
            &permissions_cpi::sync_get_group, 
            &permissions_cpi::async_get_group);
    }

///////////////////////////////////////////////////////////////////////////////
}}

