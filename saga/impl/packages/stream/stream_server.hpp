#ifndef SAGA_IMPL_PACKAGES_COMM_STREAM_STREAMSERVICE_HPP
#define SAGA_IMPL_PACKAGES_COMM_STREAM_STREAMSERVICE_HPP

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/server.hpp")
#endif
//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Use, modification and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif

#include <boost/shared_ptr.hpp>

#include <string>

#include <saga/saga/session.hpp>

#include <saga/impl/call.hpp>
#include <saga/impl/packages/stream/stream_server_cpi.hpp>
#include <saga/impl/engine/proxy.hpp>
#include <saga/impl/engine/monitorable.hpp>
#include <saga/impl/engine/permissions.hpp>

#ifdef SAGA_DEBUG
#include <saga/impl/packages/stream/preprocessed/server.hpp>
#else

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: "preprocessed/server.hpp")
#endif
///////////////////////////////////////////////////////////////////////////////
namespace saga
{
  namespace impl
  {
    class SAGA_STREAM_PACKAGE_EXPORT server 
      : public proxy,
        public saga::impl::monitorable,
        public saga::impl::permissions
    {
    public:
      typedef saga::stream::state state; 

    private:
      typedef v1_0::stream_server_cpi stream_server_cpi;
      typedef v1_0::preference_type preference_type;
      typedef saga::impl::monitorable monitorable_base;

      // initialize newly attached CPI instance
      void init();

    public:
      /**
       * Constructor of server, sets the location of the server, and gets
       * an instance of the Saga engine
       *
       * @param loc the full path and name of the server
       * @param s   the session handle
       */
      server (saga::session const& s, saga::url loc);

      /*! destructor of server, destroys server and the 
       *  adaptor it is bound to */
      ~server (void);

      // factory support
      void create_impl_sync(saga::stream::server& retval);
      static saga::task create_impl_async(saga::session const& s, saga::url const& rm);

      // stream server methods
      SAGA_CALL_IMPL_DECL_1 (connect, double)
      SAGA_CALL_IMPL_DECL_1 (serve, double)
      SAGA_CALL_IMPL_DECL_0 (get_url)
      SAGA_CALL_IMPL_DECL_1 (close, double)

      ///////////////////////////////////////////////////////////////////////
      //  return the monitorable interface to the facade 
      saga::impl::monitorable* get_monitorable() { return this; }
      saga::impl::monitorable const* get_monitorable() const { return this; }

      ///////////////////////////////////////////////////////////////////////
      //  return the permissions interface to the facade 
      saga::impl::permissions* get_permissions() { return this; }
      saga::impl::permissions const* get_permissions() const { return this; }

    };

  } // namespace saga

} // namespace impl
///////////////////////////////////////////////////////////////////////////////
#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif

#endif // SAGA_DEBUG

#endif // SAGA_IMPL_PACKAGES_COMM_STREAM_STREAMSERVICE_HPP


