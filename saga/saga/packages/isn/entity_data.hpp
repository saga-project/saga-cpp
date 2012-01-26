//  Copyright (c) 2009 STFC
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef SAGA_PACKAGES_ISN_ENTITY_DATA_HPP
#define SAGA_PACKAGES_ISN_ENTITY_DATA_HPP
#include <string>
#include <vector>
#include <iostream>

#include <saga/saga/util.hpp>
#include <saga/saga/base.hpp>
#include <saga/saga/types.hpp>
#include <saga/saga/session.hpp>
#include <saga/saga/call.hpp>
#include <saga/saga/detail/attribute.hpp>
#include <saga/saga/packages/isn/config.hpp>

namespace saga { namespace impl { class entity_data; } }

namespace saga
{
    /*! \brief The <I>Information System Navigation (ISN)</I> API package
     *
     *  The ISN API provides an interfaces to retrieve data from an
     * information model. A possible starting point for navigation would be a 
     * service, as selected with the saga::sd API. Alternatively 
     * it is possible to start with a selected entity type rather than a 
     * service. An optional filter can be used to restrict the results returned. 
     *
     * The quality of the information returned will depend upon the quality of 
     * the data in the back-end system or systems.
     */  
  namespace isn {

/**
 * Provides read access to the data of an entity. This class implements the
 * saga::attributes interface and
 * offers getter methods for the user to read key/value pairs.
 * Access to the keys and values is through the
 * <code>saga::attributes</code> interface. The class
 * provides no other methods.
 * 
 */
  class SAGA_ISN_PACKAGE_EXPORT entity_data
    : public saga::object,
      public saga::detail::attribute<entity_data>
  {
     protected:
       friend struct saga::detail::attribute<entity_data>;
       friend class saga::impl::entity_data;

       explicit entity_data(saga::impl::entity_data* impl);

       using saga::detail::attribute<entity_data>::remove_attribute;
    
     public:
       entity_data(void);
       explicit entity_data(saga::object const& o);

       using saga::detail::attribute<entity_data>::get_attribute;
       using saga::detail::attribute<entity_data>::get_vector_attribute;
       using saga::detail::attribute<entity_data>::set_attribute;
       using saga::detail::attribute<entity_data>::set_vector_attribute;

      ~entity_data (void);

      // default operator=() and copy ctor are ok
      entity_data &operator= (saga::object const& o);

   }; // entity_data
  } // namespace isn
} // namespace saga
////////////////////////////////////////////////////////////////////////
#endif
