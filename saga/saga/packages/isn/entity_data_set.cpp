//  Copyright (c) 2009 STFC
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <memory>
#include <iostream>
#include <algorithm>
#include <vector>
#include <sstream>

#include <saga/saga/adaptors/task.hpp>
#include <saga/saga/isn.hpp>
#include <saga/impl/isn.hpp>
#include <saga/saga/detail/call.hpp>
#include <saga/saga/exception.hpp>

namespace saga { namespace isn {
/*
 * entity_data_set
 */
entity_data_set::entity_data_set(const std::string& model,
                                 const std::string& entityType,
                                 const std::string& filter,
                                 session const& sess,
                                 const std::string& url)
   : object(new saga::impl::entity_data_set(model,
                                            entityType,
                                            filter,
                                            sess,
                                            url))
{
}

entity_data_set::entity_data_set(const std::string& model,
                                 const std::string& entityType,
                                 const std::string& filter)
   : object(new saga::impl::entity_data_set(model,
                                            entityType,
                                            filter,
                                            saga::detail::get_the_session(),
                                            std::string()))
{
}

//Copy constructor
entity_data_set::entity_data_set(saga::impl::entity_data_set* impl)
   : object(impl)
{
}

entity_data_set::entity_data_set(saga::object const& o)
   : saga::object(o)
{
   if ( this->get_type() != saga::object::ServiceDescription )
   {
      SAGA_THROW("Bad type conversion.", saga::BadParameter);
   }
}

//Return a copy of the entities in this entity_data_set
std::vector<saga::isn::entity_data> entity_data_set::get_data() const
{
   return get_impl()->get_data();
}

saga::impl::entity_data_set* entity_data_set::get_impl() const
{ 
   typedef saga::object base_type;
   return
      static_cast<saga::impl::entity_data_set*>(this->base_type::get_impl()); 
}

saga::isn::entity_data_set
   entity_data_set::get_related_entities(const std::string& relatedName,
                                         const std::string& filter) const
{
   return get_impl()->get_related_entities(relatedName,
                                           filter);
}

std::vector<std::string>
   entity_data_set::list_related_entity_names() const
{
   return get_impl()->list_related_entity_names();
}

std::string entity_data_set::get_information_system_url() const
{
   return get_impl()->get_information_system_url();
}

std::size_t entity_data_set::get_entity_count() const
{
   return get_impl()->get_entity_count();
}

} //namespace isn
} //namespace saga

