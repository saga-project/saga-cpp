//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/asio.hpp>
#include <boost/cstdint.hpp>

#include <map>
#include <vector>

#include <saga/saga/exception.hpp>
#include <saga/saga/url.hpp>
#include <saga/impl/config.hpp>

#include <saga/saga/adaptors/task.hpp>
#include <saga/saga/adaptors/metric.hpp>

#include <saga/saga/stream.hpp>

#include "stream_adaptor.hpp"
#include "stream.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace stream
{

    ///////////////////////////////////////////////////////////////////////////
    namespace detail
    {
        std::string get_host(saga::url const& url)
        {
            return url.get_host();
        }
        
        std::string get_port(saga::url const& url)
        {
            int port = url.get_port();
            if (-1 == port)
                port = SAGA_DEFAULT_STREAM_DEFAULT_PORT;
            return boost::lexical_cast<std::string>(port);
        }
    }

///////////////////////////////////////////////////////////////////////////////
stream::stream (proxy* p, cpi_info const& info, saga::ini::ini const& glob_ini, 
        saga::ini::ini const& adap_ini, TR1::shared_ptr<saga::adaptor> adaptor)
  : base_cpi (p, info, adaptor, cpi::Fixme)
{
    {
        // we implement only 'any' and 'tcp' schemes
        instance_data_type data(this);
        saga::url service_url(data->location_);
        std::string scheme(service_url.get_scheme());
        if (!scheme.empty() && scheme != "tcp" && scheme != "any")
        {
            SAGA_ADAPTOR_THROW("Can't use schemes others from 'any' or 'tcp' " 
                "for stream.", saga::adaptors::AdaptorDeclined);
        }
    }

    saga::stream::state state = saga::stream::New;
    {
        dyn_instance_data_type data(this);
        if (!data.is_valid()) {
            instance_data_type static_data(this);
            if (!static_data->is_default_) 
            {
                SAGA_ADAPTOR_THROW(
                    "The instance data is not supported by this adaptor.",
                    saga::adaptors::AdaptorDeclined);
            }
        }
        else {
            // initialize if we already have a connected socket
            mutex_type::scoped_lock l(this->mtx_);
            sock_ = data->sock_;
            state = saga::stream::Open;
        }
    }
    update_state(state);
}

///////////////////////////////////////////////////////////////////////////////
stream::~stream (void)
{
    try {
    // make sure connection gets closed before the end
        if (sock_ && sock_->is_open())
            sock_->close();
        sock_.reset();
    }
    catch (boost::system::system_error const& e) {
        SAGA_OSSTREAM errstrm;
        errstrm << "stream::~stream: could not close connection: "
                << e.what();
        SAGA_LOG_CRITICAL(SAGA_OSSTREAM_GETSTRING(errstrm).c_str());
    } 
}

///////////////////////////////////////////////////////////////////////////////
void stream::check_if_open (std::string const& functionname, 
        saga::url const& location)
{
    saga::stream::state state = retrieve_state(); 
    if (saga::stream::Open != state || !sock_->is_open())
    {
        SAGA_OSSTREAM errstrm;
        errstrm << functionname << ": stream (" << location.get_url() 
                << ") is not in 'Open' state, current state is: " 
                << saga::adaptors::get_state_name(state);
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(errstrm),
            saga::IncorrectState);
    }
}

///////////////////////////////////////////////////////////////////////////////
void stream::update_state(saga::stream::state newstate)
{
    // update state attribute and promote state change to the metric callbacks
    saga::monitorable monitor (this->proxy_);
    saga::adaptors::metric m (
        monitor.get_metric(saga::stream::metrics::stream_state));
    m.set_attribute(saga::attributes::metric_value, 
        saga::adaptors::stream_state_enum_to_value(newstate));
    m.fire();

    // if we reached the 'Error' state, fire the 'stream_exception' metric
    if (newstate == saga::stream::Error) {
        m = monitor.get_metric(saga::stream::metrics::stream_exception);
        m.set_attribute(saga::attributes::metric_value, "1");
        m.fire();
    }
}

saga::stream::state stream::retrieve_state() 
{
    // get state attribute 
    saga::monitorable monitor (this->proxy_);
    saga::metric m (monitor.get_metric(saga::stream::metrics::stream_state));
    return (saga::stream::state)saga::adaptors::stream_state_value_to_enum(
        m.get_attribute(saga::attributes::metric_value));
}

///////////////////////////////////////////////////////////////////////////////
// get_url()
void stream::sync_get_url (saga::url & url)
{
    instance_data_type data (this);
    url = data->location_;
}

void stream::sync_get_context(saga::context& ctx)
{
    SAGA_ADAPTOR_THROW("Not Implemented (stream::sync_get_context)", 
        saga::NotImplemented);
}

void stream::sync_connect(saga::impl::void_t&, double timeout)
{
    if (timeout != -1.0)
    {
        SAGA_OSSTREAM errstrm;
        errstrm << "stream::sync_connect: timeout (other than -1.0) is "
                   "not supported, provided value: "
                << timeout;
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(errstrm),
            saga::NotImplemented);
    }

    // verify stream is still 'New'
    saga::stream::state state = retrieve_state(); 
    if (saga::stream::New != state)
    {
        SAGA_OSSTREAM errstrm;
        errstrm << "stream::sync_connect: stream is not in 'New' state, "
                   "current state is: " 
                << saga::adaptors::get_state_name(state);
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(errstrm),
            saga::IncorrectState);
    }

    // now try to connect
    try {
        saga::url location;
        {
            instance_data_type data(this);
            location = data->location_;
        }

    // try to resolve the given url
        using boost::asio::ip::tcp;

        tcp::resolver resolver(get_adaptor()->io_service_);
        tcp::resolver::query query(tcp::v4(), detail::get_host(location), 
            detail::get_port(location));
        tcp::resolver::iterator iterator = resolver.resolve(query);

    // create and connect the socket
        mutex_type::scoped_lock l(this->mtx_);
        sock_.reset(new tcp::socket(get_adaptor()->io_service_));
        sock_->connect(*iterator);

        update_state(saga::stream::Open);
    }
    catch (boost::system::system_error const& e) {
    // set stream state
        update_state(saga::stream::Error);

    // throw appropriate exception
        SAGA_OSSTREAM errstrm;
        if (e.code() == boost::asio::error::already_connected) {
            // port already taken, spec requires to throw a BadParameter error
            errstrm << "stream::sync_connect: resource information given in "
                       "the URL cannot be used temporarily (port already taken?): "
                    << e.code().message();
            SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(errstrm), 
                saga::BadParameter);
        }

        errstrm << "stream::sync_connect: could not establish connection: "
                << e.what();
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(errstrm), 
            saga::NoSuccess);
    }
}

void 
stream::sync_wait(saga::stream::activity & ret, 
                  saga::stream::activity   act, 
                  double                   timeout)
{
    {
        instance_data_type data (this);
        check_if_open("stream::sync_wait", data->location_);
    }

    if ( timeout != 0.0 )
    {
      std::cout << "timeout: " << timeout << std::endl;
      SAGA_ADAPTOR_THROW("Timed wait Not Implemented (stream::sync_wait)", 
                         saga::NotImplemented);
    }


    if ( act & saga::stream::Read )
    {
      if ( sock_->available () > 0 )
      {
        ret = static_cast <saga::stream::activity> (ret | saga::stream::Read);
      }
    }

    if ( act & saga::stream::Write )
    {
      if ( sock_->is_open () )
      {
        ret = static_cast <saga::stream::activity> (ret | saga::stream::Write);
      }
    }

    if ( act & saga::stream::Exception )
    {
      if ( ! sock_->is_open () )
      {
        ret = static_cast <saga::stream::activity> (ret | saga::stream::Exception);
      }
    }
}

void stream::sync_close(saga::impl::void_t& ret, double timeout)
{
    // verify stream is 'Open'
    {
        instance_data_type data (this);
        check_if_open("stream::sync_close", data->location_);
    }

    // positive timeout is not supported
    if (timeout > 0.0)
    {
        SAGA_ADAPTOR_THROW(
            "stream_server::sync_close: positive timeout parameter values "
            "are not supported.", saga::BadParameter);
    }

    // close the socket and free associated resources
    mutex_type::scoped_lock l(this->mtx_);
    try {
        sock_->close();
        sock_.reset();
        update_state(saga::stream::Closed);
    }
    catch (boost::system::system_error const& e) {
    // set stream state
        update_state(saga::stream::Error);

    // throw appropriate exception
        SAGA_OSSTREAM errstrm;
        errstrm << "stream::sync_close: could not close connection: "
                << e.what();
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(errstrm), 
            saga::NoSuccess);
    }
}

bool read_completed(boost::system::error_code const& err, 
    std::size_t bytes_transferred, boost::uint64_t size)
{
    return bytes_transferred >= size;
}

void 
stream::sync_read(saga::ssize_t& ret, saga::mutable_buffer data, 
    saga::ssize_t len_in)
{
    // verify stream is 'Open'
    {
        instance_data_type data (this);
        check_if_open("stream::sync_wait", data->location_);
    }

    mutex_type::scoped_lock l(this->mtx_);
    boost::system::error_code err = boost::asio::error::try_again;

    // read message length
    boost::uint64_t size = 0;
    std::size_t reply_length = boost::asio::read(*sock_,
        boost::asio::buffer(&size, sizeof(size)),
        boost::bind(&read_completed, _1, _2, sizeof(size)),
        err);
    if (err) {
        SAGA_OSSTREAM errstrm;
        errstrm << "stream::sync_read: error reading from stream: " 
                << err.message();
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(errstrm),
            saga::NoSuccess);
    }
    if (reply_length != sizeof(size)) {
        SAGA_OSSTREAM errstrm;
        errstrm << "stream::sync_read: eof reading from stream: " 
                << "expected to read " << sizeof(size) 
                << ", got " << reply_length << " bytes.";
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(errstrm),
            saga::NoSuccess);
    }

    // read message
    std::size_t read_bytes = boost::asio::read(*sock_, 
        boost::asio::buffer(data.get_data(), (std::min)(size, (boost::uint64_t)len_in)), 
        boost::bind(&read_completed, _1, _2, size), 
        err);

    if (err) {
        SAGA_OSSTREAM errstrm;
        errstrm << "stream::sync_read: error reading from stream: " 
                << err.message();
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(errstrm),
            saga::NoSuccess);
    }
    if (read_bytes != size) {
        SAGA_OSSTREAM errstrm;
        errstrm << "stream::sync_read: eof reading from stream: " 
                << "expected to read " << len_in 
                << ", got " << read_bytes << " bytes.";
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(errstrm),
            saga::NoSuccess);
    }

    ret = read_bytes;
}

bool write_completed(boost::system::error_code const& err, 
    std::size_t bytes_transferred, boost::uint64_t size)
{
    return bytes_transferred >= size;
}

void 
stream::sync_write(saga::ssize_t& ret, saga::const_buffer data, 
    saga::ssize_t len_out)
{
    // verify stream is 'Open'
    {
        instance_data_type data (this);
        check_if_open("stream::sync_write", data->location_);
    }

    mutex_type::scoped_lock l(this->mtx_);

    boost::uint64_t size = data.get_size();
    std::vector<boost::asio::const_buffer> buffers;
    buffers.push_back(boost::asio::buffer(&size, sizeof(size)));
    buffers.push_back(boost::asio::buffer(data.get_data(), size));

    boost::system::error_code err = boost::asio::error::try_again;
    std::size_t written_bytes = boost::asio::write(*sock_,
        buffers,
        boost::bind(&write_completed, _1, _2, sizeof(size) + len_out),
        err);

    if (err) {
        SAGA_OSSTREAM errstrm;
        errstrm << "stream::sync_write: error writing to stream: " 
                << err.message();
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(errstrm),
            saga::NoSuccess);
    }
    if (written_bytes != sizeof(size) + data.get_size()) {
        SAGA_OSSTREAM errstrm;
        errstrm << "stream::sync_write: eof writing to stream: " 
                << "expected to write " << len_out 
                << ", wrote " << written_bytes << " bytes.";
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(errstrm),
            saga::NoSuccess);
    }

    ret = written_bytes - sizeof(size);
}

#if !defined(SAGA_DEFAULT_STREAM_ADAPTOR_NO_ASYNCS)
///////////////////////////////////////////////////////////////////////////////
// This adaptor implements the async functions based on its own synchronous
// functions for testing purposes only. Since there is no principal need
// to do so, we allow these to be 'switched off'.

saga::task stream::async_get_url () 
{
    return (saga::adaptors::task("stream::async_get_url",
        shared_from_this(), &stream::sync_get_url)); 
}

saga::task stream::async_get_context()
{
    return (saga::adaptors::task("stream::async_get_context",
        shared_from_this(), &stream::sync_get_context)); 
}

saga::task stream::async_connect(double timeout)
{
    return (saga::adaptors::task("stream::async_connect",
        shared_from_this(), &stream::sync_connect, timeout)); 
}

saga::task 
stream::async_wait(saga::stream::activity act, double timeout)
{
    return (saga::adaptors::task("stream::async_wait",
        shared_from_this(), &stream::sync_wait, act, timeout)); 
}

saga::task stream::async_close(double timeout)
{
    return (saga::adaptors::task("stream::async_close",
        shared_from_this(), &stream::sync_close, timeout)); 
}

saga::task 
stream::async_read(saga::mutable_buffer data, saga::ssize_t len_in)
{
    return (saga::adaptors::task("stream::async_read",
        shared_from_this(), &stream::sync_read, data, len_in)); 
}

saga::task 
stream::async_write(saga::const_buffer data, saga::ssize_t len_in)
{
    return (saga::adaptors::task("stream::async_write",
        shared_from_this(), &stream::sync_write, data, len_in)); 
}

#endif

} // namespace stream
