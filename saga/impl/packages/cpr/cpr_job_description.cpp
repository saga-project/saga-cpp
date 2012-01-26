//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga/object.hpp>
#include <saga/saga/task.hpp>
#include <saga/saga/packages/cpr/cpr_job_description.hpp>
#include <saga/impl/packages/cpr/cpr_job_description.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl {

    cpr_job_description::cpr_job_description ()
    :   saga::impl::description (saga::object::CPRJobDescription)
    {
    }
    
    cpr_job_description::cpr_job_description (attribute const& rhs_attr)
    :   saga::impl::description (saga::object::CPRJobDescription)
    {
    }
    
    cpr_job_description::~cpr_job_description (void) 
    {
    }

    // Generate a exact deep copy of this object
    saga::object cpr_job_description::clone() const
    {
        return saga::cpr::description(
            new cpr_job_description (static_cast <attribute_base const &> (*this)));
    }

///////////////////////////////////////////////////////////////////////////////
}}  // namespace saga::impl

