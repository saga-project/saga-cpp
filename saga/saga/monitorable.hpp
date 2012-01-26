//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2007 Ole Weidner (oweidner@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_MONITORABLE_HPP
#define SAGA_MONITORABLE_HPP

// include STL
#include <string>
#include <vector>

// include dependent spec sections
#include <saga/saga/util.hpp>
#include <saga/saga/base.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable : 4251 4231 4660)
#endif

///////////////////////////////////////////////////////////////////////////////
namespace saga
{
    /// @cond
    /** These methods are not within API scope */
    typedef bool callback_function_type(saga::object, saga::metric, saga::context);
    typedef TR1::function<callback_function_type> callback;
    /// @endcond
    
    /*! \brief The monitorable interface is implemented by those SAGA objects
     * which can be monitored, i.e. which have one or more associated metrics.  
     *
     *   The interface allows introspection of these metrics, and allows to add
     * callbacks to these metrics which get called if these metrics change. 
     * Several methods of this interface reflect similar methods on the metric class
     * - the additional string argument name identifies the metric these methods act upon.
     */
    class SAGA_EXPORT monitorable
    {
    protected:
        /// @cond
        /** These methods are not within API scope */
        TR1::shared_ptr <saga::impl::object> impl_;

        saga::impl::monitorable* get_monitorable (void);
        saga::impl::monitorable const* get_monitorable (void) const;
        /// @endcond

    public:
        /*! \brief Brief %description starts here     
         *
         *
         */
        typedef unsigned int cookie_handle;
        
        /*! \brief Brief %description starts here     
         *
         *
         */
        monitorable (saga::object rhs);
        
        /*! \brief Brief %description starts here     
         *
         *
         */
        explicit monitorable (saga::impl::object *impl);    // adaptor interface
    
        /*! \brief Brief %description starts here     
         *
         *
         */
        virtual ~monitorable (void);
        
        /*! \brief Brief %description starts here     
         *
         *
         */
        virtual std::vector<saga::metric> list_metrics (void) const;
        
        /*! \brief Returns a metric instance, identified by name.
         * \param name Name of the metric to be returned.    
         * \return Metric instance identified by name
         *
         *
         */
        virtual saga::metric get_metric (std::string name) const;

        /*! \brief Adds a callback to the specified metric.     
         * \param name Identifies the metric to which the cb is to be added 
         * \param cb Reference to callback class instance to be registered 
         * \return Handle for callback removal
         *
         *
         */
        virtual cookie_handle add_callback (std::string name, saga::callback cb);
        
        /*! \brief Removes a callback from the specified metric 
         * \param name Identifies the metric for which cb is to be removed 
         * \param cookie Identifies the cb to be removed.
         *
         *
         */
        virtual void remove_callback (std::string name, cookie_handle  cookie);
    };

    /*! \brief The steerable interface is implemented by saga objects which can
     * be steered, i.e. which have writable metrics, and which might allow to add
     * new metrics.     
     *
     *   Steerable objects also implement the monitorable interface. 
     * The method add_metric() allows to implement steerable applications. In
     * particular, the saga::self object is steerable, and allows to add metrics. 
     */
    class SAGA_EXPORT steerable
    :   public monitorable
    {
    protected:
        /// @cond
        /** These methods are not within API scope */
        saga::impl::steerable* get_steerable (void);
        saga::impl::steerable const* get_steerable (void) const;
        /// @endcond

    public:
        /*! \brief Brief %description starts here     
         *
         *
         */
        steerable (saga::object rhs);
        
        /*! \brief Brief %description starts here     
         *
         *
         */
        explicit steerable (saga::impl::object *impl);    // adaptor interface

        /*! \brief Brief %description starts here     
         *
         *
         */
        virtual ~steerable (void);
        
        /*! \brief Adds a metric instance to the application instance.
         * \param metric Metric to be added.
         * \return Indicates the success.
         *
         *
         */
        virtual bool add_metric (saga::metric m);
        
        /*! \brief Removes a metric instance.    
         * \param name Identifies the metric to be removed
         *
         *
         */
        virtual void remove_metric (std::string name);
        
        /*! \brief Pushes a new metric value to the backend 
         * \param name Identifies the metric to be fired.       
         *
         *
         */
        virtual void fire_metric (std::string name);
    };

///////////////////////////////////////////////////////////////////////////////
} // namespace saga
                                                                             
#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

#endif // SAGA_MONITORABLE_HPP

