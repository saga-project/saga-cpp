#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/rpc.cpp")
#endif
//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif

#include <saga/saga/util.hpp>
#include <saga/saga/adaptors/task.hpp>

// needs tasks for async ops
#include <saga/impl/rpc.hpp>
#include <saga/impl/call.hpp>
#include <saga/impl/engine/task.hpp>

// own header file
#include <saga/saga/adaptors/instance_data.hpp>
#include <saga/saga/adaptors/packages/rpc_cpi_instance_data.hpp>

#ifdef SAGA_DEBUG
#include <saga/impl/packages/rpc/preprocessed/rpc.cpp>
#else

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: "preprocessed/rpc.cpp")
#endif

///////////////////////////////////////////////////////////////////////////////
namespace saga 
{ 
  namespace impl 
  {
    rpc::rpc (saga::session const& s, saga::url name)
        : proxy (saga::object::RPC, s)
    {
      // initialize the instance data
      typedef adaptors::v1_0::rpc_cpi_instance_data instance_data_type;
      typedef adaptors::instance_data <instance_data_type> rpc_instance_data;

      {
          rpc_instance_data data;
          data.init_data (this, TR1::shared_ptr<instance_data_type>(
              new instance_data_type (name)));
      }   // lock goes out of scope
    }

    rpc::~rpc (void) 
    {
        // release all attached adaptors now
        saga::impl::proxy::release_cpis();

        // free the instance data
        typedef adaptors::v1_0::rpc_cpi_instance_data instance_data_type;
        typedef adaptors::instance_data <instance_data_type> rpc_instance_data;

        rpc_instance_data data;
        data.release_data (this);
    }

    void rpc::init()
    {
        try {
            // initialize a CPI instance and execute required functions 
            this->initcpi(saga::adaptors::rpc_cpi);
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

    ///////////////////////////////////////////////////////////////////////////
    // factory support
    void rpc::create_impl_sync(saga::rpc::rpc& retval)
    {
        this->init();
        retval = saga::rpc::rpc(this);
    }

    saga::task rpc::create_impl_async(saga::session const& s, saga::url const& rm)
    {
        TR1::shared_ptr<rpc> impl(new rpc(s, rm));

        bool (rpc::*prep)(saga::rpc::rpc&, saga::uuid) = NULL;
        return saga::adaptors::task("__init__", TR1::shared_ptr<v1_0::cpi>(), 
            impl.get(), &rpc::create_impl_sync, prep);
    }

    // rpc methods
    SAGA_CALL_IMPL_IMPL_1 (rpc, rpc_cpi, call, std::vector<saga::rpc::parameter>&)
    SAGA_CALL_IMPL_IMPL_1 (rpc, rpc_cpi, close, double)

  } // namespace impl

} // namespace saga
///////////////////////////////////////////////////////////////////////////////

#endif // SAGA_DEBUG

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif
