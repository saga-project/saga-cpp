//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Use, modification and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

// include the object implementation we want to use
#include <boost/version.hpp>
#include <saga/impl/engine/object.hpp>

// include the basic header file
#include <saga/saga/exception.hpp>
#include <saga/saga/object.hpp>

// we need saga::session and saga::uuid
#include <saga/saga/session.hpp>
#include <saga/saga/uuid.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga 
{
  object::object (saga::impl::object * init)
#if BOOST_VERSION >= 103900
    : impl_ (init)
#else
    : impl_ (init->_internal_weak_this.use_count() ? 
        init->shared_from_this() : TR1::shared_ptr<saga::impl::object>(init))
#endif
  {
  }

  object::object (TR1::shared_ptr<saga::impl::object> init)
    : impl_ (init)
  {
  }

  // constructor for objects without session
  object::object ()
  {
  }

  saga::object::type object::get_type (void) const
  {
    return impl_ ? (impl_->get_type()) : Unknown;
  }

  saga::uuid object::get_id (void) const
  {
    return impl_ ? impl_->get_uuid () : uuid();
  }

  saga::session& object::get_session()
  {
    return (impl_->get_session ());
  }
  saga::session const& object::get_session() const
  {
    return (impl_->get_session ());
  }

  saga::object object::clone() const
  {
    return impl_->clone();
  }
  
} // namespace saga 
///////////////////////////////////////////////////////////////////////////////

