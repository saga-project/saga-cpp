//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  job LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga/util.hpp>
#include <saga/saga/base.hpp>

#include <saga/saga/adaptors/task.hpp>
#include <saga/saga/adaptors/instance_data.hpp>
#include <saga/saga/adaptors/context_cpi_instance_data.hpp>

#include <saga/impl/context_cpi.hpp>
#include <saga/impl/engine/context_base.hpp>


///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl 
{
    context::context()
      : proxy(saga::object::Context, saga::detail::get_the_session())
    {
        // initialize the instance data
        typedef adaptors::v1_0::context_cpi_instance_data 
            instance_data_type;
        typedef adaptors::instance_data <instance_data_type>       
            context_instance_data;

        {
            context_instance_data data;
            data.init_data(this, 
                TR1::shared_ptr <instance_data_type>(new instance_data_type));
        } // lock goes out of scope
    }
    
    context::context(attribute const& rhs_attr)
    :   saga::impl::proxy(saga::object::Context, saga::detail::get_the_session()),
        attribute_base(rhs_attr)
    {}

    void context::init()
    {
        try {
            // initialize a CPI instance and execute required functions
            this->initcpi("context_cpi");
        }
        catch (saga::exception const& e) {
            // map special exceptions
            switch (e.get_error()) {
            case saga::adaptors::AdaptorDeclined:
            case saga::adaptors::NoAdaptor:
            case saga::adaptors::NoAdaptorInfo:
            case saga::adaptors::Unexpected:
                SAGA_THROW_PLAIN(e.get_object(), e.what(), saga::NoSuccess);
            // just rethrow otherwise
            default:
               throw;
            }
        }
    }

    // API function
    SAGA_CALL_IMPL_IMPL_0(context, context_cpi, set_defaults)

///////////////////////////////////////////////////////////////////////////////
}}
