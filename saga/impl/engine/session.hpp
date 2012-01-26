//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005 Michel Zandstra (michelzandstra@gmail.com)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_IMPL_ENGINE_SESSION_HPP
#define SAGA_IMPL_ENGINE_SESSION_HPP

#include <list>

#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <saga/impl/config.hpp>
#include <saga/impl/object.hpp>
#include <saga/impl/engine/engine.hpp>
#include <saga/impl/engine/manage_workitems.hpp>

#include <saga/saga/context.hpp>
#include <saga/saga/uuid.hpp>
#include <saga/saga/adaptors/register_workitem.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl 
{
    // forward declaration only
    struct base_context_list;

#if defined(__AIX__) && defined(__GNUC__)     // see comment in saga/impl_base.hpp
    struct aix_hack
    {
        aix_hack(bool is_default)
          : global_di_called_(is_default)
        {
            if (is_default)
                _GLOBAL__DI(); 
        }
        ~aix_hack()
        {
            if (global_di_called_)
                _GLOBAL__DD(); 
        }

        bool global_di_called_;
    };
#else
    struct aix_hack
    {
        aix_hack(bool is_default) {}
        ~aix_hack() {}
    };
#endif

    /**
    * Session: provides the functionality of a session handle.
    *
    * Multiple context's can be added to the session handle. Session 
    * also handles the shared adaptor data of the session, and 
    * contains an instance of the saga::engine that is used by 
    * the proxy classes.
    *
    * @see Saga API, adaptor_data/instance_data, engine
    */
    class session : public saga::impl::object
    {
    private:
        typedef v1_0::preference_type preference_type;
        typedef v1_0::cpi cpi;

        /*! the list of contexts */
        mutable base_context_list* contexts_;
        mutable v1_0::preference_type preferences_;
        mutable bool has_preferences_;
        bool is_default_;
        bool is_opened_;

        aix_hack global_di_aix_;

        /*! the instance of the saga::engine */
        mutable engine engine_;

        /*! the manager for registered work items */
        manage_workitems workitems_;

    public:
        /**
        * Constructor / Destructor
        */
        session (bool is_default = false) 
          : object(saga::object::Session), 
            contexts_(NULL), has_preferences_(false), 
            is_default_(is_default), is_opened_(true), 
            global_di_aix_(is_default)
        {}
        ~session (void);

        // saga::object interface
        saga::object clone() const; 

        // test if this is the a default session
        SAGA_EXPORT bool is_default_session() const
        {
            return is_default_;
        }

        SAGA_EXPORT void add_proto_context (
            std::vector<std::pair<std::string, std::string> > const& entries);

        SAGA_EXPORT void add_proto_context (
            std::vector<std::pair<std::string, std::string> > const& entries,
            std::vector<std::pair<std::string, std::vector<std::string> > > const& vector_entries);

        /**
        * Returns the context attached to the session
        *
        * @return  the context list
        */
        std::vector<saga::context> list_contexts() const;

        /**
        * Closes the object.
        *
        * @param timeout value
        */
        void close (double timeout = 0.0);

        /**
        * Removes a context from the context list
        *
        * @param the context to be added
        */
        SAGA_EXPORT void remove_context (saga::context const& c);

        /**
        * Adds the context to the context list
        *
        * @param the context to be added
        */
        SAGA_EXPORT void add_context (saga::context const& c);

        // release all contexts stored in the context list
        // this is a helper function called during shutdown to break the 
        // circular dependency of the session and the held contexts
        void release_contexts();

        /**
        * Creates an adaptor object implementing the interface
        *
        * @param cpi_name   a string specifying the interface for which an object 
        *                   is needed
        * @param op_name    a description method wanted from the adaptor
        * @param prefs      a description table describing the adaptor needed
        *
        * @return a pointer of type T* to the created object, or NULL of no suitable 
        *         adaptor could be found
        *
        * @see saga::adaptor_selector::select_adaptor
        */
        v1_0::cpi* get_adaptor (std::string     const & cpi_name,
                                std::string     const & op_name, 
                                preference_type const & prefs,
                                proxy                 * proxy_, 
                                adaptor_selector::adaptor_info_list_type & no_no_list,
                                v1_0::op_info         & oi) const
        {
            if (!is_opened_)
            {
                SAGA_THROW("get_adaptor: session has been closed", 
                    saga::IncorrectState)
                return NULL;
            }
            if (!engine_.has_been_initialized()) 
            {
                mutex_type::scoped_lock lock(mtx_);
                engine_.late_initialize(this);
            }
            return engine_.get_adaptor (cpi_name, op_name, prefs, no_no_list, proxy_, oi);
        }

        /**
        * Tests, whether a given adaptor object implements the required interface
        *
        * @param adaptor    a pointer to the adaptor to test
        * @param cpi_name   a string specifying the interface for which an object 
        *                   is needed
        * @param op_name    a description method wanted from the adaptor
        * @param prefs      a description table describing the adaptor needed
        *
        * @return a pointer of type T* to the created object, or NULL of no suitable 
        *         adaptor could be found
        *
        * @see saga::adaptor_selector::select_adaptor
        */
        bool test_adaptor (v1_0::cpi const* adaptor, 
            std::string const& cpi_name, std::string const& op_name,
            preference_type const& prefs, v1_0::op_info& oi) const
        {
            if (!is_opened_)
            {
                SAGA_THROW("test_adaptor: session has been closed", 
                    saga::IncorrectState)
                return false;
            }

            if (!engine_.has_been_initialized()) 
            {
                mutex_type::scoped_lock lock(mtx_);
                engine_.late_initialize(this);
            }
            return engine_.test_adaptor(adaptor, cpi_name, op_name, prefs, oi);
        }

        // return the name of the adaptor with the given adaptor id
        std::string get_adaptor_name(saga::uuid id) const
        {
            if (!is_opened_)
            {
                SAGA_THROW("get_adaptor_name: session has been closed", 
                    saga::IncorrectState)
                return "";
            }
            if (!engine_.has_been_initialized()) 
            {
                mutex_type::scoped_lock lock(mtx_);
                engine_.late_initialize(this);
            }
            return engine_.get_adaptor_name(id);
        }

        // fill context related preferences from this session instance
        v1_0::preference_type const& get_preferences() const;

        ///////////////////////////////////////////////////////////////////////
        // work item registration
        adaptors::workitem_cookie_handle register_workitem(
            adaptors::workitem_function f, adaptors::time_type const&);
        adaptors::workitem_cookie_handle register_workitem(
            adaptors::workitem_function f, adaptors::duration_type const&);
        void unregister_workitem(adaptors::workitem_cookie_handle h);

        ///////////////////////////////////////////////////////////////////////
        // logging support
        bool module_needs_logging(char const* modulename) const;

    private:
        friend class boost::serialization::access;

        void save(boost::archive::text_oarchive& ar, const unsigned int version) const;
        void load(boost::archive::text_iarchive & ar, const unsigned int version);

        BOOST_SERIALIZATION_SPLIT_MEMBER();
    }; // class session

///////////////////////////////////////////////////////////////////////////////
}}  // namespace saga::impl

#endif // SAGA_IMPL_ENGINE_SESSION_HPP

