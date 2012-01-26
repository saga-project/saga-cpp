//  Copyright (c) 2005-2010 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_ADAPTORS_REGISTER_WORKITEM_HPP
#define SAGA_ADAPTORS_REGISTER_WORKITEM_HPP

#include <saga/saga/util.hpp>
#include <saga/saga/session.hpp>

#include <boost/date_time/posix_time/posix_time_duration.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/system/error_code.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable: 4251 4231 4660)
#endif

namespace saga { namespace adaptors
{
    ///////////////////////////////////////////////////////////////////////////
    // we use the boost::posix_time::ptime type for time representation
    typedef boost::posix_time::ptime time_type;

    // we use the boost::posix_time::time_duration type as the duration 
    // representation
    typedef boost::posix_time::time_duration duration_type;

    ///////////////////////////////////////////////////////////////////////////
    // This is the prototype of any work item to be registered with this 
    // interface. The function will be called as soon as the expiration time
    // specified while registering is reached. This function will be called at 
    // most once, so re-occurring events need to be registered separately.
    typedef TR1::function<void(boost::system::error_code const&)> 
        workitem_function;

    ///////////////////////////////////////////////////////////////////////////
    typedef unsigned int workitem_cookie_handle;

    ///////////////////////////////////////////////////////////////////////////
    // This is the main API for adaptors to register a work item to be executed
    // at a certain point in time or after a certain amount of time
    SAGA_EXPORT workitem_cookie_handle register_workitem(
        workitem_function f, time_type const& expire_at, 
        saga::session const& s = saga::detail::get_the_session());

    SAGA_EXPORT workitem_cookie_handle register_workitem(
        workitem_function f, duration_type const& expire_from_now,
        saga::session const& s = saga::detail::get_the_session());

    ///////////////////////////////////////////////////////////////////////////
    // This is the API allowing to cancel the execution of a registered work
    // item. Note, this will call the registered function but will get passed
    // the error code 'booast::asio::error::operation_aborted'. If the 
    // referenced work item has already expired but not called yet it cannot
    // be unregistered anymore and will be called with a success code.
    SAGA_EXPORT void unregister_workitem(workitem_cookie_handle,
        saga::session const& s = saga::detail::get_the_session());
}}

#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

#endif


