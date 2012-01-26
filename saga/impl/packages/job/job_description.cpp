//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga/object.hpp>
#include <saga/saga/task.hpp>
#include <saga/saga/packages/job/job_description.hpp>
#include <saga/impl/packages/job/job_description.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl {

    description::description ()
    :   saga::impl::object(saga::object::JobDescription)
    {
    }
    
    description::description (saga::object::type t)
    :   saga::impl::object (t)
    {
    }
    
    description::description (attribute const& rhs_attr)
    :   saga::impl::object(saga::object::JobDescription),
        attribute_base(rhs_attr)
    {
    }
    
    description::~description (void) 
    {
    }

    // Generate a exact deep copy of this object
    saga::object description::clone() const
    {
        return saga::job::description(
            new description(static_cast<attribute_base const&>(*this)));
    }

///////////////////////////////////////////////////////////////////////////////
}}  // namespace saga::impl

