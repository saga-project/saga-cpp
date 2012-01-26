//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga/packages/rpc/version.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga {

// Returns the version of the SAGA rpc package
unsigned long get_rpc_package_version()
{
    return SAGA_VERSION_PACKAGE_RPC_FULL;
}

// Returns the implemented SAGA rpc package API version
unsigned long get_rpc_package_api_version()
{
    return SAGA_VERSION_PACKAGE_RPC_API;
}

///////////////////////////////////////////////////////////////////////////////
}   // namespace saga

