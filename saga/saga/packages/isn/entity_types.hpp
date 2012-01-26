//  Copyright (c) 2009 STFC
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef _ENTITY_TYPES_HPP_
#define _ENTITY_TYPES_HPP_

#include <string>
#include <map>

/// @cond
typedef struct
{
   std::string adaptorName;
   bool multivalued;
   std::string type;
} ENTITY_ATTR_MAP_TYPE;

typedef struct
{
   std::string primaryKey;
   std::string secondaryKey;
   bool reverseLookup;
   bool directLookup;
   std::string entityLdapName;
} ENTITY_RELATIONSHIP_TYPE;

typedef struct
{
   std::string adaptorName;
   std::map<std::string, ENTITY_ATTR_MAP_TYPE> attrs;
   std::multimap<std::string, ENTITY_RELATIONSHIP_TYPE> relatedEntities;
} ENTITY_ATTR_TYPE;
/// @endcond

#endif

