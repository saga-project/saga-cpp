//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga/util.hpp>
#include <saga/saga/exception.hpp>
#include <saga/saga/object.hpp>
#include <saga/saga/task.hpp>
#include <saga/saga/attribute.hpp>
#include <saga/saga/context.hpp>

#include <saga/impl/exception.hpp>
#include <saga/impl/engine/object.hpp>
#include <saga/impl/engine/attribute.hpp> 
#include <saga/impl/engine/metric.hpp>
#include <saga/impl/engine/context_base.hpp>

#include <saga/saga/detail/attribute_impl.hpp>

namespace saga 
{
    attribute::attribute (saga::object rhs)
      : impl_ (rhs.get_impl_sp())
    {
    }

    attribute::attribute (saga::impl::object *impl)
      : impl_ (impl->shared_from_this())
    {
    }

    attribute::attribute (void)
      : impl_ (new saga::impl::object (saga::object::Unknown))
    {
    }

    attribute::~attribute (void)
    {
    }

    namespace detail
    {
        ///////////////////////////////////////////////////////////////////////////
        //  implement the attribute functions (we need to explicitly specialize 
        //  the template because the functions are not implemented inline)
        template struct SAGA_EXPORT_REPEAT attribute<saga::attribute>;

        template struct SAGA_EXPORT attribute_priv<saga::attribute, task_base::Sync>;
        template struct SAGA_EXPORT attribute_priv<saga::attribute, task_base::Async>;
        template struct SAGA_EXPORT attribute_priv<saga::attribute, task_base::Task>;

        template struct SAGA_EXPORT attribute_sync<saga::attribute>;
    }

///////////////////////////////////////////////////////////////////////////////
} // namespace saga

