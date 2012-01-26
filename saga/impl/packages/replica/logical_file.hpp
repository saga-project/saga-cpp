#ifndef SAGA_IMPL_PACKAGES_DATA_REPLICA_LOGICAL_FILE_HPP
#define SAGA_IMPL_PACKAGES_DATA_REPLICA_LOGICAL_FILE_HPP

#if defined(__WAVE__)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/logical_file.hpp")
#endif
//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if defined(__WAVE__)
#pragma wave option(preserve: 0, output: null)
#endif

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif 

#include <boost/shared_ptr.hpp>

#include <string>
#include <vector>

#include <saga/saga/session.hpp>
#include <saga/impl/call.hpp>
#include <saga/impl/engine/proxy.hpp>
#include <saga/impl/packages/replica/logical_file_cpi.hpp>
#include <saga/impl/engine/attribute.hpp>

#include <saga/impl/packages/namespace/namespace_entry.hpp>

#ifdef SAGA_DEBUG
#include <saga/impl/packages/replica/preprocessed/logical_file.hpp>
#else

#if defined(__WAVE__)
#pragma wave option(output: "preprocessed/logical_file.hpp")
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
     * and don't have to worry about adaptors. It extends saga::file::file_cpi.
     */
    class logical_file 
    :   public saga::impl::namespace_entry, // v1_0::logical_file_cpi
        public saga::impl::attribute
    {
      typedef v1_0::logical_file_cpi logical_file_cpi;
      typedef v1_0::preference_type preference_type;
      typedef saga::impl::attribute attribute_base;

      // initialize newly attached CPI instance
      void init();

      explicit logical_file(attribute const& attr);

     public:
      /**
       * Constructor of file, sets the location of the file, and gets
       * an instance of the Saga engine
       *
       * @param url the full path and name of the file
       * @param s   the session handle
       */
      logical_file (saga::session const& s, saga::url url, int mode);

      /*! destructor of logical_file, destroys file and the adaptor it is 
       *  bound to */
      ~logical_file (void);

      // factory support
      void create_impl_sync(replica::logical_file& retval);
      static saga::task create_impl_async(saga::session const& s, 
          saga::url const& url, int mode);

      // API functions not covered by namespace_entry
      SAGA_CALL_IMPL_DECL_1(add_location, saga::url)
      SAGA_CALL_IMPL_DECL_1(remove_location, saga::url)
      SAGA_CALL_IMPL_DECL_2(update_location, saga::url, saga::url)
      SAGA_CALL_IMPL_DECL_0(list_locations)
      SAGA_CALL_IMPL_DECL_2(replicate, saga::url, int)

      saga::impl::attribute* get_attributes() { return this; }
      saga::impl::attribute const* get_attributes() const { return this; }

      // get_proxy returns the internal implementation of the 
      // impl::object interface
      saga::impl::proxy* get_proxy() { return this; }
      saga::impl::proxy const* get_proxy() const { return this; }

      // Generate a exact deep copy of this object
      saga::object clone() const;

    };

  } // namespace impl

} // namespace saga
///////////////////////////////////////////////////////////////////////////////
#if defined(__WAVE__)
#pragma wave option(output: null)
#endif

#endif // !defined(SAGA_DEBUG)
#endif // SAGA_IMPL_PACKAGES_DATA_REPLICA_LOGICAL_FILE_HPP
