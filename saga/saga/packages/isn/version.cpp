//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga/packages/isn/version.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga {

// Returns the version of the SAGA isn package
unsigned long get_isn_package_version()
{
    return SAGA_VERSION_PACKAGE_ISN_FULL;
}

// Returns the implemented SAGA isn package API version
unsigned long get_isn_package_api_version()
{
    return SAGA_VERSION_PACKAGE_ISN_API;
}

///////////////////////////////////////////////////////////////////////////////
}   // namespace saga

