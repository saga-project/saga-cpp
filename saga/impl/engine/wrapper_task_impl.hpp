//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005-2006 Stephan Hirmer (stephan.hirmer@gmail.com)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(BOOST_PP_IS_ITERATING)

#if !defined(SAGA_IMPL_ENGINE_WRAPPER_TASK_IMPL_HPP)
#define SAGA_IMPL_ENGINE_WRAPPER_TASK_IMPL_HPP

#include <saga/impl/config.hpp>

#include <boost/preprocessor/iterate.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/comma_if.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>

#define BOOST_PP_ITERATION_PARAMS_1                                           \
    (3, (1, SAGA_ARGUMENT_LIMIT, "saga/impl/engine/wrapper_task_impl.hpp"))   \
    /**/

#include BOOST_PP_ITERATE()

#endif // SAGA_IMPL_ENGINE_WRAPPER_TASK_IMPL_HPP

///////////////////////////////////////////////////////////////////////////////
//
//  Preprocessor vertical repetition code
//
///////////////////////////////////////////////////////////////////////////////
#else // defined(BOOST_PP_IS_ITERATING)

#define L BOOST_PP_ITERATION()

#define GET_TEMP_ARG(z, M, _) BOOST_PP_COMMA_IF(M) FuncArg ## M, Arg ## M
#define GET_TEMP_ARG_TN(z, M, _) BOOST_PP_COMMA_IF(M) typename FuncArg ## M, typename Arg ## M
#define GET_DEFAULT_ARG(z, M, _) BOOST_PP_COMMA() phoenix::nil_t, phoenix::nil_t

    template <
        typename BaseCpi, typename Base, typename RetVal, 
        BOOST_PP_REPEAT(L, GET_TEMP_ARG_TN, _)
    >
    class wrapper_task<
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

        // the function to execute in the new thread.
        void (Base::*exec_)(
            RetVal&, BOOST_PP_ENUM_PARAMS (L, FuncArg), saga::uuid);

        // the bulk preparing function. Used to pass the parameters
        // to a given bulk adaptor.                                    
        bool (Base::*prep_func_)(
            RetVal&, BOOST_PP_ENUM_PARAMS (L, FuncArg), saga::uuid);
        
        // the tuple to hold the function arguments
        typedef phoenix::tuple<BOOST_PP_ENUM_PARAMS (L, Arg)> tuple_type;
        
        // the set of parameters needed to execute the encapsulated function.
        tuple_type func_args_;

        enum { tuple_size = tuple_type::length };
            
// generates the following:
// func_args_[phoenix::tuple_index<0>()], func_args_[phoenix::tuple_index<1>()], ...
//
#define GETARG(z, M, _) BOOST_PP_COMMA_IF(M) func_args_[phoenix::tuple_index<M>()]

        /*! the function to be bound to the future */
        int bond (void)
        {
            saga::impl::task_base::state_setter setter (*this);
            int return_code = 1;

            try {
                TR1::shared_ptr<Base> adp(
                    TR1::static_pointer_cast<Base>(this->cpi_instance_));
                                  
                (adp.get()->*exec_)(saga::detail::any_cast<RetVal&>(this->retval_), 
                    BOOST_PP_REPEAT(L, GETARG, _), this->get_uuid());

            // set state to finished
                setter.state_ = saga::task::Done;
                return_code = 0;
            }
            catch ( saga::exception const& e ) {
                mutex_type::scoped_lock l(this->saga::impl::object::mtx_);
                this->found_saga_exception_ = true;
                this->saved_exception_object_ = e.get_object();
                this->saved_exception_list_ = e.get_all_exceptions();
                this->saved_exception_error_ = e.get_error();
            }
            catch ( std::exception const& e ) {
                mutex_type::scoped_lock l(this->saga::impl::object::mtx_);
                this->found_exception_ = true;
                this->saved_exception_ = e;
            }
            catch ( ... ) {
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

      /**
        * constructor
        */
      wrapper_task (std::string const& func_name, TR1::shared_ptr<BaseCpi> cpi, 
            void (Base::*sync)(
                RetVal&, BOOST_PP_ENUM_PARAMS (L, FuncArg), saga::uuid),
            BOOST_PP_ENUM_BINARY_PARAMS (L, Arg, arg),
            bool (Base::*prep)(
                RetVal&, BOOST_PP_ENUM_PARAMS (L, FuncArg), saga::uuid) = NULL)
        : base_type(func_name, cpi), exec_(sync), prep_func_(prep),
          func_args_(BOOST_PP_ENUM_PARAMS(L, arg))
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
          if (exec_)
          {
              if (this->get_state () != saga::task_base::New || is_bulk_treated)
              {
                    SAGA_THROW("incorrect state: task is not pending!", 
                        saga::IncorrectState);
              }
              
              // set state to running, propagate value
              mutex_type::scoped_lock l(this->saga::impl::object::mtx_);
              this->set_state(saga::task_base::Running);

              // executing the function, using futures and the boost/bind
              this->thread_ = boost::futures::simple_future<int> (
                  TR1::bind (&wrapper_task::bond, this));

              return 1;
          }

          // nothing to run???
          BOOST_ASSERT(false);
          return 0;
      }
      
      // pass all parameters to the bulk_adaptor
      virtual void visit_args(v1_0::cpi* bulk_adaptor)
      {
          if(NULL != prep_func_ && NULL != bulk_adaptor && is_bulk_treated)
          {
              // execute the encapsulated function.
              ((static_cast<Base *>(bulk_adaptor))->*prep_func_)(
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
