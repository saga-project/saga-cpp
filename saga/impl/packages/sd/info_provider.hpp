//  Copyright (c) 2009 STFC
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef INC_INFO_PROVIDER

#define INC_INFO_PROVIDER

#include <vector>
#include <string>
#include <saga/saga/util.hpp>
#include <saga/saga/packages/sd/service_description.hpp>

/* Base class for underlying information providers (BDII, RGMA) */
///@cond
namespace saga
{
   namespace impl
   {
      class info_provider
      {
         // service_description to get service data
         friend class saga::impl::service_description;

         public:
         info_provider() { };
         virtual ~info_provider() { };
         protected:

         static inline void
            set_attribute(saga::sd::service_description* svc_desc,
                          std::string attr_name,
                          std::string attr_value)
         {
            svc_desc->set_attribute(attr_name, attr_value); 
         }

         static inline void
            set_vector_attribute(saga::sd::service_description* svc_desc,
                                 std::string attr_name,
                                 std::vector<std::string> attr_value)
         {
            svc_desc->set_vector_attribute(attr_name, attr_value);
         }

         static inline void set_data(saga::sd::service_description* svc_desc,
                                     std::string data_key, 
                                     std::string data_value)
         {
            svc_desc->set_data(data_key, data_value); 
         }

         static inline void
            set_session(saga::sd::service_description* svc_desc,
                        const TR1::shared_ptr<saga::session> sess)
         {
            svc_desc->set_session(sess); 
         }
      };
   } //namespace impl
} //namespace saga
///@endcond
#endif
