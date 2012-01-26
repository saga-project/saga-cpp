//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_IMPL_ENGINE_MONITORABLE_HPP
#define SAGA_IMPL_ENGINE_MONITORABLE_HPP

#include <boost/thread.hpp>

// include stl
#include <map>
#include <vector>
#include <string>

#include <saga/saga/monitorable.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable : 4251 4231 4660)
#endif

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl {

    ///////////////////////////////////////////////////////////////////////////
    class SAGA_EXPORT monitorable
    {
    protected:
        typedef boost::recursive_mutex mutex_type;
        mutable mutex_type mtx_;

        /*! holds the metrics for this object */
        std::vector<std::pair<saga::metric, bool> > metrics;

    public:
        bool has_metric(saga::metric m) const;
        bool has_metric(std::string const& name) const;

    // initialization
        void add_metric_to_metrics(saga::metric m, bool predefined = true);
        void add_metrics_to_metrics(std::vector<saga::metric> const& m,
            bool predefined = true);

        typedef saga::monitorable::cookie_handle cookie_handle;
        typedef std::vector<std::pair<saga::metric, bool> > metric_list_type;

        monitorable();
        ~monitorable();

        // introspection
        std::vector<saga::metric> list_metrics (void) const;
        saga::metric get_metric (std::string name) const;

        // callback handling
        cookie_handle add_callback(std::string name, saga::callback f);
        void remove_callback(std::string name, cookie_handle cookie);
    };

    ///////////////////////////////////////////////////////////////////////////
    class SAGA_EXPORT steerable
    :   public impl::monitorable
    {
    public:
        steerable();
        ~steerable();

        bool add_metric (saga::metric m);
        void remove_metric (std::string name);
        void fire_metric (std::string name);
    };
    
///////////////////////////////////////////////////////////////////////////////
}} // namespace saga::impl

#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

#endif // SAGA_IMPL_ENGINE_MONITORABLE_HPP

