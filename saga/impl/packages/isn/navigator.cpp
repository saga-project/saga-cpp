//  Copyright (c) 2009 STFC
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <saga/saga/adaptors/task.hpp>
#include <saga/saga/adaptors/instance_data.hpp>
#include <saga/saga/adaptors/packages/navigator_cpi_instance_data.hpp>

#include <saga/saga/packages/isn/entity_data.hpp>
#include <saga/impl/packages/isn/entity_data.hpp>
#include <saga/saga/packages/isn/navigator.hpp>
#include <saga/impl/packages/isn/navigator.hpp>

namespace saga 
{
  namespace impl
  {
    navigator::navigator (std::string model,
                          saga::session const& s,
                          saga::url loc)
       : proxy (saga::object::EntityNavigator, s)
    {
       // initialize the instance data
       typedef adaptors::v1_0::navigator_cpi_instance_data instance_data_type;
       typedef adaptors::instance_data <instance_data_type> navigator_instance_data;
       
       {
           navigator_instance_data data;
           // anything to pass to instance_data_type ?
           data.init_data(this, TR1::shared_ptr <instance_data_type>
                                (new instance_data_type(model, loc)));
       }  
    }

    void navigator::init()
    {
       // initialize a CPI instance and execute required functions
       this->initcpi(saga::adaptors::navigator_cpi);
    }

    navigator::~navigator(void)
    {
       // free instance data
       typedef adaptors::v1_0::navigator_cpi_instance_data instance_data_type;
       typedef adaptors::instance_data <instance_data_type> navigator_instance_data;
 
       navigator_instance_data data;
       data.release_data (this);
    }

    ///////////////////////////////////////////////////////////////////////////
    // factory support
    void navigator::create_impl_sync(saga::isn::navigator& retval)
    {
        this->init();
        retval = saga::isn::navigator(this);
    }

    saga::task navigator::create_impl_async(std::string const& model,
                                            saga::session const& s,
                                            saga::url const& loc)
    {
        TR1::shared_ptr<navigator> impl(new navigator(model, s, loc));

        bool (navigator::*prep)(saga::isn::navigator&, saga::uuid) = NULL;
        return saga::adaptors::task("__init__", TR1::shared_ptr<v1_0::cpi>(), 
            impl.get(), &navigator::create_impl_sync, prep);
    }

    // methods for navigator
    SAGA_CALL_IMPL_IMPL_1 (navigator, navigator_cpi, 
                          list_related_entity_names, std::string)

    SAGA_CALL_IMPL_IMPL_4 (navigator,
                           navigator_cpi, 
                           get_related_entities,
                           std::string,
                           std::string,
                           std::string,
                           std::vector<saga::isn::entity_data>)

    SAGA_CALL_IMPL_IMPL_2 (navigator,
                           navigator_cpi, 
                           get_entities,
                           std::string,
                           std::string)
  } // namespace impl
} // namespace saga
////////////////////////////////////////////////////////////////////////////////////////////////////

