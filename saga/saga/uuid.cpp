//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Use, modification and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

// include the session implementation we want to use
#include <iostream>

#include <saga/saga/uuid.hpp>
#include <saga/impl/engine/uuid.hpp> // FIXME impl/uuid.hpp

///////////////////////////////////////////////////////////////////////////////
namespace saga 
{
  uuid::uuid (void)
    : impl_ (new saga::impl::uuid ())
  {
  }

  uuid::uuid (char const * uuid_str)
    : impl_ (new saga::impl::uuid (uuid_str))
  {
  }

  std::string uuid::string (void) const
  {
    return (impl_->string ());
  }

  void uuid::create (void)
  {
    impl_->create ();
  }

  void uuid::create (char const * str)
  {
    impl_->create (str);
  }

  /// @cond 
  /** Doxygen generates namespace function from friend impls... */
  bool operator== (uuid const & lhs, uuid const & rhs)
  {
    return (*(lhs.impl_.get()) == *(rhs.impl_.get()));
  }

  bool operator!= (uuid const & lhs, uuid const & rhs)
  {
    return !(lhs == rhs);
  }

  bool operator<  (uuid const & lhs, uuid const & rhs)
  {
    return (*(lhs.impl_.get()) < *(rhs.impl_.get()));
  }

  bool operator>  (uuid const & lhs, uuid const & rhs)
  {
    return (*(lhs.impl_.get()) > *(rhs.impl_.get()));
  }

  bool operator<= (uuid const & lhs, uuid const & rhs)
  {
    return !(lhs > rhs);
  }

  bool operator>= (uuid const & lhs, uuid const & rhs)
  {
    return !(lhs < rhs);
  }

  std::ostream & operator<< (std::ostream & ostrm, uuid const   & rhs)
  {
    return (ostrm << rhs.string());
  }

  std::istream & operator>> (std::istream & istrm, uuid         & rhs)
  {
    std::string instr;
    istrm >> instr;

    rhs.impl_->create (instr.c_str ());
    return istrm;
  }
  /// @endcond

} // namespace saga
///////////////////////////////////////////////////////////////////////////////

