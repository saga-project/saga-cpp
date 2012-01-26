//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga/context.hpp>
#include <saga/saga/stream.hpp>
#include <saga/saga/adaptors/task.hpp>
#include <saga/saga/detail/task_get_result_impl.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga
{
    /// @cond 
    /* BEGIN: Exclude from Doxygen */
    namespace detail
    {
        template <> struct disable_reconvert<saga::stream::activity> : boost::mpl::true_ {};
    }
    /// @endcond 
    /* END: Exclude from Doxygen */

    ///////////////////////////////////////////////////////////////////////////
    //  implement all task::get_result<> functions needed in filesystem package
    template SAGA_STREAM_PACKAGE_EXPORT saga::stream::stream&
    task::get_result<saga::stream::stream>();
    template SAGA_STREAM_PACKAGE_EXPORT saga::stream::stream const&
    task::get_result<saga::stream::stream>() const;

    template SAGA_STREAM_PACKAGE_EXPORT saga::stream::server&
    task::get_result<saga::stream::server>();
    template SAGA_STREAM_PACKAGE_EXPORT saga::stream::server const&
    task::get_result<saga::stream::server>() const;

    template SAGA_STREAM_PACKAGE_EXPORT saga::stream::activity&
    task::get_result<saga::stream::activity>();
    template SAGA_STREAM_PACKAGE_EXPORT saga::stream::activity const&
    task::get_result<saga::stream::activity>() const;

///////////////////////////////////////////////////////////////////////////////
}
