//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2006 Stephan Hirmer (shirmer@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <cerrno>
#include <utility>
#include <ctime>
#include <list>

#include <saga/saga/util.hpp>
#include <saga/saga/exception.hpp>
#include <saga/saga/adaptors/task_declaration.hpp>

#include <saga/impl/runtime.hpp>
#include <saga/impl/config.hpp>
#include <saga/impl/session.hpp>

#include <saga/impl/engine/bulk_analyser.hpp>
#include <saga/impl/engine/bulk_analyser_impr.hpp>

#include <saga/impl/engine/task_container.hpp>

#include <saga/impl/engine/proxy.hpp>

#include <saga/saga/adaptors/bulk_strategy_simple_sort.hpp>
#include <saga/saga/adaptors/bulk_strategy_try_exec.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga 
{ 
  namespace impl 
  {
    task_container::task_container (void)
    :   object(saga::object::TaskContainer)
    {
    }
    
    task_container::~task_container (void) 
    {
//         if(NULL != bs.get())
//             bs->complete_wait(); 
    }
    
    // According to the strawman API, this function throws nothing!
    // perhaps it should throw AllreadyAdded ?
    void task_container::add_task (saga::task & t)
    {
      task_list.push_back (t);
    }

    void task_container::remove_task (saga::task t)
    {
      tasks_type::iterator task_iter = task_list.begin ();

      // searching through the list of tasks
      // and comparing the 'real' task-object (impl).
      // see task.hpp bottom for further details.

      while ( task_iter != task_list.end () )
      {
        if ( *task_iter == t ) 
        {
          task_list.erase (task_iter);
          return;
        }

        ++task_iter;
      }

      SAGA_THROW("task_container: The task to remove was not found!",
        saga::DoesNotExist); 
    }

    std::vector<saga::task> const& task_container::remove_returned_tasks(
        std::vector<saga::task> const& ret)
    {
        tasks_type::const_iterator end = ret.end();
        for (tasks_type::const_iterator it = ret.begin(); it != end; ++it)
        {
            remove_task(*it);
        }
        return ret;
    }

    std::vector<saga::task> 
    task_container::wait (saga::task_container::wait_mode mode, float timeout) 
    {
//       if(NULL != bs.get())
//         bs->complete_wait();
      
      mutex_type::scoped_lock l(this->saga::impl::object::mtx_);
      size_t task_list_size = task_list.size();

      if (0 == task_list_size) {
          SAGA_THROW("This container does not contain any tasks!",
            saga::DoesNotExist);
      }

      tasks_type ret = get_tasks_with_state (saga::task_base::Canceled);
      if (ret.size() == task_list_size) {
          SAGA_THROW("All tasks in this container have been canceled!",
            saga::IncorrectState);
      }

      // get the finished tasks
      ret = get_tasks_with_state (saga::task_base::Done);

      // if already all the tasks have finished, we return immediately
      if (ret.size() == task_list_size) {
          task_list.clear();
          return ret;
      }

      // --> 1st case: wait until all tasks have finished
      if (timeout < 0)
      {
          if (saga::task_container::All == mode) {
              wait_for_all_tasks_to_finish();
              ret = task_list;
              task_list.clear();
              return ret;
          }

          BOOST_ASSERT(saga::task_container::Any == mode);

          do {
              wait_for_one_task_to_finish();
              ret = get_tasks_not_with_state (saga::task_base::Running);
          } while (ret.empty() && !task_list.empty());

          return remove_returned_tasks(ret);
      }

      // --> 2nd case: wait for timeout seconds and return the finished
      // tasks
      if (timeout > 0.0)
      {
        std::time_t start_time = std::time (0);

        while ( (timeout - (std::difftime (std::time (0), start_time))) > 0 )
        {
          ret = get_tasks_with_state (saga::task_base::Done);

          if (saga::task_container::All == mode) {
            if (ret.size() == task_list_size) {
              task_list.clear();
              break;
            }
          }
          else {
            BOOST_ASSERT(saga::task_container::Any == mode);
            if (!ret.empty()) {
              remove_returned_tasks(ret);
              break;
            }
          }

          // we consider 5msec as a good slice
          task_base::sleep (5);
        }
        return ret;
      }

      // --> 3rd case: timeout = 0.0 then return immediately
      ret = get_tasks_with_state (saga::task_base::Done);
      return remove_returned_tasks(ret);
    }

    std::vector<saga::task> 
    task_container::get_tasks_with_state (saga::task_base::state state) 
    {
        tasks_type ret;

        iterator_type end = task_list.end();
        for (iterator_type it = task_list.begin(); it != end; ++it)
        {
            if (it->get_state () == state)
            {
                ret.push_back (*it);
            }
        }
        return ret;
    }

    std::vector<saga::task> 
    task_container::get_tasks_not_with_state (saga::task_base::state state) 
    {
        tasks_type ret;

        iterator_type end = task_list.end();
        for (iterator_type it = task_list.begin(); it != end; ++it)
        {
            if (it->get_state() != state)
            {
                ret.push_back (*it);
            }
        }
        return ret;
    }

    void task_container::wait_for_all_tasks_to_finish (void) 
    {
        // bs->complete_wait();
        std::for_each(task_list.begin(), task_list.end(), 
            TR1::bind(&saga::task::wait, TR1::placeholders::_1, -1.0));
    }

    bool task_container::state_changed(saga::metric m, boost::condition& c, 
        boost::mutex& mtx)
    {
        // make sure, this gets executed only after wait has been entered
        boost::mutex::scoped_lock l(state_change_mtx);

        int state = boost::lexical_cast<int>(
            m.get_attribute(saga::attributes::metric_value));
        if (state != saga::task_base::Running)
            c.notify_one();                         // break wait
        return state == saga::task_base::Running;   // call again as long as we're running
    }

    struct handle_map 
      : std::list<std::pair<saga::task, saga::monitorable::cookie_handle> >
    {
        typedef std::pair<saga::task, saga::monitorable::cookie_handle> value_type;
        typedef std::list<value_type> handle_list_type;

        ~handle_map()
        {
            // remove callback on all tasks 
            handle_list_type::iterator mapend = this->end();
            for (handle_list_type::iterator mit = this->begin(); 
                 mit != mapend; ++mit)
            {
                try {
                    (*mit).first.remove_callback(saga::metrics::task_state, 
                        (*mit).second);
                }
                catch (saga::exception const&) {
                    /* ignore errors here */;
                }
            }
        }
    };

    void task_container::wait_for_one_task_to_finish()
    {
        // set up metric callback for all tasks
        handle_map handles;
        {
            boost::mutex::scoped_lock l(state_change_mtx);

            tasks_type::iterator end = task_list.end();
            for (tasks_type::iterator it = task_list.begin(); it != end; ++it)
            {
                if (runtime::get_task_impl(*it)->has_metric(saga::metrics::task_state)) 
                {
                    saga::monitorable::cookie_handle h = (*it).add_callback(
                        saga::metrics::task_state, boost::bind(
                            &task_container::state_changed, this, _2, 
                            boost::ref(state_change_cond), 
                            boost::ref(state_change_mtx)));

                    typedef handle_map::value_type value_type;
                    handles.push_back(value_type(*it, h));
                }
                else {
                    // metric is unknown, we should be in Done state
                    BOOST_ASSERT(saga::task_base::Done == (*it).get_state());
                }
            }

            // wait for one task to change state from Running (no timeout)
            if (!task_list.empty()) {
                boost::xtime xt;
                boost::xtime_get(&xt, boost::TIME_UTC);
                xt.nsec += boost::xtime::xtime_nsec_t(5.0e6);

                state_change_cond.timed_wait(l, xt);
            }
        }
    }

    std::vector <saga::task_base::state> task_container::get_states()
    {
        std::vector<saga::task_base::state> ret;

        iterator_type end = task_list.end();
        for (iterator_type it = task_list.begin(); it != end; ++it)
        {
            ret.push_back (it->get_state());
        }
        return ret;
    }
    
    void task_container::simple_run()
    {
        // tasks are now treated one by one, so we reset the bulk-treatment flag.
        set_tasks_for_bulk_treatement(saga::impl::task_base::not_bulk_treated);
        std::for_each(task_list.begin(), task_list.end(), 
            TR1::bind(&saga::task::run, TR1::placeholders::_1));
    }
    
    void task_container::set_state_for_all(saga::task_base::state s)
    {
        iterator_type end = task_list.end();
        for (iterator_type it = task_list.begin(); it != end; ++it)
        {
            runtime::get_impl(*it)->set_state(s);
        }
    }
    
    void task_container::set_state_by_uuid(std::vector<saga::uuid>& ids,
                                           saga::task_base::state s)
    {
        typedef std::vector<saga::uuid>::iterator iterator_type;
        iterator_type end = ids.end();
        for (iterator_type it = ids.begin(); it != end; ++it)
        {
            saga::task t = get_task_by_uuid(*it);
            runtime::get_impl(t)->set_state(s);
        }
    }
    
    saga::task task_container::get_task_by_uuid(saga::uuid const& id)
    {
        iterator_type end = task_list.end();
        for (iterator_type it = task_list.begin(); it != end; ++it)
        {
            if ( (*it).get_id() == id )
                return *it;
        }

        SAGA_THROW("No such uuid within current task_container",
          saga::DoesNotExist); 
        return saga::task(saga::task_base::Done);
    }
    

    void task_container::set_tasks_for_bulk_treatement(
        task_base::bulk_treatment_state s)
    {
        iterator_type end = task_list.end();
        for (iterator_type it = task_list.begin(); it != end; ++it)
        {
            runtime::get_impl(*it)->set_bulk_treatment(s);
        }
    }
    
    
    void task_container::run()
    {
        if (task_list.empty()) {
            SAGA_THROW("This container does not contain any tasks!",
              saga::DoesNotExist);
        }

        iterator_type end = task_list.end();
        for (iterator_type it = task_list.begin(); it != end; ++it)
        {
            if ((*it).get_state() == saga::task_base::New)
                (*it).run();
        }
    }
//         using namespace boost;
//         
//         saga::adaptors::task_blocker_for_bulk bulk_blocker(this);
//         
//         // pass the task to an analyzer    
//         
//         bulk_analyser::sorted_tc_type res;
//         
//         if (saga::detail::safe_getenv("SAGA_IMPR_BULK_SORTING") )
//         {
//             bulk_analyser_impr::sorted_tc_type res;
//             saga::impl::bulk_analyser_impr a;
//             a.analyse_bulk(*this, res); 
//         }      
//         else
//         {
//             saga::impl::bulk_analyser a;
//             a.analyse_bulk(*this, res); 
//         }         
//         
//                 
//         // according to chosen strategy ...
//         if (saga::detail::safe_getenv("SAGA_TRY_EXEC_BULK_HANDLING") )
//         {
//             saga::adaptors::bulk_strategy_try_exec strat;
//             strat.apply(res);
//             using namespace saga::adaptors;
//             bs = TR1::shared_ptr<bulk_strategy_try_exec>(new bulk_strategy_try_exec());
//             bs->apply(res);
//         }
//         else
//         {
//             saga::adaptors::bulk_strategy_simple_sort strat;
//             strat.apply(res);
//             using namespace saga::adaptors;
//             bs = TR1::shared_ptr<bulk_strategy_simple_sort>
//                                                 (new bulk_strategy_simple_sort());
//             bs->apply(res);
//         }
//     }

    void task_container::cancel (void)
    {
        if (task_list.empty()) {
            SAGA_THROW("This container does not contain any tasks!",
              saga::DoesNotExist);
        }

        std::for_each(task_list.begin(), task_list.end(), 
            TR1::bind(&saga::task::cancel, TR1::placeholders::_1));
    }

    std::vector<saga::task> task_container::list_tasks (void) const
    {
        return task_list;
    }

  } // namespace impl

} // namespace saga

