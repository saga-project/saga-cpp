//  Copyright (c) 2009 STFC
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef SAGA_PACKAGES_ISN_ENTITY_DATA_SET_HPP
#define SAGA_PACKAGES_ISN_ENTITY_DATA_SET_HPP

#include <string>
#include <map>
#include <vector>
#include <iostream>

#include <saga/saga/util.hpp>
#include <saga/saga/base.hpp>
#include <saga/saga/types.hpp>
#include <saga/saga/call.hpp>
#include <saga/saga/detail/attribute.hpp>
#include <saga/saga/packages/isn/config.hpp>

#include "entity_data.hpp"

namespace saga { namespace impl { class entity_data_set; } }

namespace saga
{
   namespace isn {

/**
 * <p>
 * Provides the means to navigate around the information model for a selected
 * entity and gives access to the {@link entity_data} objects.
 * </p>
 * <p>
 * Navigation consists of moving from entity to entity within an information
 * model, as expressed in the GLUE entity relationship model. Navigation can
 * also be from entity to related entity. A list of possible navigation steps
 * from an <code>entity_data_set</code> object is returned by the
 * <code>list_related_entity_names</code> method. Navigation to a set of related
 * entities is achieved with the <code>get_related_entities</code> method, which
 * returns a new <code>entity_data_set</code> object. N.B. navigation is from a
 * set of <code>entity_data</code> objects to a new set, a many to many
 * relationship.
 * </p>
 * In order to restrict the number of <code>entity_data</code> objects returned
 * in the <code>entity_data_set</code> object, a filter may be used with the
 * <code>get_related_entities</code> method. The filter MUST only include
 * attributes from the related entity and it will be applied to the related
 * entities.
 * </p>
 */
class SAGA_ISN_PACKAGE_EXPORT entity_data_set
   : public saga::object
{
   protected:
      friend class saga::impl::entity_data_set;

      explicit entity_data_set(saga::impl::entity_data_set* impl);

      saga::impl::entity_data_set* get_impl(void) const;

   public:
/**
     * Creates an <code>EntityDataSet</code> that contains the set of entities
     * that pass the specified filter. The filter MUST only include attributes
     * from the named entity. Details about the filter can be found in the <a
     * href="package-summary.html#filters">package</a> description. The url
     * specified is to assist the implementation to locate the underlying
     * information system such that it can be queried.
     * 
     * @param model
     *            a string containing the name of the information model
     * @param entityType
     *            a string containing the name of the entity to navigate
     * @param filter
     *            a string containing the filter for filtering entities, may be
     *            <code>null</code>
     * @param sess
     *            the session handle
     * @param url
     *            the URL to guide the implementation
     * @return the entity_data_set instance
     * @throws bad_parameter
     *             if the related name is not valid, or the filter is not valid
     * @throws does_not_exist
     *             if the url is syntactically valid, but no service can be
     *             contacted at that URL
     * @throws no_success
     *             if no result can be returned because of information system or
     *             other internal problems
     * @throws not_implemented
     *             if not implemented by that SAGA implementation at all
     */
      entity_data_set(const std::string& model,
                      const std::string& entityType,
                      const std::string& filter,
                      session const& sess,
                      const std::string& url)
         SAGA_THROW_SPEC(throw(saga::bad_parameter,
                               saga::does_not_exist,
                               saga::no_success,
                               saga::not_implemented));

    /**
     * Creates an <code>entity_data_set</code> that contains the set of entities
     * that pass the specified filter. The filter MUST only include attributes
     * from the named entity. Details about the filter can be found in the <a
     * href="package-summary.html#filters">package</a> description.
     * 
     * @param model
     *            a string containing the name of the information model
     * @param entityType
     *            a string containing the type of the entity to navigate to
     * @param filter
     *            a string containing the filter for filtering entities
     * @return the entity_data_set instance
     * @throws BadParameter
     *             if the related name is not valid, or the filter is not valid
     * @throws DoesNotExist
     *             if the url is syntactically valid, but no service can be
     *             contacted at that URL
     * @throws NoSuccess
     *             if no result can be returned because of information system or
     *             other internal problems
     * @throws NotImplemented
     *             if not implemented by that SAGA implementation at all
     */
      entity_data_set(const std::string& model,
                      const std::string& entityType,
                      const std::string& filter)
         SAGA_THROW_SPEC(throw(saga::bad_parameter,
                               saga::does_not_exist,
                               saga::no_success,
                               saga::not_implemented));

      explicit entity_data_set(saga::object const& o);

      ~entity_data_set(){}

    /**
     * Returns a vector of <code>entity_data</code> objects.
     * 
     * @return a vector of <code>entity_data</code> objects
     *  associated with this entity
     */
      std::vector<saga::isn::entity_data> get_data() const
         SAGA_THROW_SPEC(throw());

    /**
     * Returns an <code>entity_data_set</code> object for the given entity name
     * and matching the filter string. The filter MUST only include attributes
     * from the related entity. More details about the filter can be found in
     * the <a href="package-summary.html#filters">package</a> description. N.B.
     * There is a special case where there is a self relationship between
     * entities, i.e. "AdminDomain" in GLUE 2, in such cases the keywords
     * <code>up</code> and <code>down</code> may be used in place of the
     * name of the related entity to navigate to. For example where
     * AdminDomain="rl.ac.uk" up may return AdminDomain="ac.uk".
     * 
     * @param relatedName
     *            a string containing the name of the related entity to
     *            navigate to
     * @param filter
     *            a string containing the filter for filtering related
     *            entities, may be empty
     * @return a related entity_data_set matching the specified filter string
     * @throws BadParameter
     *             if the related name is not valid, or the filter is not valid
     * @throws NoSuccess
     *             if no result can be returned because of information system
     *             or other internal problems
     * @see #list_related_entity_names()
     */
      saga::isn::entity_data_set
         get_related_entities(const std::string& relatedName,
                              const std::string& filter = std::string()) const
         SAGA_THROW_SPEC(throw(saga::bad_parameterm saga::no_success));

/**
     * Returns a set of names of those entities that may be navigated to, from
     * this entity_data_set. N.B There is a special case where there is a self
     * relationship between entities, i.e. "AdminDomain" in GLUE 2, in such
     * cases the keywords <code>up</code> and <code>down</code> will also be
     * returned as appropriate.
     * 
     * @return a list of names of related entities
     */
      std::vector<std::string>
         list_related_entity_names() const;

      std::string get_information_system_url() const;

      std::size_t get_entity_count() const;
};

} //namespace isn
} //namespace saga
////////////////////////////////////////////////////////////////////////
#endif

