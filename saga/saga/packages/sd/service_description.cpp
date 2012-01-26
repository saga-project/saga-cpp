//  Copyright (c) 2009 STFC
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#define SAGA_NO_IMPORT_ATTRIBUTE
#include <iostream>
#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>

#include <boost/assign/list_inserter.hpp>
#include <boost/assign/std/vector.hpp>

#include <saga/saga/sd.hpp>
#include <saga/impl/sd.hpp>
#include <saga/saga/exception.hpp>

#include <saga/saga/detail/attribute_impl.hpp>

#define SD_DEBUG 0

///////////////////////////////////////////////////////////////////////////////
namespace saga
{
  namespace sd {

  service_description::service_description (saga::url loc)
    : object (new saga::impl::service_description())
  {
      using namespace boost::assign;

      // initialize list of valid keys
      static char const* const valid_keys[] = 
      {
          attributes::service_description_url,
          attributes::service_description_type,
          attributes::service_description_uid,
          attributes::service_description_site,
          attributes::service_description_name,
          attributes::service_description_relatedservices,
          attributes::service_description_information_provider_url,
          attributes::service_description_vo,
          attributes::service_description_interface_version,
          attributes::service_description_implementation_version,
          attributes::service_description_implementor,
          attributes::service_description_capability,
          NULL
      };
      this->init_keynames(valid_keys);

      static char const* const attributes_scalar_rw[] = 
      {
          attributes::service_description_url,
          attributes::service_description_type,
          attributes::service_description_uid,
          attributes::service_description_site,
          attributes::service_description_name,
          attributes::service_description_information_provider_url,
          attributes::service_description_interface_version,
          attributes::service_description_implementation_version,
          attributes::service_description_implementor,
          NULL,
      };

      static char const* const attributes_vector_rw[] = 
      {
          attributes::service_description_relatedservices,
          attributes::service_description_vo,
          attributes::service_description_capability,
          NULL,
      };

      // initialize attribute implementation
      this->init(NULL, attributes_scalar_rw, NULL, attributes_vector_rw);
      this->init(false, true); // not extensible and cache only implementation

      // initialize the information_provider_url attribute
      set_attribute(attributes::service_description_information_provider_url,
          loc.get_url());
  }

  service_description::service_description(saga::impl::service_description *impl, saga::url loc)
    : object (impl)
  {
    //std::cout << "SC2: Uid = " << this->get_id().string() << " Type = " << this->get_type() << std::endl;

    // initialize the information_provider_url attribute
    set_attribute(attributes::service_description_information_provider_url,
        loc.get_url());
  }

  service_description::~service_description (void) SAGA_THROW_SPEC(throw())
  {
  }

  /////////////////////////////////////////////////////////////////////////////
  service_description::service_description (saga::object const& o)
    : saga::object (o)
  {
      if (this->get_type() != saga::object::ServiceDescription)
      {
          SAGA_THROW("Bad type conversion.", saga::BadParameter);
      }
  }

  service_description &service_description::operator= (saga::object const& o)
     SAGA_THROW_SPEC(throw())
  {
      return saga::object::operator=(o), *this;
  }

  saga::impl::service_description* service_description::get_impl() const
  { 
      typedef saga::object base_type;
      return static_cast<saga::impl::service_description*>(this->base_type::get_impl()); 
  }

  TR1::shared_ptr <saga::impl::service_description> service_description::get_impl_sp(void) const
  { 
    // FIXME: this needs documentation
    typedef saga::object base_type;
    return TR1::static_pointer_cast <saga::impl::service_description>(
        this->base_type::get_impl_sp());
  }

  /////////////////////////////////////////////////////////////////////////////
  std::string service_description::get_url() SAGA_THROW_SPEC(throw())
  {
     if (!this->attribute_exists(attributes::service_description_url)) 
     {
        SAGA_THROW("Attribute Url does not exist", saga::BadParameter);
     }
     return this->get_attribute(attributes::service_description_url);
  }

  saga::sd::service_data &service_description::get_data() SAGA_THROW_SPEC(throw())
  {
    return get_impl()->get_data();
  }

  std::vector<saga::sd::service_description>
     service_description::get_related_services()
        SAGA_THROW_SPEC(throw(saga::authorization_failed,
              saga::authentication_failed,
              saga::timeout,
              saga::no_success))
  {
     std::vector<saga::sd::service_description> rsvc_list;
     
     if (!this->attribute_exists(attributes::service_description_relatedservices)) 
     {
        return rsvc_list;
        //SAGA_THROW("Attribute Related_Services does not exist", saga::BadParameter);
     }
     else
     {
        std::vector<std::string> rsvc_uids = this->get_vector_attribute(
                                            attributes::service_description_relatedservices);
        if( rsvc_uids.size() > 0)
        {
           std::string vo_filter = "";
           std::string data_filter = "";
           std::string svc_filter = "";
           if (rsvc_uids.size() > 1) svc_filter = "(";
           for (unsigned int i = 0; i < rsvc_uids.size(); i++) 
           {
               svc_filter.append("(Uid = '");
               svc_filter.append(rsvc_uids[i]);
               svc_filter.append("')");
               if (i != (rsvc_uids.size()-1) && rsvc_uids.size() > 1)
                  svc_filter.append(" OR ");
           }
           if (rsvc_uids.size() > 1) svc_filter.append(")");
#if SD_DEBUG
           std::cout << "Related service filter = " << svc_filter << std::endl;
#endif
           //Get information provider URL from our attributes
           std::string loc;
           if ( attribute_exists(attributes::service_description_information_provider_url) )
           {
              loc = get_attribute(attributes::service_description_information_provider_url);
           }

           discoverer d (this->saga::object::get_impl()->get_session(), loc);
           rsvc_list = d.list_services(svc_filter, data_filter, vo_filter);
        }
     }
     return rsvc_list;
  }

  void service_description::set_data(std::string data_key, std::string data_value)
  {
      get_impl()->set_data(data_key, data_value);
  }

  void service_description::set_session(TR1::shared_ptr<saga::session> sess)
  {
     static_cast<impl::service_description*>(
        this->saga::object::get_impl())->set_session(sess);
  }
 } // namespace sd

  namespace detail
  {
    /////////////////////////////////////////////////////////////////////////////
    //  implement the attribute functions (we need to explicitly specialize
    //  the template because the functions are not implemented inline)
    template struct SAGA_SD_PACKAGE_EXPORT_REPEAT attribute<sd::service_description>;

    template struct SAGA_SD_PACKAGE_EXPORT attribute_priv<sd::service_description, task_base::Async>;
    template struct SAGA_SD_PACKAGE_EXPORT attribute_priv<sd::service_description, task_base::Task>;
    template struct SAGA_SD_PACKAGE_EXPORT attribute_priv<sd::service_description, task_base::Sync>;

    template struct SAGA_SD_PACKAGE_EXPORT attribute_sync<sd::service_description>;
  }

///////////////////////////////////////////////////////////////////////////////////
} // namespace saga

