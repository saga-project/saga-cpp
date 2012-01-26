#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/stream.cpp")
#endif
//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Use, modification and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 0, output: null)
#endif

//  this is needed in every file including detail/attribute_impl.hpp and not 
//  belonging to the engine
#define SAGA_NO_IMPORT_ATTRIBUTE

//  this is needed in every file including detail/monitorable_impl.hpp and not 
//  belonging to the engine
#define SAGA_NO_IMPORT_MONITORABLE

#include <boost/assign/list_inserter.hpp>
#include <boost/assign/std/vector.hpp>

// include the package we implement
#include <saga/saga/context.hpp>
#include <saga/saga/stream.hpp>
#include <saga/impl/stream.hpp>
#include <saga/saga/detail/call.hpp>

#include <saga/saga/detail/attribute_impl.hpp>
#include <saga/saga/detail/monitorable_impl.hpp>

#ifdef SAGA_DEBUG
#include <saga/saga/packages/stream/preprocessed/stream.cpp>
#else

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/stream.cpp")
#endif
///////////////////////////////////////////////////////////////////////////////
namespace saga 
{

    namespace stream 
    {
    
    namespace metrics
    {
        /// @cond
        saga::metrics::init_data const stream_metric_data[] = 
        {
            {
                saga::stream::metrics::stream_state,
                "Metric fires if the state of the stream changes,"
                  " and has the value of the new state.",
                saga::attributes::metric_mode_readonly,
                "1", 
                saga::attributes::metric_type_enum,
                "1"   // New
            },
            {
                saga::stream::metrics::stream_read,
                "Metric fires if a stream gets readable.",
                saga::attributes::metric_mode_readonly,
                "1", 
                saga::attributes::metric_type_trigger,
                "0"
            },
            {
                saga::stream::metrics::stream_write,
                "Metric fires if a stream gets writable.",
                saga::attributes::metric_mode_readonly,
                "1", 
                saga::attributes::metric_type_trigger,
                "0"
            },
            {
                saga::stream::metrics::stream_exception,
                "Metric fires if a stream has an error condition.",
                saga::attributes::metric_mode_readonly,
                "1", 
                saga::attributes::metric_type_trigger,
                "0"
            },
            {
                saga::stream::metrics::stream_dropped,
                "Metric fires if a stream gets dropped by the remote party.",
                saga::attributes::metric_mode_readonly,
                "1", 
                saga::attributes::metric_type_trigger,
                "0"
            }
        };
        /// @endcond
    }



    stream::stream (session const& s, saga::url url)
      : saga::object (new saga::impl::stream (s, url))
    {
        init_attributes();
        init_metrics();
        this->saga::object::get_impl()->init();
    }

    stream::stream (saga::url url)
      : saga::object (new saga::impl::stream (detail::get_the_session (), url))
    {
        init_attributes();
        init_metrics();
        this->saga::object::get_impl()->init();
    }

    stream::stream (saga::impl::stream *impl)
      : saga::object (impl)
    {
        init_attributes();
        init_metrics();
        this->saga::object::get_impl()->init();
    }

    stream::stream (int)
    {
    }

    void stream::init_attributes()
    {
        using namespace boost::assign;
        
        // initialize attributes
        std::vector<std::string> valid_keys;
        valid_keys += 
                attributes::stream_bufsize, 
                attributes::stream_timeout, 
                attributes::stream_blocking,
                attributes::stream_compression,
                attributes::stream_nodelay,
                attributes::stream_reliable
            ;

        // initialize list of valid keys          
        this->init_keynames(valid_keys);

        strmap_type attributes_scalar_rw;
        insert(attributes_scalar_rw)
                (attributes::stream_bufsize, "")
                (attributes::stream_timeout, "")
                (attributes::stream_blocking, "")
                (attributes::stream_compression, "")
                (attributes::stream_nodelay, "")
                (attributes::stream_reliable, "")
            ;

        // initialize attribute implementation
        this->attribute_base_type::init (strmap_type(), attributes_scalar_rw);
    }
    
    void stream::init_metrics()
    {
        // initialize metrics
        std::vector<saga::metric> metrics;
        for (unsigned int i = 0; 
             i < sizeof(metrics::stream_metric_data)/sizeof(saga::metrics::init_data);
             ++i)
        {
            saga::metrics::init_data const* p = &saga::stream::metrics::stream_metric_data[i];
            saga::metric m(*this, p->name, p->description, p->mode, p->unit, 
                p->type, p->value);
            metrics.push_back(m);
        }
        this->monitorable_base_type::init (metrics);
    }
    
    stream::stream (void)
      : saga::object (new saga::impl::stream (detail::get_the_session(), saga::url()))
    {
        init_attributes();
        init_metrics();
    }

    stream::~stream (void)
    {
    }

    ///////////////////////////////////////////////////////////////////////////
    stream::stream (saga::object const& o)
      : saga::object (o)
    {
        if (this->get_type() != saga::object::Stream)
        {
            SAGA_THROW("Bad type conversion.", saga::BadParameter);
        }
    }

    stream &stream::operator= (saga::object const& o)
    {
        return saga::object::operator=(o), *this;
    }

    saga::impl::stream* stream::get_impl() const
    { 
        typedef saga::object base_type;
        return static_cast<saga::impl::stream*>(this->base_type::get_impl()); 
    }

    TR1::shared_ptr <saga::impl::stream> stream::get_impl_sp(void) const
    { 
        // FIXME: this needs documentation
        typedef saga::object base_type;
        return TR1::static_pointer_cast <saga::impl::stream>(
            this->base_type::get_impl_sp());
    }

    // factory
    SAGA_CALL_CREATE_IMP_2(stream, impl::stream, session const&, saga::url)

    // stream inspection methods
    SAGA_CALL_CONST_IMP_0 (stream, get_url)
    SAGA_CALL_CONST_IMP_0 (stream, get_context)

    // stream methods
    SAGA_CALL_IMP_1 (stream, connect, double)
    SAGA_CALL_IMP_2 (stream, wait, saga::stream::activity, double)
    SAGA_CALL_IMP_1 (stream, close, double)
    SAGA_CALL_IMP_2 (stream, read, saga::mutable_buffer, saga::ssize_t)
    SAGA_CALL_IMP_2 (stream, write, saga::const_buffer, saga::ssize_t)
    
  } // namespace saga

  namespace detail
  {
    /////////////////////////////////////////////////////////////////////////////
    //  implement the attribute functions (we need to explicitly specialize 
    //  the template because the functions are not implemented inline)
    template struct SAGA_STREAM_PACKAGE_EXPORT_REPEAT attribute<stream::stream>;

    template struct SAGA_STREAM_PACKAGE_EXPORT attribute_priv<stream::stream, task_base::Async>;
    template struct SAGA_STREAM_PACKAGE_EXPORT attribute_priv<stream::stream, task_base::Task>;
    template struct SAGA_STREAM_PACKAGE_EXPORT attribute_priv<stream::stream, task_base::Sync>;

    template struct SAGA_STREAM_PACKAGE_EXPORT attribute_sync<stream::stream>;

    /////////////////////////////////////////////////////////////////////////////
    //  implement the monitorable functions (we need to explicitly specialize 
    //  the template because the functions are not implemented inline)
    template struct SAGA_STREAM_PACKAGE_EXPORT monitorable<stream::stream>;
  }

  namespace adaptors
  {
    saga::stream::state stream_state_value_to_enum(std::string const& val)
    {
      if (val == saga::stream::attributes::stream_state_new) 
        return saga::stream::New;

      if (val == saga::stream::attributes::stream_state_open) 
        return saga::stream::Open;

      if (val == saga::stream::attributes::stream_state_closed) 
        return saga::stream::Closed;

      if (val == saga::stream::attributes::stream_state_dropped) 
        return saga::stream::Dropped;

      if (val == saga::stream::attributes::stream_state_error) 
        return saga::stream::Error;

      return saga::stream::Unknown;
    }

    std::string stream_state_enum_to_value(int s)
    {
      switch(s) {
      case saga::stream::New:
        return saga::stream::attributes::stream_state_new;

      case saga::stream::Open:
        return saga::stream::attributes::stream_state_open;

      case saga::stream::Closed:
        return saga::stream::attributes::stream_state_closed;

      case saga::stream::Dropped:
        return saga::stream::attributes::stream_state_dropped;

      case saga::stream::Error:
        return saga::stream::attributes::stream_state_error;

      default:
      case saga::stream::Unknown:
        break;
      }
      return saga::stream::attributes::stream_state_unknown;
    }
  }

///////////////////////////////////////////////////////////////////////////////////
}
#endif // SAGA_DEBUG

