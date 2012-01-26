//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_ADAPTORS_ATTRIBUTE_CACHE_HPP
#define SAGA_ADAPTORS_ATTRIBUTE_CACHE_HPP

#include <vector>
#include <string>

#include <saga/saga/adaptors/task_declaration.hpp>
#include <saga/saga/adaptors/attribute_cpi.hpp>

#include <saga/impl/engine/proxy.hpp>
#include <saga/impl/engine/cpi_info.hpp>
#include <saga/impl/engine/attribute_cache.hpp>
#include <saga/impl/engine/register_members.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable : 4251 4231 4660)
#endif

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace adaptors {

    //  This class provides a default implementation of the attribute_cpi 
    //  interface by redirecting the calls into the SAGA engine, utilizing the
    //  internal attribute cache.
    template <typename Derived, typename Base>
    class attribute_cache_cpi : public Base
    {
    private:
        typedef saga::impl::proxy proxy;
        typedef saga::impl::v1_0::cpi cpi;
        typedef saga::impl::v1_0::cpi_info cpi_info;
        typedef saga::impl::v1_0::preference_type preference_type;
        typedef std::vector<std::string> strvec_type;

        saga::impl::attribute_cache *get_cache() const
            { return this->get_proxy()->get_attributes()->get_cache(); }
        
    public:
        attribute_cache_cpi(proxy* p, cpi_info const& info, 
                TR1::shared_ptr<saga::adaptor> adaptor, cpi::flags flags)
        :   Base(p, info, adaptor, flags)
        {}
        
        virtual ~attribute_cache_cpi (void) 
        {}

        // synchronous attribute_cpi interface redirect all calls to the
        // default cache implementation
        void sync_get_attribute(std::string& ret, std::string key)
            { ret = get_cache()->get_attribute(key); }
        void sync_set_attribute(saga::impl::void_t&, std::string key, std::string val)
            { get_cache()->set_attribute(key, val); }
        void sync_get_vector_attribute(strvec_type& ret, std::string key)
            { get_cache()->get_vector_attribute(key, ret); }
        void sync_set_vector_attribute(saga::impl::void_t&, std::string key, strvec_type val)
            { get_cache()->set_vector_attribute(key, val); }
        void sync_remove_attribute(saga::impl::void_t&, std::string key)
            { get_cache()->remove_attribute(key); }
        void sync_list_attributes(strvec_type& ret)
            { get_cache()->list_attributes(ret); }
        void sync_find_attributes(strvec_type& ret, std::string pattern)
            { get_cache()->find_attributes(pattern, ret); }
        void sync_attribute_exists(bool& ret, std::string key)
            { ret = get_cache()->attribute_exists(key); }
        void sync_attribute_is_readonly(bool& ret, std::string key)
            { ret = get_cache()->attribute_is_readonly(key); }
        void sync_attribute_is_writable(bool& ret, std::string key)
            { ret = get_cache()->attribute_is_writable(key); }
        void sync_attribute_is_vector(bool& ret, std::string key)
            { ret = get_cache()->attribute_is_vector(key); }
        void sync_attribute_is_extended(bool& ret, std::string key)
            { ret = get_cache()->attribute_is_extended(key); }

#if !defined(SAGA_ATTRIBUTE_CPI_NO_ASYNCS)
        // asynchronous attribute_cpi interface simply wraps the synchronous 
        // calls implemented in the most derived class
        saga::task async_get_attribute(std::string key)
            { return saga::adaptors::task(
                  "attribute_cache_cpi::async_get_attribute",
                  this->shared_from_this(), &Derived::sync_get_attribute, key);
            }
        saga::task async_set_attribute(std::string key, std::string val)
            { return saga::adaptors::task(
                  "attribute_cache_cpi::async_set_attribute",
                  this->shared_from_this(), &Derived::sync_set_attribute, 
                  key, val);
            }
        saga::task async_get_vector_attribute(std::string key)
            { return saga::adaptors::task(
                  "attribute_cache_cpi::async_get_vector_attribute",
                  this->shared_from_this(), &Derived::sync_get_vector_attribute, 
                  key);
            }
        saga::task async_set_vector_attribute(std::string key, strvec_type val)
            { return saga::adaptors::task(
                  "attribute_cache_cpi::async_set_vector_attribute",
                  this->shared_from_this(), &Derived::sync_set_vector_attribute, 
                  key, val);
            }
        saga::task async_remove_attribute(std::string key)
            { return saga::adaptors::task(
                  "attribute_cache_cpi::async_remove_attribute",
                  this->shared_from_this(), &Derived::sync_remove_attribute, 
                  key);
            }
        saga::task async_list_attributes()
            { return saga::adaptors::task(
                  "attribute_cache_cpi::async_list_attributes",
                  this->shared_from_this(), &Derived::sync_list_attributes);
            }
        saga::task async_find_attributes(std::string pattern)
            { return saga::adaptors::task(
                  "attribute_cache_cpi::async_find_attributes",
                  this->shared_from_this(), &Derived::sync_find_attributes, 
                  pattern);
            }
        saga::task async_attribute_exists(std::string key)
            { return saga::adaptors::task(
                  "attribute_cache_cpi::async_attribute_exists",
                  this->shared_from_this(), &Derived::sync_attribute_exists, 
                  key);
            }
        saga::task async_attribute_is_readonly(std::string key)
            { return saga::adaptors::task(
                  "attribute_cache_cpi::async_attribute_is_readonly",
                  this->shared_from_this(), &Derived::sync_attribute_is_readonly, 
                  key);
            }
        saga::task async_attribute_is_writable(std::string key)
            { return saga::adaptors::task(
                  "attribute_cache_cpi::async_attribute_is_writable",
                  this->shared_from_this(), &Derived::sync_attribute_is_writable, 
                  key);
            }
        saga::task async_attribute_is_vector(std::string key)
            { return saga::adaptors::task(
                  "attribute_cache_cpi::async_attribute_is_vector",
                  this->shared_from_this(), &Derived::sync_attribute_is_vector, 
                  key);
            }
        saga::task async_attribute_is_extended(std::string key)
            { return saga::adaptors::task(
                  "attribute_cache_cpi::async_attribute_is_extended",
                  this->shared_from_this(), &Derived::sync_attribute_is_extended, 
                  key);
            }
#endif // !SAGA_ATTRIBUTE_CPI_NO_ASYNCS
    };
    
///////////////////////////////////////////////////////////////////////////////
}} // namespace saga::adaptors
                                                                             
#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

#endif


