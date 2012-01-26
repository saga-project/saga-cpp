//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_SAGA_ADAPTORS_ATTRIBUTE_CPI_WRAPPER_HPP
#define SAGA_SAGA_ADAPTORS_ATTRIBUTE_CPI_WRAPPER_HPP

#include <string>
#include <vector>

#include <saga/saga/adaptors/attribute_cpi.hpp>
#include <saga/impl/engine/attribute_interface.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace adaptors
{
    ///////////////////////////////////////////////////////////////////////////
    //  This wrapper is used in conjunction with the saga::attribute and 
    //  saga::adaptors::attribute classes when these are used from within a cpi 
    //  instance to access the very own attributes of this cpi instance 
    class const_attribute_cpi_wrapper 
    :   public saga::impl::attribute_interface
    {
        typedef std::vector <std::string> strvec_type;

    public:
        // we const_cast here while guaranteeing to fail all attempts to write 
        // access to this instance
        const_attribute_cpi_wrapper(saga::impl::v1_0::attribute_cpi const* cpi_instance)
        :   attributes_(const_cast<saga::impl::v1_0::attribute_cpi*>(cpi_instance))
        {
        }
        
        saga::task 
        get_attribute(std::string key, bool is_sync)
        {
            if (is_sync) { 
                saga::task t(saga::task::Done);
                attributes_->
                    sync_get_attribute(t.get_result<std::string>(), key);
                return t;
            } 
            return attributes_->async_get_attribute(key);
        }
        saga::task 
        set_attribute(std::string key, std::string val, bool is_sync)
        {
            SAGA_THROW("const_attribute_cpi_wrapper::set_vector"
                       " is not implemented (and rightly so!)", 
                       saga::NotImplemented);
            return saga::task();
        }
        saga::task 
        get_vector_attribute(std::string key, bool is_sync)
        {
            if (is_sync) { 
                saga::task t(saga::task::Done);
                attributes_->
                    sync_get_vector_attribute(t.get_result<strvec_type>(), key);
                return t;
            } 
            return attributes_->async_get_vector_attribute(key);
        }
        saga::task 
        set_vector_attribute(std::string key, strvec_type val, bool is_sync)
        {
            SAGA_THROW("const_attribute_cpi_wrapper::set_vector_attribute"
                       " is not implemented (and rightly so!)", 
                       saga::NotImplemented);
            return saga::task();
        }
        saga::task 
        remove_attribute(std::string key, bool is_sync)
        {
            SAGA_THROW("const_attribute_cpi_wrapper::remove_attribute"
                       " is not implemented (and rightly so!)", 
                       saga::NotImplemented);
            return saga::task();
        }
        saga::task 
        list_attributes(bool is_sync)
        {
            if (is_sync) { 
                saga::task t(saga::task::Done);
                attributes_->sync_list_attributes(
                    t.get_result<strvec_type>());
                return t;
            } 
            return attributes_->async_list_attributes();
        }
        saga::task 
        find_attributes(std::string pattern, bool is_sync)
        {
            if (is_sync) { 
                saga::task t(saga::task::Done);
                attributes_->sync_find_attributes(
                    t.get_result<strvec_type>(), pattern);
                return t;
            } 
            return attributes_->async_find_attributes(pattern);
        }
        saga::task 
        attribute_exists(std::string key, bool is_sync)
        {
            if (is_sync) { 
                saga::task t(saga::task::Done);
                attributes_->sync_attribute_exists(
                    t.get_result<bool>(), key);
                return t;
            } 
            return attributes_->async_attribute_exists(key);
        }
        saga::task 
        attribute_is_readonly(std::string key, bool is_sync)
        {
            if (is_sync) { 
                saga::task t(saga::task::Done);
                attributes_->sync_attribute_is_readonly(
                    t.get_result<bool>(), key);
                return t;
            } 
            return attributes_->async_attribute_is_readonly(key);
        }
        saga::task 
        attribute_is_writable(std::string key, bool is_sync)
        {
            if (is_sync) { 
                saga::task t(saga::task::Done);
                attributes_->sync_attribute_is_writable(
                    t.get_result<bool>(), key);
                return t;
            } 
            return attributes_->async_attribute_is_writable(key);
        }
        saga::task 
        attribute_is_vector(std::string key, bool is_sync)
        {
            if (is_sync) { 
                saga::task t(saga::task::Done);
                attributes_->sync_attribute_is_vector(
                    t.get_result<bool>(), key);
                return t;
            } 
            return attributes_->async_attribute_is_vector(key);
        }
        saga::task 
        attribute_is_extended(std::string key, bool is_sync)
        {
            if (is_sync) { 
                saga::task t(saga::task::Done);
                attributes_->sync_attribute_is_extended(
                    t.get_result<bool>(), key);
                return t;
            } 
            return attributes_->async_attribute_is_extended(key);
        }

    private:
        saga::impl::v1_0::attribute_cpi* attributes_;
    };

    ///////////////////////////////////////////////////////////////////////////
    // same as above, but allowing to change attributes
    class attribute_cpi_wrapper 
    :   public saga::impl::attribute_interface
    {
        typedef std::vector <std::string> strvec_type;

    public:
        attribute_cpi_wrapper(saga::impl::v1_0::attribute_cpi* cpi_instance)
        :   attributes_(cpi_instance)
        {
        }
        
        saga::task 
        get_attribute(std::string key, bool is_sync)
        {
            if (is_sync) { 
                saga::task t(saga::task::Done);
                attributes_->
                    sync_get_attribute(t.get_result<std::string>(), key);
                return t;
            } 
            return attributes_->async_get_attribute(key);
        }
        saga::task 
        set_attribute(std::string key, std::string val, bool is_sync)
        {
            if (is_sync) { 
                saga::task t(saga::task::Done);
                saga::impl::void_t void_result;
                attributes_->sync_set_attribute(void_result, key, val);
                return t;
            } 
            return attributes_->async_set_attribute(key, val);
        }
        saga::task 
        get_vector_attribute(std::string key, bool is_sync)
        {
            if (is_sync) { 
                saga::task t(saga::task::Done);
                attributes_->
                    sync_get_vector_attribute(t.get_result<strvec_type>(), key);
                return t;
            } 
            return attributes_->async_get_vector_attribute(key);
        }
        saga::task 
        set_vector_attribute(std::string key, strvec_type val, bool is_sync)
        {
            if (is_sync) { 
                saga::task t(saga::task::Done);
                saga::impl::void_t void_result;
                attributes_->sync_set_vector_attribute(void_result, key, val);
                return t;
            } 
            return attributes_->async_set_vector_attribute(key, val);
        }
        saga::task 
        remove_attribute(std::string key, bool is_sync)
        {
            if (is_sync) { 
                saga::task t(saga::task::Done);
                saga::impl::void_t void_result;
                attributes_->sync_remove_attribute(void_result, key);
                return t;
            } 
            return attributes_->async_remove_attribute(key);
        }
        saga::task 
        list_attributes(bool is_sync)
        {
            if (is_sync) { 
                saga::task t(saga::task::Done);
                attributes_->sync_list_attributes(
                    t.get_result<strvec_type>());
                return t;
            } 
            return attributes_->async_list_attributes();
        }
        saga::task 
        find_attributes(std::string pattern, bool is_sync)
        {
            if (is_sync) { 
                saga::task t(saga::task::Done);
                attributes_->sync_find_attributes(
                    t.get_result<strvec_type>(), pattern);
                return t;
            } 
            return attributes_->async_find_attributes(pattern);
        }
        saga::task 
        attribute_exists(std::string key, bool is_sync)
        {
            if (is_sync) { 
                saga::task t(saga::task::Done);
                attributes_->sync_attribute_exists(
                    t.get_result<bool>(), key);
                return t;
            } 
            return attributes_->async_attribute_exists(key);
        }
        saga::task 
        attribute_is_readonly(std::string key, bool is_sync)
        {
            if (is_sync) { 
                saga::task t(saga::task::Done);
                attributes_->sync_attribute_is_readonly(
                    t.get_result<bool>(), key);
                return t;
            } 
            return attributes_->async_attribute_is_readonly(key);
        }
        saga::task 
        attribute_is_writable(std::string key, bool is_sync)
        {
            if (is_sync) { 
                saga::task t(saga::task::Done);
                attributes_->sync_attribute_is_writable(
                    t.get_result<bool>(), key);
                return t;
            } 
            return attributes_->async_attribute_is_writable(key);
        }
        saga::task 
        attribute_is_vector(std::string key, bool is_sync)
        {
            if (is_sync) { 
                saga::task t(saga::task::Done);
                attributes_->sync_attribute_is_vector(
                    t.get_result<bool>(), key);
                return t;
            } 
            return attributes_->async_attribute_is_vector(key);
        }
        saga::task 
        attribute_is_extended(std::string key, bool is_sync)
        {
            if (is_sync) { 
                saga::task t(saga::task::Done);
                attributes_->sync_attribute_is_extended(
                    t.get_result<bool>(), key);
                return t;
            } 
            return attributes_->async_attribute_is_extended(key);
        }

    private:
        saga::impl::v1_0::attribute_cpi* attributes_;
    };

///////////////////////////////////////////////////////////////////////////////
}}    // namespace saga::adaptors

#endif // !SAGA_SAGA_ADAPTORS_ATTRIBUTE_CPI_WRAPPER_HPP


