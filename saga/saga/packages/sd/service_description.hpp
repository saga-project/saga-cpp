//  Copyright (c) 2009 STFC
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef SAGA_PACKAGES_SD_SERVICE_DESCRIPTION_HPP

#define SAGA_PACKAGES_SD_SERVICE_DESCRIPTION_HPP

#include <saga/saga/util.hpp>
#include <saga/saga/base.hpp>
#include <saga/saga/types.hpp>
#include <saga/saga/session.hpp>
#include <saga/saga/call.hpp>
#include <saga/saga/detail/attribute.hpp>
#include <saga/saga/exception.hpp>
#include <saga/saga/url.hpp>

#include <saga/saga/packages/sd/config.hpp>

#include <saga/saga/packages/sd/service_data.hpp>

namespace saga
{
   namespace impl
   {
      class info_provider;
      class discoverer_cpi_impl;
   }
}

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable: 4251 4231 4275 4660)
#endif

namespace saga 
{
  namespace sd {
  namespace attributes 
  {
    ///////////////////////////////////////////////////////////////////////////
    //  attribute names for service_description

    char const* const service_description_url = "Url";
    char const* const service_description_type = "Type";
    char const* const service_description_uid = "Uid";
    char const* const service_description_site = "Site";
    char const* const service_description_name = "Name";
    char const* const service_description_relatedservices = "RelatedServices";
    char const* const service_description_vo = "VO";
    char const* const service_description_implementation_version =
       "ImplementationVersion";
    char const* const service_description_interface_version =
       "InterfaceVersion";
    char const* const service_description_capability = "Capabilities";
    char const* const service_description_information_provider_url =
       "InformationServiceUrl";
    char const* const service_description_implementor = "Implementor";
    
  }

   /////////////////////////////////////////////////////////
  //  service description
/**
 * <p>
 * Read access to the top level data of the service and a means to navigate to
 * related services. This class implements the
 * attributes interface and
 * offers getter methods to obtain details of that service. The attributes are
 * based on those found in GLUE. They are:
 * </p>
 * <dl>
 * <dt><code>Url</code></dt>
 * <dd>url to contact the service. The get_url method obtains
 * the same information.
 * <dt><code>Type</code></dt>
 * <dd>type of service. This field is not an empty string.</dd>
 * <dt><code>Uid</code></dt>
 * <dd>unique identifier of service. This field is not an empty string.</dd>
 * <dt><code>Site</code></dt>
 * <dd>name of site. This field is not an empty string.</dd>
 * <dt><code>Name</code></dt>
 * <dd>name of service - not necessarily unique. This field is not an empty
 * string.</dd>
 * <dt><code>implementor</code></dt>
 * <dd>name of the organisation providing the implementation of the service.
 * This field is not an empty string.</dd>
 * <dt><code>Related_Services</code></dt>
 * <dd>uids of related services. This returns the uids of the related services.
 * This is unlike the method get_related_services
 * which returns a vector of service_description objects.</dd>
 * </dl>
 */
  class SAGA_SD_PACKAGE_EXPORT service_description
      : public saga::object,
        public saga::detail::attribute<service_description>
  {
    protected:
       /// @cond
       /** These methods are not within API scope */
       TR1::shared_ptr <saga::impl::service_description> get_impl_sp(void) const;
       saga::impl::service_description* get_impl (void) const;
       /// @endcond

       friend struct saga::detail::attribute<service_description>;
       friend class saga::impl::service_description;
       friend class saga::impl::discoverer_cpi_impl;
       friend class saga::impl::info_provider;

       explicit service_description(saga::impl::service_description *impl, saga::url loc);
       void set_session(TR1::shared_ptr<saga::session> sess);
       void set_data(std::string data_key, std::string data_value);

       using saga::detail::attribute<service_description>::set_attribute;
       using saga::detail::attribute<service_description>::set_vector_attribute;

    protected:
    /**
     * Default constructor
     */
       service_description (saga::url loc); 
       explicit service_description (saga::object const& o);

    public:
       using saga::detail::attribute<service_description>::get_attribute;
       using saga::detail::attribute<service_description>::get_vector_attribute;

    /**
     * Destructor
     */
      ~service_description (void) SAGA_THROW_SPEC(throw());

      // default operator=() and copy ctor are ok
      service_description &operator= (saga::object const& o) 
          SAGA_THROW_SPEC(throw());

      // special getters 
    /**
     * Returns the <code>URL</code> to contact the service. The
     * <code>URL</code> may also be obtained using the
     * <code>saga::attributes</code> interface.
     * 
     * @return a string containing the URL to contact this service
     */
      std::string get_url() SAGA_THROW_SPEC(throw());

    /**
     * Returns a vector of related services. Alternatively, the
     * <code>saga::attributes</code> interface may be used to get the uids of
     * the related services.
     * 
     * @return a set of related services. This may be an empty set.
     * @throws authorization_failed
     *                 if none of the available contexts of the used session
     *                 could be used for successful authorization. That error
     *                 indicates that the resource could not be accessed at all,
     *                 and not that an operation was not available due to
     *                 restricted permissions.
     * @throws authentication_failed
     *                 if none of the available session contexts could
     *                 successfully be used for authentication
     * @throws timeout
     *                 if a remote operation did not complete successfully
     *                 because the network communication or the remote service
     *                 timed out
     * @throws no_success
     *                 if no result can be returned because of information
     *                 system or other internal problems
     */
      std::vector<saga::sd::service_description> get_related_services()
         SAGA_THROW_SPEC(throw(saga::authorization_failed,
               saga::authentication_failed,
               saga::timeout,
               saga::no_success));

    /**
     * Returns a <code>service_data</code> object with the service data
     * key/value pairs.
     * 
     * @return the service data for this service.
     *         This may be empty, i.e.has no attributes at all.
     */
      saga::sd::service_data &get_data() SAGA_THROW_SPEC(throw());
  }; // service_description
 } // namespace sd
} // namespace saga
///////////////////////////////////////////////////////////////////////////////

// re-enable warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

#endif // !defined(SAGA_PACKAGES_SD_SERVICE_DESCRIPTION_HPP)

