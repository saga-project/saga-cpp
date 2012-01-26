//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2007 Ole Weidner (oweidner@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

/// @cond 
/* BEGIN: Exclude from Doxygen */

#ifndef SAGA_UTIL_HPP
#define SAGA_UTIL_HPP

#include <utility>
#include <climits>
#include <saga/saga/base.hpp>

#include <boost/config.hpp>

// Include Boost specific libraries we rely on in the interface, but which are 
// _not_ replaceable by standard components. We use these as a convenience to 
// avoid reimplementing bicycles. If independence from Boost some day will be a 
// requirement these will have to be replaced.
#include <boost/cstdint.hpp>

// Include Boost specific libraries we rely on in the interface, but which
// will be replaced by standard headers as soon as TR1 implementations will be
// available more widely
#if defined(SAGA_USE_TR1_NAMESPACE)

#if defined(SAGA_PRINT_USED_NAMESPACE)
#if defined(_MSC_VER) || defined(__BORLANDC__) || defined(__DMC__)
#pragma message("Note: using namespace std::tr1")
#elif defined(__GNUC__) || defined(__HP_aCC) || defined(__SUNPRO_CC) || defined(__IBMCPP__)
#warning "using namespace std::tr1"
#endif
#endif

#include <type_traits>  // type traits
#include <memory>       // shared_ptr, enable_shared_from_this
#include <functional>   // function, ref, bind
#include <regex>        // regex
#include <array>        // array
#define TR1 std::tr1    // define proper namespace
#else

#if defined(SAGA_PRINT_USED_NAMESPACE)
#if defined(_MSC_VER) || defined(__BORLANDC__) || defined(__DMC__)
#pragma message("Note: using namespace boost")
#elif defined(__GNUC__) || defined(__HP_aCC) || defined(__SUNPRO_CC) || defined(__IBMCPP__)
#warning "using namespace boost"
#endif
#endif

#include <boost/ref.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/function.hpp>
#include <boost/regex.hpp>
#include <boost/array.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/is_reference.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#define TR1 boost       // define proper namespace

// pull in bind placeholders into boost::placeholders


namespace boost { namespace placeholders 
{
    using ::_1;
    using ::_2;
    using ::_3;
    using ::_4;
    using ::_5;
    using ::_6;
    using ::_7;
    using ::_8;
    using ::_9;
}}
#endif

#include <saga/saga/export_definitions.hpp>

///////////////////////////////////////////////////////////////////////////////
// maximum count of possible API parameters
#ifndef SAGA_ARGUMENT_LIMIT
#define SAGA_ARGUMENT_LIMIT  6              // default argument count limit
#endif

// make sure this file gets included before any of the Phoenix files
#if defined(PHOENIX_LIMIT) && PHOENIX_LIMIT < 6
# if defined(PHOENIX_TUPLES_HPP)
#  error "This file should be included before any of the Phoenix includes, or #define PHOENIX_LIMIT >= 6."
# else
#  undef PHOENIX_LIMIT
# endif
#endif

#if !defined(PHOENIX_LIMIT)
# define PHOENIX_LIMIT SAGA_ARGUMENT_LIMIT   // tuple size limit
#endif

///////////////////////////////////////////////////////////////////////////////
//  Character used to delimit several saga ini paths
#ifdef BOOST_WINDOWS // windows
#  define SAGA_INI_PATH_DELIMITER     ";"
#  define SAGA_SHARED_LIB_EXTENSION   ".dll"
#  define SAGA_PATH_DELIMITERS        "\\/"
#else                // unix like
#  define SAGA_INI_PATH_DELIMITER     ":"
#  define SAGA_PATH_DELIMITERS        "/"
#  ifdef SAGA_APPLE   // apple
#    define SAGA_SHARED_LIB_EXTENSION ".dylib"
#  else              // linux & co
#    define SAGA_SHARED_LIB_EXTENSION ".so"
#  endif
#endif

///////////////////////////////////////////////////////////////////////////////
// define missing POSIX functions
#if defined(BOOST_WINDOWS)
#define snprintf _snprintf
#endif

///////////////////////////////////////////////////////////////////////////////
// We don't need to export the plugins functions for SAGA lite
#if defined(BUILD_SAGA_LITE)
#define BOOST_PLUGIN_NO_EXPORT_API
#endif

///////////////////////////////////////////////////////////////////////////////
// on Windows the debug versions of the libraries have mangled entry points 
#if !defined(BOOST_WINDOWS)
# define SAGA_MANGLE_ADAPTOR_NAME(name)        BOOST_PP_CAT(libsaga_adaptor_, name)
# define SAGA_MANGLE_ADAPTOR_NAME_STR(name)    "libsaga_adaptor_" + name
# define SAGA_MANGLE_PACKAGE_NAME(name)        BOOST_PP_CAT(libsaga_package_, name)
# define SAGA_MANGLE_PACKAGE_NAME_STR(name)    "libsaga_package_" + name
# define SAGA_MANGLE_ENGINE_NAME(name)         BOOST_PP_CAT(libsaga_, name)
# define SAGA_MANGLE_ENGINE_NAME_STR(name)     "libsaga_" + name
#elif defined(_DEBUG)
# define SAGA_MANGLE_ADAPTOR_NAME(name)         BOOST_PP_CAT(name, d)
# define SAGA_MANGLE_ADAPTOR_NAME_STR(name)     name + "d"
# define SAGA_MANGLE_PACKAGE_NAME(name)         BOOST_PP_CAT(name, d)
# define SAGA_MANGLE_PACKAGE_NAME_STR(name)     name + "d"
# define SAGA_MANGLE_ENGINE_NAME(name)          BOOST_PP_CAT(saga_, BOOST_PP_CAT(name, d))
# define SAGA_MANGLE_ENGINE_NAME_STR(name)      "saga_" + name + "d"
#else
# define SAGA_MANGLE_ADAPTOR_NAME(name)         name
# define SAGA_MANGLE_ADAPTOR_NAME_STR(name)     name
# define SAGA_MANGLE_PACKAGE_NAME(name)         name
# define SAGA_MANGLE_PACKAGE_NAME_STR(name)     name
# define SAGA_MANGLE_ENGINE_NAME(name)          BOOST_PP_CAT(saga_, name)
# define SAGA_MANGLE_ENGINE_NAME_STR(name)      "saga_" + name
#endif

///////////////////////////////////////////////////////////////////////////////
// some people like to have exception specifications, allow to use them if
// SAGA_USE_EXCEPTION_SPECIFICATIONS is defined
#if defined(SAGA_USE_EXCEPTION_SPECIFICATIONS)
#define SAGA_THROW_SPEC(x) x
#else
#define SAGA_THROW_SPEC(x) 
#endif

///////////////////////////////////////////////////////////////////////////////
// Make sure every module has a name
#if !defined(SAGA_MODULE_NAME)
# if defined(SAGA_PACKAGE_NAME)
#   define SAGA_MODULE_NAME                 SAGA_PACKAGE_NAME
#   define SAGA_MANGLED_MODULE_NAME         SAGA_MANGLE_PACKAGE_NAME(SAGA_PACKAGE_NAME)
# elif defined(SAGA_ADAPTOR_NAME)
#   define SAGA_MODULE_NAME                 SAGA_ADAPTOR_NAME
#   define SAGA_MANGLED_MODULE_NAME         SAGA_MANGLE_ADAPTOR_NAME(SAGA_ADAPTOR_NAME)
# elif defined(SAGA_ENGINE_EXPORTS)
#   define SAGA_MODULE_NAME                 saga_core
#   define SAGA_MANGLED_MODULE_NAME         SAGA_MANGLE_ENGINE_NAME(core)
# else
#   define SAGA_MODULE_NAME                 unknown
#   define SAGA_MANGLED_MODULE_NAME         unknown
# endif
#endif

#endif  // SAGA_UTIL_HPP

/// @endcond 
/* END: Exclude from Doxygen */
