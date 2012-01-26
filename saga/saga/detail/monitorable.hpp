//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_SAGA_DETAIL_MONITORABLE_HPP
#define SAGA_SAGA_DETAIL_MONITORABLE_HPP

// include stl
#include <vector>
#include <string>

// include dependent spec sections
#include <saga/saga/util.hpp>
#include <saga/saga/base.hpp>
#include <saga/saga/monitorable.hpp>

// For symbol import/export macros
#include <saga/saga-defs.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable : 4251 4231 4660)
#endif

///////////////////////////////////////////////////////////////////////////////
// for modules not belonging to the engine attributes should not be exported
#if !defined (SAGA_CREATE_PREPROCESSED_FILES) && !defined(SAGA_EXPORT_MONITORABLE)

# if defined(SAGA_ENGINE_EXPORTS) || defined(SAGA_MONITORABLE_EXPORTS)
#   define SAGA_EXPORT_MONITORABLE SAGA_SYMBOL_EXPORT
# elif !defined (SAGA_NO_IMPORT_MONITORABLE) && !defined(BUILD_SAGA_LITE)
#   define SAGA_EXPORT_MONITORABLE SAGA_SYMBOL_IMPORT
# else
#   define SAGA_EXPORT_MONITORABLE /* empty */
# endif

#endif // !SAGA_CREATE_PREPROCESSED_FILES

///////////////////////////////////////////////////////////////////////////////
namespace saga 
{
    namespace detail
    {
        
        /*! \brief   The SAGA %monitorable interface.
         *
         *  \details    TODO .
         */
        template <typename Derived>
        struct SAGA_EXPORT_MONITORABLE monitorable 
        {
            /*! \brief   A %cookie_handle is a unique identifier for a callback.
             *
             *  \details    TODO .
             */
            typedef saga::monitorable::cookie_handle cookie_handle;
            
            ///@cond
            void init (std::vector <saga::metric> const & metrics);
            
            Derived & derived (void) 
            { 
                return static_cast <Derived&> (*this); 
            }
            
            Derived const & derived (void) const 
            { 
                return static_cast <Derived const &> (*this); 
            }
            ///@endcond
            
            /*! \brief     Lists all %metrics assiciated with this %object instance.
             *
             *  \return    A vector of available. %metrics
             *
             *  \details    TODO .
             */
            std::vector <saga::metric> list_metrics (void) const;
            
            /*! \brief     Returns a %metric associated with this %object instance.
             *
             *  \param     name The metric's name
             *  \return    The %metric identified by name
             *
             *  \details    TODO .
             */
            saga::metric get_metric (std::string name) const;
            
            /*! \brief Adds a callback to this %object instance.
             *
             *  \param     name The new callback's name
             *  \param     cb The callback %object
             *  \return    A unique handle to the new callback
             *
             *  \details    TODO .
             */
            cookie_handle add_callback (std::string name, saga::callback cb);
            
            /*! \brief     Removes a callback from this %object instance.
             *
             *  \param     name The callback's name
             *  \param     cookie The callback's unique handle
             *
             *  \details    TODO .
             */
            void remove_callback (std::string name, cookie_handle cookie);
        };
    } 
} // namespace saga::detail
///////////////////////////////////////////////////////////////////////////////

#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

#endif // SAGA_SAGA_DETAIL_MONITORABLE_HPP

