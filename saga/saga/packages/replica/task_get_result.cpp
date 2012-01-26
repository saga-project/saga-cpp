//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga/replica.hpp>
#include <saga/saga/adaptors/task.hpp>
#include <saga/saga/detail/task_get_result_impl.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga
{
    ///////////////////////////////////////////////////////////////////////////
    //  implement all task::get_result<> functions needed in filesystem package
    template SAGA_REPLICA_PACKAGE_EXPORT saga::replica::logical_file&
    task::get_result<saga::replica::logical_file>();
    template SAGA_REPLICA_PACKAGE_EXPORT saga::replica::logical_file const&
    task::get_result<saga::replica::logical_file>() const;

    template SAGA_REPLICA_PACKAGE_EXPORT saga::replica::logical_directory&
    task::get_result<saga::replica::logical_directory>();
    template SAGA_REPLICA_PACKAGE_EXPORT saga::replica::logical_directory const&
    task::get_result<saga::replica::logical_directory>() const;

///////////////////////////////////////////////////////////////////////////////
}

