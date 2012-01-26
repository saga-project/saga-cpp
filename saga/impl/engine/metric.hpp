//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005 Stephan Hirmer (stephan.hirmer@gmail.com)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_IMPL_ENGINE_METRIC_HPP
#define SAGA_IMPL_ENGINE_METRIC_HPP

#include <string>
#include <map>

// lexical_cast will be in TR2
#include <boost/lexical_cast.hpp>

#include <saga/saga/util.hpp>
#include <saga/saga/exception.hpp>
#include <saga/saga/metric.hpp>

#include <saga/impl/config.hpp>
#include <saga/impl/exception.hpp>
#include <saga/impl/engine/object.hpp>
#include <saga/impl/engine/attribute.hpp>
#include <saga/impl/exception_list.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl {

    /**
      * This class stores monitoring information.
      *
      * Saga adaptors or saga proxy classes are able to define metrics
      * and add them to a metric-list, which is kept by a task.
      * Therefor, there is a function cpi::addMetric (metric m), 
      * which adds a TR1::shared_ptr of a metric to this metric-list.
      * 
      * FIXME: is that naming useful?
      * @note the name of a metric has to be in the following form:
      * <metric-owner>_<-monitored-operation>[_<short-description>]
      * e.g.: standard-local-file-adaptor_FILE-COPY_progress
      */
    class metric 
      : public saga::impl::object,
        public saga::impl::attribute
    {
    private:
      typedef saga::impl::attribute attribute_base;
      typedef saga::metric::frequency frequency;
      typedef std::map<saga::metric::metric_cookie, saga::callback> 
          metric_callbacks;

      metric_callbacks callbacks_;
      TR1::weak_ptr<saga::impl::object> target_;

      // this global mutex is used to protect the cookie counter of the metric
      static boost::mutex &mutex_instance()
      {
          static boost::mutex mutex;
          return mutex;
      }
      static void mutex_init()
      {
          mutex_instance();
      }

      static saga::metric::metric_cookie get_next_cookie()
      {
          static saga::metric::metric_cookie cookie = 0;

          static boost::once_flag been_here = BOOST_ONCE_INIT;
          boost::call_once(mutex_init, been_here);
          
          boost::mutex::scoped_lock lock(mutex_instance());
          return ++cookie;
      }

      metric (saga::object target, attribute const& rhs_attr)
        : saga::impl::object(saga::object::Metric),
          attribute_base(rhs_attr),
          target_(runtime::get_impl_object_sp(target))
      {
      }
      
    public:
      //! Destructor.
      ~metric(void)
      {
      }

      metric ()
        : object(saga::object::Metric)
      {
      }

      //! verbose constructor
      explicit metric (saga::object target)
        : saga::impl::object(saga::object::Metric), 
          target_(runtime::get_impl_object_sp(target))
      {
      }

      // promote the current value to the subscribed callbacks
      void fire (saga::context ctx) 
      {
          mutex_type::scoped_lock l(mtx_);

          TR1::shared_ptr<impl::object> target_ptr = target_.lock();
          if (!target_ptr)
              return;         // return, if target object doesn't exist anymore

          saga::object target (
              runtime::get_object(TR1::shared_ptr<impl::object>(target_ptr)));
          saga::metric m (runtime::get_object(this->shared_from_this()));

          exception_list exceptions;
          metric_callbacks::iterator end = callbacks_.end();
          for (metric_callbacks::iterator it = callbacks_.begin(); it != end; /**/) 
          {
              try {
                  bool result = (*it).second(target, m, ctx);
                  if (!result) {
                      // remove this callback from the list
                      metric_callbacks::iterator next = it;
                      ++next;
                      callbacks_.erase(it);
                      it = next;
                  }
                  else {
                      ++it;
                  }
              }
              catch (saga::exception const& e) {
                  exceptions.add(e);
                  ++it;
              }
          } 

        // if this is a trigger type metric, reset value to zero
        saga::task t = attribute_exists(attributes::metric_type, true);
        if (t.get_result<bool>()) 
        {
            t = get_attribute(attributes::metric_type, true);
            if (t.get_result<std::string>() == attributes::metric_type_trigger)
            {
                set_attribute(attributes::metric_value, "0", true);
            }
        }

        // throw exceptions occurred during callbacks
        if (0 != exceptions.get_error_count())
        {
            SAGA_THROW_PLAIN_LIST(this, exceptions);
        }
      }

      saga::metric::metric_cookie 
      add_callback (saga::callback cb) 
      { 
          saga::metric::metric_cookie new_cookie = get_next_cookie();

          mutex_type::scoped_lock l(mtx_);
          callbacks_.insert(metric_callbacks::value_type(new_cookie, cb));
          return new_cookie;
      }
      
      void remove_callback(saga::metric::metric_cookie cookie) 
      { 
          mutex_type::scoped_lock l(mtx_);
          metric_callbacks::const_iterator it = callbacks_.find(cookie);
          if (it == callbacks_.end())
          {
              std::string c (boost::lexical_cast<std::string>(cookie));
              SAGA_THROW("Could not find callback: '" + c + "'.", 
                  saga::DoesNotExist);
          }
          callbacks_.erase(cookie);
      }

      // saga::object interface
      // deep copy operation
      saga::object clone() const
      {
          mutex_type::scoped_lock l(mtx_);
          saga::object target (
              runtime::get_object(TR1::shared_ptr<impl::object>(target_)));
          return saga::metric(new saga::impl::metric(target,
              static_cast<attribute_base const&>(*this)));
      }
      
      // return interface's
      saga::impl::attribute* get_attributes() { return this; }
      saga::impl::attribute const* get_attributes() const { return this; }

    };

///////////////////////////////////////////////////////////////////////////////
}} // namespace saga::impl

#endif // SAGA_IMPL_ENGINE_METRIC_HPP

