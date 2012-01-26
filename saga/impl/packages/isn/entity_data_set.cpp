//  Copyright (c) 2009 STFC
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <memory>
#include <iostream>
#include <algorithm>
#include <vector>
#include <sstream>

#include <saga/saga/isn.hpp>
#include <saga/impl/isn.hpp>

namespace saga { namespace impl {
/*
 * entity_data_set
 */
entity_data_set::entity_data_set(const std::string& model,
                                 const std::string& entityType,
                                 const std::string& filter,
                                 saga::session const& sess,
                                 const std::string& url)
   : saga::impl::object(saga::object::EntityDataSet)
{
   init_eds(model, entityType, filter, sess, url, true);
}

entity_data_set::entity_data_set(const std::string& model,
                                 const std::string& entityType,
                                 const std::string& filter,
                                 saga::session const& sess,
                                 const std::string& url,
                                 bool populate)
   : saga::impl::object(saga::object::EntityDataSet)
{
   init_eds(model, entityType, filter, sess, url, populate);
}

void entity_data_set::init_eds(const std::string& model,
                               const std::string& entityType,
                               const std::string& filter,
                               saga::session const& sess,
                               const std::string& url,
                               bool populate)
{
   set_information_system_url(url);

   _model = model;

   _session = sess;

   _name = entityType;

   if ( populate == true )
   {
      saga::isn::navigator nav(model, sess, url);
      _entities = nav.get_entities(entityType, filter);
   }
}

//Copy constructor
entity_data_set::entity_data_set(const entity_data_set& other)
   : saga::impl::object(saga::object::EntityDataSet)
{
   this->_model = other._model;
   this->_session = other._session;
   this->_entities = other._entities;
   this->_name = other._name;
   this->_relatedEntities = other._relatedEntities;
   this->_informationServer = other._informationServer;
}

//Assignment operator, implemented in terms of copy construction
entity_data_set& entity_data_set::operator=(const entity_data_set& rhs)
{
   //this is self-assignment safe
   //so we don't need to test for it
   entity_data_set temp(rhs);
   swap(temp);
   return *this;
}

//Non-throwing swap funtion.
//Allows exception safe assignment in terms of copy construction
//Absolutely nothing here can throw
void entity_data_set::swap(entity_data_set& other) SAGA_THROW_SPEC(throw())
{
   std::swap(_model, other._model);
   std::swap(_session, other._session);
   std::swap(_entities, other._entities);
   std::swap(_name, other._name);
   std::swap(_relatedEntities, other._relatedEntities);
   std::swap(_informationServer, other._informationServer);
}

//Standard destructor
entity_data_set::~entity_data_set()
{
}

// Generate a exact deep copy of this object
saga::object entity_data_set::clone() const
{
    return saga::isn::entity_data_set(
        new entity_data_set(*this));
}

//Return a copy of the entities in this entity_data_set
std::vector<saga::isn::entity_data> entity_data_set::get_data() const
{
   return _entities;
}

//Return an entity_data_set containing the entities related to this one
//by the relationship specified by relatedName
saga::isn::entity_data_set
   entity_data_set::get_related_entities(const std::string& relatedName,
                                         const std::string& filter) const
{
   std::vector<std::string> relEnt = list_related_entity_names();

   std::vector<std::string>::const_iterator relIter = relEnt.begin();

   while ( relIter != relEnt.end() )
   {
      if ( *relIter == relatedName )
      {
         break;
      }

      else
      {
         ++relIter;
      }
   }

   //Check this relationship exists
   if ( relIter == relEnt.end() )
   {
      std::string err("Unknown relationship ");
      err += relatedName;
      SAGA_THROW(err, saga::BadParameter);
   }

   //Create our entity_data_set
   entity_data_set* retVal =
      new entity_data_set(_model,
                          relatedName,
                          filter,
                          _session,
                          _informationServer.get_string(),
                          false);

   //Now we need to get all the details from the related entities
   saga::isn::navigator nav(_model, _session, _informationServer);
   retVal->_entities = nav.get_related_entities(_name,
                                                relatedName,
                                                filter,
                                                _entities);

   return saga::isn::entity_data_set(retVal);
}

//List the relationships that this entity_data_set supports,
//i.e. the paths that we can traverse from this object
std::vector<std::string>
   entity_data_set::list_related_entity_names() const
{
   //We cache the list of related entities
   //so first check to see if we have it
   if ( _relatedEntities.empty() )
   {
      saga::isn::navigator nav(_model, _informationServer);
      _relatedEntities = nav.list_related_entity_names(_name);
   }

   return _relatedEntities;
}

} //namespace impl
} //namespace saga

