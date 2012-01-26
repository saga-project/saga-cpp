//  Copyright (c) 2009 STFC
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef SAGA_PACKAGES_SD_SERVICE_DATA_HPP
#define SAGA_PACKAGES_SD_SERVICE_DATA_HPP
#include <string>
#include <vector>

#include <saga/saga/util.hpp>
#include <saga/saga/base.hpp>
#include <saga/saga/types.hpp>
#include <saga/saga/session.hpp>
#include <saga/saga/call.hpp>
#include <saga/saga/detail/attribute.hpp>
#include <saga/saga/packages/sd/config.hpp>


namespace saga {
 namespace sd {
  class service_description;
 }
}

namespace saga
{

  namespace sd {

/**
 * Read access to the key/value pairs of a service_description instance.
 * This class implements the
 * saga::attributes interface and
 * offers getter methods for the user to read key/value pairs defined by the
 * service publisher. Service publishers are completely free to define their own
 * key names. Access to the keys and values is through the
 * <code>saga::attributes</code> interface. The class provides no other
 * methods.
 */
  class SAGA_SD_PACKAGE_EXPORT service_data
    : public saga::object,
      public saga::detail::attribute<service_data>
  {
     protected:
       friend struct saga::detail::attribute<service_data>;
       friend class saga::impl::service_data;
       friend class saga::impl::service_description;

       explicit service_data(saga::impl::service_data *impl);

       using saga::detail::attribute<service_data>::set_attribute;
       using saga::detail::attribute<service_data>::set_vector_attribute;
       using saga::detail::attribute<service_data>::remove_attribute;
    
     public:
       service_data (void);
       explicit service_data (saga::object const& o);

       using saga::detail::attribute<service_data>::get_attribute;
       using saga::detail::attribute<service_data>::get_vector_attribute;

      ~service_data (void);

      // default operator=() and copy ctor are ok
      service_data &operator= (saga::object const& o);

   }; // service_data
  } // namespace sd
} // namespace saga
////////////////////////////////////////////////////////////////////////
#endif
