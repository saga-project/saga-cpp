//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_IMPL_ENGINE_TASK_INTERFACE_HPP
#define SAGA_IMPL_ENGINE_TASK_INTERFACE_HPP

#include <string>
#include <set>

#include <sys/types.h>

#include <boost/config.hpp>
#include <boost/shared_ptr.hpp>

#include <saga/saga/uuid.hpp>
#include <saga/saga/task_base.hpp>

#include <saga/impl/config.hpp>
#include <saga/impl/engine/preferences.hpp>
#include <saga/saga/detail/hold_any.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl {

    /**
      * Base interface for a task implementation 
      */
    struct SAGA_EXPORT task_interface 
    {
        /**
        * Default destructor
        */
        virtual ~task_interface (void) {}
        
        /**
          * return the uuid of this implementation object 
          */    
        virtual saga::uuid get_id(void) const = 0;

        /**
          * returns the function name of the operation encapsulated
          * in this task
          */
        virtual std::string get_func_name(void) const = 0;

        /**
         * returns the session, which the task belongs to.
         */
        virtual saga::session& get_session() = 0;
        virtual saga::session const& get_session() const = 0;

        /**
          * execute the encapsulated operation
          */            
        virtual int run (void) = 0;

        /**
          * Blocking call for waiting until the function ran in task::run(void) 
          * will finish.
          * 
          * @return 1 if the task is finished, 0 otherwise
          * 
          * @throw still_canceled exception
          */
        virtual bool wait (double timeout) = 0;

        /**
          * re-throw any caught exceptions
          */
        virtual void rethrow (void) const = 0;

        /**
          * @return the current state of the task.
          */
        virtual saga::task_base::state get_state (void) const = 0;

         /**
           * not in the facade
           * used to change the state of a task, executed in a bulk
           */
        virtual void set_state(saga::task_base::state s) = 0;
        
        /**
          * Cancels the running task.
          */
        virtual void cancel (void) = 0;
        
        /**
          * returns the saga object associated with this task
          */
        virtual saga::object get_object() const = 0;

        /**
          * returns the result associated with this task
          */
        virtual saga::detail::hold_any& get_result() = 0;

        // set the adaptor selector state
        virtual void set_selector_state(TR1::shared_ptr<impl::adaptor_selector_state>) = 0;

        // store an exception in a task instance
        virtual void set_task_exception(saga::impl::object const *obj,
            saga::impl::exception_list const& l, saga::error e) = 0;

        // try to restart operation using the next adaptor
        virtual bool restart() = 0;

#if !defined(SAGA_DISABLE_BULK_OPS)
        ///////////////////////////////////////////////////////////////////////
        //  bulk interface
        ///////////////////////////////////////////////////////////////////////
        
        /**
          * internal bulk states, a task can be in
          */
        enum bulk_treatment_state
        {
            not_bulk_treated    = 0,    /* This task is not treated within a bulk */
            actual_bulk_treated = 1,    /* This task is currently handled through a bulk */
            was_bulk_treated    = 2    /* This task was handled through a bulk */
        };
        
        /**
          * the same as for bulk_treatment_state, but the adaptor is responsible
          * for the asynchronism of the operation.
          */
        enum bulk_async_in_adaptor_state
        {
            not_async_in_adaptor  = 0,  /* all the asynchronism is handled in the task */
            will_async_in_adaptor = 1,  /* the asynchronism will be handled in the adaptor, 
                                           but the handling did not start yet */
            bulk_async_in_adaptor = 2   /* bulk handling is done asynchronous in the adaptor */
        };
        
        /**
          * not in the facade
          * used to handle semantic, if this task is handled within a bulk
          * towards the application, the task is normally running
          * but is actually treated in special bulk handling
          */
        virtual void set_external_treatment (
            bool block_for_bulk_treatment_ = true) = 0;
        
        /** 
          * Stores the fact, that the task is handled within a task.
          */ 
        virtual void set_bulk_treatment (
            bulk_treatment_state s = actual_bulk_treated) = 0;
        
        /** 
          * stores the fact, that the task is handled within a bulk, and that
          * this is done asynchronously in the adaptor.
          */
        virtual void set_external_bulk_async(
            bulk_async_in_adaptor_state s = will_async_in_adaptor) = 0;

         /*!
          * not in the facade - used for bulk-operations
          * searches an appropriate adaptor 
          * same functionality like the get_adaptor function in impl::session
          * @remark added here in task, because only the task has the
          *         necessary information to search for an appropriated adaptor
          *         by using its template parameters.
          */
        virtual v1_0::cpi*  
        get_bulk_adaptor(std::string const& cpi_name, 
            std::string const& op_name,
            v1_0::preference_type const& prefs) = 0;

        /*!
          * not in the facade - used for bulk-operations
          * executes the bulk preparation function, and passes by this way
          * its parameters to a bulk adaptor. 
          * By this way, the bulk adaptor obtains all the necessary information
          * to be able to execute the function on behalf on the original task
          * @param bulk_adaptor the bulk adaptor, who will be used to 
          *        execute the function encapsulated in this task within a bulk
          */
        virtual void visit_args(v1_0::cpi* bulk_adaptor) = 0;
#endif // SAGA_DISABLE_BULK_OPS
    };  // class task_interface

///////////////////////////////////////////////////////////////////////////////
}} // namespace saga::impl

#endif  // SAGA_IMPL_ENGINE_TASK_INTERFACE_HPP
