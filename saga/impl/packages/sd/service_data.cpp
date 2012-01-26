//  Copyright (c) 2009 STFC
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <saga/saga/version.hpp>
#include <saga/saga/object.hpp>
#include <saga/saga/task.hpp>
#include <saga/saga/packages/sd/service_data.hpp>
#include <saga/impl/packages/sd/service_data.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl {

    service_data::service_data ()
    :   saga::impl::object(saga::object::ServiceData)
    {
    }

    service_data::service_data (attribute const& rhs_attr)
    :   saga::impl::object(saga::object::ServiceData),
        attribute_base(rhs_attr)
    {
    }

    service_data::~service_data (void)
    {
    }

    // Generate a exact deep copy of this object
    saga::object service_data::clone() const
    {
        return saga::sd::service_data(
            new service_data(static_cast<attribute_base const&>(*this)));
    }

///////////////////////////////////////////////////////////////////////////////
}}  // namespace saga::impl

