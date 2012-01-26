//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga/task.hpp>
#include <saga/saga/adaptors/metric.hpp>
#include <saga/saga/packages/sd/adaptors/service_description.hpp>

#include <saga/impl/sd.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace adaptors {

    service_description::service_description (saga::url loc)
        : saga::sd::service_description (new saga::impl::service_description (), loc)
    {
        this->saga::object::get_impl()->init();
    }

    service_description::service_description (TR1::shared_ptr<saga::impl::service_description> impl)
      : saga::sd::service_description (saga::impl::runtime::get_object(
            TR1::static_pointer_cast<saga::impl::object>(impl)))
    {
        this->saga::object::get_impl()->init();
    }

    service_description::~service_description()
    {
    }

///////////////////////////////////////////////////////////////////////////////
}}    // namespace saga::adaptors

