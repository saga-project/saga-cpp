//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2008 Andre Merzky (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga/cpr.hpp>
#include <saga/saga/adaptors/task.hpp>
#include <saga/saga/detail/task_get_result_impl.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga
{
    /// @cond 
    /* BEGIN: Exclude from Doxygen */
    namespace detail
    {
        template <> struct disable_reconvert<saga::job::state> : boost::mpl::true_ {};
        template <> struct disable_reconvert<saga::job::ostream> : boost::mpl::true_ {};
        template <> struct disable_reconvert<saga::job::istream> : boost::mpl::true_ {};
    }
    /// @endcond 
    /* END: Exclude from Doxygen */

    //  implement all task::get_result<> functions needed in cpr package
    template SAGA_CPR_PACKAGE_EXPORT saga::cpr::job&
    task::get_result<saga::cpr::job>();
    template SAGA_CPR_PACKAGE_EXPORT saga::cpr::job const&
    task::get_result<saga::cpr::job>() const;

    template SAGA_CPR_PACKAGE_EXPORT saga::cpr::checkpoint&
    task::get_result<saga::cpr::checkpoint>();
    template SAGA_CPR_PACKAGE_EXPORT saga::cpr::checkpoint const&
    task::get_result<saga::cpr::checkpoint>() const;

    template SAGA_CPR_PACKAGE_EXPORT saga::cpr::directory&
    task::get_result<saga::cpr::directory>();
    template SAGA_CPR_PACKAGE_EXPORT saga::cpr::directory const&
    task::get_result<saga::cpr::directory>() const;

    template SAGA_CPR_PACKAGE_EXPORT saga::cpr::description&
    task::get_result<saga::cpr::description>();
    template SAGA_CPR_PACKAGE_EXPORT saga::cpr::description const&
    task::get_result<saga::cpr::description>() const;

    template SAGA_CPR_PACKAGE_EXPORT saga::cpr::self& 
    task::get_result<saga::cpr::self>();
    template SAGA_CPR_PACKAGE_EXPORT saga::cpr::self const&
    task::get_result<saga::cpr::self>() const;

    template SAGA_CPR_PACKAGE_EXPORT saga::cpr::service&
    task::get_result<saga::cpr::service>();
    template SAGA_CPR_PACKAGE_EXPORT saga::cpr::service const&
    task::get_result<saga::cpr::service>() const;
}

