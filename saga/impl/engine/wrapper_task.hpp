//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005-2006 Stephan Hirmer (shirmer@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_IMPL_ENGINE_WRAPPER_TASK_HPP
#define SAGA_IMPL_ENGINE_WRAPPER_TASK_HPP

#include <string>
#include <vector>
#include <ctime>
#include <iostream>
#include <sys/types.h>

// lexical_cast will be in TR2
#include <boost/lexical_cast.hpp>
#include <boost/assert.hpp>
#include <boost/futures.hpp>

#include <saga/saga/metric.hpp>
// #include <saga/saga/task.hpp>
// #include <saga/saga/task_base.hpp>
#include <saga/impl/runtime.hpp>
#include <saga/impl/engine/task_fwd.hpp>
#include <saga/impl/engine/task_base.hpp>

#include <saga/impl/config.hpp>

#include <saga/impl/engine/cpi.hpp>
#include <saga/impl/engine/session.hpp>

///////////////////////////////////

#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable: 4251 4231 4275 4660)
#endif

/// @cond
/** Hide boost::futures from Doxygen */

using namespace boost::futures;

/// @endcon

namespace saga
{
  namespace impl
  {
    /**
     * Generic task class, for encapsulating asynchronous function calls.
     *
     * Almost the same as saga::impl::task, with the slight difference
     * that here, the function to execute, takes a further parameter
     * more precisely a saga::uuid.
     * This is needed to enable monitoring of tasks, where the adaptor
     * implements the asynchronity.
     *
     * @see the saga::impl::task for a detailed description.
     */
    ///////////////////////////////////////////////////////////////////////////
    template <
        typename BaseCpi, typename Base, typename RetVal
    >
    class wrapper_task <
            BaseCpi, Base, RetVal, 
            phoenix::nil_t, phoenix::nil_t, phoenix::nil_t, phoenix::nil_t, 
            phoenix::nil_t, phoenix::nil_t, phoenix::nil_t, phoenix::nil_t, 
            phoenix::nil_t, phoenix::nil_t, phoenix::nil_t, phoenix::nil_t, 
            phoenix::nil_t, phoenix::nil_t
        >
      : public saga::impl::task_base
    {
    public:
        typedef typename saga::impl::object::mutex_type mutex_type;
        
    private:
        typedef saga::impl::task_base base_type;
        
        void (Base::*exec_)(RetVal, saga::uuid);
        bool (Base::*prep_func_)(RetVal, saga::uuid);
        
        /*! the function to be bound to the future */
        int bond (void)
        {
            saga::impl::task_base::state_setter setter (*this);
            int return_code = 1;

            try {
                TR1::shared_ptr<Base> adp(
                    TR1::static_pointer_cast<Base>(this->cpi_instance_));

                (adp.get()->*exec_)(saga::detail::any_cast<RetVal&>(this->retval_), 
                    this->get_uuid());
                
                // set state to finished
                setter.state_ = saga::task::Done;
                return_code = 0;
            }
            catch (saga::exception const& e) {
                mutex_type::scoped_lock l(this->saga::impl::object::mtx_);
                this->found_saga_exception_ = true;
                this->saved_exception_object_ = e.get_object();
                this->saved_exception_list_ = e.get_all_exceptions();
                this->saved_exception_error_ = e.get_error();
            }
            catch (std::exception const& e) {
                mutex_type::scoped_lock l(this->saga::impl::object::mtx_);
                this->found_exception_ = true;
                this->saved_exception_ = e;
            }
            catch (...) {
                mutex_type::scoped_lock l(this->saga::impl::object::mtx_);

                TR1::shared_ptr<saga::impl::object> impl(
                    TR1::static_pointer_cast<saga::impl::object>(
                        TR1::const_pointer_cast<saga::impl::proxy>(
                            this->cpi_instance_->get_proxy()->shared_from_this())));

                this->found_saga_exception_ = true;
                this->saved_exception_object_ = runtime::get_object(impl);
                this->saved_exception_list_.clear();
                this->saved_exception_message_ = "Unspecified error caught";
                this->saved_exception_error_ = (saga::error)saga::adaptors::Unexpected;
            }
            return return_code;
        }

    public:
        /**
        * Default destructor
        */
        ~wrapper_task (void) 
        {
            try {
                // while not finished
                while (saga::task_base::Running == this->get_state() && !this->wait(0))
                    this->base_type::sleep(5);     // we consider 5msec as a good slice
            }
            catch (saga::exception const&) {
            // nothing to do here, just return
            }
        }

        wrapper_task (std::string const& func_name, 
                TR1::shared_ptr<BaseCpi> cpi, 
                void (Base::*func)(RetVal&, saga::uuid),
                bool (Base::*prep)(RetVal&, saga::uuid) = NULL)
            : base_type (func_name, cpi), exec_(func), prep_func_(prep)
        {   
        }

        /**
        * Executes the operation specified by the task_factory.
        * @note this function returns immediately.
        * @throw not_pending exception, if the task you want to run
        * is not pending.
        */
        int run (void)
        {
            // test if there is a function to execute
            if (exec_)
            {
                if (this->get_state () != saga::task_base::New)
                {
                    SAGA_THROW("incorrect state: task is not pending!", 
                        saga::IncorrectState);
                }
#if !defined(SAGA_DISABLE_BULK_OPS)
                if (this->is_bulk_treated)
                {
                    SAGA_THROW("incorrect state: task is not pending!", 
                        saga::IncorrectState);
                }
#endif
                // set state to running, propagate value
                mutex_type::scoped_lock l(this->saga::impl::object::mtx_);
                this->set_state(saga::task_base::Running);

                // executing the function, using futures and the boost/bind
                this->thread_ = boost::futures::simple_future <int> (
                    TR1::bind (&wrapper_task::bond, this));

                return 1;
            }
            
            // nothing to run???
            BOOST_ASSERT(false);
            return 0;
        }
        
#if !defined(SAGA_DISABLE_BULK_OPS)
        /*!
         * @see cpi class for a description of this function
         */
        virtual void visit_args(v1_0::cpi* bulk_adaptor)
        {
            if(NULL != prep_func_ && NULL != bulk_adaptor && is_bulk_treated)
            {
               // execute the encapsulated function.
               (static_cast<Base*>(bulk_adaptor)->*prep_func_)(
                  saga::detail::any_cast<RetVal&>(this->retval_), this->get_uuid());

                // and save the bulk_adaptor.
                cpi_instance_ = bulk_adaptor->shared_from_this();
                if (will_async_in_adaptor == is_external_bulk_async)
                {
                    is_external_bulk_async = bulk_async_in_adaptor;
                }
            }
        }
#endif

        ///////////////////////////////////////////////////////////////////////
        //  return the task_interface to the facade 
        virtual saga::impl::task_interface* 
            get_task_interface() { return this; }
        virtual saga::impl::task_interface const* 
            get_task_interface() const { return this; }

        ///////////////////////////////////////////////////////////////////////
        //  return the monitorable interface to the facade 
        virtual saga::impl::monitorable* 
            get_monitorable() { return this; }
        virtual saga::impl::monitorable const* 
            get_monitorable() const { return this; }

        virtual saga::uuid get_id() const { return this->get_uuid(); }
    };
    
    ///////////////////////////////////////////////////////////////////////////
    // bring in higher order functions
    #include <saga/impl/engine/wrapper_task_impl.hpp>

}}  // namespace saga::impl

#endif  // SAGA_IMPL_ENGINE_WRAPPER_TASK_HPP


