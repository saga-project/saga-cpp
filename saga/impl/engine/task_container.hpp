//  Copyright (c) 2005-2006 Andre   Merzky (andre@merzky.net)
//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005-2006 Stephan Hirmer (shirmer@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef TASK_CONTAINER_HPP
#define TASK_CONTAINER_HPP

#include <vector>
#include <boost/thread/condition.hpp>

#include <saga/impl/config.hpp>
#include <saga/impl/engine/task_fwd.hpp>
#include <saga/impl/engine/object.hpp>
#include <saga/saga/adaptors/bulk_strategy_base.hpp>

#include <saga/impl/engine/monitorable.hpp>
#include <saga/impl/engine/task_interface.hpp>

#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable: 4251 4231 4275 4660)
#endif

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl
{
    
    /**
      * Class to store and handle some tasks through one handle.
      */
    class task_container 
    :   public saga::impl::object,
        public saga::impl::monitorable
    {
     private:
       /*! list of all tasks added to this container */
       typedef std::vector<saga::task> tasks_type;
  
       typedef tasks_type::iterator iterator_type;
       typedef tasks_type::const_iterator const_iterator_type;

       typedef saga::impl::object::mutex_type mutex_type;

       boost::mutex state_change_mtx;
       boost::condition state_change_cond;
       tasks_type task_list;

       /**
         * searches for the tasks out of task_list, which have the 
         * wanted state.
         * 
         * @param state the state of the tasks, you are searching for.
         * 
         * @return set of tasks, which have already finished
         */
       tasks_type get_tasks_with_state (saga::task_base::state state);
       tasks_type get_tasks_not_with_state (saga::task_base::state state);

       /**
         * Waits for all tasks, stored in task_list to finish.
         */
       void wait_for_all_tasks_to_finish();
       /**
         * Waits for one task stored in task_list to finish.
         */
       void wait_for_one_task_to_finish();

       /**
        * searches and returns a task, with a given uuid
        */
       saga::task get_task_by_uuid(saga::uuid const& id);
       
       TR1::shared_ptr<saga::adaptors::bulk_strategy_base> bs;

       // call back to be invoked when a task changes its state
       bool state_changed(saga::metric m, boost::condition& c, boost::mutex& mtx);

       // remove all tasks in the parameter
       std::vector<saga::task> const& remove_returned_tasks(
          std::vector<saga::task> const& ret);

     public:
       task_container (void);
       ~task_container (void);

       /**
        * Adds a task to this container.
        * 
        * @param t the task, we want to add to this container
        */
       void add_task (saga::task & t);

       /**
        * Removes a task from this container.
        * 
        * @param t the task, we want to remove from this container
        * 
        * @throw no_such_task, if the task which you want to remove
        *        is not part of this container.
        */
       void remove_task (saga::task t);

       /**
         * Blocking function, which waits timeout sec for the tasks to 
         * finish.  If they finish all before the end of this time, the 
         * function returns immediately.
         *
         * @return a set of tasks, which have already finished
         * 
         * @param timeout number of seconds this function will wait for 
         *                tasks to finish before returning.
         * @note timeout <  0.0 : wait until all tasks have finished.
         *       timeout == 0.0 : returns set of finished tasks immed.
         *       timeout >  0.0 : wait for timeout seconds, but if task 
         *                        finish all before this time, return.
         *       
         * @throw all_tasks_canceled, if all the tasks in this container
         *        have been canceled already.
         */
       tasks_type wait (saga::task_container::wait_mode mode, float timeout);

       /**
         * @returns a set, specifying the current state of all tasks, 
         *          stored in this container.
         */
       std::vector <saga::task_base::state> get_states (void);

       /**
         * Cancels every task in this container.
         */
       void cancel (void);

       /**
         * Provides access to list of all tasks of this container.
         * 
         * @return the list of all tasks stored in this container.
         */
       tasks_type list_tasks (void) const;
       
       /**
         * executes the tasks contained in this task container.
         * tries to use bulk adaptors if available
         * if not, executes the tasks one by one
         */
       void run ();
       
       /**
         * runs the tasks in the taskcontainer one by one, without
         * considerating possible bulk adaptors. Not in the facade.
         */
       void simple_run();
       
       /**
         * sets one state to all the tasks contained in the taskcontainer
         * @param s the state to set. Not in the facade.
         */
       void set_state_for_all(saga::task_base::state s);
       
       
       /**
         * allows to set the state of a task. The task is specified by
         * its uuid. Not in the facade.
         */
       void 
       set_state_by_uuid(std::vector<saga::uuid>& ids, saga::task_base::state s);

       /**
         * not in the facade.
         *
         * sets the bulk state for all the tasks, within this container
         */
       void 
       set_tasks_for_bulk_treatement(task_interface::bulk_treatment_state s = 
          task_interface::actual_bulk_treated);       
       
        ///////////////////////////////////////////////////////////////////////
        //  return the monitorable interface to the facade 
        virtual saga::impl::monitorable* 
            get_monitorable() { return this; }
        virtual saga::impl::monitorable const* 
            get_monitorable() const { return this; }

    };

///////////////////////////////////////////////////////////////////////////////
}} // namespace saga::impl

#endif // TASK_CONTAINER_HPP

