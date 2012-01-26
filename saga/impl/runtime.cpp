//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Use, modification and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

// include dependent spec sections
#include <saga/saga/util.hpp>
#include <saga/saga/base.hpp>
#include <saga/impl/runtime.hpp>

#include <saga/saga/session.hpp>
#include <saga/impl/session.hpp>
#include <saga/saga/context.hpp>
#include <saga/impl/context.hpp>

#include <saga/saga/adaptors/task_declaration.hpp>

#include <saga/saga/task_container.hpp>
#include <saga/impl/engine/task_container.hpp>

#include <saga/saga/url.hpp>
#include <saga/impl/url.hpp>

/// @cond
/** These methods are not within API scope */
namespace saga { namespace impl {

    // the SAGA runtime helper class
    SAGA_EXPORT impl::context* 
        runtime::get_impl (saga::context ctx)
    {
        return ctx.get_impl();
    }

    SAGA_EXPORT TR1::shared_ptr<impl::context> 
        runtime::get_impl_sp(saga::context c)
    {
        return (c.get_impl_sp());
    }

    SAGA_EXPORT impl::session*
        runtime::get_impl (saga::session s)
    {
        return (s.get_impl ());
    }

    SAGA_EXPORT TR1::shared_ptr<impl::session> 
        runtime::get_impl_sp(saga::session s)
    {
        return (s.get_impl_sp());
    }

    SAGA_EXPORT impl::url*
        runtime::get_impl (saga::url u)
    {
        return u.get_impl();
    }

    SAGA_EXPORT impl::exception*
        runtime::get_impl (saga::exception e)
    {
        return e.get_impl();
    }

    SAGA_EXPORT TR1::shared_ptr<impl::url> 
        runtime::get_impl_sp(saga::url u)
    {
        return (u.get_impl_sp());
    }

    SAGA_EXPORT impl::task_base*
        runtime::get_task_impl (saga::task t)
    {
        return t.get_impl();
    }

    SAGA_EXPORT task_interface const*
        runtime::get_impl (saga::task const& t)
    {
        return (t.get_task_if ());
    }

    SAGA_EXPORT task_interface*
        runtime::get_impl (saga::task& t)
    {
        return (t.get_task_if ());
    }

    SAGA_EXPORT task_container*
        runtime::get_impl (saga::task_container tc)
    {
        return (tc.get_impl ());
    }

    // get the implementation of a given object
    SAGA_EXPORT impl::object*
        runtime::get_impl_object (saga::object obj)
    {
      return (obj.get_impl());
    }

    SAGA_EXPORT TR1::shared_ptr<impl::object> 
        runtime::get_impl_object_sp(saga::object obj)
    {
      return (obj.get_impl_sp());
    }

    // re-create a saga::object from the impl
    SAGA_EXPORT saga::object 
        runtime::get_object(TR1::shared_ptr<saga::impl::object> impl)
    {
        return saga::object(impl);
    }

    SAGA_EXPORT saga::context 
        runtime::get_object(TR1::shared_ptr<saga::impl::context> impl)
    {
        return saga::context(impl);
    }

}}
/// @endcond

