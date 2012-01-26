//  Copyright (c) 2009 STFC
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef SAGA_IMPL_PACKAGES_ISN_NAVIGATOR_CPI_HPP
#define SAGA_IMPL_PACKAGES_ISN_NAVIGATOR_CPI_HPP

#include <string>
#include <iosfwd>

#include <saga/saga/util.hpp>
#include <saga/saga/types.hpp>
#include <saga/saga/exception.hpp>
#include <saga/saga/isn.hpp>

#include <saga/saga/adaptors/instance_data.hpp>
#include <saga/saga/adaptors/packages/navigator_cpi_instance_data.hpp>
#include <saga/saga/adaptors/permissions_cpi.hpp>

#include <saga/impl/call.hpp>
#include <saga/impl/config.hpp>
#include <saga/impl/engine/cpi.hpp>
#include <saga/impl/engine/register_members.hpp>

namespace saga { namespace impl { namespace v1_0 {

   /* This class declares all functions of "navigator" defined in the SAGA API.
    * It extends the base class saga::cpi
    *
   */
    class navigator_cpi 
        : public permissions_cpi
    {
    public:
        navigator_cpi (proxy *p, cpi_info const& info,
                     TR1::shared_ptr<saga::adaptor> adaptor, cpi::flags flags)
             : permissions_cpi (p, info, adaptor, flags)
        {
        }

        virtual ~navigator_cpi (void)
        {
        }

        cpi::type get_type() const { return cpi::EntityNavigator; }

        SAGA_CALL_CPI_DECL_VIRT_1(navigator_cpi,
                                  std::vector<std::string>,
                                  list_related_entity_names, 
                                  std::string)

        SAGA_CALL_CPI_DECL_VIRT_4(navigator_cpi,
                                  std::vector<saga::isn::entity_data>,
                                  get_related_entities,
                                  std::string,
                                  std::string,
                                  std::string,
                                  std::vector<saga::isn::entity_data>)

        SAGA_CALL_CPI_DECL_VIRT_2(navigator_cpi,
                                  std::vector<saga::isn::entity_data>,
                                  get_entities,
                                  std::string,
                                  std::string)
     }; // class navigator_cpi
}}} // namespace saga::impl::v1_0

/////////////////////////////////////////////////////////////////////////////////////////////
namespace saga { namespace adaptors { namespace v1_0 {

/////////////////////////////////////////////////////////////////////////////////////////////
// register navigator CPI functions
    template <typename Derived>
    inline bool
    register_navigator_functions(
        std::vector<saga::impl::v1_0::cpi_info>& infos,
        saga::impl::v1_0::cpi::maker_type maker,
        saga::impl::v1_0::preference_type const& prefs,
        saga::uuid const& cpi_uuid, saga::uuid const& adaptor_uuid,
        std::string const& cpi_name)
    {
       bool retval = false;
       saga::impl::v1_0::cpi_info info(saga::adaptors::navigator_cpi, 
           cpi_name, maker, prefs, cpi_uuid, adaptor_uuid);

       typedef saga::impl::v1_0::navigator_cpi base_cpi;

       SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, list_related_entity_names, prefs)
       SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, get_related_entities, prefs)
       SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, get_entities, prefs)

       infos.push_back(info);
       return retval;  // is true if at least one function got registered
     }


     ///////////////////////////////////////////////////////////////////////////////////////
     template <typename Derived, typename Mutex = boost::recursive_mutex>
     class navigator_cpi 
         : public saga::impl::v1_0::navigator_cpi
     {
     protected:
         typedef saga::impl::v1_0::navigator_cpi base_type;
         typedef saga::impl::proxy proxy;
         typedef saga::impl::v1_0::cpi cpi;
         typedef saga::impl::v1_0::cpi_info cpi_info;
         typedef saga::impl::v1_0::preference_type preference_type;
         typedef Mutex mutex_type;

         mutex_type mtx_;  // secure access to cpi instance data

     public:
         navigator_cpi (proxy * p, cpi_info const& info, 
                 TR1::shared_ptr<saga::adaptor> adaptor, cpi::flags flags)
              : saga::impl::v1_0::navigator_cpi (p, info, adaptor, flags)
         {
         }

         virtual ~navigator_cpi (void)
         {
         }

         ////////////////////////////////////////////////////////////////////////////////
         // instance data
         // data associated with the API object instance, i.e. visible to all 
         // CPI object instances associated with the API object instance
         // regardless of the shared library (adaptor) they are implemented in.
         typedef saga::adaptors::v1_0::navigator_cpi_instance_data
                    navigator_cpi_instance_data;
         friend class saga::adaptors::instance_data<navigator_cpi_instance_data>;
         typedef      saga::adaptors::instance_data<navigator_cpi_instance_data>
              instance_data;

         ///////////////////////////////////////////////////////////////////////
         // generic factory function
         static cpi* cpi_maker (proxy                * p,
                               cpi_info       const & info,
                               saga::ini::ini const & glob_ini,
                               saga::ini::ini const & adap_ini,
                               TR1::shared_ptr<saga::adaptor> adaptor)
         {
            return new Derived (p, info, glob_ini, adap_ini, adaptor);
         }

         //////////////////////////////////////////////////////////////////////
         // register implemented functions
         static void
         register_cpi(std::vector<cpi_info>& infos, preference_type prefs, 
               saga::uuid adaptor_uuid) 
         {
             // register navigator CPI functions
             saga::uuid cpi_uuid;
             register_navigator_functions<Derived>(infos,
                 &saga::adaptors::v1_0::navigator_cpi<Derived>::cpi_maker,
                 prefs, cpi_uuid, adaptor_uuid, saga::adaptors::navigator_cpi);
         }

         //////////////////////////////////////////////////////////////////////
         // generic up-cast of the impl pointer
         TR1::shared_ptr<Derived> shared_from_this()
         {
            return TR1::static_pointer_cast<Derived>(
                this->base_type::shared_from_this());
         }
      };

///////////////////////////////////////////////////////////////////////////////
}}} // namespace saga::adaptors::v1_0

#endif // SAGA_IMPL_PACKAGES_ISN_NAVIGATOR_CPI_HPP

