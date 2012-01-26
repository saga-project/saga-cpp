//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Use, modification and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/preprocessor/stringize.hpp>
#include <saga/saga/version.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga 
{
  // Returns the version of the SAGA engine
  unsigned long get_engine_version (void)
  {
    return SAGA_VERSION_FULL;
  }

  // Returns the implemented SAGA API version
  unsigned long get_saga_version (void)
  {
    return SAGA_VERSION_API;
  }

#if !defined(BOOST_WINDOWS)
  char const SAGA_CHECK_VERSION[] = BOOST_PP_STRINGIZE(SAGA_CHECK_VERSION);
#else
  char const* SAGA_CHECK_VERSION() 
  {
    return BOOST_PP_STRINGIZE(SAGA_CHECK_VERSION);
  }
#endif

///////////////////////////////////////////////////////////////////////////////
} // namespace saga

