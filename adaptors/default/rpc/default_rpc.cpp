//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <ctime>
#include <iostream>

#include <saga/saga/exception.hpp>
#include <saga/saga/url.hpp>
#include <saga/saga/adaptors/task.hpp>

#include <saga/impl/config.hpp>

#include "default_rpc.hpp"
#include "rpc_connection.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace {

    ///////////////////////////////////////////////////////////////////////////
    // ensure to get a non-empty function name
    inline std::string ensure_function_name(saga::url fn_url)
    {
        std::string name(fn_url.get_path());
        if (name.empty()) 
        {
            name = "system.methodHelp";
        }
        return name;
    }

    ///////////////////////////////////////////////////////////////////////////
    // ensure to get a non-empty host name
    inline std::string ensure_host_name(saga::url fn_url)
    {
        std::string name(fn_url.get_host());
        if (name.empty()) 
        {
            name = "localhost";
        }
        return name;
    }

    ///////////////////////////////////////////////////////////////////////////
    // ensure to get a non-empty host name
    inline int ensure_port(saga::url fn_url)
    {
        int port = fn_url.get_port();
        if (-1 == port) 
        {
            port = 6789;
        }
        return port;
    }

///////////////////////////////////////////////////////////////////////////////
}   // namespace 

///////////////////////////////////////////////////////////////////////////////
namespace rpc
{

/*
 * constructor
 */
rpc_cpi_impl::rpc_cpi_impl (proxy* p, cpi_info const& info, 
        saga::ini::ini const& glob_ini, saga::ini::ini const& adap_ini,
        TR1::shared_ptr<saga::adaptor> adaptor)
:   base_cpi(p, info, adaptor, cpi::Noflags), connection_(NULL)
{
    // first usage of this adaptor
    instance_data data (this);
    saga::url fn_url(data->funcname_);

    // we understand only 'any' and 'xmlrpc' url schemes
    std::string scheme(fn_url.get_scheme());
    if (!scheme.empty() && scheme != "xmlrpc" && scheme != "any")
    {
        SAGA_OSSTREAM strm;
        strm << "rpc::rpc_cpi_impl::init: "
                "cannot handle rpc function name: " << fn_url.get_url();
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
    }

    try {
        // create XmlRpc client, establish connection
        connection_ = new connection(ensure_host_name(fn_url), 
            ensure_port(fn_url), ensure_function_name(fn_url)); 
    }
    catch (saga::adaptors::exception const& e) {
        // just repackage exception
        SAGA_ADAPTOR_THROW(e.what(), e.get_error());
    }
    catch (XmlRpc::XmlRpcException const& e) {
        SAGA_ADAPTOR_THROW("Rpc server connect failed: " + e.getMessage(),
            saga::NoSuccess);
    }
}

/*
 * destructor
 */
rpc_cpi_impl::~rpc_cpi_impl (void)
{
    if (connection_->is_open()) {
        try {
            connection_->close();
        }
        catch (...) {
            ;   // just catch everything and continue
        }
    }
    delete connection_;
}

/*
 * SAGA API functions
 */
void 
rpc_cpi_impl::sync_call (saga::impl::void_t&, std::vector<saga::rpc::parameter>& args)
{
    if (NULL == connection_ || !connection_->is_open()) {
        SAGA_ADAPTOR_THROW("Rpc connection to server has been closed.", 
            saga::IncorrectState);
    }
    
    // route the call through the existing rpc client
    try {
        connection_->call(args);
    }
    catch (saga::adaptors::exception const& e) {
        // just repackage exception
        SAGA_ADAPTOR_THROW(e.what(), e.get_error());
    }
    catch (XmlRpc::XmlRpcException const& e) {
        SAGA_ADAPTOR_THROW("Rpc call failed: " + e.getMessage(),
            saga::NoSuccess);
    }
}

void rpc_cpi_impl::sync_close(saga::impl::void_t&, double timeout)
{
    if (NULL == connection_ || !connection_->is_open()) {
        SAGA_ADAPTOR_THROW("Rpc connection to server has already been closed.", 
            saga::IncorrectState);
    }
    
    // route the call through the existing rpc client
    try {
        connection_->close();
    }
    catch (saga::adaptors::exception const& e) {
        // just repackage exception
        SAGA_ADAPTOR_THROW(e.what(), e.get_error());
    }
    catch (XmlRpc::XmlRpcException const& e) {
        SAGA_ADAPTOR_THROW("Rpc call failed: " + e.getMessage(),
            saga::NoSuccess);
    }
}

#if ! defined(SAGA_DEFAULT_FILE_ADAPTOR_NO_ASYNCS)
///////////////////////////////////////////////////////////////////////////////
// The async call is something special because it creates a new connection to 
// the XmlRpc server which is used for this call only. Note that the async 
// close has no relation to this and does not influence any of the connections
// created by the async call.
void rpc_cpi_impl::async_call_proc (saga::impl::void_t&, 
    TR1::shared_ptr<connection> conn, std::vector<saga::rpc::parameter>& args)
{
    if (!conn->is_open()) {
        SAGA_ADAPTOR_THROW("Rpc connection to server has been closed.", 
            saga::IncorrectState);
    }
    
    // route the call through the existing rpc client
    try {
        conn->call(args);
    }
    catch (saga::adaptors::exception const& e) {
        // just repackage exception
        SAGA_ADAPTOR_THROW(e.what(), e.get_error());
    }
    catch (XmlRpc::XmlRpcException const& e) {
        SAGA_ADAPTOR_THROW("Rpc call failed: " + e.getMessage(),
            saga::NoSuccess);
    }
}

saga::task 
rpc_cpi_impl::async_call(std::vector<saga::rpc::parameter>& args)
{
    instance_data data (this);
    saga::url fn_url(data->funcname_);

    TR1::shared_ptr<connection> conn(new connection(ensure_host_name(fn_url), 
            ensure_port(fn_url), ensure_function_name(fn_url)));
            
    return saga::adaptors::task("rpc_cpi_impl::sync_call", shared_from_this(), 
        &rpc_cpi_impl::async_call_proc, conn, TR1::ref(args));
}

///////////////////////////////////////////////////////////////////////////////
// async close just asynchronously closes the main connection 
saga::task rpc_cpi_impl::async_close(double timeout)
{
    return saga::adaptors::task("rpc_cpi_impl::sync_close", 
        shared_from_this(), &rpc_cpi_impl::sync_close, timeout);
}
#endif

///////////////////////////////////////////////////////////////////////////////
}   // namespace rpc
