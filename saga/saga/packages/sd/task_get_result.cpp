//  Copyright (c) 2009 STFC
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <vector>

#include <saga/saga/sd.hpp>
#include <saga/saga/adaptors/task.hpp>
#include <saga/saga/detail/task_get_result_impl.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga
{
    ///////////////////////////////////////////////////////////////////////////
    //  implement all task::get_result<> functions needed in sd package
    template SAGA_SD_PACKAGE_EXPORT std::vector<saga::sd::service_description>&
    task::get_result<std::vector<saga::sd::service_description> >();
    template SAGA_SD_PACKAGE_EXPORT std::vector<saga::sd::service_description> const&
    task::get_result<std::vector<saga::sd::service_description> >() const;

    template SAGA_SD_PACKAGE_EXPORT saga::sd::discoverer&
    task::get_result<saga::sd::discoverer>();
    template SAGA_SD_PACKAGE_EXPORT saga::sd::discoverer const&
    task::get_result<saga::sd::discoverer>() const;

}
