#ifndef SAGA_IMPL_PACKAGES_COMM_STREAM_STREAM_HPP
#define SAGA_IMPL_PACKAGES_COMM_STREAM_STREAM_HPP

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/stream.hpp")
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
#include <saga/impl/packages/stream/stream_cpi.hpp>
#include <saga/impl/engine/proxy.hpp>
#include <saga/impl/engine/attribute.hpp>
#include <saga/impl/engine/monitorable.hpp>

#ifdef SAGA_DEBUG
#include <saga/impl/packages/stream/preprocessed/stream.hpp>
#else

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: "preprocessed/stream.hpp")
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
     * and don't have to worry about adaptors. It extends saga::stream_cpi.
     */
    class SAGA_STREAM_PACKAGE_EXPORT stream 
      : public proxy,
        public saga::impl::attribute,
        public saga::impl::monitorable
    {
    public:
      typedef saga::stream::state state; 

    private:
      typedef v1_0::stream_cpi stream_cpi;
      typedef v1_0::preference_type preference_type;
      typedef saga::impl::attribute attribute_base;
      typedef saga::impl::monitorable monitorable_base;

      // initialize newly attached CPI instance
      void init();

    public:
      /**
       * Constructor of stream, sets the location of the stream, and gets
       * an instance of the Saga engine
       *
       * @param loc the full path and name of the stream
       * @param s   the session handle
       */
      stream (saga::session const& s, saga::url loc);
      stream (saga::adaptors::v1_0::stream_cpi_instance_data* data, 
           saga::session const & s);

      /*! destructor of stream, destroys stream and the adaptor it is 
       *  bound to */
      ~stream (void);

      // factory support
      void create_impl_sync(saga::stream::stream& retval);
      static saga::task create_impl_async(saga::session const& s, saga::url const& rm);

      /**
       * @return location
       */
      SAGA_CALL_IMPL_DECL_0 (get_url)
      SAGA_CALL_IMPL_DECL_0 (get_context)

      SAGA_CALL_IMPL_DECL_1 (connect, double)
      SAGA_CALL_IMPL_DECL_2 (wait, saga::stream::activity, double)
      SAGA_CALL_IMPL_DECL_1 (close, double)
      SAGA_CALL_IMPL_DECL_2 (read, saga::mutable_buffer, saga::ssize_t)
      SAGA_CALL_IMPL_DECL_2 (write, saga::const_buffer, saga::ssize_t)

      ///////////////////////////////////////////////////////////////////////
      //  return the monitorable and attribute interfaces to the facade 
      saga::impl::monitorable* get_monitorable() { return this; }
      saga::impl::monitorable const* get_monitorable() const { return this; }
      saga::impl::attribute* get_attributes() { return this; }
      saga::impl::attribute const* get_attributes() const { return this; }
    };

///////////////////////////////////////////////////////////////////////////////
}} // namespace saga::impl

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif

#endif // SAGA_DEBUG

#endif // SAGA_IMPL_PACKAGES_COMM_STREAM_STREAM_HPP

