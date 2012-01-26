//  Copyright (c) 2009 STFC
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef SAGA_SAGA_ADAPTORS_NAVIGATOR_CPI_INSTANCE_DATA
#define SAGA_SAGA_ADAPTORS_NAVIGATOR_CPI_INSTANCE_DATA

#include <string>

#include <saga/saga/adaptors/config.hpp>
#include <saga/saga/types.hpp>
#include <saga/saga/adaptors/instance_data_base.hpp>

namespace saga { namespace adaptors { namespace v1_0
{

   struct navigator_cpi_instance_data 
      : public adaptors::instance_data_base
   {
      //Location of information provider
      saga::url location_;

      //Information provider model
      std::string model_;

        // TODO: decide what other instance data
        // is required for the navigator
        navigator_cpi_instance_data (std::string model, saga::url location)
           : location_(location), model_(model)
        {
        }
   };

}}}   // namespace saga::adaptors::v1_0
      
#endif
