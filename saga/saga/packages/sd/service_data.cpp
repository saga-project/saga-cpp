//  Copyright (c) 2009 STFC
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#define SAGA_NO_IMPORT_ATTRIBUTE

#include <iostream>

#include <saga/saga/sd.hpp>
#include <saga/impl/sd.hpp>

#include <saga/saga/detail/attribute_impl.hpp>

using namespace std;

namespace saga
{
  namespace sd {
   service_data::service_data ()
    : object (new saga::impl::service_data())
  {
      //initialize attribute implementation
      this->init(true, true); // arg1: extensible (to allow arbitrary key names) and 
                              // arg2: cache only implementation
      //std::cout << "DC1: Uid = " << this->get_id().string() << " Type = " << this->get_type() << std::endl;
  }

  service_data::service_data(saga::impl::service_data *impl)
    : object (impl)
  {
      //std::cout << "DC2: Uid = " << this->get_id().string() << " Type = " << this->get_type() << std::endl;
  }

  service_data::~service_data (void)
  {
     //std::cout << this->get_id().string() << " Destructor" << std::endl;
  }

  /////////////////////////////////////////////////////////////////////////////
  service_data::service_data (saga::object const& o)
    : saga::object (o)
  {
      if (this->get_type() != saga::object::ServiceData)
      {
          SAGA_THROW("Bad type conversion.", saga::BadParameter);
      }
  }

  service_data &service_data::operator= (saga::object const& o)
  {
      return saga::object::operator=(o), *this;
  }

 } // namespace sd

  namespace detail
  {
    /////////////////////////////////////////////////////////////////////////////
    //  implement the attribute functions (we need to explicitly specialize
    //  the template because the functions are not implemented inline)
    template struct SAGA_SD_PACKAGE_EXPORT_REPEAT attribute<sd::service_data>;

    template struct SAGA_SD_PACKAGE_EXPORT attribute_priv<sd::service_data, task_base::Async>;
    template struct SAGA_SD_PACKAGE_EXPORT attribute_priv<sd::service_data, task_base::Task>;
    template struct SAGA_SD_PACKAGE_EXPORT attribute_priv<sd::service_data, task_base::Sync>;

    template struct SAGA_SD_PACKAGE_EXPORT attribute_sync<sd::service_data>;
  }

///////////////////////////////////////////////////////////////////////////////////
} // namespace saga
