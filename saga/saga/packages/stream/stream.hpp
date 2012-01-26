//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2007 Ole Weidner (oweidner@cct.lsu.edu)
// 
//  Use, modification and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_COMM_STREAM_STREAM_HPP
#define SAGA_COMM_STREAM_STREAM_HPP

// include stl
#include <string>
#include <vector>

// include dependent spec sections
#include <saga/saga/util.hpp>
#include <saga/saga/call.hpp>
#include <saga/saga/base.hpp>
#include <saga/saga/session.hpp>
#include <saga/saga/task.hpp>
#include <saga/saga/buffer.hpp>
#include <saga/saga/url.hpp>
#include <saga/saga/context.hpp>

#include <saga/saga/detail/attribute.hpp>
#include <saga/saga/detail/monitorable.hpp>

// include package config
#include <saga/saga/packages/stream/config.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable: 4251 4231 4660)
#endif

namespace saga
{
    /*! \brief The <I>Streams</I> API package
     */
    namespace stream {
    
    /*! \brief   Available %attributes defined for the Stream API package
     */
    namespace attributes 
    {
        //  read write attributes
        char const* const stream_bufsize = "Bufsize";
        char const* const stream_timeout = "Timeout";
        char const* const stream_blocking = "Blocking";
        char const* const stream_compression = "Compression";
        char const* const stream_nodelay = "Nodelay";
        char const* const stream_reliable = "Reliable";
    }
    
    /*! \brief Available %metrics defined for the Stream API package
     *
     */
    namespace metrics 
    {
        char const* const stream_state = "stream.State";
        char const* const stream_read = "stream.Read";
        char const* const stream_write = "stream.Write";
        char const* const stream_exception = "stream.Exception";
        char const* const stream_dropped = "stream.Dropped";
    }

    namespace attributes 
    {
        // string based representation of stream states
        char const* const stream_state_unknown = "Unknown";
        char const* const stream_state_new = "New";
        char const* const stream_state_open = "open";
        char const* const stream_state_closed = "Closed";
        char const* const stream_state_dropped = "Dropped";
        char const* const stream_state_error = "Error";
    }

    /*! \brief The <I>Streams</I> API package 
     *
     *   Detailed %description here...
     */  

    /*! \brief A SAGA stream can be in several states.      
     *
     *   A SAGA stream can be in several states. The stream states are
     * New, Open, Closed, Dropped and Error. 
     * All method calls, apart from the DESTRUCTOR, will cause an 
     *IncorrectState exception if the stream is in a final state. 
     */
    enum state
    {
        Unknown      = -1,
        New          =  1,
        Open         =  2,
        Closed       =  3,
        Dropped      =  4,
        Error        =  5
    };

    /*! \brief The SAGA stream API allows for event driven communication.   
     *
     *   A stream can flag activities, i.e. Read, Write and Exception, 
     * and the application can react on these activities. It is possible 
     * to poll for these events (using wait() with a potential timeout), 
     * or to get asynchronous notification of these events, by using the 
     * respective metrics. 
     */     
    enum activity
    {
        Read         =  1,
        Write        =  2,
        Exception    =  4
    };

    /*! \brief This is the ob ject that encapsulates all client stream ob jects.      
     *
     *
     */
    class SAGA_STREAM_PACKAGE_EXPORT stream 
        : public saga::object,
          public saga::detail::attribute<stream>,
          public saga::detail::monitorable<stream>
    {
        /// @cond
        /** These methods are not within API scope */
        friend struct saga::detail::attribute<stream>;    // needs to access get_impl()
        friend struct saga::detail::monitorable<stream>;  // needs to access get_impl()

        typedef saga::detail::attribute<stream> attribute_base_type;
        typedef saga::detail::monitorable<stream> monitorable_base_type;
        /// @endcond

    public:

      /*! \brief Brief %description starts here     
       *
       *
       */
      //typedef stream_base::state state; 

    protected:
        /// @cond
        /** These methods are not within API scope */
        TR1::shared_ptr <saga::impl::stream> get_impl_sp(void) const;
        saga::impl::stream* get_impl (void) const;
        friend class saga::impl::stream;
        friend struct saga::detail::create_default<stream>;

        explicit stream (saga::impl::stream *);
        explicit stream (int);    // dummy constructor to create empty facade
        /// @endcond

    private:
        // factory
        SAGA_CALL_CREATE_PRIV_2(session const&, saga::url)

        // stream inspection methods
        SAGA_CALL_CONST_PRIV_0(get_url)
        SAGA_CALL_CONST_PRIV_0(get_context)

        // stream methods
        SAGA_CALL_PRIV_1(connect, double)
        SAGA_CALL_PRIV_2(wait, saga::stream::activity, double)
        SAGA_CALL_PRIV_1(close, double)
        SAGA_CALL_PRIV_2(read, saga::mutable_buffer, saga::ssize_t)
        SAGA_CALL_PRIV_2(write, saga::const_buffer, saga::ssize_t)

        void init_attributes();
        void init_metrics();

    public:
        /*! \brief Initializes a client client stream, for later connection to an server.      
         *
         *
         */
        explicit stream (session const & s, saga::url url = saga::url());

        /*! \brief Initializes a client client stream, for later connection to an server.       
         *
         *
         */
        explicit stream (saga::url url);

        /*! \brief Initializes a client client stream, for later connection to an server.        
         *
         *
         */
        stream ();

        /*! \brief Initializes a client client stream, for later connection to an server.        
         *
         *
         */
        explicit stream (saga::object const& o);

        /*! \brief Destroys an stream object     
         *
         *
         */
        ~stream (void);

        /*! \brief Factory function resembling stream(session, url) 
         *         constructor
         */
        static stream create(session const& s, saga::url name = saga::url())
        {
            return stream(s, name);
        }
        /// @cond
        SAGA_CALL_CREATE_2_DEF_1(session const&, saga::url, saga::url())
        /// @endcond

        /*! \brief Factory function resembling stream(url) constructor
         */
        static stream create(saga::url name = saga::url())
        {
            return stream(name);
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
        stream &operator= (saga::object const& o);

        /*! \brief Get the URL used for creating this stream     
         *         
         *  \return String containing the URL of the connection. 
         *
         *
         */
        saga::url get_url() const
        {
            saga::task t = get_urlpriv(saga::task_base::Sync()); 
            return t.get_result<saga::url>();
        }
        SAGA_CALL_CONST_PUB_0_DEF_0(get_url)

        /*! \brief Return remote authorization info     
         *         
         *  \return Remote context
         *
         *
         */
        saga::context get_context() const
        {
            saga::task t = get_contextpriv(saga::task_base::Sync()); 
            return t.get_result<saga::context>();
        }
        SAGA_CALL_CONST_PUB_0_DEF_0(get_context)

        /*! \brief Establishes a connection to the target defined during the 
         *         construction of the stream   
         *         
         *
         *
         */
        void connect(double timeout  = -1.0) 
        {
            saga::task t = connectpriv(timeout, saga::task_base::Sync()); 
            return t.get_result();
        }
        SAGA_CALL_PUB_1_DEF_1(connect, double, -1.0)

        /*! \brief Check if stream is ready for reading/writing, or if it has 
         *         entered an error state  
         *         
         *  \param what Parameter list of activity types to wait for 
         *  \param timeout Number of seconds to wait 
         *  \return activity type causing the call to return 
         *
         *
         */
        saga::stream::activity
        wait(saga::stream::activity what, double timeout = -1.0) 
        {
            saga::task t = waitpriv(what, timeout, saga::task_base::Sync()); 
            return t.get_result<saga::stream::activity>();
        }
        SAGA_CALL_PUB_2_DEF_1(wait, saga::stream::activity, double, -1.0)

        /*! \brief Closes an active connection    
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

        /*! \brief Read a raw buffer from stream    
         *         
         *  \param length Maximum number of bytes that can be copied in to the buffer. 
         *  \param buffer Empty buffer passed in to get filled 
         *  \return Number of bytes read, if successful. (0 is also valid)
         *
         *
         */
        saga::ssize_t read(saga::mutable_buffer buffer, saga::ssize_t length = 0) 
        {
            saga::task t = readpriv(buffer, length, saga::task_base::Sync()); 
            return t.get_result<saga::ssize_t>();
        }
        SAGA_CALL_PUB_2_DEF_1(read, saga::mutable_buffer, saga::ssize_t, 0)

        /*! \brief Write a raw buffer to stream    
         *         
         *  \param length Number of bytes of data in the buffer 
         *  \param buffer Raw array containing data that will be sent out via socket 
         *  \return Bytes written if successful 
         *
         *
         */
        saga::ssize_t write(saga::const_buffer buffer, saga::ssize_t length = 0) 
        {
            saga::task t = writepriv(buffer, length, saga::task_base::Sync()); 
            return t.get_result<saga::ssize_t>();
        }
        SAGA_CALL_PUB_2_DEF_1(write, saga::const_buffer, saga::ssize_t, 0)
    };

  }

  namespace detail
  {
      // we need to specialize the create_default template (used in hold_any) 
      // to be able to create a truly default constructed stream (empty 
      // facade)
      template<>
      struct create_default<saga::stream::stream>
      {
          static saga::stream::stream* call()
          {
              return new saga::stream::stream(1);    // use dummy constructor
          }
          template <typename T_> static void call(T_* obj) 
          { 
              new (obj) saga::stream::stream(1); 
          }
      };
  }

    /// @cond
    namespace adaptors
    {
        saga::stream::state SAGA_STREAM_PACKAGE_EXPORT 
            stream_state_value_to_enum(std::string const& val);
        std::string SAGA_STREAM_PACKAGE_EXPORT 
            stream_state_enum_to_value(int s);
    }
    /// @endcond
} 

// re-enable warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

#endif // SAGA_COMM_STREAM_STREAM_HPP

