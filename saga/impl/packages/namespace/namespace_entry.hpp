#ifndef SAGA_IMPL_PACKAGES_DATA_NAMESPACE_NAMESPACEENTRY_HPP
#define SAGA_IMPL_PACKAGES_DATA_NAMESPACE_NAMESPACEENTRY_HPP

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/namespace_entry.hpp")
#endif
//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005 Michel Zandstra [michelzandstra@gmail.com]
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif
#include <boost/shared_ptr.hpp>

#include <string>

#include <saga/saga/session.hpp>

#include <saga/impl/call.hpp>
#include <saga/impl/engine/proxy.hpp>
#include <saga/impl/engine/monitorable.hpp>
#include <saga/impl/engine/permissions.hpp>

#include <saga/impl/packages/namespace/namespace_entry_cpi.hpp>

#ifdef SAGA_DEBUG
#include <saga/impl/packages/namespace/preprocessed/namespace_entry.hpp>
#else

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: "preprocessed/namespace_entry.hpp")
#endif
///////////////////////////////////////////////////////////////////////////////
namespace saga 
{ 
  namespace impl 
  {
    /**
     * This class acts as a proxy class, hiding all the nasty Saga stuff
     * (like creating and using adaptors) for the user. Users just call
     * the functions of this class like to would on every local object,
     * and don't have to worry about adaptors. It extends
     * saga::name_space::namespace_entry_cpi.
     */
    class SAGA_NAMESPACE_PACKAGE_EXPORT namespace_entry 
      : public proxy,
        public saga::impl::monitorable,
        public saga::impl::permissions
    {
      typedef v1_0::namespace_entry_cpi namespace_entry_cpi;
      typedef v1_0::preference_type preference_type;
      
      bool need_to_free_instance_data;

      // initialize newly attached CPI instance
      void init();

    protected:
      explicit namespace_entry (saga::session const & s, 
          saga::object::type t = saga::object::NSEntry);
      explicit namespace_entry (saga::object::type t);

     public:
      /**
       * Constructor of namespace_entry, sets the location of the
       * namespace_entry, and gets
       * an instance of the Saga engine
       *
       * @param loc the full path and name of the namespace_entry
       * @param s   the session handle
       */
      namespace_entry (saga::session const & s, saga::url loc, int mode,
          saga::object::type t = saga::object::NSEntry);

      /*! destructor of namespace_entry, destroys namespace_entry and the adaptor it is 
       *  bound to */
      ~namespace_entry (void);

      // factories
      void create_impl_sync(name_space::entry& retval);
      static saga::task create_impl_async(saga::session const& s, saga::url const& url, int mode);

      // basic properties
      SAGA_CALL_IMPL_DECL_0(get_url)
      SAGA_CALL_IMPL_DECL_0(get_cwd)
      SAGA_CALL_IMPL_DECL_0(get_name)
  
      // navigation/query methods
      SAGA_CALL_IMPL_DECL_0(read_link)
      SAGA_CALL_IMPL_DECL_0(is_dir)
      SAGA_CALL_IMPL_DECL_0(is_entry)
      SAGA_CALL_IMPL_DECL_0(is_link)
  
      // entry management methods
      SAGA_CALL_IMPL_DECL_2(copy, saga::url, int)
      SAGA_CALL_IMPL_DECL_2(link, saga::url, int)
      SAGA_CALL_IMPL_DECL_2(move, saga::url, int)
      SAGA_CALL_IMPL_DECL_1(remove, int)
      SAGA_CALL_IMPL_DECL_1(close, double)
  
      // permissions with flags
      SAGA_CALL_IMPL_DECL_3(permissions_allow, std::string, int, int)
      SAGA_CALL_IMPL_DECL_3(permissions_deny, std::string, int, int)

      /////////////////////////////////////////////////////////////////////////
      //  return the monitorable interface to the facade 
      virtual saga::impl::monitorable* 
          get_monitorable() { return this; }
      virtual saga::impl::monitorable const* 
          get_monitorable() const { return this; }

      /////////////////////////////////////////////////////////////////////////
      //  return the permissions interface to the facade 
      virtual saga::impl::permissions* 
          get_permissions() { return this; }
      virtual saga::impl::permissions const* 
          get_permissions() const { return this; }

      /////////////////////////////////////////////////////////////////////////
      // get_proxy returns the internal implementation of the 
      // impl::object interface
      virtual saga::impl::proxy* 
          get_proxy() { return this; }
      virtual saga::impl::proxy const* 
          get_proxy() const { return this; }

      // Generate a exact deep copy of this object
      saga::object clone() const;
    };

  }  // namespace impl

} // namespace saga
///////////////////////////////////////////////////////////////////////////////
#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif
#endif // SAGA_DEBUG

#endif // SAGA_IMPL_PACKAGES_DATA_NAMESPACE_NAMESPACEENTRY_HPP


