//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/version.hpp>
#include <boost/assert.hpp>

#include <saga/saga/util.hpp>
#include <saga/saga/adaptors/task_declaration.hpp>
#include <saga/impl/runtime.hpp>

#include <saga/impl/engine/session.hpp>
#include <saga/impl/engine/proxy.hpp>

#include <saga/saga/adaptors/instance_data_base.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl { 

    ///////////////////////////////////////////////////////////////////////////
    // handle API object instance data
    TR1::shared_ptr<adaptors::instance_data_base> 
    proxy::register_instance_data (
        TR1::shared_ptr <adaptors::instance_data_base> o) 
    {
        {
            mutex_type::scoped_lock lock(instance_data_mtx_);
            instance_data_ = o;
        }
#if BOOST_VERSION < 103500
        boost::detail::thread::lock_ops<mutex_type>::lock(o->get_mutex());
#else
        o->get_mutex().lock();
#endif
        return o;
    }

    void proxy::release_instance_data (void) 
    {
        mutex_type::scoped_lock lock(instance_data_mtx_);
        instance_data_.reset();
    }

    TR1::shared_ptr<adaptors::instance_data_base>
    proxy::check_out_instance_data (void)
    {
        TR1::shared_ptr<adaptors::instance_data_base> item;
        {
            mutex_type::scoped_lock lock(instance_data_mtx_);
            item = instance_data_;
        }
        if (!item) 
        {
            SAGA_THROW("instance_data: could not check_out instance_data.",
                       saga::DoesNotExist);
        }
#if BOOST_VERSION < 103500
        boost::detail::thread::lock_ops<mutex_type>::lock(
            item->get_mutex());
#else
        item->get_mutex().lock();
#endif
        return item;
    }

    void proxy::check_in_instance_data (void)
    {
        TR1::shared_ptr<adaptors::instance_data_base> item;
        {
            mutex_type::scoped_lock lock(instance_data_mtx_);
            item = instance_data_;
        }
        if (!item) 
        {
            SAGA_THROW("instance_data: could not check_in instance_data.",
                saga::DoesNotExist);
        }
#if BOOST_VERSION < 103500
        boost::detail::thread::lock_ops<mutex_type>::unlock(
            item->get_mutex());
#else
        item->get_mutex().unlock();
#endif
    }

    ///////////////////////////////////////////////////////////////////////////
    TR1::shared_ptr<proxy> proxy::shared_from_this (void)
    {
        typedef saga::impl::object base_type;

        return TR1::static_pointer_cast<proxy>(
            this->base_type::shared_from_this());
    }

    TR1::shared_ptr<proxy const> proxy::shared_from_this (void) const
    {
        typedef saga::impl::object base_type;
        
        return TR1::static_pointer_cast<proxy const>(
            this->base_type::shared_from_this());
    }

    ///////////////////////////////////////////////////////////////////////////
    bool proxy::get_adaptor (std::string     const & ops_cpi,  
                             std::string     const & op_name, 
                             preference_type const & prefs,  
                             adaptor_selector::adaptor_info_list_type &no_no_list, 
                             v1_0::op_info         & oi)
    {
#if BOOST_VERSION >= 103900
        TR1::shared_ptr<impl::object> this_;
        try {
            this_ = this->shared_from_this();
        }
        catch(TR1::bad_weak_ptr const&) {
            return false;
        }
#else
        if (0 == this->_internal_weak_this.use_count())
            return false;
#endif

        // ask session for a new CPI instance
        v1_0::cpi* cpi = runtime::get_impl(session_)->
            get_adaptor(ops_cpi, op_name, prefs, this, no_no_list, oi);

        if (NULL != cpi) 
        {
            cpis_.add_new(cpi);
            return true;
        }
        
        return false;
    }

    bool proxy::test_adaptor(cpi_type cpi, std::string const& cpi_name, 
        std::string const& op_name, preference_type const& prefs, 
        v1_0::op_info& oi)
    {
#if BOOST_VERSION >= 103900
        TR1::shared_ptr<impl::object> this_;
        try {
            this_ = this->shared_from_this();
        }
        catch(TR1::bad_weak_ptr const&) {
            return false;
        }
#else
        if (0 == this->_internal_weak_this.use_count())
            return false;
#endif

        //  ask adaptor whether the requested mode is supported for the given 
        //  CPI call
        return runtime::get_impl(session_)->
            test_adaptor(cpi.get(), cpi_name, op_name, prefs, oi);
    }

    run_mode proxy::test_adaptor_helper(cpi_type cpi,
        std::string const& cpi_name, std::string const& call_name,   
        preference_type const& prefs, bool is_sync, v1_0::op_info& oi)
    {
        if (is_sync) {
            if (test_adaptor (cpi, cpi_name, "sync_"  + call_name, prefs, oi)) 
                return (Sync_Sync);  

            if (test_adaptor (cpi, cpi_name, "async_" + call_name, prefs, oi)) 
                return (Sync_Async); 
        }
        else {
            if (test_adaptor(cpi, cpi_name, "async_" + call_name, prefs, oi))
                return (Async_Async); 

            if (test_adaptor(cpi, cpi_name, "sync_"  + call_name, prefs, oi))
                return (Async_Sync); 
        }

        saga::uuid id (cpi->get_adaptor_info().get_adaptor_id());

        SAGA_THROW(std::string("Adaptor '") + 
            runtime::get_impl(session_)->get_adaptor_name(id) + 
            "' doesn't implement " + cpi_name + "::" + call_name, 
            saga::adaptors::NoAdaptor);
        return Unknown;
    }

    bool proxy::initcpi(std::string const& cpi_name)
    {
        preference_type const& prefs(detail::get_preferences(get_session()));
        adaptor_selector::adaptor_info_list_type no_no_list;
        exception_list exceptions;
        v1_0::op_info oi ("__init__");
        while (true) {
            try {
                // create new CPI instance (remember, this is called from the 
                // inside of the constructor of the API object)
                mutex_type::scoped_lock lock(mtx_);
                run_mode mode = get_adaptor_for_init(cpi_name, 
                    prefs, no_no_list, oi);
                BOOST_ASSERT(!cpis_.empty());
                BOOST_ASSERT(Sync_Sync == mode);
                return true;
            }
            catch (saga::exception const& e) {
                exceptions.add(e);            // save this exception

                // throw, if we don't find anything
                if (e.get_error() == (saga::error)saga::adaptors::NoAdaptor)
                    SAGA_THROW_PLAIN_LIST(this, exceptions);
            }
        }
    }

    run_mode proxy::get_adaptor_helper(std::string const& cpi_name,   
        std::string const& call_name, preference_type const& prefs,   
        bool is_sync, adaptor_selector::adaptor_info_list_type &no_no_list, 
        v1_0::op_info& oi)
    {
        // find a new adaptor matching the requirements
        if (is_sync) {
            if (get_adaptor (cpi_name, "sync_"  + call_name, 
                prefs, no_no_list, oi)) 
            {
                return (Sync_Sync);  
            }
            if (get_adaptor (cpi_name, "async_" + call_name, 
                prefs, no_no_list, oi)) 
            {
                return (Sync_Async); 
            }
        }
        else {
            if (get_adaptor (cpi_name, "async_" + call_name, 
                prefs, no_no_list, oi))
            {
                return (Async_Async); 
            }
            if (get_adaptor (cpi_name, "sync_"  + call_name, 
                prefs, no_no_list, oi))
            {
                return (Async_Sync); 
            }
        }

        SAGA_THROW("No (other) adaptor implements " + cpi_name + "::" + 
            call_name, saga::adaptors::NoAdaptor);
        return Unknown;
    }

    run_mode proxy::get_adaptor_for_init(std::string const& cpi_name, 
        preference_type const& prefs,
        adaptor_selector::adaptor_info_list_type &no_no_list, 
        v1_0::op_info& oi)
    {
        // find a new adaptor matching the requirements
        if (get_adaptor(cpi_name, "__init__", prefs, no_no_list, oi)) 
            return (Sync_Sync);  

        SAGA_THROW("No adaptor succeeded in executing constructor for " + 
            cpi_name, saga::adaptors::NoAdaptor);
        return Unknown;
    }

    run_mode proxy::select_run_mode (std::string     const & cpi_name,   
                                     std::string     const & call_name, 
                                     preference_type const & prefs,   
                                     bool                    is_sync, 
                                     adaptor_selector::adaptor_info_list_type & no_no_list, 
                                     v1_0::op_info         & oi)
    {
        // first try all the cpi instances we have loaded already
        exception_list exceptions;

        cpi_list::iterator end = cpis_.end ();

        for (cpi_list::iterator it = cpis_.begin(); it != end; ++it)
        {
            v1_0::cpi_info info;

            try {
                // retrieve info to be able to store it in no_no_list later
                info = (*it)->get_adaptor_info();

                if (std::find_if (no_no_list.begin(), 
                                  no_no_list.end(), 
                                  v1_0::same_cpi(info)) != no_no_list.end())
                {
                    continue;     // this cpi was checked already -- ignore this time
                }
                
                // ask this cpi instance if it is able to handle the request.
                // throws if not.
                run_mode mode = test_adaptor_helper(*it, cpi_name, call_name, 
                                                    prefs, is_sync, oi);    
                cpis_.make_current(it);
                return mode;
            }
            catch (saga::exception const& e) {
                exceptions.add(e);            // save this exception

                // throw, if we don't find anything or if this cpi has the 
                // Fixme flag set
                if (e.get_error() == (saga::error)saga::adaptors::NoAdaptorInfo ||
                    ((*it)->get_flags() & v1_0::cpi::Fixme))
                {
                    SAGA_THROW_PLAIN_LIST(this, exceptions);
                }
                
                no_no_list.push_back(info);   // just try the next adaptor
            }
        }

        // then try to instantiate a new instance
        try {
            return get_adaptor_helper(cpi_name, call_name, prefs, is_sync, 
                no_no_list, oi);
        }
        catch (saga::exception const& e) {
            exceptions.add(e);
            SAGA_THROW_PLAIN_LIST(this, exceptions);
        }
        return Unknown;
    }

    saga::object::type proxy::get_cpitype() const
    {
        mutex_type::scoped_lock lock(mtx_);
        if (cpis_.empty())
            return (saga::object::type)v1_0::cpi::UnknownCPI;
        return (saga::object::type)(*cpis_.begin())->get_type();
    }
    
///////////////////////////////////////////////////////////////////////////////
}} // namespace saga::impl

