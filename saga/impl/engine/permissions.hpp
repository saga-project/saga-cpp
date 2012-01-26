//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_IMPL_ENGINE_PERMISSIONS_HPP
#define SAGA_IMPL_ENGINE_PERMISSIONS_HPP

#include <string>

#include <saga/impl/call.hpp>
#include <saga/impl/engine/permissions_interface.hpp>
#include <saga/saga/adaptors/permissions_cpi.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable : 4251 4231 4660)
#endif

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl
{
    // in SAGA, permissions is actually an interface. 
    class SAGA_EXPORT permissions : public permissions_interface
    {
    private:
        typedef v1_0::permissions_cpi permissions_cpi;
        typedef v1_0::preference_type preference_type;
        
    public:
        ~permissions() {} 
        
        // setters/getters 
        
        /** void permissions_allow(std::string object, int i)
         */
        SAGA_CALL_IMPL_DECL_2(permissions_allow, std::string, int)
        SAGA_CALL_IMPL_DECL_2(permissions_deny, std::string, int)
        SAGA_CALL_IMPL_DECL_2(permissions_check, std::string, int)
        SAGA_CALL_IMPL_DECL_0(get_owner)
        SAGA_CALL_IMPL_DECL_0(get_group)

        // get_proxy returns the internal implementation of the 
        // impl::object interface
        virtual saga::impl::proxy* get_proxy();
        virtual saga::impl::proxy const* get_proxy() const;
    };

///////////////////////////////////////////////////////////////////////////////
}} // namespace saga::impl

#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

#endif 

