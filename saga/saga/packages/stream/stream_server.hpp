//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2007 Ole Weidner (oweidner@cct.lsu.edu)
// 
//  Use, modification and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_COMM_STREAM_STREAMSERVICE_HPP
#define SAGA_COMM_STREAM_STREAMSERVICE_HPP

// include stl
#include <string>

// include dependent spec sections
#include <saga/saga/util.hpp>
#include <saga/saga/call.hpp>
#include <saga/saga/base.hpp>
#include <saga/saga/task.hpp>
#include <saga/saga/session.hpp>
#include <saga/saga/detail/monitorable.hpp>
#include <saga/saga/detail/permissions.hpp>

// include package config
#include <saga/saga/packages/stream/config.hpp>

// need stream
#include <saga/saga/stream.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable: 4251 4231 4660)
#endif

////////////////////////////////////////////////////////////////////////////////
namespace saga
{
    namespace stream {

    namespace metrics 
    {
        char const* const server_clientconnect = "server.ClientConnect";
    }
  
    /*! \brief The stream_server object establishes a listening/server
     * object that waits for client connections. 
     *
     *   It can only be used as a factory for client sockets. It doesn’t
     * do any read/write I/O.
     */    
    class SAGA_STREAM_PACKAGE_EXPORT server 
        : public saga::object,
          public saga::detail::monitorable<server>,
          public saga::detail::permissions<server>
    {
        /// @cond
        /** These methods are not within API scope */
        friend struct saga::detail::monitorable<server>;  // needs to access get_impl()
        friend struct saga::detail::permissions<server>;
        typedef saga::detail::monitorable<server> monitorable_base_type;
        /// @endcond

    protected:
        /// @cond
        /** These methods are not within API scope */
        TR1::shared_ptr <saga::impl::server> get_impl_sp(void) const;
        saga::impl::server* get_impl (void) const;
        friend class saga::impl::server;
        friend struct saga::detail::create_default<server>;

        explicit server (saga::impl::server* impl);
        explicit server (int);    // dummy constructor to create empty facade
        /// @endcond

    private:
        // factory
        SAGA_CALL_CREATE_PRIV_2(session const&, saga::url)

        // stream server methods
        SAGA_CALL_CONST_PRIV_0(get_url)
        SAGA_CALL_PRIV_1(connect, double)
        SAGA_CALL_PRIV_1(serve, double)
        SAGA_CALL_PRIV_1(close, double)

        void init_metrics();

    public:
        /*! \brief Creates a new stream_server object.      
         *
         *
         */   
        explicit server (session const& s, saga::url url = saga::url());

        /*! \brief Creates a new stream_server object.     
         *
         *
         */     
        explicit server (saga::url url);

        /*! \brief Creates a new stream_server object.     
         *
         *
         */      
        server ();

        /*! \brief Creates a new stream_server object.     
         *
         *
         */     
        explicit server (saga::object const& o);

        /*! \brief Destructor for stream_server object.     
         *
         *
         */       
        ~server (void);

        /*! \brief Factory function resembling server(session, url) 
         *         constructor
         */
        static server create(session const& s, saga::url name = saga::url())
        {
            return server(s, name);
        }
        /// @cond
        SAGA_CALL_CREATE_2_DEF_1(session const&, saga::url, saga::url())
        /// @endcond

        /*! \brief Factory function resembling server(url) constructor
         */
        static server create(saga::url name = saga::url())
        {
            return server(name);
        }
        template <typename Tag>
        static saga::task create(saga::url name = saga::url())
        {
            return create<Tag>(detail::get_the_session(), name);
        }

        /*! \brief Brief %description starts here     
         *
         *
         */       
        server &operator= (saga::object const& o);

        /*! \brief Get the URL to be used to connect to this server    
         *         
         *  \return String containing the URL of the connection
         *
         *
         */     
        saga::url get_url() const 
        {
            saga::task t = get_urlpriv(saga::task_base::Sync()); 
            return t.get_result<saga::url>();
        }
        SAGA_CALL_CONST_PUB_0_DEF_0(get_url)

        /*! \brief Establishes a connection to this service
         */
        saga::stream::stream connect(double timeout = -1.0) 
        {
            saga::task t = connectpriv(timeout, saga::task_base::Sync()); 
            return t.get_result<saga::stream::stream>();
        }
        SAGA_CALL_PUB_1_DEF_1(connect, double, -1.0)

        /*! \brief Wait for incoming client connections   
         *         
         *  \param timeout Number of seconds to wait for client 
         *  \return New connected stream object 
         *
         *
         */     
        saga::stream::stream serve(double timeout = 0.0) 
        {
            saga::task t = servepriv(timeout, saga::task_base::Sync()); 
            return t.get_result<saga::stream::stream>();
        }
        SAGA_CALL_PUB_1_DEF_1(serve, double, -1.0)

        /*! \brief Closes this stream service   
         *         
         *
         *
         */     
        void close(double timeout = 0.0) 
        {
          saga::task t = closepriv(timeout, saga::task_base::Sync()); 
          t.get_result ();
        }
        SAGA_CALL_PUB_1_DEF_1(close, double, 0.0)
    };
    
    }   // namespace saga::stream

    namespace detail
    {
        // we need to specialize the create_default template (used in hold_any) 
        // to be able to create a truly default constructed server (empty 
        // facade)
        template<>
        struct create_default<saga::stream::server>
        {
            static saga::stream::server* call()
            {
                return new saga::stream::server(1);    // use dummy constructor
            }
            template <typename T_> static void call(T_* obj) 
            { 
                new (obj) saga::stream::server(1); 
            }
        };
    }

}

// re-enable warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

#endif // SAGA_COMM_STREAM_STREAMSERVICE_HPP

