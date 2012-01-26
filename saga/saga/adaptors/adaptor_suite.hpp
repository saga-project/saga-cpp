//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_SAGA_ADAPTORS_ADAPTOR_SUITE_HPP
#define SAGA_SAGA_ADAPTORS_ADAPTOR_SUITE_HPP

#include <saga/saga/adaptors/adaptor_suite_data.hpp>

#include <boost/spirit/core/non_terminal/impl/static.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga 
{
  /// @cond
  /** These methods are not within API scope */
  class adaptor_suite 
  {
    private:
      // this mutex is used to synchronize access to the adaptor suite member data
      typedef boost::recursive_mutex mutex_type;
      mutex_type mtx_;


    public:
      // support functions for the adaptor_data helper class
      void lock_data (void) 
      {
#if BOOST_VERSION < 103500
        boost::detail::thread::lock_ops <mutex_type>::lock (mtx_);
#else
        mtx_.lock ();
#endif
      }

      void unlock_data (void) 
      {
#if BOOST_VERSION < 103500
        boost::detail::thread::lock_ops <mutex_type>::unlock (mtx_);
#else
        mtx_.unlock ();
#endif
      }
  };
  /// @endcond

} // namespace saga
///////////////////////////////////////////////////////////////////////////////

#endif // SAGA_SAGA_ADAPTORS_ADAPTOR_SUITE_HPP

