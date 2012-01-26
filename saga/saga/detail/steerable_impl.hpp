//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_SAGA_DETAIL_STEERABLE_IMPL_HPP
#define SAGA_SAGA_DETAIL_STEERABLE_IMPL_HPP

// include stl
#include <vector>
#include <string>

#include <saga/saga/metric.hpp>
#include <saga/impl/exception.hpp>

// include dependent spec sections
#include <saga/saga/detail/steerable.hpp>
#include <saga/impl/engine/monitorable.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable : 4251 4231 4660)
#endif

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace detail
{
    // the steerable API interface is implemented by this template

    // metric handling
    template <typename Derived>
    inline bool
    steerable<Derived>::add_metric (saga::metric m) 
    {
        if (!derived().is_impl_valid()) {
            SAGA_THROW("The object has not been properly initialized.",
                saga::IncorrectState);
        }
        return derived().get_impl()->get_steerable()->add_metric(m);
    }
    
    template <typename Derived>
    inline void
    steerable<Derived>::remove_metric (std::string name)
    {
        if (!derived().is_impl_valid()) {
            SAGA_THROW("The object has not been properly initialized.",
                saga::IncorrectState);
        }
        derived().get_impl()->get_steerable()->remove_metric(name);
    }

    template <typename Derived>
    inline void 
    steerable<Derived>::fire_metric(std::string name)
    {
        if (!derived().is_impl_valid()) {
            SAGA_THROW("The object has not been properly initialized.",
                saga::IncorrectState);
        }
        derived().get_impl()->get_steerable()->fire_metric(name);
    }
  
///////////////////////////////////////////////////////////////////////////////
}}   // namespace saga::detail

#endif // SAGA_SAGA_DETAIL_STEERABLE_IMPL_HPP

