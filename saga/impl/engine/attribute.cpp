//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <vector>
#include <string>
#include <iostream>

#include <boost/function_output_iterator.hpp>
#include <boost/ref.hpp>

#include <saga/saga/exception.hpp>
#include <saga/saga/task.hpp>
#include <saga/saga/context.hpp>
#include <saga/saga/adaptors/task.hpp>
#include <saga/saga/adaptors/attribute_cpi.hpp>

#include <saga/impl/exception.hpp>
#include <saga/impl/engine/attribute.hpp>
#include <saga/impl/engine/context_base.hpp>
#include <saga/impl/engine/proxy.hpp>
#include <saga/impl/engine/sync_async.hpp>
#include <saga/impl/engine/run_mode_wrapper.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl
{
    ///////////////////////////////////////////////////////////////////////////
    attribute::attribute (void)
    :   cache_only_(false)
    {
    }

    // copy constructor
    attribute::attribute  (attribute const& rhs)
    :   cache_only_(rhs.cache_only_), attributes_(rhs.attributes_)
    {
    }

    bool attribute::is_valid_key(std::string const& key) const
    {
        return attributes_.is_valid_key(key);
    }

    void attribute::init (bool extensible, bool cache_only)
    {
        cache_only_ = cache_only;
        attributes_.init(extensible);
    }

    void attribute::init (strmap_type const& ro_scalar, 
        strmap_type const& rw_scalar, strmap_type const& ro_vector, 
        strmap_type const& rw_vector)
    {
        attributes_.init(ro_scalar, rw_scalar, ro_vector, rw_vector);
    }

    void attribute::init (char const* const* ro_scalar, 
        char const* const* rw_scalar, char const* const* ro_vector, 
        char const* const* rw_vector)
    {
        attributes_.init(ro_scalar, rw_scalar, ro_vector, rw_vector);
    }

    void attribute::init_keynames(strvec_type const& keynames)
    {
        attributes_.init_keynames(keynames);
    }

    void attribute::init_keynames(char const* const* keynames)
    {
        attributes_.init_keynames(keynames);
    }

    bool attribute::attributes_extensible (void) const
    {
        return attributes_.attributes_extensible();
    }

    attribute attribute::clone() const
    {
        return *this;
    }
    
    bool attribute::is_equal(attribute const& rhs) const
    {
        return attributes_.is_equal(rhs.attributes_);
    }

    ///////////////////////////////////////////////////////////////////////////
    saga::impl::proxy* attribute::get_proxy()
    {
        SAGA_THROW(
            "This object does not expose the SAGA proxy interface", 
            saga::NotImplemented);
        return NULL;
    }

    saga::impl::proxy const* attribute::get_proxy() const
    {
        SAGA_THROW(
            "This object does not expose the SAGA proxy interface", 
            saga::NotImplemented);
        return NULL;
    }

    ///////////////////////////////////////////////////////////////////////////
    //  API functions, these get routed through a adaptor (if one exists) and
    //  otherwise use the implementation of the associated attribute_cache
    saga::task 
    attribute::get_attribute (std::string key, bool sync) 
    {
        if (!cache_only_) {
            typedef saga::impl::v1_0::attribute_cpi attribute_cpi;

            return saga::impl::execute_sync_async(get_proxy(),
                "attribute_cpi", "get_attribute", 
                "attribute::get_attribute", sync, 
                &attribute_cpi::sync_get_attribute, 
                &attribute_cpi::async_get_attribute, key);
        }
        saga::task t(saga::task::Done);
        t.get_result<std::string>() = attributes_.get_attribute(key);
        return t;
    }

    std::string attribute::get_attribute_sync(std::string const& key)
    {
        if (!cache_only_) 
            return attribute_interface::get_attribute_sync(key);
        return attributes_.get_attribute(key);
    }

    ///////////////////////////////////////////////////////////////////////////
    saga::task 
    attribute::set_attribute (std::string key, std::string val, bool sync)
    {
        if (!cache_only_) {
            typedef saga::impl::v1_0::attribute_cpi attribute_cpi;

            return saga::impl::execute_sync_async(get_proxy(),
                "attribute_cpi", "set_attribute", 
                "attribute::set_attribute", sync, 
                &attribute_cpi::sync_set_attribute, 
                &attribute_cpi::async_set_attribute, 
                key, val);
        }
        attributes_.set_attribute(key, val);
        return saga::task(saga::task::Done);
    }

    void attribute::set_attribute_sync(std::string const& key, 
        std::string const& val)
    {
        if (!cache_only_) 
            attribute_interface::set_attribute_sync(key, val);
        else
            attributes_.set_attribute(key, val);
    }

    ///////////////////////////////////////////////////////////////////////////
    saga::task 
    attribute::set_vector_attribute (std::string key, strvec_type val, bool sync)
    {
        if (!cache_only_) {
            typedef saga::impl::v1_0::attribute_cpi attribute_cpi;

            return saga::impl::execute_sync_async(get_proxy(),
                "attribute_cpi", "set_vector_attribute", 
                "attribute::set_vector_attribute", sync, 
                &attribute_cpi::sync_set_vector_attribute, 
                &attribute_cpi::async_set_vector_attribute, 
                key, val);
        }
        attributes_.set_vector_attribute(key, val);
        return saga::task(saga::task::Done);
    }

    void attribute::set_vector_attribute_sync(std::string const& key, 
        strvec_type const& val)
    {
        if (!cache_only_) 
            attribute_interface::set_vector_attribute_sync(key, val);
        else
            attributes_.set_vector_attribute(key, val);
    }

    ///////////////////////////////////////////////////////////////////////////
    // set/get vector attribute
    saga::task 
    attribute::get_vector_attribute (std::string key, bool sync) 
    {
        if (!cache_only_) {
            typedef saga::impl::v1_0::attribute_cpi attribute_cpi;

            return saga::impl::execute_sync_async(get_proxy(),
                "attribute_cpi", "get_vector_attribute", 
                "attribute::get_vector_attribute", sync, 
                &attribute_cpi::sync_get_vector_attribute, 
                &attribute_cpi::async_get_vector_attribute, 
                key);
        }
        saga::task t (saga::task::Done);
        attributes_.get_vector_attribute(key, t.get_result<attribute::strvec_type>());
        return t;
    }

    void attribute::get_vector_attribute_sync(std::string const& key, 
        strvec_type& val)
    {
        if (!cache_only_) 
            attribute_interface::get_vector_attribute_sync(key, val);
        else
            attributes_.get_vector_attribute(key, val);
    }

    ///////////////////////////////////////////////////////////////////////////
    saga::task 
    attribute::remove_attribute (std::string key, bool sync) 
    {
        if (!cache_only_) {
            typedef saga::impl::v1_0::attribute_cpi attribute_cpi;

            return saga::impl::execute_sync_async(get_proxy(),
                "attribute_cpi", "remove_attribute", 
                "attribute::remove_attribute", sync, 
                &attribute_cpi::sync_remove_attribute, 
                &attribute_cpi::async_remove_attribute, key);
        }
        attributes_.remove_attribute(key);
        return saga::task(saga::task::Done);
    }

    void attribute::remove_attribute_sync(std::string const& key)
    {
        if (!cache_only_) 
            attribute_interface::remove_attribute_sync(key);
        else
            attributes_.remove_attribute(key);
    }

    ///////////////////////////////////////////////////////////////////////////
    // list attribute
    saga::task 
    attribute::list_attributes (bool sync) 
    {
        if (!cache_only_) {
            typedef saga::impl::v1_0::attribute_cpi attribute_cpi;

            return saga::impl::execute_sync_async(get_proxy(),
                "attribute_cpi", "list_attributes", 
                "attribute::list_attributes", sync, 
                &attribute_cpi::sync_list_attributes, 
                &attribute_cpi::async_list_attributes);
        }
        saga::task t (saga::task::Done);
        attributes_.list_attributes(t.get_result<attribute::strvec_type>());
        return t;
    }

    void attribute::list_attributes_sync(strvec_type& retval)
    {
        if (!cache_only_) 
            attribute_interface::list_attributes_sync(retval);
        else
            attributes_.list_attributes(retval);
    }

    ///////////////////////////////////////////////////////////////////////////
    saga::task
    attribute::find_attributes (std::string pattern, bool sync) 
    {
        if (!cache_only_) {
            typedef saga::impl::v1_0::attribute_cpi attribute_cpi;

            return saga::impl::execute_sync_async(get_proxy(),
                "attribute_cpi", "find_attributes", 
                "attribute::find_attributes", sync, 
                &attribute_cpi::sync_find_attributes, 
                &attribute_cpi::async_find_attributes, pattern);
        }
        saga::task t (saga::task::Done);
        attributes_.find_attributes(pattern, t.get_result<attribute::strvec_type>());
        return t;
    }

    void attribute::find_attributes_sync(std::string const& pattern, 
        strvec_type& retval)
    {
        if (!cache_only_) 
            attribute_interface::find_attributes_sync(pattern, retval);
        else
            attributes_.find_attributes(pattern, retval);
    }

    ///////////////////////////////////////////////////////////////////////////
    saga::task 
    attribute::attribute_exists (std::string key, bool sync) 
    {
        if (!cache_only_) {
            typedef saga::impl::v1_0::attribute_cpi attribute_cpi;

            return saga::impl::execute_sync_async(get_proxy(),
                "attribute_cpi", "attribute_exists", 
                "attribute::attribute_exists", sync, 
                &attribute_cpi::sync_attribute_exists, 
                &attribute_cpi::async_attribute_exists, key);
        }
        saga::task t (saga::task::Done);
        t.get_result<bool>() = attributes_.attribute_exists(key);
        return t;
    }

    bool attribute::attribute_exists_sync(std::string const& key)
    {
        if (!cache_only_) 
            return attribute_interface::attribute_exists_sync(key);
        return attributes_.attribute_exists(key);
    }

    ///////////////////////////////////////////////////////////////////////////
    saga::task 
    attribute::attribute_is_readonly (std::string key, bool sync) 
    {
        if (!cache_only_) {
            typedef saga::impl::v1_0::attribute_cpi attribute_cpi;

            return saga::impl::execute_sync_async(get_proxy(),
                "attribute_cpi", "attribute_is_readonly", 
                "attribute::attribute_is_readonly", sync, 
                &attribute_cpi::sync_attribute_is_readonly, 
                &attribute_cpi::async_attribute_is_readonly, key);
        }
        saga::task t (saga::task::Done);
        t.get_result<bool>() = attributes_.attribute_is_readonly(key);
        return t;
    }

    bool attribute::attribute_is_readonly_sync(std::string const& key)
    {
        if (!cache_only_) 
            return attribute_interface::attribute_is_readonly_sync(key);
        return attributes_.attribute_is_readonly(key);
    }

    ///////////////////////////////////////////////////////////////////////////
    saga::task 
    attribute::attribute_is_writable (std::string key, bool sync) 
    {
        if (!cache_only_) {
            typedef saga::impl::v1_0::attribute_cpi attribute_cpi;

            return saga::impl::execute_sync_async(get_proxy(),
                "attribute_cpi", "attribute_is_writable", 
                "attribute::attribute_is_writable", sync, 
                &attribute_cpi::sync_attribute_is_writable, 
                &attribute_cpi::async_attribute_is_writable, key);
        }
        saga::task t (saga::task::Done);
        t.get_result<bool>() = attributes_.attribute_is_writable(key);
        return t;
    }

    bool attribute::attribute_is_writable_sync(std::string const& key)
    {
        if (!cache_only_) 
            return attribute_interface::attribute_is_writable_sync(key);
        return attributes_.attribute_is_writable(key);
    }

    ///////////////////////////////////////////////////////////////////////////
    saga::task 
    attribute::attribute_is_vector (std::string key, bool sync) 
    {
        if (!cache_only_) {
            typedef saga::impl::v1_0::attribute_cpi attribute_cpi;

            return saga::impl::execute_sync_async(get_proxy(),
                "attribute_cpi", "attribute_is_vector", 
                "attribute::attribute_is_vector", sync, 
                &attribute_cpi::sync_attribute_is_vector, 
                &attribute_cpi::async_attribute_is_vector, key);
        }
        saga::task t (saga::task::Done);
        t.get_result<bool>() = attributes_.attribute_is_vector(key);
        return t;
    }

    bool attribute::attribute_is_vector_sync(std::string const& key)
    {
        if (!cache_only_) 
            return attribute_interface::attribute_is_vector_sync(key);
        return attributes_.attribute_is_vector(key);
    }

    ///////////////////////////////////////////////////////////////////////////
    saga::task 
    attribute::attribute_is_extended (std::string key, bool sync) 
    {
        if (!cache_only_) {
            typedef saga::impl::v1_0::attribute_cpi attribute_cpi;

            return saga::impl::execute_sync_async(get_proxy(),
                "attribute_cpi", "attribute_is_extended", 
                "attribute::attribute_is_extended", sync, 
                &attribute_cpi::sync_attribute_is_extended, 
                &attribute_cpi::async_attribute_is_extended, key);
        }
        saga::task t (saga::task::Done);
        t.get_result<bool>() = attributes_.attribute_is_extended(key);
        return t;
    }

    bool attribute::attribute_is_extended_sync(std::string const& key)
    {
        if (!cache_only_) 
            return attribute_interface::attribute_is_extended_sync(key);
        return attributes_.attribute_is_extended(key);
    }

///////////////////////////////////////////////////////////////////////////////
}} // namespace saga::impl

