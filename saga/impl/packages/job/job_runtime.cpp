//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga/packages/job/ostream.hpp>
#include <saga/saga/packages/job/istream.hpp>
#include <saga/impl/packages/job/job_runtime.hpp>
#include <saga/impl/packages/job/istream.hpp>
#include <saga/impl/packages/job/ostream.hpp>

/// @cond
/** These methods are not within API scope */
///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl 
{
    job_runtime::handle_type 
    job_runtime::get_handle(saga::job::istream const& s, bool detach)
    {
        return s.impl_->get_handle(detach);
    }
    
    job_runtime::handle_type 
    job_runtime::get_handle(saga::job::ostream const& s, bool detach)
    {
        return s.impl_->get_handle(detach);
    }

///////////////////////////////////////////////////////////////////////////////
}}
/// @endcond

