//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(SAGA_PACKAGE_ISN_VERSION_HPP)
#define SAGA_PACKAGE_ISN_VERSION_HPP

#include <saga/saga/util.hpp>
#include <saga/saga/version.hpp>
#include <saga/saga/packages/isn/config.hpp>

///////////////////////////////////////////////////////////////////////////////
//  The version of the SAGA isn package
//
//  SAGA_VERSION_PACKAGE_STREAM_FULL & 0x0000FF is the sub-minor version
//  SAGA_VERSION_PACKAGE_STREAM_FULL & 0x00FF00 is the minor version
//  SAGA_VERSION_PACKAGE_STREAM_FULL & 0xFF0000 is the major version
//
#define SAGA_VERSION_PACKAGE_ISN_FULL        SAGA_VERSION_FULL
                                                 
#define SAGA_VERSION_PACKAGE_ISN_MAJOR        SAGA_VERSION_MAJOR
#define SAGA_VERSION_PACKAGE_ISN_MINOR        SAGA_VERSION_MINOR
#define SAGA_VERSION_PACKAGE_ISN_SUBMINOR     SAGA_VERSION_SUBMINOR
                                                 
#define SAGA_VERSION_PACKAGE_ISN_DATE         SAGA_VERSION_DATE
                                                 
#define SAGA_VERSION_PACKAGE_ISN_SUBMINORMASK SAGA_VERSION_SUBMINORMASK

///////////////////////////////////////////////////////////////////////////////
//  The SAGA isn package API version: here V1.0.0
#define SAGA_VERSION_PACKAGE_ISN_API          SAGA_VERSION_API

///////////////////////////////////////////////////////////////////////////////
//
//  SAGA_VERSION_PACKAGE_STREAM_ISCOMPATIBLE()
// 
//  The SAGA_VERSION_ISCOMPATIBLE macro tests, if the version of the loaded
//  SAGA engine is compatible with the version the calling application was 
//  compiled against.
//
#define SAGA_VERSION_PACKAGE_ISN_ISCOMPATIBLE_EX(version)                      \
    (((version) & ~SAGA_VERSION_PACKAGE_ISN_SUBMINORMASK) >=                   \
      (SAGA_VERSION_PACKAGE_ISN_FULL & ~SAGA_VERSION_PACKAGE_ISN_SUBMINORMASK)) \
  /**/

#define SAGA_VERSION_PACKAGE_ISN_ISCOMPATIBLE()                                \
    SAGA_VERSION_PACKAGE_ISN_ISCOMPATIBLE_EX(                                  \
      saga::get_isn_package_version())                                         \
  /**/

///////////////////////////////////////////////////////////////////////////////
namespace saga {

// Returns the version of the SAGA isn package
SAGA_ISN_PACKAGE_EXPORT unsigned long get_isn_package_version();

// Returns the implemented SAGA isn package API version
SAGA_ISN_PACKAGE_EXPORT unsigned long get_isn_package_api_version();

///////////////////////////////////////////////////////////////////////////////
}   // namespace saga

#endif // !SAGA_PACKAGE_ISN_VERSION_HPP


