//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/thread.hpp>
#include <saga/saga/base.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga
{
  namespace detail
  {
    // this global mutex is used to protect the getenv function which is not
    // reenterant
    static boost::mutex &mutex_instance()
    {
        static boost::mutex mutex;
        return mutex;
    }
    static void mutex_init()
    {
        mutex_instance();
    }

    /// @cond
    /** These methods are not within API scope TODO: wrong namespace?!? */
    char *safe_getenv(char const* name)
    {
        static boost::once_flag been_here = BOOST_ONCE_INIT;
        boost::call_once(mutex_init, been_here);

        boost::mutex::scoped_lock l(mutex_instance());
        return getenv(name);
    }
    /// @endcond
    
  } // detail
} // saga
///////////////////////////////////////////////////////////////////////////////
