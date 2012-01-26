//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <vector>
#include <string>
#include <algorithm>

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/function_output_iterator.hpp>

#include <saga/saga/exception.hpp>
#include <saga/saga/metric.hpp>

#include <saga/impl/exception.hpp>
#include <saga/impl/engine/monitorable.hpp>

/////////////////////////////////////////////////////////////////
namespace saga { namespace impl {

    /////////////////////////////////////////////////////////////////
    monitorable::monitorable ()
    {
    }

    monitorable::~monitorable (void)
    {
    }

    ///////////////////////////////////////////////////////////////////////////
    // initialization
    void monitorable::add_metric_to_metrics(saga::metric m, bool predefined)
    {
        mutex_type::scoped_lock l(mtx_);
        if (has_metric(m))
        {
            std::string name (m.get_attribute(saga::attributes::metric_name));
            SAGA_THROW("Duplicate metric: '" + name + "'.", 
                saga::AlreadyExists);
        }
        metrics.push_back(metric_list_type::value_type(m, predefined));
    }

    void monitorable::add_metrics_to_metrics(std::vector<saga::metric> const& m,
        bool predefined)
    {
        mutex_type::scoped_lock l(mtx_);
        std::for_each(m.begin(), m.end(), 
            TR1::bind(&monitorable::add_metric_to_metrics, this, 
                TR1::placeholders::_1, predefined));
    }

    // test, if metric was already added 
    bool monitorable::has_metric(std::string const& name) const
    {
        mutex_type::scoped_lock l(mtx_);
        metric_list_type::const_iterator end = metrics.end();
        for (metric_list_type::const_iterator it = metrics.begin();
            it != end; ++it)
        {
            if (name == (*it).first.get_attribute(saga::attributes::metric_name))
                return true;
        }
        return false;
    }

    // test, if metric was already added 
    bool monitorable::has_metric(saga::metric m) const
    {
        return has_metric(m.get_attribute(saga::attributes::metric_name));
    }

    ///////////////////////////////////////////////////////////////////////////
    // introspection
    struct metric_appender
    {
        typedef monitorable::metric_list_type::value_type value_type;
        
        metric_appender(std::vector<saga::metric>& v) : v_(v) {}
        void operator()(value_type v) { v_.push_back(v.first); }
        std::vector<saga::metric>& v_;
    };

    std::vector<saga::metric> 
    monitorable::list_metrics (void) const
    {
        mutex_type::scoped_lock l(mtx_);
        std::vector<saga::metric> result;
        std::copy(metrics.begin(), metrics.end(), 
            boost::make_function_output_iterator(metric_appender(result)));
        return result;
    }

    saga::metric 
    monitorable::get_metric (std::string name) const
    {
        mutex_type::scoped_lock l(mtx_);
        metric_list_type::const_iterator end = metrics.end();
        for (metric_list_type::const_iterator it = metrics.begin();
            it != end; ++it)
        {
            if (name == (*it).first.get_attribute(saga::attributes::metric_name))
                return (*it).first;
        }

        SAGA_THROW("Could not find metric: '" + name + "'.", 
            saga::DoesNotExist);
        return saga::metric();
    }
    
    // callback handling
    saga::monitorable::cookie_handle 
    monitorable::add_callback(std::string name, saga::callback f)
    {
        saga::metric m (get_metric(name));
        return m.add_callback(f);
    }
    
    void monitorable::remove_callback(std::string name, 
        saga::monitorable::cookie_handle cookie)
    {
        saga::metric m (get_metric(name));
        m.remove_callback(cookie);
    }

    /////////////////////////////////////////////////////////////////
    steerable::steerable (void)
    {
    }

    steerable::~steerable (void)
    {
    }

    bool steerable::add_metric (saga::metric m)
    {
        this->add_metric_to_metrics(m);
        return true;
    }
    
    void steerable::remove_metric (std::string name)
    {
        mutex_type::scoped_lock l(mtx_);
        metric_list_type::iterator end = metrics.end();
        for (metric_list_type::iterator it = metrics.begin();
            it != end; ++it)
        {
            if (name == (*it).first.get_attribute(saga::attributes::metric_name))
            {
                if ((*it).second)
                {
                    SAGA_THROW(
                        "Could not remove predefined metric: '" + name + "'.", 
                        saga::BadParameter);
                }
                this->metrics.erase(it);
                return;
            }
        }

        SAGA_THROW("Could not find metric: '" + name + "'.", 
            saga::DoesNotExist);
    }
    
    void steerable::fire_metric (std::string name)
    {
        saga::metric m (get_metric(name));
        m.fire();
    }

///////////////////////////////////////////////////////////////////////////////
}} // namespace saga::impl

