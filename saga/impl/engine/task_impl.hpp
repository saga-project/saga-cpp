//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005 Stephan Hirmer (stephan.hirmer@gmail.com)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(BOOST_PP_IS_ITERATING)

#if !defined(SAGA_IMPL_ENGINE_TASK_IMPL_HPP)
#define SAGA_IMPL_ENGINE_TASK_IMPL_HPP

#include <saga/impl/config.hpp>

#include <boost/preprocessor/iterate.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/comma_if.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>

#include <boost/assert.hpp>
#include <boost/utility/enable_if.hpp>

// BOOST_VERIFY is new in Boost 1.35
#ifndef BOOST_VERIFY
#   define BOOST_VERIFY(expr) ((void)(expr))
#endif

#define BOOST_PP_ITERATION_PARAMS_1                                           \
    (3, (1, SAGA_ARGUMENT_LIMIT, "saga/impl/engine/task_impl.hpp"))           \
    /**/

#include BOOST_PP_ITERATE()

#endif // SAGA_IMPL_ENGINE_TASK_IMPL_HPP

///////////////////////////////////////////////////////////////////////////////
//
//  Preprocessor vertical repetition code
//
///////////////////////////////////////////////////////////////////////////////
#else // defined(BOOST_PP_IS_ITERATING)

#define L BOOST_PP_ITERATION()

#define GET_TEMP_ARG(z, M, _)                                                 \
    BOOST_PP_COMMA_IF(M) FuncArg ## M BOOST_PP_COMMA() Arg ## M               \
/**/

#define GET_TEMP_ARG_TN(z, M, _)                                              \
    BOOST_PP_COMMA_IF(M) typename FuncArg ## M                                \
    BOOST_PP_COMMA() typename Arg ## M                                        \
/**/

#define GET_DEFAULT_ARG(z, M, _)                                              \
    BOOST_PP_COMMA() phoenix::nil_t BOOST_PP_COMMA() phoenix::nil_t           \
/**/

    template <
        typename BaseCpi, typename Base, typename RetVal, 
        BOOST_PP_REPEAT(L, GET_TEMP_ARG_TN, _)
    >
    class task <
            BaseCpi, Base, RetVal,
            BOOST_PP_REPEAT(L, GET_TEMP_ARG, _)
            BOOST_PP_REPEAT(BOOST_PP_INC(BOOST_PP_SUB(SAGA_ARGUMENT_LIMIT, L)), 
                GET_DEFAULT_ARG, _) 
    >
      : public saga::impl::task_base
    {
    public:
        typedef typename saga::impl::object::mutex_type mutex_type;

    private:
        typedef saga::impl::task_base base_type;

#undef GET_DEFAULT_ARG   
#undef GET_TEMP_ARG_TN
#undef GET_TEMP_ARG

        void (Base::*exec_)(RetVal&, BOOST_PP_ENUM_PARAMS(L, FuncArg));
        bool (Base::*prep_func_)(
            RetVal&, BOOST_PP_ENUM_PARAMS(L, FuncArg), saga::uuid);

        // the tuple to hold the function arguments
        typedef phoenix::tuple<BOOST_PP_ENUM_PARAMS(L, Arg)>  tuple_type;
        tuple_type func_args_;

        enum { tuple_size = tuple_type::length };

// generates the following:
// func_args_[phoenix::tuple_index<0>()], func_args_[phoenix::tuple_index<1>()], ...
//
#define GETARG(z, M, _) BOOST_PP_COMMA_IF(M) func_args_[phoenix::tuple_index<M>()]


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

                    (adp.get()->*exec_)(saga::detail::any_cast<RetVal&>(this->retval_), 
                        BOOST_PP_REPEAT(L, GETARG, _));

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
            if (this->selector_state_) {
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
                    this->exec_ = (void (Base::*)(RetVal&, BOOST_PP_ENUM_PARAMS(L, FuncArg)))exec;
                    this->prep_func_ = (bool (Base::*)(RetVal&, BOOST_PP_ENUM_PARAMS(L, FuncArg), saga::uuid))prep;

                    return true;
                }
                catch (saga::exception const& /*e*/) {
//                    this->state_->add_exception(e);
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
        * Default destructor
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
        * constructor.
        */
      task (std::string const& func_name_, TR1::shared_ptr<BaseCpi> cpi, proxy* prxy, 
            void (Base::*sync)(RetVal&, BOOST_PP_ENUM_PARAMS(L, FuncArg)),
            BOOST_PP_ENUM_BINARY_PARAMS(L, Arg, arg),
            bool (Base::*prep)(
                RetVal&, BOOST_PP_ENUM_PARAMS(L, FuncArg), saga::uuid))
        : base_type(func_name_, cpi, prxy), exec_(sync), prep_func_(prep),
          func_args_(BOOST_PP_ENUM_PARAMS(L, arg))
      {
      }

      /**
      * Executes the operation, which was put in this task.
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
        * declared here, implementation is in external file task_get_bulk_adaptor_impl.
        */
      virtual v1_0::cpi* 
      get_bulk_adaptor(std::string const& cpi_name,
          std::string const& op_name, v1_0::preference_type const & prefs)
      {
          saga::session s = this->proxy_->get_session();

          // FIXME: temporary hack! this list should get passed from the outside
          adaptor_selector::adaptor_info_list_type no_no_list;
          v1_0::op_info oi (op_name);
          return (runtime::get_impl(s)->get_adaptor(cpi_name, op_name, prefs,
              proxy_.get(), no_no_list, oi));
      }

      /**boost::is_base_of<v1_0::cpi, Base>
        * passes all the function parameters to a special bulk adaptor.
        *
        * declared here, implementation is in external file task_get_bulk_adaptor.
        */
        
      template <typename Base_>
      typename boost::enable_if<boost::is_base_of<v1_0::cpi, Base_> >::type 
      visit_args_enabled(v1_0::cpi* bulk_adaptor)
      {
          if(NULL != prep_func_ && NULL != bulk_adaptor && is_bulk_treated)
          {
              // execute the prepare function on the selected bulk adaptor.
              ((static_cast<Base*>(bulk_adaptor))->*prep_func_)(
                  saga::detail::any_cast<RetVal&>(this->retval_), 
                  BOOST_PP_REPEAT(L, GETARG, _), this->get_uuid());

              // and save the bulk_adaptor.
              cpi_instance_ = bulk_adaptor->shared_from_this();

              if (will_async_in_adaptor == is_external_bulk_async)
              {
                  is_external_bulk_async = bulk_async_in_adaptor;
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
#undef GETARG
      
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
    
#undef L
#endif // defined(BOOST_PP_IS_ITERATING)
