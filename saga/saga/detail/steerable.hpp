//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_SAGA_DETAIL_STEERABLE_HPP
#define SAGA_SAGA_DETAIL_STEERABLE_HPP

// include stl
#include <vector>
#include <string>

// include dependent spec sections
#include <saga/saga/util.hpp>
#include <saga/saga/base.hpp>
#include <saga/saga/monitorable.hpp>
#include <saga/saga/detail/monitorable.hpp>

// For symbol import/export macros
#include <saga/saga-defs.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable : 4251 4231 4660)
#endif

///////////////////////////////////////////////////////////////////////////////
// for modules not belonging to the engine attributes should not be exported
#if !defined (SAGA_CREATE_PREPROCESSED_FILES) && !defined(SAGA_EXPORT_STEERABLE)

# if defined(SAGA_ENGINE_EXPORTS) || defined(SAGA_STEERABLE_EXPORTS)
#   define SAGA_EXPORT_STEERABLE SAGA_SYMBOL_EXPORT
# elif !defined (SAGA_NO_EXPORT_STEERABLE) && !defined(BUILD_SAGA_LITE)
#   define SAGA_EXPORT_STEERABLE SAGA_SYMBOL_IMPORT
# else
#   define SAGA_EXPORT_STEERABLE /* empty */
# endif

#endif // !SAGA_CREATE_PREPROCESSED_FILES

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace detail
    {
        /*! \brief   The SAGA %steerable interface.
         *
         *  \details    TODO .
         */
        template <typename Derived>
        struct SAGA_EXPORT_STEERABLE steerable
        {
            ///@cond
            Derived& derived() 
            { return static_cast<Derived&>(*this); }
            Derived const& derived() const 
            { return static_cast<Derived const&>(*this); }
            ///@endcond
            
            /*! \brief     Adds a %metric to this %object instance.
             *
             *  \param     m The %metric to add
             *
             *  \details    TODO .
             */
            bool add_metric (saga::metric m);
            
            /*! \brief     Removes a %metric from this %object instance.
             *
             *  \param     name The metric's name
             *
             *  \details    TODO .
             */
            void remove_metric (std::string name);

            /*! \brief     Push a new %metric value to the backend.
             *
             *  \param     name The %metric to be fired 
             *
             *  \details    TODO .
             */
            void fire_metric (std::string name);
        };
        
        ///////////////////////////////////////////////////////////////////////////////
    }}   // namespace saga::detail

#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

#endif // SAGA_SAGA_DETAIL_STEERABLE_HPP

