//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Use, modification and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga/packages/stream/version.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga {

// Returns the version of the SAGA stream package
unsigned long get_stream_package_version()
{
    return SAGA_VERSION_PACKAGE_STREAM_FULL;
}

// Returns the implemented SAGA stream package API version
unsigned long get_stream_package_api_version()
{
    return SAGA_VERSION_PACKAGE_STREAM_API;
}

///////////////////////////////////////////////////////////////////////////////
}   // namespace saga

