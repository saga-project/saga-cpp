//  Copyright (c) 2009 STFC
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef SAGA_IMPL_PACKAGES_SD_SERVICE_DESCRIPTION_HPP
#define SAGA_IMPL_PACKAGES_SD_SERVICE_DESCRIPTION_HPP

#include <saga/saga/util.hpp>
#include <saga/impl/config.hpp>
#include <saga/impl/engine/attribute.hpp>
#include <saga/impl/engine/object.hpp>

#include <saga/impl/packages/sd/info_provider.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable: 4251 4231 4275 4660)
#endif

///////////////////////////////////////////////////////////////////////////////
namespace saga
{
  namespace impl
  {
    // service description implementation
    class service_description
      : public saga::impl::object,
        public saga::impl::attribute
    {
     private: 
        TR1::shared_ptr<saga::session> _session;
 
        typedef saga::impl::attribute attribute_base;
        explicit service_description(service_description const& attr);

     private: 
        saga::sd::service_data data;

     protected:

        void set_session(TR1::shared_ptr<saga::session> sess); 
        void set_data(std::string const& data_key, 
            std::string const& data_value);

        friend class saga::sd::service_description;
     public:
      /**
       * Constructor of service_description
       */
       service_description();
      ~service_description ();
      saga::impl::attribute* get_attributes() { return this; }
      saga::impl::attribute const* get_attributes() const { return this; }

      // Generate a exact deep copy of this object
      saga::object clone() const;

      saga::sd::service_data &get_data();
    };

  } // namespace impl

} // namespace saga

///////////////////////////////////////////////////////////////////////////////
// re-enable warnings about dependent classes not being exported from the dll

#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

#endif // !defined(SAGA_IMPL_PACKAGES_SD_SERVICE_DESCRIPTION_HPP)
