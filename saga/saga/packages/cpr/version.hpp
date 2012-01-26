//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2008 Andre Merzky (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_PACKAGE_CPR_VERSION_HPP
#define SAGA_PACKAGE_CPR_VERSION_HPP

#include <saga/saga/util.hpp>
#include <saga/saga/version.hpp>
#include <saga/saga/packages/cpr/config.hpp>

///////////////////////////////////////////////////////////////////////////////
//  The version of the SAGA file package
//
//  SAGA_VERSION_PACKAGE_CPR_FULL & 0x0000FF is the sub-minor version
//  SAGA_VERSION_PACKAGE_CPR_FULL & 0x00FF00 is the minor version
//  SAGA_VERSION_PACKAGE_CPR_FULL & 0xFF0000 is the major version
//
#define SAGA_VERSION_PACKAGE_CPR_FULL         SAGA_VERSION_FULL

#define SAGA_VERSION_PACKAGE_CPR_MAJOR        SAGA_VERSION_MAJOR
#define SAGA_VERSION_PACKAGE_CPR_MINOR        SAGA_VERSION_MINOR
#define SAGA_VERSION_PACKAGE_CPR_SUBMINOR     SAGA_VERSION_SUBMINOR

#define SAGA_VERSION_PACKAGE_CPR_DATE         SAGA_VERSION_DATE

#define SAGA_VERSION_PACKAGE_CPR_SUBMINORMASK SAGA_VERSION_SUBMINORMASK

///////////////////////////////////////////////////////////////////////////////
//  The SAGA cpr package API version: here V1.0.0
#define SAGA_VERSION_PACKAGE_CPR_API          SAGA_VERSION_API

///////////////////////////////////////////////////////////////////////////////
//
//  SAGA_VERSION_PACKAGE_CPR_ISCOMPATIBLE()
// 
//  The SAGA_VERSION_ISCOMPATIBLE macro tests, if the version of the loaded
//  SAGA engine is compatible with the version the calling application was 
//  compiled against.
//
#define SAGA_VERSION_PACKAGE_CPR_ISCOMPATIBLE_EX(version)                     \
(((version) & ~SAGA_VERSION_PACKAGE_CPR_SUBMINORMASK) >=                  \
(SAGA_VERSION_PACKAGE_CPR_FULL & ~SAGA_VERSION_PACKAGE_CPR_SUBMINORMASK)) \
/**/

#define SAGA_VERSION_PACKAGE_CPR_ISCOMPATIBLE()                               \
SAGA_VERSION_PACKAGE_CPR_ISCOMPATIBLE_EX(saga::get_cpr_package_version()) \
/**/

///////////////////////////////////////////////////////////////////////////////
namespace saga {
    
    // Returns the version of the SAGA cpr package
    SAGA_CPR_PACKAGE_EXPORT unsigned long get_cpr_package_version (void);
    
    // Returns the implemented SAGA cpr package API version
    SAGA_CPR_PACKAGE_EXPORT unsigned long get_cpr_package_api_version (void);
    
} // namespace saga
///////////////////////////////////////////////////////////////////////////////

#endif // SAGA_PACKAGE_CPR_VERSION_HPP


