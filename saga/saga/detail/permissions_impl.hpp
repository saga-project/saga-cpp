//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_SAGA_DETAIL_PERMISSIONS_IMPL_HPP
#define SAGA_SAGA_DETAIL_PERMISSIONS_IMPL_HPP

#include <saga/impl/exception.hpp>
#include <saga/saga/detail/permissions.hpp>
#include <saga/saga/detail/dispatch_priv.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace detail
{
    template <typename Derived>
    inline impl::permissions_interface* 
    permissions<Derived>::get_perm (void) 
    { 
        if (!derived().is_impl_valid()) {
            SAGA_THROW("The object has not been properly initialized.",
                saga::IncorrectState);
        }
        return derived().get_impl()->get_permissions(); 
    }
    
    template <typename Derived>
    inline impl::permissions_interface* 
    permissions<Derived>::get_perm (void) const 
    { 
        if (!derived().is_impl_valid()) {
            SAGA_THROW("The object has not been properly initialized.",
                saga::IncorrectState);
        }
        return derived().get_impl()->get_permissions(); 
    }

    /// @cond
    /////////////////////////////////////////////////////////////////////////////
    /* setters/getters */
    struct permissions_allow_priv 
    {
        template <typename Derived>
        static saga::task 
        call (Derived& this_, std::string id, int p, bool sync) 
        {
            return this_.get_perm()->permissions_allow(id, p, sync);
        }
    };

    template <typename Derived, typename Tag>
    inline saga::task
    permissions_priv<Derived, Tag>::permissions_allow (Derived const& this_, 
            std::string id, int p) 
    {
        return dispatch_priv<Tag>::
            template call<permissions_allow_priv>(this_, id, p);
    }

    /////////////////////////////////////////////////////////////////////////////
    struct permissions_deny_priv 
    {
        template <typename Derived>
        static saga::task 
        call (Derived& this_, std::string id, int p, bool sync) 
        {
            return this_.get_perm()->permissions_deny(id, p, sync);
        }
    };

    template <typename Derived, typename Tag>
    inline saga::task
    permissions_priv<Derived, Tag>::permissions_deny (Derived const& this_, 
            std::string id, int p) 
    {
        return dispatch_priv<Tag>::
            template call<permissions_deny_priv>(this_, id, p);
    }

    /////////////////////////////////////////////////////////////////////////////
    struct permissions_check_priv 
    {
        template <typename Derived>
        static saga::task 
        call (Derived& this_, std::string id, int p, bool sync) 
        {
            return this_.get_perm()->permissions_check(id, p, sync);
        }
    };

    template <typename Derived, typename Tag>
    inline saga::task
    permissions_priv<Derived, Tag>::permissions_check (Derived const& this_, 
            std::string id, int p) 
    {
        return dispatch_priv<Tag>::
            template call<permissions_check_priv>(this_, id, p);
    }

    /////////////////////////////////////////////////////////////////////////////
    struct get_owner_priv 
    {
        template <typename Derived>
        static saga::task 
        call (Derived& this_, bool sync) 
        {
            return this_.get_perm()->get_owner(sync);
        }
    };

    template <typename Derived, typename Tag>
    inline saga::task
    permissions_priv<Derived, Tag>::get_owner (Derived const& this_) 
    {
        return dispatch_priv<Tag>::template call<get_owner_priv>(this_);
    }

    /////////////////////////////////////////////////////////////////////////////
    struct get_group_priv 
    {
        template <typename Derived>
        static saga::task 
        call (Derived& this_, bool sync) 
        {
            return this_.get_perm()->get_group(sync);
        }
    };

    template <typename Derived, typename Tag>
    inline saga::task
    permissions_priv<Derived, Tag>::get_group (Derived const& this_) 
    {
        return dispatch_priv<Tag>::template call<get_group_priv>(this_);
    }
    /// @endcond
    
///////////////////////////////////////////////////////////////////////////////
}}

#endif


