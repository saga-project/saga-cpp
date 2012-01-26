//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ADAPTORS_DEFAULT_STREAMSERVICE_HPP
#define ADAPTORS_DEFAULT_STREAMSERVICE_HPP

#include <string>

#include <saga/saga/util.hpp>
#include <saga/saga/adaptors/adaptor.hpp>

#include <saga/impl/engine/proxy.hpp>
#include <saga/impl/packages/stream/stream_server_cpi.hpp>

#include "stream_adaptor.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace stream
{
    ///////////////////////////////////////////////////////////////////////////
    class stream_server 
        : public saga::adaptors::v1_0::stream_server_cpi<stream_server>
    {
    private:
        typedef saga::adaptors::v1_0::stream_server_cpi<stream_server> 
            base_cpi;

        boost::asio::io_service io_service_;
        boost::asio::ip::tcp::acceptor acceptor_;

        TR1::shared_ptr<saga_default_stream_adaptor> get_adaptor() 
        { 
            return TR1::static_pointer_cast<saga_default_stream_adaptor>(
                this->adaptor_);
        }

        void initialize_service();

    public:
        /*! constructor of the stream_server cpi */
        stream_server  (proxy* p, cpi_info const& info, 
            saga::ini::ini const& glob_ini, saga::ini::ini const& adap_ini, 
            TR1::shared_ptr<saga::adaptor> adaptor);

        /*! destructor of the stream_server cpi */
        ~stream_server (void);

        /*! implementation of SAGA::stream_server::get_url */
        void sync_get_url (saga::url & url);
        void sync_serve (saga::stream::stream& strm, double timeout);
        void sync_close (saga::impl::void_t& ret, double timeout);
        void sync_connect (saga::stream::stream& ret, double timeout);

#if !defined(SAGA_DEFAULT_STREAM_ADAPTOR_NO_ASYNCS)
        // This adaptor implements the async functions based on its own synchronous
        // functions for testing purposes only. Since there is no principal need
        // to do so, we allow these to be 'switched off'.
        saga::task async_get_url ();
        saga::task async_serve (double timeout);
        saga::task async_close (double timeout);
        saga::task async_connect (double timeout);
#endif
    };  // class stream_server

}   // namespace stream

#endif // ADAPTORS_DEFAULT_STREAMSERVICE_HPP

