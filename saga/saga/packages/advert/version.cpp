//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga/packages/advert/version.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga 
{
    // Returns the version of the SAGA advert package
    unsigned long get_advert_package_version (void)
    {
        return SAGA_VERSION_PACKAGE_ADVERT_FULL;
    }

    // Returns the implemented SAGA file package API version
    unsigned long get_advert_package_api_version (void)
    {
        return SAGA_VERSION_PACKAGE_ADVERT_API;
    }

} // namespace saga
///////////////////////////////////////////////////////////////////////////////

