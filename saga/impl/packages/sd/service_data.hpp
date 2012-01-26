//  Copyright (c) 2009 STFC
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef SAGA_IMPL_PACKAGES_SD_SERVICE_DATA_HPP
#define SAGA_IMPL_PACKAGES_SD_SERVICE_DATA_HPP

#include <saga/impl/config.hpp>
#include <saga/impl/engine/attribute.hpp>
#include <saga/impl/engine/object.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga
{
  namespace impl
  {
    // service description implementation
    class service_data
      : public saga::impl::object,
        public saga::impl::attribute
    {
      typedef saga::impl::attribute attribute_base;

      explicit service_data(attribute const& attr);
      public:

      /**
       * Constructor of service_data
       */
       service_data ();
      ~service_data ();
      saga::impl::attribute* get_attributes() { return this; }
      saga::impl::attribute const* get_attributes() const { return this; }

      // Generate a exact deep copy of this object
      saga::object clone() const;
    };

  } // namespace impl

} // namespace saga

#endif // !defined(SAGA_IMPL_PACKAGES_SD_SERVICE_DATA_HPP)
