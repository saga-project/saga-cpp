//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_IMPL_ENGINE_PERMISSIONS_INTERFACE_HPP
#define SAGA_IMPL_ENGINE_PERMISSIONS_INTERFACE_HPP

#include <saga/saga/export_definitions.hpp>
#include <saga/saga/task.hpp>

#include <string>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable : 4251 4231 4660)
#endif

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl
{
    // we define a common interface used by saga::impl::permissions
    struct SAGA_EXPORT permissions_interface
    {
        virtual ~permissions_interface (void) {}

        virtual saga::task permissions_allow(
            std::string id, int p, bool is_sync = false) = 0;
        virtual saga::task permissions_deny(
            std::string id, int p, bool is_sync = false) = 0;
        virtual saga::task permissions_check(
            std::string id, int p, bool is_sync = false) = 0;
        virtual saga::task get_owner(bool is_sync = false) = 0;
        virtual saga::task get_group(bool is_sync = false) = 0;
    };

}}  // namespace saga::impl

#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

#endif // SAGA_IMPL_ENGINE_ATTRIBUTE_INTERFACE_HPP


