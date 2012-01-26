//  Copyright (c) 2010 Andre Merzky
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_DETAIL_HPP
#define SAGA_DETAIL_HPP

namespace saga 
{
  namespace detail
  {
    ///////////////////////////////////////////////////////////////////////////
    // define a thread safe getenv() function (use this instead of direct calls
    // to getenv
    
    /// @cond
    /** These methods are not within API scope TODO: wrong namespace?!? */
    SAGA_EXPORT char* safe_getenv(const char *);
    /// @endcond
  }
}

#endif // SAGA_DETAIL_HPP




