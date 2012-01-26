//  Copyright (c) 2009 STFC
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef SAGA_SAGA_ADAPTORS_DISCOVERER_CPI_INSTANCE_DATA

#define SAGA_SAGA_ADAPTORS_DISCOVERER_CPI_INSTANCE_DATA

#include <string>

#include <saga/saga/packages/sd/discoverer.hpp>
#include <saga/saga/adaptors/config.hpp>
#include <saga/saga/types.hpp>
#include <saga/saga/adaptors/instance_data_base.hpp>

namespace saga { namespace adaptors { namespace v1_0
{

   struct discoverer_cpi_instance_data 
      : public adaptors::instance_data_base
   {
      //Location of information provider
      saga::url location_;

        // TODO: decide what other instance data
        // is required for discoverer
        discoverer_cpi_instance_data (saga::url location)
           : location_(location)
        {
        }
   };

}}}   // namespace saga::adaptors::v1_0
      
#endif
