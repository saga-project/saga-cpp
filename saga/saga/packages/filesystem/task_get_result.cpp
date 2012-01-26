//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga/task.hpp>
#include <saga/saga/filesystem.hpp>
#include <saga/saga/adaptors/task.hpp>
#include <saga/saga/detail/task_get_result_impl.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga
{
    ///////////////////////////////////////////////////////////////////////////
    //  implement all task::get_result<> functions needed in filesystem package
    template SAGA_FILESYSTEM_PACKAGE_EXPORT saga::filesystem::file&
    task::get_result<saga::filesystem::file>();
    template SAGA_FILESYSTEM_PACKAGE_EXPORT saga::filesystem::file const&
    task::get_result<saga::filesystem::file>() const;

    template SAGA_FILESYSTEM_PACKAGE_EXPORT saga::filesystem::directory&
    task::get_result<saga::filesystem::directory>();
    template SAGA_FILESYSTEM_PACKAGE_EXPORT saga::filesystem::directory const&
    task::get_result<saga::filesystem::directory>() const;

///////////////////////////////////////////////////////////////////////////////
}
