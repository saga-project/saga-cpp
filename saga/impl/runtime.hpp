//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_IMPLE_ENGINE_RUNTIME_HPP
#define SAGA_IMPLE_ENGINE_RUNTIME_HPP

#include <saga/saga/util.hpp>
#include <saga/saga/base.hpp>
#include <saga/saga/impl_base.hpp>

/// @cond
/** These methods are not within API scope */
namespace saga { namespace impl {

    // The SAGA runtime helper class
    
    // This is a helper class which wraps the needed access to the 
    // internal implementation classes
    struct runtime
    {
        SAGA_EXPORT static saga::impl::session*
            get_impl (saga::session s);
        SAGA_EXPORT static TR1::shared_ptr<saga::impl::session> 
            get_impl_sp (saga::session obj);

        SAGA_EXPORT static saga::impl::context*
            get_impl (saga::context ctx);
        SAGA_EXPORT static TR1::shared_ptr<saga::impl::context> 
            get_impl_sp (saga::context obj);

        SAGA_EXPORT static saga::context 
            get_object(TR1::shared_ptr<saga::impl::context> impl);

        SAGA_EXPORT static task_interface const* get_impl (saga::task const& t);
        SAGA_EXPORT static task_interface* get_impl (saga::task& t);
        SAGA_EXPORT static task_container*
            get_impl (saga::task_container t);

        SAGA_EXPORT static saga::impl::object*
            get_impl_object (saga::object obj);
        SAGA_EXPORT static TR1::shared_ptr<saga::impl::object> 
            get_impl_object_sp (saga::object obj);

        SAGA_EXPORT static saga::object 
            get_object(TR1::shared_ptr<saga::impl::object> impl);

        SAGA_EXPORT static saga::impl::url*
            get_impl (saga::url u);
        SAGA_EXPORT static TR1::shared_ptr<saga::impl::url> 
            get_impl_sp (saga::url obj);

        SAGA_EXPORT static impl::task_base*
            get_task_impl(saga::task t);

        SAGA_EXPORT static saga::impl::exception*
            get_impl (saga::exception e);
    };
  }
} 
/// @endcond

#endif // SAGA_IMPLE_ENGINE_RUNTIME_HPP

