//  Copyright (c) 2005-2010 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_IMPL_ENGINE_MANAGE_WORKITEMS_HPP
#define SAGA_IMPL_ENGINE_MANAGE_WORKITEMS_HPP

#include <saga/saga/adaptors/register_workitem.hpp>
#include <boost/thread/mutex.hpp>

namespace saga { namespace impl
{
    ///////////////////////////////////////////////////////////////////////////
    // forward declarations only
    class workitem;
    class io_service_pool;

    ///////////////////////////////////////////////////////////////////////////
    class manage_workitems
    {
    public:
        manage_workitems();
        ~manage_workitems();

        adaptors::workitem_cookie_handle register_workitem(
            adaptors::workitem_function f, adaptors::time_type const&);
        adaptors::workitem_cookie_handle register_workitem(
            adaptors::workitem_function f, adaptors::duration_type const&);
        void unregister_workitem(adaptors::workitem_cookie_handle h);

        void tidy();

    protected:
        void delete_workitem(adaptors::workitem_cookie_handle h);

    private:
        typedef boost::recursive_mutex mutex_type;
        mutable mutex_type mtx_;

        typedef std::map<
            adaptors::workitem_cookie_handle, TR1::shared_ptr<workitem> > 
        workitem_map_type;

        io_service_pool* services_;
        workitem_map_type workitems_;

        adaptors::workitem_cookie_handle counter_;
    };
}}

#endif


