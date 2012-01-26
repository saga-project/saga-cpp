//  Copyright (c) 2009 STFC
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <saga/saga/version.hpp>
#include <saga/saga/object.hpp>
#include <saga/saga/task.hpp>

#include <saga/saga/packages/isn/entity_data.hpp>
#include <saga/impl/packages/isn/entity_data.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl {

    entity_data::entity_data ()
    :  saga::impl::object(saga::object::EntityData)
    {
    }

    entity_data::entity_data (attribute const& rhs_attr)
    :   saga::impl::object(saga::object::EntityData),
        attribute_base(rhs_attr)
    {
    }

    entity_data::~entity_data (void)
    {
    }

    // Generate an exact deep copy of this object
    saga::object entity_data::clone() const
    {
        return saga::isn::entity_data(
            new entity_data(static_cast<attribute_base const&>(*this)));
    }

///////////////////////////////////////////////////////////////////////////////
}}  // namespace saga::impl

