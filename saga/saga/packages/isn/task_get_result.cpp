//  Copyright (c) 2009 STFC
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <vector>

#include <saga/saga/isn.hpp>
#include <saga/saga/adaptors/task.hpp>
#include <saga/saga/detail/task_get_result_impl.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga
{
    ///////////////////////////////////////////////////////////////////////////
    //  implement all task::get_result<> functions needed in sd package
    /*template SAGA_SD_PACKAGE_EXPORT std::vector<saga::sd::service_description>&
    task::get_result<std::vector<saga::sd::service_description> >();
    template SAGA_SD_PACKAGE_EXPORT std::vector<saga::sd::service_description> const&
    task::get_result<std::vector<saga::sd::service_description> >() const;*/

    template SAGA_ISN_PACKAGE_EXPORT saga::isn::navigator&
    task::get_result<saga::isn::navigator>();
    template SAGA_ISN_PACKAGE_EXPORT saga::isn::navigator const&
    task::get_result<saga::isn::navigator>() const;

    template SAGA_ISN_PACKAGE_EXPORT std::vector<saga::isn::entity_data>&
    task::get_result<std::vector<saga::isn::entity_data> >();
    template SAGA_ISN_PACKAGE_EXPORT std::vector<saga::isn::entity_data> const&
    task::get_result<std::vector<saga::isn::entity_data> >() const;

}
