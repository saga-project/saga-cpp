//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2006 Stephan Hirmer (shirmer@cct.lsu.edu)
//  Copyright (c) 2007 Ole Weidner (oweidner@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_TASK_HPP
#define SAGA_TASK_HPP

// include dependent spec sections
#include <saga/saga/base.hpp>
#include <saga/saga/util.hpp>
#include <saga/saga/task_base.hpp>
#include <saga/saga/object.hpp>
#include <saga/saga/error.hpp>

#include <saga/saga/uuid.hpp>
#include <saga/saga/detail/monitorable.hpp>
#include <saga/saga/detail/hold_any.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable: 4251 4231 4275 4660)
#endif

///////////////////////////////////////////////////////////////////////////////
namespace saga 
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @cond 
    namespace metrics 
    {
        ///////////////////////////////////////////////////////////////////////////
        //  metric names for task
        
        char const* const task_state = "task.state";
        
    }
    /// @endcond

    /*! \brief Brief %description starts here
     *
     *   Operations performed in highly heterogeneous distributed environments may
     * take a long time to complete, and it is thus desirable to have the ability to 
     * perform operations in an asynchronous manner. The SAGA task model provides this
     * ability to all other SAGA classes.
     */
    class SAGA_EXPORT task 
      : public object,
        public saga::detail::monitorable<task>,
        public task_base
    {
    private:
        /// @cond
        /** These methods are not within API scope */
        friend struct saga::impl::runtime;
        friend struct saga::detail::monitorable<task>;  // needs to access get_impl()
        /// @endcond

    protected:
        /// @cond
        /** These methods are not within API scope */
        TR1::shared_ptr <saga::impl::task_base> get_impl_sp(void) const;
        saga::impl::task_base* get_impl (void) const;

        saga::impl::task_interface* get_task_if (void);
        saga::impl::task_interface const* get_task_if (void) const;
        typedef saga::detail::monitorable<task> monitorable_base;

        explicit task (saga::impl::object* init);
        explicit task (saga::object const& o);
        task &operator= (saga::object const& o);
        /// @endcond

    public:
        /*! \brief Brief %description starts here
         *
         *
         */
        typedef task_base::state state;
        typedef task_base::Sync Sync;
        typedef task_base::Async Async;
        typedef task_base::Task Task;

        task();

        /*! \brief Brief %description starts here
         *
         *
         */
        explicit task (saga::task_base::state t);

        /*! \brief Destroys the object.
         *
         *
         */
        ~task ();

        /*! \brief Brief %description starts here     
         *
         *
         */
        friend SAGA_EXPORT 
        bool operator== (task const & lhs, task const & rhs);

        /*! \brief Brief %description starts here     
         *
         *
         */
        friend SAGA_EXPORT 
        bool operator< (task const & lhs, task const & rhs);

        /*! \brief Starts the asynchronous operation.      
         *
         *
         */
        void run(void);

        /*! \brief Cancels the asynchronous operation.     
         * \param timeout Time for freeing resources.
         *
         *
         */
        void cancel(void);

        /*! \brief Waits for the task to finish.      
         * \param timeout Second to wait
         * \return Indicating if the task is done running.
         *
         *
         */
        bool wait(double timeout = -1.0);

        /*! \brief Gets the state of the task. 
         * \return State of the task.
         *
         *
         */
        state get_state();

        /*! \brief Re-throws any exception a failed task caught. 
         *
         *
         */
        void rethrow();

        /*! \brief Gets the object from which this task was created 
         * \return Object this task was created from.
         *
         *
         */
        saga::object get_object() const;

        /*! \brief Gets the result of the asynchronous operation 
         * \return Return value of async method.
         *
         *
         */
        template <typename Retval>
        Retval& get_result();

        template <typename Retval>
        Retval const& get_result() const;

        /// Synchronization for API functions with no return value only
        void get_result();
        void get_result() const;
    };

    namespace detail 
    {
        /////////////////////////////////////////////////////////////////////////////
        // some global functions to simplify the implementation of sync/async/task
        // versions of method calls

        /// @cond
        /** These methods are not within API scope TODO: wrong namespace?!?*/
        SAGA_EXPORT inline saga::task run (saga::task t) 
        { 
            t.run(); 
            return t; 
        }

        SAGA_EXPORT inline saga::task wait (saga::task t, double timeout = -1.0) 
        { 
            t.wait (timeout); 
            return t; 
        }

        SAGA_EXPORT inline saga::task run_wait (saga::task t, double timeout = -1.0) 
        { 
            if (saga::task_base::New == t.get_state()) 
            {
                t.run(); 
                t.wait(timeout); 
            }
            return t; 
        }

        ///////////////////////////////////////////////////////////////////////
        //  Helper function to extract task result from implementation without
        //  exposing any details
        SAGA_EXPORT detail::hold_any& get_task_result(saga::task t);

        ///////////////////////////////////////////////////////////////////////
        // store the selector state to be used with this task instance
        SAGA_EXPORT void set_selector_state(saga::task t, 
            TR1::shared_ptr<impl::adaptor_selector_state> state);

        ///////////////////////////////////////////////////////////////////////
        // store an exception in a task instance
        SAGA_EXPORT saga::task set_task_exception(saga::task t, 
            saga::impl::object const *obj, saga::impl::exception_list const& l,
            saga::error e);
        /// @endcond
    }

    ///////////////////////////////////////////////////////////////////////////////
} // namespace saga

// re-enable warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

#endif // SAGA_TASK_HPP

