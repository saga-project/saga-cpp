//  Copyright (c) 2009 STFC
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef SAGA_IMPL_PACKAGES_ISN_ENTITY_DATA_SET_HPP
#define SAGA_IMPL_PACKAGES_ISN_ENTITY_DATA_SET_HPP

#include <saga/impl/config.hpp>
#include <saga/impl/engine/attribute.hpp>
#include <saga/impl/engine/object.hpp>
#include <saga/saga/url.hpp>

#include "entity_data_set.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace saga
{
  namespace impl
  {
    // Entity Data Set implementation
    class SAGA_ISN_PACKAGE_EXPORT entity_data_set
      : public saga::impl::object
    {
      explicit entity_data_set(entity_data_set const& eds);

      public:

      /**
       * Constructor of entity_data_set
       */
      entity_data_set(const std::string& model,
                      const std::string& entityType,
                      const std::string& filter,
                      saga::session const& sess,
                      const std::string& url);

      entity_data_set& operator=(const entity_data_set& rhs);
      ~entity_data_set();

      std::vector<saga::isn::entity_data> get_data() const;

      saga::isn::entity_data_set
         get_related_entities(const std::string& relatedName,
                              const std::string& filter = std::string()) const;

      std::vector<std::string>
         list_related_entity_names() const;

      std::string get_information_system_url() const
      {
         return _informationServer.get_string();
      }

      std::size_t get_entity_count()
      {
         return _entities.size();
      }

      // Generate an exact deep copy of this object
      saga::object clone() const;

      private:
      entity_data_set(const std::string& model,
                      const std::string& entityType,
                      const std::string& filter,
                      saga::session const& sess,
                      const std::string& url,
                      bool populate);

      void init_eds(const std::string& model,
                    const std::string& entityType,
                    const std::string& filter,
                    saga::session const& sess,
                    const std::string& url,
                    bool populate);

      void set_information_system_url(const std::string& server)
      {
         _informationServer = server;
      }

      void swap(entity_data_set& other) SAGA_THROW_SPEC(throw());

      void populateEntity(saga::isn::entity_data& entity,
                          const std::multimap<std::string,
                                              std::string>& data) const;

      //the list of Entities that this entity_data_set contains
      std::vector<saga::isn::entity_data> _entities;

      //Model type
      std::string _model;

      //Name of this entity_data_set in both Glue and LDAP format
      std::string _name;

      saga::url _informationServer;

      saga::session _session;

      //Store the related entities in a vector
      mutable std::vector<std::string> _relatedEntities;
    };

  } // namespace impl

} // namespace saga

#endif // !defined(SAGA_IMPL_PACKAGES_ISN_ENTITY_DATA_SET_HPP)

