//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2006 Stephan Hirmer (shirmer@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <ctime>

// lexical_cast will be in TR2
#include <boost/lexical_cast.hpp>

#include <saga/saga/object.hpp>
#include <saga/saga/task.hpp>
#include <saga/saga/attribute.hpp>
#include <saga/saga/adaptors/metric.hpp>
#include <saga/saga/adaptors/task_declaration.hpp>
#include <saga/impl/engine/task_base.hpp>
#include <saga/impl/engine/proxy.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl 
{
    task_base::~task_base (void) 
    {
    }
    
    task_base::task_base(saga::task_base::state s /*= task_base::New*/)
      : saga::impl::object(saga::object::Task),
        found_exception_      (false),
        found_saga_exception_ (false),
        saved_exception_error_((saga::error)adaptors::Success),
        is_external_async     (false),
        is_bulk_treated       (not_bulk_treated),
        is_external_bulk_async(not_async_in_adaptor)
    {
        if (s == task_base::Done) {
            get_state_func = &task_base::get_state_done;
        }
        else if (s == task_base::Failed) {
            get_state_func = &task_base::get_state_failed;
        }
        else {
            get_state_func = &task_base::get_state_task;
            create_state_metric(s);
        }
    }
    
    task_base::task_base (std::string const & func_name,
                TR1::shared_ptr <v1_0::cpi> cpi_instance, impl::proxy* prxy,
                task_base::state s) 
        : saga::impl::object(saga::object::Task),
          func_name_            (func_name),
          cpi_instance_         (cpi_instance),
          proxy_                (prxy->shared_from_this()),
          found_exception_      (false),
          found_saga_exception_ (false),
          saved_exception_error_((saga::error)adaptors::Success),
          is_external_async     (false),
          is_bulk_treated       (not_bulk_treated),
          is_external_bulk_async(not_async_in_adaptor)
    {
        if (s == task_base::Done) {
            get_state_func = &task_base::get_state_done;
        }
        else if (s == task_base::Failed) {
            get_state_func = &task_base::get_state_failed;
        }
        else {
            get_state_func = &task_base::get_state_task;
            create_state_metric(s);
        }
    }

    saga::session& task_base::get_session()
    {
        return proxy_->get_session(); 
    }
    saga::session const& task_base::get_session() const
    {
        return proxy_->get_session(); 
    }

    void task_base::create_state_metric(saga::task_base::state s)
    {
        saga::metric state (runtime::get_object(
                TR1::static_pointer_cast<saga::impl::object>(proxy_)), 
            saga::metrics::task_state,
            "Metric to monitor the current state of the task, "
                "e.g. New, Running, Canceled, Done, or Failed",
            attributes::metric_mode_readonly,
            "1", 
            attributes::metric_type_enum,
            boost::lexical_cast <std::string> (s));

        this->add_metric_to_metrics(state);
    }

    /**
      * In general, there are two cases: 
      * 1. the adaptor provides a synchronous function
      * 2. the adaptor provides a asynchronous function.
      *
      * In the first case:
      *    hence, the task itself provides the asynchronity
      *    by creating a new thread to run the adaptor function therein
      *    Then, the task is able to return information about
      *    its state by itself.
      * In the second case:
      *    the asynchronity is provided by the adaptor, so the task
      *    has no information about the current execution status
      *    of the function it executes.
      *    The only thing, it can do, is to ask the adaptor for this
      *    information and to pass it back to the caller.
      */
    saga::task_base::state task_base::get_state (void) const
    {
        mutex_type::scoped_lock l(this->saga::impl::object::mtx_);
        return (this->*get_state_func)();
    }

    saga::task_base::state task_base::get_state_running (void) const
    {
        return saga::task_base::Running;
    }

    saga::task_base::state task_base::get_state_done (void) const
    {
        return saga::task_base::Done;
    }

    saga::task_base::state task_base::get_state_failed (void) const
    {
        return saga::task_base::Failed;
    }

    saga::task_base::state task_base::get_state_adaptor (void) const
    {
        BOOST_ASSERT(is_external_async || 
            (is_bulk_treated && bulk_async_in_adaptor == is_external_bulk_async));
        return cpi_instance_->get_state(this->get_id());
    }

    saga::task_base::state task_base::get_state_task (void) const
    {
        saga::metric state (this->get_metric(saga::metrics::task_state));
        return (saga::task_base::state) boost::lexical_cast <int>(
            state.get_attribute (saga::attributes::metric_value));
    }

    std::string task_base::get_func_name(void) const
    {
        mutex_type::scoped_lock l(this->saga::impl::object::mtx_);
        return func_name_;
    }

    ///////////////////////////////////////////////////////////////////////////
    v1_0::cpi* task_base::get_bulk_adaptor(std::string const& cpi_name,
                      std::string const& op_name,
                      v1_0::preference_type const& prefs)
    { 
        SAGA_THROW("Do not call get_bulk_adaptor() on 'task_base' class!", 
            saga::NotImplemented);
        return NULL;
    }

    void task_base::visit_args(v1_0::cpi* bulk_adaptor)
    {
        SAGA_THROW("Do not call visit_args() on 'task_base' class!", 
            saga::NotImplemented);
    }

    int task_base::run (void)
    { 
        SAGA_THROW("Do not call run() on 'task_base' class!", 
            saga::NotImplemented);
        return 0;
    }

    saga::uuid task_base::get_id() const 
    { 
        SAGA_THROW("Do not call get_id() on 'task_base' class!", 
            saga::NotImplemented);
        return saga::uuid();
    }

    bool task_base::state_changed(saga::metric m, boost::condition& c)
    {
        mutex_type::scoped_lock l(this->saga::impl::object::mtx_);
        int state = boost::lexical_cast<int>(
            m.get_attribute(saga::attributes::metric_value));
        if (state != saga::task_base::Running)
            c.notify_one();                         // break timed wait
        return state == saga::task_base::Running;   // call again as long as we're running
    }

    bool task_base::wait (double timeout)
    {
        // TODO HARTMUT: timeout in futures
        
        // use a consistent value throughout this routine
        saga::task_base::state state = this->get_state();
        
        if ( (saga::task_base::New == state) 
              && (!is_bulk_treated) && (!is_external_async) )
        {
            SAGA_THROW("task not running, yet: is still pending!",
                saga::IncorrectState);
        }

        // if the task has finished, we return immediately
        if (saga::task_base::Done == state ||
            saga::task_base::Canceled == state ||
            saga::task_base::Failed == state)
        {
            return true;
        }

#if !defined(SAGA_DISABLE_BULK_OPS)
        // if there is bulk treatment applied to this task and if
        // this bulk treatment is implemented asynchronously in the
        // bulk adaptor, we wait for this task to finish
        // and set its state to DONE.
        if (is_bulk_treated && 
            bulk_async_in_adaptor == is_external_bulk_async &&
            cpi_instance_->wait(timeout, this->get_id()) )
        {
            // FIXME: this is correct only for timeout < 0
            if (actual_bulk_treated == is_bulk_treated)
                set_state(saga::task_base::Done);
            return true;            
        }
#endif

        // if the adaptor provides the asynchronism, we ask him to
        // wait for the task.
        if (is_external_async && !is_bulk_treated &&
            cpi_instance_->wait(timeout, this->get_id()) )
        {
            // FIXME: this is correct only for timeout < 0
            if (actual_bulk_treated == is_bulk_treated)
                set_state(saga::task_base::Done);
            return true;
        }

        // --> 1st case: wait until the task has finished
        if (timeout < 0.0)
        {
            // blocking call, in order to wait for the thread to finish!
            thread_();
            return true;
        }

        // --> 2nd case: wait for timeout seconds and return the finished tasks
        if (timeout > 0.0)
        {
            mutex_type::scoped_lock l(this->saga::impl::object::mtx_);
            if (this->has_metric(saga::metrics::task_state)) {
                boost::condition c;

                // set up metric callback
                saga::monitorable::cookie_handle h = 
                    this->add_callback(saga::metrics::task_state, 
                        boost::bind(&task_base::state_changed, this, _2, boost::ref(c)));

                // wait for the timeout or the state to change from Running
                boost::xtime xt;
                boost::xtime_get(&xt, boost::TIME_UTC);
                xt.nsec += boost::xtime::xtime_nsec_t(timeout * 1e9);

                // remove callback if timed out
                c.timed_wait(l, xt);
                this->remove_callback(saga::metrics::task_state, h);
            }
            else {
                // metric is unknown, we should be in Done state
                BOOST_ASSERT(saga::task_base::Done == this->get_state());
            }
        }

        // timeout == 0.0, or after waiting for timeout
        if (saga::task_base::Running != this->get_state()) 
            return true;   // the task is already in the Done state

        return false;
    } // end wait

    bool task_base::restart() 
    {
        SAGA_THROW("Do not call restart() on 'task_base' class!", 
            saga::NotImplemented);
        return false;
    }

    void task_base::rethrow (void) const
    {
        mutex_type::scoped_lock l(this->saga::impl::object::mtx_);
        if (selector_state_) {
            selector_state_->rethrow();
        }
        else {
            if (found_saga_exception_) {
                if (saved_exception_list_.empty()) {
                    SAGA_THROW_PLAIN(saved_exception_object_, 
                        saved_exception_message_, saved_exception_error_);
                }
                else {
                    SAGA_THROW_PLAIN_LIST_EX(saved_exception_object_, 
                        saved_exception_list_, saved_exception_error_);
                }
            }
            else if (found_exception_) {
                throw saved_exception_;
            }
        }
    }

    void task_base::cancel (void)
    {
        // TODO HARTMUT: provide cancel on future
        //
        // Short description what is happening without cancel!
        // ->  the current function call keeps going until it finishes by its 
        //     own with success or until it is aborted because of exceptions 
        //     etc.
        // ->  towards application, it seems that the task is canceled, but it 
        //     is not, so problems might arise because of the application 
        //     changing variables used by the still running task, or even the 
        //     other way round
        // ->  etc. etc.
        //
        // Conclusion: a real cancel operation, killing the thread seems to be
        //                   necessary! => thread safety, etc
        // FIXME: use attrib!
        // TODO: consider bulk treatment while implementing cancel.
        
        // if the adaptor handles the asynchronism by itself,
        // we ask him to finish the execution of this task.
        if (is_external_async)
        {
            cpi_instance_->cancel(this->get_id());
            return;
        }

        // change task state and promote the new value to the listeners
        set_state(saga::task_base::Canceled);
    }

    saga::object task_base::get_object() const
    {
        return runtime::get_object(
            TR1::static_pointer_cast<saga::impl::object>(proxy_));
    }

    void task_base::set_state(saga::task_base::state s)
    {
        mutex_type::scoped_lock l(this->saga::impl::object::mtx_);

        // change task state and promote the new value to the listeners
        saga::adaptors::metric m(this->get_metric(saga::metrics::task_state));
        m.set_attribute (saga::attributes::metric_value, 
            boost::lexical_cast<std::string>(s));
        m.fire();
    }

    ///////////////////////////////////////////////////////////////////////////
    void task_base::set_selector_state(
        TR1::shared_ptr<impl::adaptor_selector_state> state)
    {
        selector_state_ = state;
    }

    void task_base::set_task_exception(saga::impl::object const *obj,
        saga::impl::exception_list const& l, saga::error e)
    {
        TR1::shared_ptr<saga::impl::object> impl(
            TR1::const_pointer_cast<saga::impl::object>(
                obj->shared_from_this()));

        saved_exception_object_ = runtime::get_object(impl);
        saved_exception_list_ = l.get_all_exceptions();
        saved_exception_error_ = e; 

        found_saga_exception_ = true;
    }

    ///////////////////////////////////////////////////////////////////////////
    void task_base::set_external_treatment (bool is_external_treated_/* = true*/)
    {
        mutex_type::scoped_lock l(this->saga::impl::object::mtx_);

        is_external_async = is_external_treated_;
        if (not_bulk_treated == is_bulk_treated && is_external_async)
        {
            get_state_func = &task_base::get_state_adaptor;
            return;
        }
        get_state_func = &task_base::get_state_task;
    }

    void task_base::set_bulk_treatment (bulk_treatment_state s/*=actual_bulk_treated*/)
    {
        mutex_type::scoped_lock l(this->saga::impl::object::mtx_);

        is_bulk_treated = s;

        if (actual_bulk_treated == is_bulk_treated)
        {
            get_state_func = &task_base::get_state_running;
            return;
        }

        if (not_bulk_treated == is_bulk_treated && is_external_async)
        {
            get_state_func = &task_base::get_state_adaptor;
            return;
        }

        get_state_func = &task_base::get_state_task;
    }
    
    void task_base::set_external_bulk_async(bulk_async_in_adaptor_state s /*= 
                                            will_async_in_adaptor*/)
    {
        mutex_type::scoped_lock l(this->saga::impl::object::mtx_);

        is_external_bulk_async = s;
        if (bulk_async_in_adaptor == is_external_bulk_async &&
            was_bulk_treated == is_bulk_treated)
        {
            get_state_func = &task_base::get_state_adaptor;
            return;
        }
        get_state_func = &task_base::get_state_task;
    }

    namespace adaptors
    {
        task_base::state task_state_value_to_enum(std::string const& val)
        {
            if (val == attributes::task_state_new) 
                return task_base::New;
            if (val == attributes::task_state_done) 
                return task_base::Done;
            if (val == attributes::task_state_running) 
                return task_base::Running;
            if (val == attributes::task_state_failed) 
                return task_base::Failed;
            if (val == attributes::task_state_canceled) 
                return task_base::Canceled;
            return task_base::Unknown;
        }

        std::string task_state_enum_to_value(int s)
        {
            switch(s) {
            case task_base::New:
                return attributes::task_state_new;

            case task_base::Done:
                return attributes::task_state_done;

            case task_base::Running:
                return attributes::task_state_running;

            case task_base::Failed:
                return attributes::task_state_failed;

            case task_base::Canceled:
                return attributes::task_state_canceled;

            default:
            case task_base::Unknown:
                break;
            }
            return attributes::task_state_unknown;
        }
    }

///////////////////////////////////////////////////////////////////////////////
}} // namespace saga::impl

