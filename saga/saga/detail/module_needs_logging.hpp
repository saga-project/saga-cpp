//  Copyright (c) 2005-2011 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_DETAIL_MODULE_NEEDS_LOGGING_HPP
#define SAGA_DETAIL_MODULE_NEEDS_LOGGING_HPP

#include <saga/saga/util.hpp>
#include <saga/saga/export_definitions.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable: 4251 4231 4660)
#endif

namespace saga 
{ 
    // forward declaration
    class session;

    namespace detail 
    {
        ///@cond
        // the default session as singleton, and the no-session singleton
        SAGA_EXPORT session& get_the_session();
        ///@endcond

        ///////////////////////////////////////////////////////////////////////
        // This is the main API for adaptors query whether log messages from 
        // the given module name (adaptor name) need to be processed.
        SAGA_EXPORT bool module_needs_logging(char const* module_name, 
            saga::session const& s = saga::detail::get_the_session());
    }
}

#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

#endif


