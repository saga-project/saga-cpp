//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_SAGA_DETAIL_MONITORABLE_IMPL_HPP
#define SAGA_SAGA_DETAIL_MONITORABLE_IMPL_HPP

// include stl
#include <vector>
#include <string>

#include <saga/saga/metric.hpp>
#include <saga/impl/exception.hpp>

// include dependent spec sections
#include <saga/impl/engine/monitorable.hpp>
#include <saga/saga/detail/monitorable.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable : 4251 4231 4660)
#endif

///////////////////////////////////////////////////////////////////////////////
namespace saga 
{
  namespace detail
  {
    // the monitorable API interface is implemented by this template
    template <typename Derived>
    void 
    monitorable <Derived>::init (std::vector <saga::metric> const & metrics)
    {
        if (!derived().is_impl_valid()) {
            SAGA_THROW("The object has not been properly initialized.",
                saga::IncorrectState);
        }
        derived().get_impl()->get_monitorable()->add_metrics_to_metrics (metrics);
    }

    // introspection
    template <typename Derived>
    inline std::vector <saga::metric> 
    monitorable <Derived>::list_metrics (void) const
    {
        if (!derived().is_impl_valid()) {
            SAGA_THROW("The object has not been properly initialized.",
                saga::IncorrectState);
        }
        return derived().get_impl()->get_monitorable()->list_metrics ();
    }
    
    template <typename Derived>
    inline saga::metric 
    monitorable <Derived>::get_metric (std::string name) const
    {
        if (!derived().is_impl_valid()) {
            SAGA_THROW("The object has not been properly initialized.",
                saga::IncorrectState);
        }
        return derived().get_impl()->get_monitorable()->get_metric (name);
    }

    // callback handling
    template <typename Derived>
    inline saga::monitorable::cookie_handle 
    monitorable <Derived>::add_callback (std::string name, saga::callback cb)
    {
        if (!derived().is_impl_valid()) {
            SAGA_THROW("The object has not been properly initialized.",
                saga::IncorrectState);
        }
        return derived().get_impl()->get_monitorable()->add_callback (name, cb);
    }
    
    template <typename Derived>
    inline void 
    monitorable <Derived>::remove_callback (std::string name, 
        saga::monitorable::cookie_handle cookie)
    {
        if (!derived().is_impl_valid()) {
            SAGA_THROW("The object has not been properly initialized.",
                saga::IncorrectState);
        }
        derived().get_impl()->get_monitorable()->remove_callback (name, cookie);
    }
  
  }
} // namespace saga::detail
///////////////////////////////////////////////////////////////////////////////

#endif // SAGA_SAGA_DETAIL_MONITORABLE_IMPL_HPP

