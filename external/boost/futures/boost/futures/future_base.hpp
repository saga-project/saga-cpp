//  future protocoll base class

//  Copyright (c) 2005 Thorsten Schuett. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUTURE_BASE_HPP
#define BOOST_FUTURE_BASE_HPP 1

#include <boost/type_traits/is_base_and_derived.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace boost {  namespace futures { 
  
  namespace detail
  {
      struct future_tag {};
  }

  /////////////////////////////////////////////////////////////////////////////
  struct unused_type 
  {
  };
  
  /////////////////////////////////////////////////////////////////////////////
  template <typename Derived>
  struct future_base : detail::future_tag
  {
    Derived& derived()
    { 
        return *static_cast<Derived*>(this);
    }

    Derived const& derived() const
    {
        return *static_cast<Derived const*>(this);
    }
  }; 

  template <typename T>
  struct is_future : is_base_and_derived<detail::future_tag, T> {};

///////////////////////////////////////////////////////////////////////////////
}}  // namespace boost::futures

#endif /* BOOST_FUTURE_BASE_HPP */
