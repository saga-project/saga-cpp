//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_EXPORT_DEFINITIONS_HPP
#define SAGA_EXPORT_DEFINITIONS_HPP

// For symbol import/export macros
#include <saga/saga-defs.hpp>
#include <boost/config.hpp>

#if !defined (SAGA_CREATE_PREPROCESSED_FILES)

# if defined(SAGA_WINDOWS)
#  if defined(BOOST_MSVC)
#   define SAGA_DEPRECATED(x) __declspec(deprecated(x))
#  endif
# elif (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 4))
#  define SAGA_DEPRECATED(x) __attribute__((__deprecated__))
# endif

#if !defined(SAGA_DEPRECATED)
#define SAGA_DEPRECATED(x)  /**/
#endif

# if defined(SAGA_WINDOWS)
#   define SAGA_SYMBOL_EXPORT   __declspec (dllexport)
#   define SAGA_SYMBOL_EXPORT_REPEAT SAGA_SYMBOL_EXPORT
#   define SAGA_SYMBOL_IMPORT   __declspec (dllimport)
#   define SAGA_SYMBOL_INTERNAL /* empty */
# elif defined(SAGA_GCC_HAVE_VISIBILITY)
#   define SAGA_SYMBOL_EXPORT   __attribute__((visibility("default")))
#   define SAGA_SYMBOL_EXPORT_REPEAT /**/
#   define SAGA_SYMBOL_IMPORT   __attribute__((visibility("default")))
# ifdef __sparc__
#   define SAGA_SYMBOL_INTERNAL 
# else
#   define SAGA_SYMBOL_INTERNAL __attribute__((visibility("hidden")))
# endif
# else
#   define SAGA_SYMBOL_EXPORT   /* empty */
#   define SAGA_SYMBOL_EXPORT_REPEAT /**/
#   define SAGA_SYMBOL_IMPORT   /* empty */
#   define SAGA_SYMBOL_INTERNAL /* empty */
# endif

///////////////////////////////////////////////////////////////////////////////
// define the export/import helper macros

# ifdef     SAGA_ENGINE_EXPORTS
#   define  SAGA_EXPORT             SAGA_SYMBOL_EXPORT
#   define  SAGA_EXPORT_REPEAT      SAGA_SYMBOL_EXPORT_REPEAT
#   define  SAGA_EXCEPTION_EXPORT   SAGA_SYMBOL_EXPORT
# elif !defined(BUILD_SAGA_LITE)
#   define  SAGA_EXPORT             SAGA_SYMBOL_IMPORT
#   define  SAGA_EXPORT_REPEAT      /**/
#   define  SAGA_EXCEPTION_EXPORT   SAGA_SYMBOL_IMPORT
# else
#   define  SAGA_EXPORT             /* empty */
#   define  SAGA_EXPORT_REPEAT      /**/
#   define  SAGA_EXCEPTION_EXPORT   /* empty */
# endif

# ifdef     SAGA_PACKAGE_EXPORTS
#   define  SAGA_PACKAGE_EXPORT     SAGA_SYMBOL_EXPORT
#   define  SAGA_PACKAGE_LOCAL      SAGA_SYMBOL_INTERNAL
# elif !defined(BUILD_SAGA_LITE)
#   define  SAGA_PACKAGE_EXPORT     SAGA_SYMBOL_IMPORT
#   define  SAGA_PACKAGE_LOCAL      SAGA_SYMBOL_INTERNAL
# else
#   define  SAGA_PACKAGE_EXPORT     /* empty */
#   define  SAGA_PACKAGE_LOCAL      /* empty */
# endif

# ifdef     SAGA_ADAPTOR_EXPORTS
#   define  SAGA_ADAPTOR_EXPORT     SAGA_SYMBOL_EXPORT
#   define  SAGA_ADAPTOR_LOCAL      SAGA_SYMBOL_INTERNAL
# elif !defined(BUILD_SAGA_LITE)
#   define  SAGA_ADAPTOR_EXPORT     SAGA_SYMBOL_IMPORT
#   define  SAGA_ADAPTOR_LOCAL      SAGA_SYMBOL_INTERNAL
# else
#   define  SAGA_ADAPTOR_EXPORT     /* empty */
#   define  SAGA_ADAPTOR_LOCAL      /* empty */
# endif

# define    SAGA_LOCAL              /* empty */

#endif // !SAGA_CREATE_PREPROCESSED_FILES

#endif


