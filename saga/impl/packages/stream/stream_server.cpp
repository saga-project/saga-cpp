#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/server.cpp")
#endif
//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
// 
//  Use, modification and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif

// needs tasks for async ops
#include <saga/saga/util.hpp>
#include <saga/saga/adaptors/task.hpp>

#include <saga/impl/call.hpp>
#include <saga/impl/engine/task.hpp>
#include <saga/impl/engine/proxy.hpp>
#include <saga/impl/engine/sync_async.hpp>

// own header file
#include <saga/impl/stream.hpp>
#include <saga/saga/adaptors/instance_data.hpp>
#include <saga/saga/adaptors/packages/stream_server_cpi_instance_data.hpp>

#ifdef SAGA_DEBUG
#include <saga/impl/packages/stream/preprocessed/server.cpp>
#else

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: "preprocessed/server.cpp")
#endif
///////////////////////////////////////////////////////////////////////////////
namespace saga 
{ 
  namespace impl 
  {
    server::server (saga::session const & s, saga::url loc) 
        : proxy   (saga::object::StreamServer, s)
    {
      // initialize the instance data
      typedef adaptors::v1_0::stream_server_cpi_instance_data instance_data_type;
      typedef adaptors::instance_data <instance_data_type> server_instance_data;

      server_instance_data data;
      data.init_data (this, 
          TR1::shared_ptr <instance_data_type>(new instance_data_type (loc)));
    }

    void server::init ()
    {
        try {
            // initialize a CPI instance and execute required functions
            this->initcpi(saga::adaptors::stream_server_cpi);
        }
        catch (saga::exception const& e) {
            // map special exceptions
            switch (e.get_error()) {
            case saga::adaptors::AdaptorDeclined:
            case saga::adaptors::NoAdaptor:
            case saga::adaptors::NoAdaptorInfo:
            case saga::adaptors::Unexpected:
                SAGA_THROW_PLAIN(e.get_object(), e.get_message(), saga::NoSuccess);
            // just rethrow otherwise
            default:
                 throw;
            }
        }
    }

    server::~server (void) 
    {  
        // release all attached adaptors now
        saga::impl::proxy::release_cpis();

        // free the instance data
        typedef adaptors::v1_0::stream_server_cpi_instance_data instance_data_type;
        typedef adaptors::instance_data <instance_data_type> server_instance_data;

        server_instance_data data;
        data.release_data (this);
    }

    ///////////////////////////////////////////////////////////////////////////
    // factory support
    void server::create_impl_sync(saga::stream::server& retval)
    {
        this->init();
        retval = saga::stream::server(this);
    }

    saga::task server::create_impl_async(saga::session const& s, saga::url const& rm)
    {
        TR1::shared_ptr<server> impl(new server(s, rm));

        bool (server::*prep)(saga::stream::server&, saga::uuid) = NULL;
        return saga::adaptors::task("__init__", TR1::shared_ptr<v1_0::cpi>(), 
            impl.get(), &server::create_impl_sync, prep);
    }

    // stream server methods
    SAGA_CALL_IMPL_IMPL_1 (server, stream_server_cpi, connect, double)
    SAGA_CALL_IMPL_IMPL_1 (server, stream_server_cpi, serve, double)
    SAGA_CALL_IMPL_IMPL_0 (server, stream_server_cpi, get_url)
    SAGA_CALL_IMPL_IMPL_1 (server, stream_server_cpi, close, double)

  } // namespace impl

} // namespace saga
////////////////////////////////////////////////////////////////////////////////

#endif // SAGA_DEBUG

