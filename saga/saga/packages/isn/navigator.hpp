//  Copyright (c) 2009 STFC
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef SAGA_PACKAGES_ISN_NAVIGATOR_HPP
#define SAGA_PACKAGES_ISN_NAVIGATOR_HPP

// include dependent spec sections
#include <saga/saga/util.hpp>
#include <saga/saga/base.hpp>
#include <saga/saga/types.hpp>
#include <saga/saga/session.hpp>
#include <saga/saga/call.hpp> 
#include <saga/saga/exception.hpp>
#include <saga/saga/url.hpp>

#include <saga/saga/packages/isn/config.hpp>

#include "entity_types.hpp"

namespace saga { namespace impl { class navigator; class entity_data_set;} }
///@cond
namespace saga
{
  namespace isn {

  class SAGA_ISN_PACKAGE_EXPORT navigator 
       : public saga::object
  {

   public:

   private:
      // factory
      SAGA_CALL_CREATE_PRIV_3(std::string const& model,
                              session const&,
                              saga::url)

      SAGA_CALL_PRIV_1 (list_related_entity_names, std::string)

      SAGA_CALL_PRIV_4 (get_related_entities,
                        std::string,
                        std::string,
                        std::string,
                        std::vector<saga::isn::entity_data>)

      SAGA_CALL_PRIV_2 (get_entities,
                        std::string,
                        std::string)

   protected:
      TR1::shared_ptr <saga::impl::navigator> get_impl_sp (void) const;
      saga::impl::navigator* get_impl (void) const;
      friend class saga::impl::navigator;
      explicit navigator(saga::impl::navigator* impl)
         SAGA_THROW_SPEC(throw(saga::not_implemented,
                               saga::incorrect_url,
                               saga::does_not_exist,
                               saga::authorization_failed,
                               saga::authentication_failed,
                               saga::timeout,
                               saga::no_success));

   public:
     explicit navigator(std::string const& model = std::string(),
                        saga::url loc = saga::url())
         SAGA_THROW_SPEC(throw(saga::not_implemented,
                               saga::incorrect_url,
                               saga::does_not_exist,
                               saga::authorization_failed,
                               saga::authentication_failed,
                               saga::timeout,
                               saga::no_success));

      explicit navigator(std::string const& model,
                         session const& s,
                         saga::url loc = saga::url())
         SAGA_THROW_SPEC(throw(saga::not_implemented,
                               saga::incorrect_url,
                               saga::does_not_exist,
                               saga::authorization_failed,
                               saga::authentication_failed,
                               saga::timeout,
                               saga::no_success));

      ~navigator (void) SAGA_THROW_SPEC(throw());

      static navigator create(std::string const& model,
                              session const& s,
                              saga::url loc = saga::url())
         SAGA_THROW_SPEC(throw(saga::not_implemented,
                               saga::incorrect_url,
                               saga::does_not_exist,
                               saga::authorization_failed,
                               saga::authentication_failed,
                               saga::timeout,
                               saga::no_success))
      {
          return navigator(model, s, loc);
      }
      SAGA_CALL_CREATE_3_DEF_1(std::string const&,
                               session const&,
                               saga::url,
                               saga::url())

      static navigator create(std::string const& model = std::string(),
                              saga::url loc = saga::url())
         SAGA_THROW_SPEC(throw(saga::not_implemented,
                               saga::incorrect_url,
                               saga::not_implemented,
                               saga::authorization_failed,
                               saga::authentication_failed,
                               saga::timeout,
                               saga::no_success))
      {
          return navigator(model, loc);
      }

      template <typename Tag>
      static saga::task create(std::string const& model = std::string(),
                               saga::url loc = saga::url())
      {
         return create<Tag>(model, saga::detail::get_the_session(), loc);
      }

      // default operator=() and copy ctor
      navigator& operator= (saga::object const& o) 
          SAGA_THROW_SPEC(throw());

      std::vector<std::string>
         list_related_entity_names(std::string entityName)
            SAGA_THROW_SPEC(throw(saga::bad_parameter,
                  saga::authorization_failed,
                  saga::authentication_failed,
                  saga::timeout,
                  saga::no_success))
      {
         saga::task t = list_related_entity_namespriv(entityName,
                                                      saga::task_base::Sync());
         return t.get_result<std::vector<std::string> >();
      }
      SAGA_CALL_PUB_1_DEF_0 (list_related_entity_names, std::string)

      std::vector<saga::isn::entity_data>
         get_related_entities(std::string entityName,
                              std::string related_entity_name,
                              std::string filter,
                              std::vector<saga::isn::entity_data> entities)
      {
         saga::task t = get_related_entitiespriv(entityName,
                                                 related_entity_name,
                                                 filter,
                                                 entities,
                                                 saga::task_base::Sync());
         return t.get_result<std::vector<saga::isn::entity_data> >();
      }
      SAGA_CALL_PUB_4_DEF_0 (get_related_entities,
                             std::string,
                             std::string,
                             std::string,
                             std::vector<saga::isn::entity_data>)

      std::vector<saga::isn::entity_data>
         get_entities(std::string entityName,
                      std::string filter)
      {
         saga::task t = get_entitiespriv(entityName,
                                         filter,
                                         saga::task_base::Sync());
         return t.get_result<std::vector<saga::isn::entity_data> >();
      }
      SAGA_CALL_PUB_2_DEF_0 (get_entities,
                             std::string,
                             std::string)
  }; // discoverer
 } // namespace isn
}  // namespace saga
///@endcond
#endif

