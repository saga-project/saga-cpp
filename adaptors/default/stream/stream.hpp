//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ADAPTORS_DEFAULT_STREAM_HPP
#define ADAPTORS_DEFAULT_STREAM_HPP

#include <string>
#include <boost/asio.hpp>

#include <saga/saga/util.hpp>
#include <saga/saga/adaptors/adaptor.hpp>

#include <saga/impl/engine/proxy.hpp>
#include <saga/impl/packages/stream/stream_cpi.hpp>

#include "stream_adaptor.hpp"
#include "stream_cpi_data.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace stream
{
    ///////////////////////////////////////////////////////////////////////////
    class stream 
        : public saga::adaptors::v1_0::stream_cpi<stream>
    {
    private:
        typedef saga::adaptors::v1_0::stream_cpi<stream> base_cpi;

        ///////////////////////////////////////////////////////////////////////
        //  instance data
        //    data associated with the API object instance, i.e. visible to all 
        //    CPI object instances associated with the API object instance 
        //    regardless of the shared library (adaptor) they are implemented in.
        //
        //  We need to redefine these here (instead using the same types in the 
        //  base class since we are using our own instance_data type.
        //  Note: we're using dynamic_instance_data to ensure the accessed data
        //        is really of the expected type
        typedef stream_cpi_data dyn_cpi_instance_data;
        friend class saga::adaptors::dynamic_instance_data<dyn_cpi_instance_data>;
        typedef      saga::adaptors::dynamic_instance_data<dyn_cpi_instance_data> 
            dyn_instance_data_type;

        // default instance data
        typedef saga::adaptors::v1_0::stream_cpi_instance_data cpi_instance_data;
        friend class saga::adaptors::instance_data<cpi_instance_data>;
        typedef      saga::adaptors::instance_data<cpi_instance_data> 
            instance_data_type;

        //  additional member data
        TR1::shared_ptr<boost::asio::ip::tcp::socket> sock_;
        
        TR1::shared_ptr<saga_default_stream_adaptor> get_adaptor() 
        { 
            return TR1::static_pointer_cast<saga_default_stream_adaptor>(
                this->adaptor_);
        }
        
    public:
        /*! constructor of the stream cpi */
        stream  (proxy* p, cpi_info const& info, 
            saga::ini::ini const& glob_ini, saga::ini::ini const& adap_ini,
            TR1::shared_ptr<saga::adaptor> adaptor);

        /*! destructor of the stream cpi */
        ~stream (void);

        /*! implementation of SAGA::stream::get_url */
        void sync_get_url (saga::url& url);
        void sync_get_context (saga::context& ctx);

        void sync_connect (saga::impl::void_t&, double timeout);
        void sync_wait (saga::stream::activity & ret,
            saga::stream::activity act, double timeout);
        void sync_close (saga::impl::void_t& ret, double timeout);
        void sync_read (saga::ssize_t& ret, saga::mutable_buffer data, 
            saga::ssize_t len_in);
        void sync_write (saga::ssize_t& ret, saga::const_buffer data, 
            saga::ssize_t len_in);
        
#if !defined(SAGA_DEFAULT_STREAM_ADAPTOR_NO_ASYNCS)
        // This adaptor implements the async functions based on its own synchronous
        // functions for testing purposes only. Since there is no principal need
        // to do so, we allow these to be 'switched off'.
        saga::task async_get_url ();
        saga::task async_get_context ();

        saga::task async_connect (double timeout);
        saga::task async_wait (saga::stream::activity act, double timeout);
        saga::task async_close (double timeout);
        saga::task async_read (saga::mutable_buffer data, saga::ssize_t len_in);
        saga::task async_write (saga::const_buffer data, saga::ssize_t len_in);
#endif

    protected:
        void update_state(saga::stream::state newstate);
        saga::stream::state retrieve_state();

        void check_if_open (std::string const& functionname, 
            saga::url const& location);
    };  // class stream

}   // namespace stream

#endif // ADAPTORS_DEFAULT_STREAM_HPP

