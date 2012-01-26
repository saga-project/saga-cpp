//  future interface functions

//  Copyright (c) 2005 Thorsten Schuett. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUTURIZE_HPP
#define BOOST_FUTURIZE_HPP 1

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

#include <boost/futures/future_base.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace boost {  namespace futures { 
  
  /////////////////////////////////////////////////////////////////////////////
  template <typename Future>
  inline typename Future::result_type
  futurize(future_base<Future> const& f)
  {
      return f.derived()();
  }

  /////////////////////////////////////////////////////////////////////////////
  template <typename Future>
  inline bool
  is_done(future_base<Future> const& f)
  {
      return f.derived().done();
  }
  
///////////////////////////////////////////////////////////////////////////////
}}  // namespace boost::futures

#endif /* BOOST_FUTURIZE_HPP */
