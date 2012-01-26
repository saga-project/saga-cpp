//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005 Michel Zandstra [michelzandstra@gmail.com]
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  job LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef CONTEXT_BASE_HPP
#define CONTEXT_BASE_HPP

#include <map>
#include <vector>
#include <string>

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>

#include <saga/saga/context.hpp>
#include <saga/saga/exception.hpp>
#include <saga/saga/adaptors/attribute.hpp>
#include <saga/impl/exception.hpp>

#include <saga/impl/config.hpp>
#include <saga/impl/context_cpi.hpp>
#include <saga/impl/engine/attribute.hpp>
#include <saga/impl/engine/object.hpp>
#include <saga/impl/engine/proxy.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl 
{
    /**
     * Context_base: base class for all context classes
     */
    class context 
      : public saga::impl::proxy, 
        public saga::impl::attribute
    {
    private:
        typedef v1_0::context_cpi context_cpi;
        typedef v1_0::preference_type preference_type;
        typedef saga::impl::attribute attribute_base;

        explicit context (attribute const& rhs_attr);
        void init();

    public:
        /** constructor, defines context type */
        explicit context();

        // saga::object interface
        saga::object clone() const
        {
            return saga::context(new saga::impl::context(
                static_cast<attribute_base const&>(*this)));
        }

        bool is_equal (saga::impl::object *rhs) const
        {
            return static_cast<attribute_base const&>(*this).is_equal(
                *(rhs->get_attributes()));
        }

        // API function
        SAGA_CALL_IMPL_DECL_0(set_defaults)

        saga::impl::attribute* get_attributes() { return this; }
        saga::impl::attribute const* get_attributes() const { return this; }

        saga::impl::proxy* get_proxy() { return this; }
        saga::impl::proxy const* get_proxy() const { return this; }

    }; // class context_base

}} // namespace saga::impl

#endif // CONTEXT_BASE_HPP

