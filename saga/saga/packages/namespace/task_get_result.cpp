//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga/util.hpp>
#include <saga/saga/namespace.hpp>
#include <saga/saga/adaptors/task.hpp>
#include <saga/saga/detail/task_get_result_impl.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga
{
    ///////////////////////////////////////////////////////////////////////////
    //  implement all task::get_result<> functions needed in namespace package
    template SAGA_NAMESPACE_PACKAGE_EXPORT saga::name_space::directory&
    task::get_result<saga::name_space::directory>();
    template SAGA_NAMESPACE_PACKAGE_EXPORT saga::name_space::directory const& 
    task::get_result<saga::name_space::directory>() const;

    template SAGA_NAMESPACE_PACKAGE_EXPORT saga::name_space::entry&
    task::get_result<saga::name_space::entry>();
    template SAGA_NAMESPACE_PACKAGE_EXPORT saga::name_space::entry const&
    task::get_result<saga::name_space::entry>() const;

///////////////////////////////////////////////////////////////////////////////
}
