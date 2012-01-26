//  Copyright (c) 2009 STFC
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <saga/saga/adaptors/task.hpp>
#include <saga/impl/packages/sd/discoverer.hpp>
#include <saga/saga/adaptors/instance_data.hpp>
#include <saga/saga/adaptors/packages/discoverer_cpi_instance_data.hpp>

namespace saga 
{
  namespace impl
  {
    discoverer::discoverer (saga::session const& s, saga::url loc)
       : proxy (saga::object::ServiceDiscoverer, s)
    {
       // initialize the instance data
       typedef adaptors::v1_0::discoverer_cpi_instance_data instance_data_type;
       typedef adaptors::instance_data <instance_data_type> discoverer_instance_data;
       
       {
           discoverer_instance_data data;
           // anything to pass to instance_data_type ?
           data.init_data(this, TR1::shared_ptr <instance_data_type>
                                (new instance_data_type(loc)));
       }  
    }

    void discoverer::init()
    {
       // initialize a CPI instance and execute required functions
       this->initcpi(saga::adaptors::discoverer_cpi);
    }

    discoverer::~discoverer(void)
    {
       // free instance data
       typedef adaptors::v1_0::discoverer_cpi_instance_data instance_data_type;
       typedef adaptors::instance_data <instance_data_type> discoverer_instance_data;
 
       discoverer_instance_data data;
       data.release_data (this);
    }

    ///////////////////////////////////////////////////////////////////////////
    // factory support
    void discoverer::create_impl_sync(saga::sd::discoverer& retval)
    {
        this->init();
        retval = saga::sd::discoverer(this);
    }

    saga::task discoverer::create_impl_async(saga::session const& s,
                                             saga::url const& loc)
    {
        TR1::shared_ptr<discoverer> impl(new discoverer(s, loc));

        bool (discoverer::*prep)(saga::sd::discoverer&, saga::uuid) = NULL;
        return saga::adaptors::task("__init__", TR1::shared_ptr<v1_0::cpi>(), 
            impl.get(), &discoverer::create_impl_sync, prep);
    }

    // methods for discoverer
    SAGA_CALL_IMPL_IMPL_2 (discoverer, discoverer_cpi, 
                          list_services2, std::string, std::string)
    SAGA_CALL_IMPL_IMPL_3 (discoverer, discoverer_cpi, 
                          list_services3, std::string, std::string, std::string)
  } // namespace impl
} // namespace saga
////////////////////////////////////////////////////////////////////////////////////////////////////

