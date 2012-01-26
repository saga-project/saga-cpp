//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_IMPL_ENGINE_DUMMY_TASK_HPP
#define SAGA_IMPL_ENGINE_DUMMY_TASK_HPP

#include <string>

#include <boost/bind.hpp>
#include <boost/config.hpp>

#include <saga/impl/config.hpp>
#include <saga/impl/engine/object.hpp>
#include <saga/impl/engine/task_base.hpp>

#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable: 4251 4231 4275 4660)
#endif

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl {

    /**
     * Dummy task class, encapsulating static task state.
     *
     * This class is not part of the saga facade, but of the implementation.
     * It is usable for returning the task::state only.
     */
    class dummy_task 
      : public saga::impl::task_base
    {
    public:
        ~dummy_task (void) 
        {}

        /**
          * constructor
          */
        explicit dummy_task (saga::task_base::state state)
          : saga::impl::task_base(state)
        {}

        ///////////////////////////////////////////////////////////////////////
        //  return the task_interface to the facade 
        SAGA_EXPORT virtual saga::impl::task_interface* 
            get_task_interface() { return this; }
        SAGA_EXPORT virtual saga::impl::task_interface const* 
            get_task_interface() const { return this; }

        SAGA_EXPORT virtual saga::uuid 
            get_id() const { return this->impl::object::get_uuid(); }

        // special implementation of get_session to return the default session
        SAGA_EXPORT virtual saga::session& get_session()
        {
            return saga::detail::get_the_session(); 
        }
        SAGA_EXPORT virtual saga::session const& get_session() const
        {
            return saga::detail::get_the_session(); 
        }
    };

///////////////////////////////////////////////////////////////////////////////
}}  // namespace saga::impl

#endif  // SAGA_IMPL_ENGINE_DUMMY_TASK_HPP
