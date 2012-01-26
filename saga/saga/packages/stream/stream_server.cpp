#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/server.cpp")
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

//  this is needed in every file including detail/monitorable_impl.hpp and not 
//  belonging to the engine
#define SAGA_NO_IMPORT_MONITORABLE
#define SAGA_NO_IMPORT_PERMISSIONS

// include the package we implement
#include <saga/saga/context.hpp>
#include <saga/saga/stream.hpp>
#include <saga/impl/stream.hpp>
#include <saga/saga/detail/call.hpp>

#include <saga/saga/detail/monitorable_impl.hpp>
#include <saga/saga/detail/permissions_impl.hpp>

#ifdef SAGA_DEBUG
#include <saga/saga/packages/stream/preprocessed/server.cpp>
#else

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/server.cpp")
#endif
///////////////////////////////////////////////////////////////////////////////
namespace saga 
{
    namespace stream {
    ///////////////////////////////////////////////////////////////////////////
    namespace metrics
    {
        /// @cond
        saga::metrics::init_data const server_metric_data[] = 
        {
            {
                saga::stream::metrics::server_clientconnect,
                "Metric fires if if a client connects.",
                saga::attributes::metric_mode_readonly,
                "1", 
                saga::attributes::metric_type_trigger,
                "0"
            }
        };
        /// @endcond
    }


    server::server (session const& s, saga::url url)
      : saga::object (new saga::impl::server (s, url))
    {
        init_metrics();
        this->saga::object::get_impl()->init();
    }

    server::server (saga::url url)
      : saga::object (new saga::impl::server (detail::get_the_session(), url))
    {
        init_metrics();
        this->saga::object::get_impl()->init();
    }

    server::server (void)
      : saga::object (new saga::impl::server (detail::get_the_session(), saga::url()))
    {
        init_metrics();
        this->saga::object::get_impl()->init();
    }

    server::server (saga::impl::server* impl)
      : saga::object (impl)
    {
        init_metrics();
    }

    server::server (int)
    {
    }

    void server::init_metrics()
    {
        // initialize metrics
        std::vector<saga::metric> metrics;
        for (unsigned int i = 0; 
             i < sizeof(saga::stream::metrics::server_metric_data)/sizeof(saga::metrics::init_data);
             ++i)
        {
            saga::metrics::init_data const* p = &metrics::server_metric_data[i];
            saga::metric m(*this, p->name, p->description, p->mode, p->unit, 
                p->type, p->value);
            metrics.push_back(m);
        }
        this->monitorable_base_type::init (metrics);
    }
    
    server::~server (void)
    {
    }

    saga::impl::server* server::get_impl() const
    { 
        typedef saga::object base_type;
        return static_cast<saga::impl::server*>(this->base_type::get_impl()); 
    }

    TR1::shared_ptr <saga::impl::server> server::get_impl_sp(void) const
    { 
      // FIXME: this needs documentation
      typedef saga::object base_type;
      return TR1::static_pointer_cast <saga::impl::server>(
          this->base_type::get_impl_sp());
    }

    ///////////////////////////////////////////////////////////////////////////
    server::server (saga::object const& o)
      : saga::object (o)
    {
        if (this->get_type() != saga::object::StreamServer)
        {
            SAGA_THROW("Bad type conversion.", saga::BadParameter);
        }
    }

    server &server::operator= (saga::object const& o)
    {
        return saga::object::operator=(o), *this;
    }

    // factory
    SAGA_CALL_CREATE_IMP_2(server, impl::server, session const&, saga::url)

    // server methods
    SAGA_CALL_IMP_1(server, connect, double)
    SAGA_CALL_IMP_1(server, serve, double)
    SAGA_CALL_CONST_IMP_0(server, get_url)
    SAGA_CALL_IMP_1(server, close, double)

  } // namespace saga

  namespace detail
  {
    /////////////////////////////////////////////////////////////////////////////
    //  implement the monitorable functions (we need to explicitly specialize 
    //  the template because the functions are not implemented inline)
    template struct SAGA_STREAM_PACKAGE_EXPORT saga::detail::monitorable<stream::server>;  

    /////////////////////////////////////////////////////////////////////////////
    //  implement the permissions functions (we need to explicitly specialize 
    //  the template because the functions are not implemented inline)
    template struct SAGA_STREAM_PACKAGE_EXPORT permissions<stream::server>;  

    template struct SAGA_STREAM_PACKAGE_EXPORT permissions_priv<stream::server, task_base::Sync>;
    template struct SAGA_STREAM_PACKAGE_EXPORT permissions_priv<stream::server, task_base::Async>;
    template struct SAGA_STREAM_PACKAGE_EXPORT permissions_priv<stream::server, task_base::Task>;
  }

///////////////////////////////////////////////////////////////////////////////////
}
#endif // SAGA_DEBUG

