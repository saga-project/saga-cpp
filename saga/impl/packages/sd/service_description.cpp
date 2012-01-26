//  Copyright (c) 2009 STFC
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <iostream>

#include <saga/saga/object.hpp>
#include <saga/saga/task.hpp>
#include <saga/saga/packages/sd/service_description.hpp>
#include <saga/impl/packages/sd/service_description.hpp>

using namespace saga::sd;

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl {

    void service_description::set_session(TR1::shared_ptr<saga::session> sess)
    {
       _session = sess;
    }

    service_description::service_description ()
    :   saga::impl::object(saga::object::ServiceDescription)
    {
    }

    service_description::service_description (service_description const& rhs)
    :   saga::impl::object(saga::object::ServiceDescription),
        attribute_base(rhs)
    {
    }

    service_description::~service_description (void)
    {
      //std::cout << "Dtor " << this << std::endl;
    }

    // Generate a exact deep copy of this object
    saga::object service_description::clone() const
    {
        return saga::sd::service_description(
            new service_description(*this), saga::url ());
    }

    void service_description::set_data(std::string const& data_key, 
        std::string const& data_value)
    {
        std::vector<std::string> attVec;

        //All data attributes are stored as vector attributes
        //so if the one we're trying to set exists
        //just append to the end of it
        if ( data.attribute_exists(data_key) )
        {
           attVec = data.get_vector_attribute(data_key);
        }

        attVec.push_back(data_value);
        data.set_vector_attribute(data_key, attVec);
    }

    saga::sd::service_data &service_description::get_data()
    {
        return data;
    }

///////////////////////////////////////////////////////////////////////////////
}}  // namespace saga::impl
