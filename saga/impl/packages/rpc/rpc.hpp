#ifndef SAGA_IMPL_PACKAGES_COMM_RPC_RPC_HPP
#define SAGA_IMPL_PACKAGES_COMM_RPC_RPC_HPP

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/rpc.hpp")
#endif
//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif

#include <string>

#include <saga/saga/session.hpp>

#include <saga/impl/call.hpp>
#include <saga/impl/packages/rpc/rpc_cpi.hpp>
#include <saga/impl/engine/proxy.hpp>
#include <saga/impl/engine/permissions.hpp>

#ifdef SAGA_DEBUG
#include <saga/impl/packages/rpc/preprocessed/rpc.hpp>
#else

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: "preprocessed/rpc.hpp")
#endif
////////////////////////////////////////////////////////////////////////////////
namespace saga
{
  namespace impl
  {
    /**
     * This class acts as a proxy class, hiding all the nasty Saga stuff
     * (like creating and using adaptors) for the user. Users just call
     * the functions of this class like to would on every local object,
     * and don't have to worry about adaptors. It extends saga::rpc_cpi.
     */
    class SAGA_RPC_PACKAGE_EXPORT rpc 
        : public proxy,
          public saga::impl::permissions
    {
    private:
      typedef v1_0::rpc_cpi rpc_cpi;
      typedef v1_0::preference_type preference_type;
      
      void init (void);

    public:
      /**
       * Constructor of rpc, sets the name of the rpc, and gets
       * an instance of the Saga engine
       *
       * @param name the name of the rpc function handle
       * @param s   the session handle
       */
      rpc (saga::session const& s, saga::url name);

      /*! destructor of rpc, destroys rpc and the adaptor it is 
       *  bound to */
      ~rpc (void);

      // factory support
      void create_impl_sync(saga::rpc::rpc& retval);
      static saga::task create_impl_async(saga::session const& s, saga::url const& rm);

      // rpc methods
      SAGA_CALL_IMPL_DECL_1(call, std::vector<saga::rpc::parameter>&)
      SAGA_CALL_IMPL_DECL_1(close, double)

      ///////////////////////////////////////////////////////////////////////
      //  return the permissions interface to the facade 
      saga::impl::permissions* get_permissions() { return this; }
      saga::impl::permissions const* get_permissions() const { return this; }

    };

  } // namespace impl

} // namespace saga
////////////////////////////////////////////////////////////////////////////////

#endif // SAGA_DEBUG

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif

#endif // SAGA_IMPL_PACKAGES_COMM_RPC_RPC_HPP

