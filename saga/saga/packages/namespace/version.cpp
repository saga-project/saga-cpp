//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga/packages/namespace/version.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga 
{
  // Returns the version of the SAGA namespace package
  unsigned long get_namespace_package_version()
  {
      return SAGA_VERSION_PACKAGE_NAMESPACE_FULL;
  }
  
  // Returns the implemented SAGA namespace package API version
  unsigned long get_namespace_package_api_version()
  {
      return SAGA_VERSION_PACKAGE_NAMESPACE_API;
  }

}   // namespace saga
///////////////////////////////////////////////////////////////////////////////

