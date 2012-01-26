//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga/util.hpp>
#include <saga/saga/exception.hpp>
#include <saga/saga/object.hpp>
#include <saga/saga/task.hpp>
#include <saga/saga/permissions.hpp>
#include <saga/saga/context.hpp>

#include <saga/impl/exception.hpp>
#include <saga/impl/engine/object.hpp>
#include <saga/impl/engine/permissions.hpp> 
#include <saga/impl/engine/context_base.hpp>

#include <saga/saga/detail/permissions_impl.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga 
{
    permissions::permissions (saga::object rhs)
      : impl_ (rhs.get_impl_sp())
    {
    }

    permissions::permissions (saga::impl::object *impl)
      : impl_ (impl->shared_from_this())
    {
    }

    permissions::~permissions (void)
    {
    }

    namespace detail
    {
        ///////////////////////////////////////////////////////////////////////////
        //  implement the permissions functions (we need to explicitly specialize 
        //  the template because the functions are not implemented inline)
        template struct SAGA_EXPORT_REPEAT permissions<saga::permissions>;

        template struct SAGA_EXPORT permissions_priv<saga::permissions, saga::task_base::Sync>;
        template struct SAGA_EXPORT permissions_priv<saga::permissions, saga::task_base::Async>;
        template struct SAGA_EXPORT permissions_priv<saga::permissions, saga::task_base::Task>;
    }

///////////////////////////////////////////////////////////////////////////////
} // namespace saga

