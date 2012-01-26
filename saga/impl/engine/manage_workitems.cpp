//  Copyright (c) 2005-2010 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga/util.hpp>
#include <saga/saga/error.hpp>
#include <saga/impl/exception.hpp>
#include <saga/impl/engine/io_service_pool.hpp>
#include <saga/impl/engine/manage_workitems.hpp>
#include <saga/impl/engine/workitem.hpp>

namespace saga { namespace impl
{
    manage_workitems::manage_workitems()
      : services_(new io_service_pool(1)), counter_(0)         // one thread for now
    {
    }

    manage_workitems::~manage_workitems()
    {
        delete services_;
        tidy();
    }

    void manage_workitems::tidy()
    {
        mutex_type::scoped_lock lock(mtx_);

        // mark all pending work items as invalid
        for (workitem_map_type::iterator it = workitems_.begin(); 
             it != workitems_.end(); ++it)
        {
            (*it).second->invalidate();
        }

        // stop all service threads
        if (services_->is_running()) 
        {
            services_->stop();
            services_->join();
            services_->clear();
        }

        for (workitem_map_type::iterator it = workitems_.begin(); 
             it != workitems_.end(); /**/)
        {
            workitem_map_type::iterator to_delete = it++;
            workitems_.erase(to_delete);
        }
    }

    ///////////////////////////////////////////////////////////////////////////
    adaptors::workitem_cookie_handle manage_workitems::register_workitem(
        adaptors::workitem_function f, adaptors::time_type const& expire_at)
    {
        mutex_type::scoped_lock lock(mtx_);

        if (!services_->is_running())
            services_->run(false);

        adaptors::workitem_cookie_handle h = ++counter_;
        TR1::shared_ptr<workitem> wi (
            new workitem(services_->get_io_service(), f, expire_at, 
                TR1::bind(&manage_workitems::delete_workitem, this, h)));

        std::pair<workitem_map_type::iterator, bool> p = workitems_.insert(
            workitem_map_type::value_type(h, wi));

        if (!p.second)
        {
            SAGA_THROW_NO_OBJECT("could not create workitem as requested",
                saga::NoSuccess);
        }

        return h;
    }

    adaptors::workitem_cookie_handle manage_workitems::register_workitem(
        adaptors::workitem_function f, 
        adaptors::duration_type const& expire_from_now)
    {
        mutex_type::scoped_lock lock(mtx_);

        if (!services_->is_running())
            services_->run(false);

        adaptors::workitem_cookie_handle h = ++counter_;
        TR1::shared_ptr<workitem> wi (
            new workitem(services_->get_io_service(), f, expire_from_now, 
                TR1::bind(&manage_workitems::delete_workitem, this, h)));

        std::pair<workitem_map_type::iterator, bool> p = workitems_.insert(
            workitem_map_type::value_type(h, wi));

        if (!p.second)
        {
            SAGA_THROW_NO_OBJECT("could not create workitem as requested",
                saga::NoSuccess);
        }

        return h;
    }

    void manage_workitems::unregister_workitem(
        adaptors::workitem_cookie_handle h)
    {
        mutex_type::scoped_lock lock(mtx_);

        if (!services_->is_running())
        {
            SAGA_THROW_NO_OBJECT("no workitems are registered", 
                saga::IncorrectState);
        }

        // now try to unregister the given work item
        workitem_map_type::iterator it = workitems_.find(h);
        if (it == workitems_.end())
        {
            SAGA_THROW_NO_OBJECT("no workitems are registered", 
                saga::BadParameter);
        }

        (*it).second->invalidate();   // cancels timer
    }

    void manage_workitems::delete_workitem(adaptors::workitem_cookie_handle h)
    {
        mutex_type::scoped_lock lock(mtx_);

        // now try to unregister the given work item
        workitem_map_type::iterator it = workitems_.find(h);
        if (it == workitems_.end())
        {
            SAGA_THROW_NO_OBJECT("no workitems are registered", 
                saga::BadParameter);
        }

        workitems_.erase(it);
    }
}}

