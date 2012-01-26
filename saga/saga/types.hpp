//  Copyright (c) 2005-2006 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2006 Hartmut Kaiser
// 
//  Use, modification and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_SPEC_TYPES_HPP
#define SAGA_SPEC_TYPES_HPP

#include <cstddef>              // std::size_t
#include <boost/config.hpp>
#include <boost/cstdint.hpp>    // boost::
#include <sys/types.h>          // ::off_t, ::ssize_t

/// @cond
/** These methods are not within API scope */

///////////////////////////////////////////////////////////////////////////////
// ssize_t isn't defined for the MS compilers
#if defined (BOOST_MSVC)

#ifndef _SSIZE_T_DEFINED
# ifdef   WIN64
   typedef signed __int64    ssize_t;
# else
   typedef _W64 signed int   ssize_t;
# endif
# define  _SSIZE_T_DEFINED
#endif // _SSIZE_T_DEFINED

#endif

///////////////////////////////////////////////////////////////////////////////
namespace saga
{
  typedef char                    char_t;

  typedef boost::int8_t           int8_t;             
  typedef boost::uint8_t          uint8_t;            
                               
  typedef boost::int16_t          int16_t;            
  typedef boost::uint16_t         uint16_t;           
                               
  typedef boost::int32_t          int32_t;            
  typedef boost::uint32_t         uint32_t;           
                               
# ifndef  BOOST_NO_INT64_T
  typedef boost::int64_t          int64_t;            
  typedef boost::uint64_t         uint64_t;           
# endif                        
                               
  typedef boost::intmax_t         intmax_t;      
  typedef boost::uintmax_t        uintmax_t;     
                               
  typedef boost::long_long_type   long_long_t;

  typedef std::size_t             size_t;
  typedef ::ssize_t               ssize_t;
  typedef ::off_t                 off_t;

} // namespace saga
///////////////////////////////////////////////////////////////////////////////

/// @endcond

#endif // SAGA_SPEC_TYPES_HPP

