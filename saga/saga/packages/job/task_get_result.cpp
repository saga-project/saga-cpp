//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga/job.hpp>
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

    ///////////////////////////////////////////////////////////////////////////
    //  implement all task::get_result<> functions needed in job package
    template SAGA_JOB_PACKAGE_EXPORT saga::job::job&
    task::get_result<saga::job::job>();
    template SAGA_JOB_PACKAGE_EXPORT saga::job::job const&
    task::get_result<saga::job::job>() const;

    template SAGA_JOB_PACKAGE_EXPORT saga::job::state&
    task::get_result<saga::job::state>();
    template SAGA_JOB_PACKAGE_EXPORT saga::job::state const&
    task::get_result<saga::job::state>() const;

    template SAGA_JOB_PACKAGE_EXPORT saga::job::ostream&
    task::get_result<saga::job::ostream>();
    template SAGA_JOB_PACKAGE_EXPORT saga::job::ostream const&
    task::get_result<saga::job::ostream>() const;
    
    template SAGA_JOB_PACKAGE_EXPORT saga::job::istream&
    task::get_result<saga::job::istream>();
    template SAGA_JOB_PACKAGE_EXPORT saga::job::istream const&
    task::get_result<saga::job::istream>() const;

    template SAGA_JOB_PACKAGE_EXPORT saga::job::self&
    task::get_result<saga::job::self>();
    template SAGA_JOB_PACKAGE_EXPORT saga::job::self const&
    task::get_result<saga::job::self>() const;

    template SAGA_JOB_PACKAGE_EXPORT saga::job::description&
    task::get_result<saga::job::description>();
    template SAGA_JOB_PACKAGE_EXPORT saga::job::description const&
    task::get_result<saga::job::description>() const;

    template SAGA_JOB_PACKAGE_EXPORT saga::job::service&
    task::get_result<saga::job::service>();
    template SAGA_JOB_PACKAGE_EXPORT saga::job::service const&
    task::get_result<saga::job::service>() const;

    ///////////////////////////////////////////////////////////////////////////////
}
