//  Copyright (c) 2005 Stephan Hirmer (shirmer@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_IMPL_ENGINE_TASK_BASE_HPP
#define SAGA_IMPL_ENGINE_TASK_BASE_HPP

#include <string>
#include <vector>
#include <ctime>
#include <cerrno>
#include <iostream>

#include <sys/types.h>

// lexical_cast will be in TR2
#include <boost/lexical_cast.hpp>
#include <boost/futures.hpp>
#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>

#include <saga/saga/util.hpp>
#include <saga/saga/uuid.hpp>
#include <saga/saga/task_base.hpp>
#include <saga/saga/exception.hpp>

#include <saga/impl/config.hpp>
#include <saga/impl/object.hpp>
#include <saga/impl/engine/cpi.hpp>
#include <saga/impl/attribute.hpp>
#include <saga/impl/engine/task_interface.hpp>
#include <saga/impl/engine/monitorable.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl {

    /**
      * Base class for the template'd task classes
      *
      * introduced in order to keep templates out of facade.
      * encapsulates all logic which is independent from the
      * exact signature of function stored in the task.
      *
      */
    class task_base 
      : public saga::task_base,
        public saga::impl::object,
        public saga::impl::task_interface,
        public saga::impl::monitorable
    {
    protected:
        /** 
          *  the name of the function encapsulated in the task
          *  used for bulk operations
          */
        std::string func_name_;

        /**
          */
        saga::detail::hold_any retval_;
        
        /**
          * Helper function to create the state metric
          */ 
        SAGA_EXPORT void create_state_metric(saga::task_base::state s);
        
    protected:
        typedef saga::impl::object::mutex_type mutex_type;
        /**
          * the thread handle
          */
        boost::futures::simple_future<int> thread_;

        /**
          * a pointer to the cpi instance, executing the encapsulated function.
          */
        TR1::shared_ptr<v1_0::cpi> cpi_instance_;

        // a shared pointer to the adaptor selector state associated with this 
        // task
        TR1::shared_ptr<impl::adaptor_selector_state> selector_state_;

        /**
          * a pointer to the proxy representing the API object this task is 
          * created from.
          */
        TR1::shared_ptr<proxy> proxy_;

        saga::task_base::state (task_base::* get_state_func)() const;

        saga::task_base::state get_state_running (void) const;
        saga::task_base::state get_state_done (void) const;
        saga::task_base::state get_state_failed (void) const;
        saga::task_base::state get_state_adaptor (void) const;
        saga::task_base::state get_state_task (void) const;

        /**
         * shows if the thread has thrown a exception during its run.
         * if somebody call a tasks function, and if an exception has
         * occurred in the thread, the caught exception gets re-thrown.
         *
         */
        bool found_exception_;

        /**
          * the exception, perhaps thrown from the thread
          */
        std::exception saved_exception_;

        /**
         * shows if the thread has thrown a saga::exception during its run.
         * if somebody call a tasks function, and if an exception has
         * occurred in the thread, the caught exception gets re-thrown.
         *
         */
        bool found_saga_exception_;

        /**
          * the exception, perhaps thrown from the thread
          */
        saga::object saved_exception_object_;
        std::vector<saga::exception> saved_exception_list_;
        std::string saved_exception_message_;
        saga::error saved_exception_error_;

        /**
          * indicates, if the adaptor will provide asynchronism.
          * needed for monitoring of the task.
          */
        bool is_external_async;

        /**
          * shows the actual bulk treatment state of the task.
          */
        bulk_treatment_state is_bulk_treated;

        /**
          * shows the actual bulk treatment state of the task,
          * if the asynchronism is in the adaptor.
          */
        bulk_async_in_adaptor_state is_external_bulk_async;

        /**
          * constructor
          */
        SAGA_EXPORT task_base (std::string const & func_name,
                    TR1::shared_ptr <v1_0::cpi> cpi_instance,
                    impl::proxy* prxy, task_base::state s = task_base::New);

        bool state_changed(saga::metric m, boost::condition& c);

        /**
         * exception safe way to set the current state of the task.
         * is created on the stack in the beginning of the thread
         */                                            
        struct state_setter 
        {    
            task_base& t_;
            task_base::state state_;

            state_setter (task_base& t, 
                    task_base::state state = task_base::Failed)
            :   t_(t), state_(state)
            {
            }

            ~state_setter()
            {
                t_.set_state(state_);   // set new task state, propagate value
            }
        };

        ///////////////////////////////////////////////////////////////////////
        // returns the result associated with this task
        SAGA_EXPORT virtual saga::detail::hold_any& get_result() 
        {
            if (!wait(-1.0))
            {
                SAGA_THROW("task not Done, and wait() failed, can't retrieve result!",
                    saga::IncorrectState);
            }
            return retval_;
        }

    public:
        /**
         * Default destructor
         */
        SAGA_EXPORT virtual ~task_base (void);

        /** 
          * Creates Dummy task
          */
        task_base(saga::task_base::state s = task_base::New);

        /**
          * returns the function name of the operation encapsulated
          * in this task
          */
        SAGA_EXPORT virtual std::string get_func_name(void) const;

        /**
         * returns the session, to which the task belongs.
         */
        SAGA_EXPORT virtual saga::session& get_session();
        SAGA_EXPORT virtual saga::session const& get_session() const;

         /*!
          * not in the facade - used for bulk-operations
          * searches an appropriate adaptor 
          * same functionality like the get_adaptor function in impl::session
          * @remark added here in task, because only the task has the
          *         necessary information to search for an appropriated adaptor
          *         by using its template parameters.
          */
        SAGA_EXPORT virtual v1_0::cpi*  
        get_bulk_adaptor(std::string const& cpi_name,
                         std::string const& op_name,
                         v1_0::preference_type const& prefs);

        /*!
          * not in the facade - used for bulk-operations
          * executes the bulk preparation function, and passes by this way
          * its parameters to a bulk adaptor. 
          * By this way, the bulk adaptor obtains all the necessary information
          * to be able to execute the function on behalf on the original task
          * @param bulk_adaptor the bulk adaptor, who will be used to 
          *        execute the function encapsulated in this task within a bulk
          */
        SAGA_EXPORT virtual void visit_args(v1_0::cpi* bulk_adaptor);

        SAGA_EXPORT virtual int run (void);

        /**
          * Blocking call for waiting until the function ran in task::run(void) will finish.
          * 
          * @return 1 if the task is finished, 0 otherwise
          * 
          * @throw still_canceled exception
          */
        SAGA_EXPORT virtual bool wait (double timeout);

        /**
          * re-throw any catch'ed exceptions
          */
        SAGA_EXPORT virtual void rethrow (void) const;

        /**
          * @return the current state of the task.
          */
        SAGA_EXPORT virtual saga::task_base::state get_state (void) const;

        /**
          * not in the facade
          * used to change the state of a task, executed in a bulk
          */
        SAGA_EXPORT virtual void set_state(saga::task_base::state s);

        /**
          * not in the facade
          * used to handle semantic, if this task is handled within a bulk
          * towards the application, the task is normally running
          * but is actually treated in special bulk handling
          */
        SAGA_EXPORT virtual void set_external_treatment (bool block_for_bulk_treatment_ = true);

        /** 
          * Stores the fact, that the task is handled within a task.
          */ 
        SAGA_EXPORT virtual void set_bulk_treatment (bulk_treatment_state s = actual_bulk_treated);

        /** 
          * stores the fact, that the task is handled within a bulk, and that
          * this is done asynchronously in the adaptor.
          */
        SAGA_EXPORT virtual void set_external_bulk_async(bulk_async_in_adaptor_state s = 
                                                         will_async_in_adaptor);

        /**
          * Cancels the running task.
          */
        SAGA_EXPORT virtual void cancel (void);

        /**
          * returns the saga object associated with this task
          */
        SAGA_EXPORT virtual saga::object get_object() const;

        /** 
          * interrupt execution for given number of microseconds
          *
          * @param microsec: number of microseconds to sleep
          */
        SAGA_EXPORT static void sleep (int microsec)
        {
            boost::xtime xt;
            boost::xtime_get(&xt, boost::TIME_UTC);
            xt.nsec += microsec * 1000;

            boost::thread::sleep(xt);
        }

        SAGA_EXPORT virtual saga::uuid get_id() const;

        // set the adaptor selector state to be used with this instance
        SAGA_EXPORT void set_selector_state(
            TR1::shared_ptr<impl::adaptor_selector_state>);

        // store an exception in a task instance
        SAGA_EXPORT void set_task_exception(saga::impl::object const *obj, 
            saga::impl::exception_list const& l, saga::error e);

        // try to restart operation using the next adaptor
        SAGA_EXPORT bool restart();

    }; // class task_base

///////////////////////////////////////////////////////////////////////////////
}} // namespace saga::impl

#endif  // SAGA_IMPL_ENGINE_TASK_BASE_HPP
