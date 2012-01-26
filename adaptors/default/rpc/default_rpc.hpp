//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ADAPTORS_DEFAULT_RPC_DEFAULT_RPC_HPP
#define ADAPTORS_DEFAULT_RPC_DEFAULT_RPC_HPP

#include <string>

#include <saga/saga/types.hpp>

#include <saga/saga/adaptors/config.hpp>
#include <saga/saga/adaptors/adaptor.hpp>
#include <saga/saga/adaptors/packages/rpc_cpi_instance_data.hpp>
#include <saga/saga/adaptors/instance_data.hpp>

#include <saga/impl/packages/rpc/rpc_cpi.hpp>
#include "rpc_connection.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace rpc
{

///////////////////////////////////////////////////////////////////////////////
/**
  * This adaptor implements the functionality of the Saga API "rpc".
  * It defines the functions declared in its base class, rpc_cpi.
  *
  * @note This adaptor is written for test purposes only; it is possible that
  * it does not function exactly like the Saga API dictates.
  * @see The documentation of the Saga API
  */
    class rpc_cpi_impl 
    :   public saga::adaptors::v1_0::rpc_cpi<rpc_cpi_impl>
    {
    private:
        typedef saga::adaptors::v1_0::rpc_cpi<rpc_cpi_impl> base_cpi;
        
    public:
        /*! constructor of the rpc adaptor */
        rpc_cpi_impl (proxy* p, cpi_info const& info, 
            saga::ini::ini const& glob_ini, saga::ini::ini const& adap_ini,
            TR1::shared_ptr<saga::adaptor> adaptor);

        /*! destructor of the rpc adaptor */
        ~rpc_cpi_impl  (void);

        /*! implementation of SAGA::rpc functions */
        void sync_call (saga::impl::void_t&, std::vector<saga::rpc::parameter>& args);
        void sync_close (saga::impl::void_t&, double timeout);

#if ! defined(SAGA_DEFAULT_FILE_ADAPTOR_NO_ASYNCS)
        saga::task async_call (std::vector<saga::rpc::parameter>& args);
        saga::task async_close (double timeout);

    protected:
        void async_call_proc (saga::impl::void_t&, TR1::shared_ptr<connection> conn, 
            std::vector<saga::rpc::parameter>& args);
#endif 

    private:
        connection* connection_;
    };  // class rpc_cpi_impl

///////////////////////////////////////////////////////////////////////////////
}   // namespace rpc

#endif // ADAPTORS_DEFAULT_RPC_DEFAULT_RPC_HPP

