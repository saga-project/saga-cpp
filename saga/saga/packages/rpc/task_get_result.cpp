//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga/rpc.hpp>
#include <saga/saga/adaptors/task.hpp>
#include <saga/saga/detail/task_get_result_impl.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga
{
    ///////////////////////////////////////////////////////////////////////////
    //  implement all task::get_result<> functions needed in rpc package
    template SAGA_RPC_PACKAGE_EXPORT saga::rpc::rpc&
    task::get_result<saga::rpc::rpc>();
    template SAGA_RPC_PACKAGE_EXPORT saga::rpc::rpc const&
    task::get_result<saga::rpc::rpc>() const;

///////////////////////////////////////////////////////////////////////////////
}
