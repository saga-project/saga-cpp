//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005-2008 Ole Weidner
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_VERSION_HPP
#define SAGA_VERSION_HPP

#include <boost/preprocessor/cat.hpp>
#include <boost/config.hpp>
#include <saga/saga/util.hpp>

// SAGA_VERSION and Co come from saga-config
#ifndef SAGA_SKIP_CONFIG_HPP
# include <saga/saga-config.hpp>
#endif

// masks needed to get at the parts of the full SAGA_VERSION
#define SAGA_VERSION_MAJORMASK    0xFF0000  //  SAGA_VERSION_FULL & 0xFF0000 is the major version
#define SAGA_VERSION_MINORMASK    0x00FF00  //  SAGA_VERSION_FULL & 0x00FF00 is the minor version
#define SAGA_VERSION_SUBMINORMASK 0x0000FF  //  SAGA_VERSION_FULL & 0x0000FF is the sub-minor version

// The version check enforces the major and minor version numbers to match for
// every compilation unit to be compiled.
#define SAGA_CHECK_VERSION                                                    \
    BOOST_PP_CAT(saga_check_version_,                                         \
        BOOST_PP_CAT(SAGA_VERSION_MAJOR,                                      \
            BOOST_PP_CAT(_, SAGA_VERSION_MINOR)))                             \
    /**/

///////////////////////////////////////////////////////////////////////////////
//  The SAGA API version: here V1.0.0
#define SAGA_VERSION_API          0x010000

///////////////////////////////////////////////////////////////////////////////
//
//  SAGA_VERSION_ISCOMPATIBLE(), SAGA_VERSION_ISCOMPATIBLE_EX()
// 
//  The SAGA_VERSION_ISCOMPATIBLE macros test, if the version of the loaded
//  SAGA engine is compatible with the version the calling application was 
//  compiled against.
//
#define SAGA_VERSION_ISCOMPATIBLE_EX(version)                                 \
    (((version) & ~SAGA_VERSION_SUBMINORMASK) >=                              \
       (SAGA_VERSION_FULL & ~SAGA_VERSION_SUBMINORMASK))                      \
  /**/
  
#define SAGA_VERSION_ISCOMPATIBLE()                                           \
    SAGA_VERSION_ISCOMPATIBLE_EX(saga::get_engine_version())                  \
  /**/

/*! \brief The SAGA root namespace
 */
namespace saga 
{
  /*! \brief Brief %description starts here     
   *
   *
   */
  SAGA_EXPORT unsigned long get_engine_version (void);
  
  /*! \brief Brief %description starts here     
   *
   *
   */
  SAGA_EXPORT unsigned long get_saga_version (void);
  
  /*! \brief Brief %description starts here     
   *
   *
   */
#if !defined(BOOST_WINDOWS)
  SAGA_EXPORT extern char const SAGA_CHECK_VERSION[];
#else
  SAGA_EXPORT char const* SAGA_CHECK_VERSION();
#endif

///////////////////////////////////////////////////////////////////////////////
} // namespace saga

// This is instantiated per translation unit, forcing to resolve the variable
// SAGA_CHECK_VERSION
namespace
{
    char const* check_saga_version()
    {
#if !defined(BOOST_WINDOWS)
        return saga::SAGA_CHECK_VERSION;
#else
        return saga::SAGA_CHECK_VERSION();
#endif
    }
}

#endif // !SAGA_VERSION_HPP

