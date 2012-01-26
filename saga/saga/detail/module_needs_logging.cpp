//  Copyright (c) 2005-2011 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga/util.hpp>
#include <saga/saga/base.hpp>
#include <saga/saga/session.hpp>
#include <saga/saga/detail/module_needs_logging.hpp>
#include <saga/impl/runtime.hpp>
#include <saga/impl/session.hpp>

namespace saga { namespace detail
{
    ///////////////////////////////////////////////////////////////////////////
    bool module_needs_logging(char const* module_name, saga::session const& s)
    {
        return impl::runtime::get_impl(s)->module_needs_logging(module_name);
    }
}}

