#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/stream.cpp")
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
#include <saga/impl/packages/stream/stream.hpp>
#include <saga/saga/adaptors/instance_data.hpp>
#include <saga/saga/adaptors/packages/stream_cpi_instance_data.hpp>

#ifdef SAGA_DEBUG
#include <saga/impl/packages/stream/preprocessed/stream.cpp>
#else

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: "preprocessed/stream.cpp")
#endif
///////////////////////////////////////////////////////////////////////////////
namespace saga 
{ 
  namespace impl 
  {
    stream::stream (saga::session const & s, saga::url loc) 
      : proxy(saga::object::Stream, s)
    {
        // initialize the instance data
        typedef adaptors::v1_0::stream_cpi_instance_data instance_data_type;
        typedef adaptors::instance_data <instance_data_type> stream_instance_data;

        stream_instance_data data;
        data.init_data (this, 
            TR1::shared_ptr <instance_data_type>(new instance_data_type (loc)));
    }

    stream::stream (saga::adaptors::v1_0::stream_cpi_instance_data* instdata, 
            saga::session const& s)
      : proxy(saga::object::Stream, s)
    {
        // initialize the instance data
        typedef adaptors::v1_0::stream_cpi_instance_data instance_data_type;
        typedef adaptors::instance_data <instance_data_type> stream_instance_data;

        stream_instance_data data;
        data.init_data(this, TR1::shared_ptr<instance_data_type>(instdata));
    }

    void stream::init()
    {
        try {
            // initialize a CPI instance and execute required functions
            this->initcpi(saga::adaptors::stream_cpi);
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

    stream::~stream (void) 
    {
        // release all attached adaptors now
        saga::impl::proxy::release_cpis();
        
        // free the instance data
        typedef adaptors::v1_0::stream_cpi_instance_data instance_data_type;
        typedef adaptors::instance_data <instance_data_type> stream_instance_data;

        stream_instance_data data;
        data.release_data (this);
    }

    ///////////////////////////////////////////////////////////////////////////
    // factory support
    void stream::create_impl_sync(saga::stream::stream& retval)
    {
        this->init();
        retval = saga::stream::stream(this);
    }

    saga::task stream::create_impl_async(saga::session const& s, saga::url const& rm)
    {
        TR1::shared_ptr<stream> impl(new stream(s, rm));

        bool (stream::*prep)(saga::stream::stream&, saga::uuid) = NULL;
        return saga::adaptors::task("__init__", TR1::shared_ptr<v1_0::cpi>(), 
            impl.get(), &stream::create_impl_sync, prep);
    }

    SAGA_CALL_IMPL_IMPL_0 (stream, stream_cpi, get_url)
    SAGA_CALL_IMPL_IMPL_0 (stream, stream_cpi, get_context)

    SAGA_CALL_IMPL_IMPL_1 (stream, stream_cpi, connect, double)
    SAGA_CALL_IMPL_IMPL_2 (stream, stream_cpi, wait, saga::stream::activity, double)
    SAGA_CALL_IMPL_IMPL_1 (stream, stream_cpi, close, double)
//     SAGA_CALL_IMPL_IMPL_2 (stream, stream_cpi, read, saga::mutable_buffer, saga::ssize_t)
    saga::task 
    stream::read (saga::mutable_buffer buff, saga::ssize_t length, bool is_sync)
    {
        // handle default arguments
        if (0 == length)
            length = buff.get_size();

        // standard procedure from here
        return saga::impl::execute_sync_async(this,
            "stream_cpi", "read", "stream_cpi::read", is_sync,
            &stream_cpi::sync_read, &stream_cpi::async_read, buff, length);
    }
//     SAGA_CALL_IMPL_IMPL_2 (stream, stream_cpi, write, saga::const_buffer, saga::ssize_t)
    saga::task 
    stream::write (saga::const_buffer buff, saga::ssize_t length, bool is_sync)
    {
        // handle default arguments
        if (0 == length)
            length = buff.get_size();

        // standard procedure from here
        return saga::impl::execute_sync_async(this,
            "stream_cpi", "write", "stream_cpi::write", is_sync,
            &stream_cpi::sync_write, &stream_cpi::async_write, buff, length);
    }

///////////////////////////////////////////////////////////////////////////////
}} // namespace saga::impl

#endif // SAGA_DEBUG

