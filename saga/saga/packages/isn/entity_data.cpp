//  Copyright (c) 2009 STFC
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#define SAGA_NO_IMPORT_ATTRIBUTE

#include <iostream>

#include <saga/saga/isn.hpp>
#include <saga/impl/isn.hpp>

#include <saga/saga/detail/attribute_impl.hpp>

namespace saga
{
  namespace isn {
   entity_data::entity_data ()
    : object (new saga::impl::entity_data())
  {
      //initialize attribute implementation
      this->init(true, true); // arg1: extensible (to allow arbitrary key names) and 
                              // arg2: cache only implementation
  }

  entity_data::entity_data(saga::impl::entity_data *impl)
    : object (impl)
  {
  }

  entity_data::~entity_data (void)
  {
  }

  /////////////////////////////////////////////////////////////////////////////
  entity_data::entity_data (saga::object const& o)
    : saga::object (o)
  {
      if (this->get_type() != saga::object::EntityData)
      {
          SAGA_THROW("Bad type conversion.", saga::BadParameter);
      }
  }

  entity_data &entity_data::operator= (saga::object const& o)
  {
      return saga::object::operator=(o), *this;
  }

 } // namespace isn

  /////////////////////////////////////////////////////////////////////////////
  //  implement the attribute functions (we need to explicitly specialize
  //  the template because the functions are not implemented inline)
  template struct SAGA_ISN_PACKAGE_EXPORT_REPEAT saga::detail::attribute<isn::entity_data>;

  template struct SAGA_ISN_PACKAGE_EXPORT saga::detail::attribute_priv<saga::isn::entity_data, task_base::Async>;
  template struct SAGA_ISN_PACKAGE_EXPORT saga::detail::attribute_priv<saga::isn::entity_data, task_base::Task>;
  template struct SAGA_ISN_PACKAGE_EXPORT saga::detail::attribute_priv<saga::isn::entity_data, task_base::Sync>;
  template struct SAGA_ISN_PACKAGE_EXPORT saga::detail::attribute_sync<saga::isn::entity_data>;

///////////////////////////////////////////////////////////////////////////////////
} // namespace saga

