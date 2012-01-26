//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  job LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_IMPL_ENGINE_OBJECT_HPP
#define SAGA_IMPL_ENGINE_OBJECT_HPP

// include boost helper
#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>

// include config
#include <saga/impl/config.hpp>

// need saga::uuid
#include <saga/saga/uuid.hpp>
#include <saga/saga/object.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl {

  /////////////////////////////////////////////////////////////////////////////
  class object 
  :  private boost::noncopyable,
     public TR1::enable_shared_from_this <object>
  {
    public:
      typedef boost::recursive_mutex mutex_type;
      mutable mutex_type mtx_;

    private:
      // store uuid - no recursion here, as uuid is not a saga::object, 
      // as it is not part of the spec.  It gets created here, and is THE uuid
      // for the saga object, and the proxy.
      saga::uuid uuid_;

      // store the object type, we need to store this in the implementation 
      // part to be able to reconstruct a saga::object from the impl only
      saga::object::type type_;

      // store the uuid of the last adaptor successfully executing a CPI 
      // function for this instance
      saga::uuid last_known_good_;

    public:
      saga::uuid get_last_known_good() const { return last_known_good_; }
      void set_last_known_good(saga::uuid u) { last_known_good_ = u; }

    public:
      SAGA_EXPORT object (saga::object::type type);
      SAGA_EXPORT virtual ~object (void) {}

      SAGA_EXPORT virtual saga::session& get_session();
      SAGA_EXPORT virtual saga::session const& get_session() const;

      SAGA_EXPORT virtual saga::uuid get_uuid() const;

      SAGA_EXPORT void set_uuid(saga::uuid const& id);
      SAGA_EXPORT void set_type(saga::object::type t);

      SAGA_EXPORT virtual saga::object::type get_cpitype() const;
      SAGA_EXPORT virtual saga::object  clone() const;
      SAGA_EXPORT virtual bool is_equal(saga::impl::object *impl) const;

      // initialize implementation object and newly attached CPI object
      // this needs to be overloaded in the implementation object
      SAGA_EXPORT virtual void init();

      // get_type returns the type of this object
      SAGA_EXPORT virtual saga::object::type get_type() const;

      // get_attributes returns the internal implementation of the 
      // impl::attributes interface
      SAGA_EXPORT virtual saga::impl::attribute* get_attributes();
      SAGA_EXPORT virtual saga::impl::attribute const* get_attributes() const;

      // get_monitorable returns the internal implementation of the 
      // impl::monitorable interface
      SAGA_EXPORT virtual saga::impl::monitorable* get_monitorable();
      SAGA_EXPORT virtual saga::impl::monitorable const* get_monitorable() const;

      // get_steerable returns the internal implementation of the 
      // impl::steerable interface
      SAGA_EXPORT virtual saga::impl::steerable* get_steerable();
      SAGA_EXPORT virtual saga::impl::steerable const* get_steerable() const;

      // get_task_interface returns the internal implementation of the 
      // impl::task_interface interface
      SAGA_EXPORT virtual saga::impl::task_interface* get_task_interface();
      SAGA_EXPORT virtual saga::impl::task_interface const* get_task_interface() const;

      // get_permissions returns the internal implementation of the 
      // impl::permissions interface
      SAGA_EXPORT virtual saga::impl::permissions* get_permissions();
      SAGA_EXPORT virtual saga::impl::permissions const* get_permissions() const;

  }; // class object

  ///////////////////////////////////////////////////////////////////////////////
}}  // namespace saga::impl

#endif // SAGA_IMPL_ENGINE_OBJECT_HPP

