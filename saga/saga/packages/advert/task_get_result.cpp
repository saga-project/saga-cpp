//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga/task.hpp>
#include <saga/saga/advert.hpp>
#include <saga/saga/adaptors/task.hpp>
#include <saga/saga/detail/task_get_result_impl.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga
{
    ///////////////////////////////////////////////////////////////////////////
    //  implement all task::get_result<> functions needed in advert package
    template SAGA_ADVERT_PACKAGE_EXPORT saga::advert::entry& 
        task::get_result<saga::advert::entry>();
    template SAGA_ADVERT_PACKAGE_EXPORT saga::advert::entry const& 
        task::get_result<saga::advert::entry>() const;

    template SAGA_ADVERT_PACKAGE_EXPORT saga::advert::directory& 
        task::get_result<saga::advert::directory>();
    template SAGA_ADVERT_PACKAGE_EXPORT saga::advert::directory const& 
        task::get_result<saga::advert::directory>() const;

    template SAGA_ADVERT_PACKAGE_EXPORT saga::object& 
        task::get_result<saga::object>();
    template SAGA_ADVERT_PACKAGE_EXPORT saga::object const& 
        task::get_result<saga::object>() const;

///////////////////////////////////////////////////////////////////////////////
}
