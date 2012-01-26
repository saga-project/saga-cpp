//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/asio.hpp>

#include <map>
#include <vector>

#include <saga/saga/exception.hpp>
#include <saga/saga/url.hpp>
#include <saga/impl/config.hpp>

#include <saga/saga/adaptors/task.hpp>
#include <saga/saga/adaptors/metric.hpp>
#include <saga/saga/adaptors/utils/ini/ini.hpp>
#include <saga/saga/packages/stream/adaptors/stream.hpp>

#include "stream_adaptor.hpp"
#include "stream_server.hpp"
#include "stream_cpi_data.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace stream
{

///////////////////////////////////////////////////////////////////////////////
namespace detail
{
    std::string get_default_url(saga::ini::ini const& adap_ini)
    {
        // try to find default url in configuration file
        if (adap_ini.has_section("preferences")) 
        {
            saga::ini::ini prefs = adap_ini.get_section ("preferences");
            if (prefs.has_entry("url"))
            {
                return prefs.get_entry("url");
            }
        }

    // throw error 
        SAGA_OSSTREAM strm;
        strm << "stream_server::init: "
                "no default stream server URL specified in configuration.";
        SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
            saga::BadParameter);
        return std::string();
    }
}

///////////////////////////////////////////////////////////////////////////////
stream_server::stream_server (proxy* p, cpi_info const& info, 
        saga::ini::ini const& glob_ini, saga::ini::ini const& adap_ini, 
        TR1::shared_ptr<saga::adaptor> adaptor)
  : base_cpi (p, info, adaptor, cpi::Noflags), 
    acceptor_(get_adaptor()->io_service_)
{
    instance_data_type data(this);
    if (data->location_.get_url().empty()) 
    {
        // choose implementation defined value
        data->location_ = detail::get_default_url(adap_ini);
    }
    
    // we implement only 'any' and 'tcp' schemes
    saga::url service_url(data->location_);
    std::string scheme(service_url.get_scheme());
    if (!scheme.empty() && scheme != "tcp" && scheme != "any")
    {
        SAGA_ADAPTOR_THROW("Can't use schemes others from 'any' or 'tcp' " 
            "for stream management.", saga::adaptors::AdaptorDeclined);
    }
}

void stream_server::initialize_service()
{
    if (acceptor_.is_open())
        return;     // initialize only once

    try {
        saga::url url;
        {
            instance_data_type data(this);
            url = data->location_;
        }

    // initialize acceptor
        mutex_type::scoped_lock lock(mtx_);
        int port = url.get_port();
        if (-1 == port) 
            port = SAGA_DEFAULT_STREAM_DEFAULT_PORT;

        using boost::asio::ip::tcp;

        tcp::endpoint endpoint(tcp::v4(), port);
        acceptor_.open(endpoint.protocol());
        acceptor_.set_option(tcp::acceptor::reuse_address(true));
        acceptor_.bind(endpoint);
        acceptor_.listen();
    }
    catch (boost::system::system_error const& e) {
        SAGA_OSSTREAM errstrm;
        errstrm << "stream_server::initialize_service: could not initialize "
                   "service: " << e.what();
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(errstrm), 
            saga::NoSuccess);
    } 
}

///////////////////////////////////////////////////////////////////////////////
stream_server::~stream_server (void)
{
    try {
        if (acceptor_.is_open()) 
            acceptor_.close();
    }
    catch (boost::system::system_error const& e) {
        SAGA_OSSTREAM errstrm;
        errstrm << "stream_server::~stream_server: could not close service: "
                << e.what();
        SAGA_LOG_CRITICAL(SAGA_OSSTREAM_GETSTRING(errstrm).c_str());
    } 
}

///////////////////////////////////////////////////////////////////////////////
// SAGA API functions
void stream_server::sync_get_url (saga::url& url)
{
    instance_data_type data(this);
    url = data->location_;
}

void stream_server::sync_serve(saga::stream::stream& result, double timeout)
{
    saga::url location;
    {
        instance_data_type data(this);
        location = data->location_; 
    }

    // make sure acceptor is initialized
    initialize_service();

    {
        mutex_type::scoped_lock lock(mtx_);

        // create the acceptor
        using boost::asio::ip::tcp;
        TR1::shared_ptr<tcp::socket> sock(new tcp::socket(get_adaptor()->io_service_));

        // try to accept a connection
        boost::system::error_code ec;
        acceptor_.accept(*sock, ec);
        if (ec) 
        {
            SAGA_OSSTREAM errstrm;
            errstrm << "stream_server::sync_serve: could not establish connection: "
                    << ec.message();
            SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(errstrm), 
                saga::BadParameter);
        }

        // create the SAGA stream object
        saga::adaptors::stream stream(
            new stream::stream_cpi_data(location, sock), 
            proxy_->get_session());

        result = stream;
    }

    // fire corresponding metric
    saga::monitorable monitor (this->proxy_);
    saga::adaptors::metric m (
        monitor.get_metric(saga::stream::metrics::server_clientconnect));
    m.set_attribute(saga::attributes::metric_value, "1");
    m.fire();
}

void stream_server::sync_close(saga::impl::void_t& ret, double timeout)
{
    // verify stream_server is operational
    if (!acceptor_.is_open())
    {
        SAGA_ADAPTOR_THROW(
            "stream_server::sync_close: stream has been closed already.",
            saga::IncorrectState);
    }

    // positive timeout is not supported
    if (timeout > 0.0)
    {
        SAGA_ADAPTOR_THROW(
            "stream_server::sync_close: positive timeout parameter values "
            "are not supported.", saga::BadParameter);
    }

    // close the service
    try {
        if (acceptor_.is_open()) 
            acceptor_.close();
    }
    catch (boost::system::system_error const& e) {
        SAGA_OSSTREAM errstrm;
        errstrm << "stream_server::sync_close: could not close service: "
                << e.what();
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(errstrm),
            saga::NoSuccess);
    } 
}

void stream_server::sync_connect (saga::stream::stream& ret, double timeout)
{
    if (timeout != -1.0)
    {
        SAGA_OSSTREAM errstrm;
        errstrm << "stream_server::sync_connect: timeout (other than -1.0) is "
                   "not supported, provided value: "
                << timeout;
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(errstrm),
            saga::NotImplemented);
    }

    saga::url url;
    {
        instance_data_type data(this);
        url = data->location_;
    }

    // create a new stream connected to this server
    saga::stream::stream strm (this->get_session(), url);
    strm.connect();

    ret = strm;
}

#if !defined(SAGA_DEFAULT_STREAM_ADAPTOR_NO_ASYNCS)
///////////////////////////////////////////////////////////////////////////////
// This adaptor implements the async functions based on its own synchronous
// functions for testing purposes only. Since there is no principal need
// to do so, we allow these to be 'switched off'.

saga::task stream_server::async_get_url () 
{
    return (saga::adaptors::task("stream_server::async_get_url",
        shared_from_this(), &stream_server::sync_get_url)); 
}

saga::task stream_server::async_serve(double timeout)
{
    return (saga::adaptors::task("stream_server::async_serve",
        shared_from_this(), &stream_server::sync_serve, 
        timeout)); 
}

saga::task stream_server::async_close(double timeout)
{
    return (saga::adaptors::task("stream_server::async_close",
        shared_from_this(), &stream_server::sync_close, timeout)); 
}

saga::task stream_server::async_connect(double timeout)
{
    return (saga::adaptors::task("stream_server::async_connect",
        shared_from_this(), &stream_server::sync_connect, timeout)); 
}

#endif

}   // namespace stream
