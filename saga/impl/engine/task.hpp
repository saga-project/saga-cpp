//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2006 Stephan Hirmer (shirmer@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_IMPL_ENGINE_TASK_HPP
#define SAGA_IMPL_ENGINE_TASK_HPP

#include <string>
#include <vector>
#include <ctime>
#include <sys/types.h>

#include <saga/saga/util.hpp>

// lexical_cast will be in TR2
#include <boost/lexical_cast.hpp>
#include <boost/futures.hpp>

#include <saga/saga/adaptors/metric.hpp>
// #include <saga/saga/task.hpp>
#include <saga/saga/task_base.hpp>
#include <saga/saga/attribute.hpp>
#include <saga/impl/runtime.hpp>
#include <saga/impl/engine/task_fwd.hpp>
#include <saga/impl/engine/task_base.hpp>

#include <saga/impl/config.hpp>
#include <saga/impl/engine/cpi.hpp>
#include <saga/impl/engine/cpi_info.hpp>
#include <saga/impl/engine/session.hpp>
#include <saga/impl/engine/task_base.hpp>
#include <saga/impl/engine/proxy.hpp>
#include <saga/impl/engine/adaptor_selector_state.hpp>

#include <boost/assert.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_base_of.hpp>

// BOOST_VERIFY is new in Boost 1.35
#ifndef BOOST_VERIFY
#   define BOOST_VERIFY(expr) ((void)(expr))
#endif

#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable: 4251 4231 4275 4660)
#endif

namespace saga
{
  namespace impl
  {
    /**
     * Generic task class, for encapsulating asynchronous function calls.
     *
     * This class is not part of the saga facade, but of the implementation.
     * Function calls are stored here and executed within a thread.
     *
     * The stored function pointer is executed together with the
     * stored function parameter within a thread as soon as run is called.
     */

    ///////////////////////////////////////////////////////////////////////////
    template <
        typename BaseCpi, typename Base, typename RetVal
    >
    class task <
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

        /* The function pointer to execute in the thread */
        void (Base::*exec_)(RetVal&);

        /** 
          * The bulk prepare function, which passes the function parameters
          * to a special bulk adaptor. 
          */
        bool (Base::*prep_func_) (RetVal&, saga::uuid);

        // for async construction, we do not have a cpi, yet, so base is not
        // cpi, but the (derived of) proxy which implements the exec which
        // actually creates the cpi.  The get_base() magic does that switch on
        // compile time.  Needs to be done at compile time, because types are
        // different.
        template <typename Base_>
        typename boost::enable_if<
            boost::is_base_of<v1_0::cpi, Base_>, TR1::shared_ptr<Base> 
        >::type 
        get_base()
        {
            return TR1::static_pointer_cast<Base_>(this->cpi_instance_);
        }

        template <typename Base_>
        typename boost::disable_if<
            boost::is_base_of<v1_0::cpi, Base_>, TR1::shared_ptr<Base_> 
        >::type 
        get_base()
        {
            return TR1::static_pointer_cast<Base_>(this->proxy_);
        }

        /*! the function to be bound to the future */
        int bond (void)
        {
            saga::impl::task_base::state_setter setter (*this);
            int return_code = 1;

            while (return_code) {
                try {
                    // adp: cpi or proxy instance which implements the req. exec
                    // function
                    TR1::shared_ptr<Base> adp (get_base<Base>());

                    (adp.get()->*exec_)(saga::detail::any_cast<RetVal&>(this->retval_));

                    // set state to finished
                    setter.state_ = saga::task_base::Done;
                    return_code = 0;

                    // store last known good adaptor in saga object
                    if (this->selector_state_) 
                        this->selector_state_->set_last_known_good();
                }
                catch (saga::exception const& e) {
                    mutex_type::scoped_lock l(this->saga::impl::object::mtx_);
                    if (this->selector_state_) {
                        this->selector_state_->add_exception(e);
                    }
                    else {
                        this->found_saga_exception_ = true;
                        this->saved_exception_object_ = e.get_object();
                        this->saved_exception_list_ = e.get_all_exceptions();
                        this->saved_exception_error_ = e.get_error();
                    }
                }
                catch (std::exception const& e) {
                    mutex_type::scoped_lock l(this->saga::impl::object::mtx_);
                    if (this->selector_state_) {
                        TR1::shared_ptr<saga::impl::object> impl(
                            TR1::static_pointer_cast<saga::impl::object>(
                                TR1::const_pointer_cast<saga::impl::proxy>(
                                    this->proxy_->shared_from_this())));

                        this->selector_state_->add_exception(
                            saga::exception(runtime::get_object(impl),
                                std::string("std::exception caught") + e.what(),
                                (saga::error)saga::adaptors::Unexpected));
                    }
                    else {
                        this->found_exception_ = true;
                        this->saved_exception_ = e;
                    }
                }
                catch (...) {
                    mutex_type::scoped_lock l(this->saga::impl::object::mtx_);

                    TR1::shared_ptr<saga::impl::object> impl(
                        TR1::static_pointer_cast<saga::impl::object>(
                            TR1::const_pointer_cast<saga::impl::proxy>(
                                this->proxy_->shared_from_this())));

                    if (this->selector_state_) {
                        this->selector_state_->add_exception(
                            saga::exception(runtime::get_object(impl),
                                "Unspecified error caught", 
                                (saga::error)saga::adaptors::Unexpected));
                    }
                    else {
                        this->found_saga_exception_ = true;
                        this->saved_exception_object_ = runtime::get_object(impl);
                        this->saved_exception_list_.clear();
                        this->saved_exception_message_ = "Unspecified error caught";
                        this->saved_exception_error_ = (saga::error)saga::adaptors::Unexpected;
                    }
                }
                if (return_code && (!this->selector_state_ || !this->restart()))
                    break;
            }
            return return_code;
        }

        // try to restart operation using the next adaptor
        bool restart() 
        {
            if (this->selector_state_) 
            {
                if (saga::task::Canceled == get_state())
                {
                    TR1::shared_ptr<saga::impl::object> impl(
                        TR1::static_pointer_cast<saga::impl::object>(
                            TR1::const_pointer_cast<saga::impl::proxy>(
                                this->proxy_->shared_from_this())));

                    this->selector_state_->add_exception(
                        saga::exception(runtime::get_object(impl),
                            "incorrect state: task has been canceled!", 
                            (saga::error)saga::IncorrectState));
                    return false;
                }

                try {
                    // we've got a selector state, so try to use the next adaptor
                    mutex_type::scoped_lock l(this->saga::impl::object::mtx_);

                    run_mode mode = impl::Unknown;
                    void (v1_0::cpi::*exec)() = NULL;
                    bool (v1_0::cpi::*prep)() = NULL;

                    this->selector_state_->restart();
                    TR1::shared_ptr<v1_0::cpi> next_cpi(
                        this->selector_state_->template get_next_cpi<v1_0::cpi>(mode, &exec, NULL, &prep));

                    BOOST_ASSERT(NULL != exec);

                    // reuse this task instance
                    this->cpi_instance_ = next_cpi;
                    this->exec_ = (void (Base::*)(RetVal&))exec;
                    this->prep_func_ = (bool (Base::*)(RetVal&, saga::uuid))prep;

                    return true;
                }
                catch (saga::exception const& /*e*/) {
//                     this->state_->add_exception(e);
                    /* the exception is in the list already */;
                }
                catch (std::exception const& e) {
                    TR1::shared_ptr<saga::impl::object> impl(
                        TR1::static_pointer_cast<saga::impl::object>(
                            TR1::const_pointer_cast<saga::impl::proxy>(
                                this->proxy_->shared_from_this())));

                    this->selector_state_->add_exception(
                        saga::exception(runtime::get_object(impl),
                            std::string("std::exception caught") + e.what(),
                            (saga::error)saga::adaptors::Unexpected));
                }
            }
            return false;
        }

    public:
        /**
          * Destructor
          *
          * waits for the task to finish, before destructing the task.
          */
        ~task (void) 
        {
            try {
                // while not finished
                if (saga::task_base::Running == this->get_state())
                    // With a negative timeout, this should always return true
                    // or throw.
                    BOOST_VERIFY(this->wait(-1.0));
            }
            catch (saga::exception const&) {
            // nothing to do here, just return
            }
        }

        /**
          * constructor
          */
        task (std::string const& func_name, TR1::shared_ptr<BaseCpi> cpi, 
              proxy* prxy, void (Base::*sync)(RetVal&), 
              bool (Base::*prep)(RetVal&, saga::uuid))
          : base_type(func_name, cpi, prxy), exec_(sync), prep_func_(prep)
        {   
        }

        /**
          * Executes the operation which is stored in this task.
          * @note this function returns immediately.
          * @throw not_pending exception, if the task you want to run
          * is not pending.
          */
        int run (void)
        {
            // test if there is a function to execute
            if (exec_)
            {
                if (this->get_state() != saga::task::New)
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
                this->set_state(saga::task::Running);

                // executing the function, using futures and the boost/bind
                this->thread_ = boost::futures::simple_future<int> (
                    TR1::bind(&task::bond, this));

                return 1;
            }

            // nothing to run???
            BOOST_ASSERT(false);
            return 0;
        }

#if !defined(SAGA_DISABLE_BULK_OPS)
        /**
         * declared here, implementation is in external file task_get_bulk_adaptor.
         */
        virtual v1_0::cpi* 
        get_bulk_adaptor (std::string const& cpi_name, 
            std::string const& op_name, v1_0::preference_type const& prefs)
        {
            saga::session s = this->proxy_->get_session();

            // FIXME: temporary hack! this list should get passed from the outside
            adaptor_selector::adaptor_info_list_type no_no_list;
          v1_0::op_info oi (op_name);
            return (runtime::get_impl(s)->get_adaptor(cpi_name, op_name,
                prefs, proxy_.get(), no_no_list, oi));
        } 

        /*!
         * passes all the function parameters to a special bulk adaptor.
         */
        template <typename Base_>
        typename boost::enable_if<boost::is_base_of<v1_0::cpi, Base_> >::type 
        visit_args_enabled(v1_0::cpi* bulk_adaptor)
        {
            if(NULL != prep_func_ && NULL != bulk_adaptor && this->is_bulk_treated)
            {
                (static_cast<Base*>(bulk_adaptor)->*prep_func_)(
                    saga::detail::any_cast<RetVal&>(this->retval_), 
                    this->get_uuid());

                // and save the bulk_adaptor.
                this->cpi_instance_ = bulk_adaptor->shared_from_this();
                if (will_async_in_adaptor == this->is_external_bulk_async)
                {
                    this->is_external_bulk_async = bulk_async_in_adaptor;
                }
            }
        }

        template <typename Base_>
        typename boost::disable_if<boost::is_base_of<v1_0::cpi, Base_> >::type 
        visit_args_enabled(v1_0::cpi* bulk_adaptor)
        {
        }

        virtual void visit_args(v1_0::cpi* bulk_adaptor)
        {
            visit_args_enabled<Base>(bulk_adaptor);
        }
#endif // SAGA_DISABLE_BULK_OPS

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
    #include <saga/impl/engine/task_impl.hpp>

}}  // namespace saga::impl

#endif  // SAGA_IMPL_ENGINE_TASK_HPP


