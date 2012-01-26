//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_IMPL_ENGINE_ATTRIBUTE_INTERFACE_HPP
#define SAGA_IMPL_ENGINE_ATTRIBUTE_INTERFACE_HPP

#include <saga/saga/export_definitions.hpp>
#include <saga/saga/task.hpp>

// include stl
#include <vector>
#include <string>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable : 4251 4231 4660)
#endif

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl
{
    // we define a common interface used by saga::impl::attribute and 
    // saga::adaptors::attribute_cpi_wrapper
    class SAGA_EXPORT attribute_interface
    {
        typedef std::vector<std::string> strvec_type;

    public:
        virtual ~attribute_interface (void) {}

        virtual saga::task get_attribute(std::string, bool is_sync = false) = 0;
        virtual saga::task set_attribute(std::string, std::string, bool is_sync = false) = 0;
        virtual saga::task get_vector_attribute(std::string, bool is_sync = false) = 0;
        virtual saga::task set_vector_attribute(std::string, strvec_type, bool is_sync = false) = 0;
        virtual saga::task remove_attribute(std::string, bool is_sync = false) = 0;
        virtual saga::task list_attributes(bool is_sync = false) = 0;
        virtual saga::task find_attributes(std::string, bool is_sync = false) = 0;
        virtual saga::task attribute_exists(std::string, bool is_sync = false) = 0;
        virtual saga::task attribute_is_readonly(std::string, bool is_sync = false) = 0;
        virtual saga::task attribute_is_writable(std::string, bool is_sync = false) = 0;
        virtual saga::task attribute_is_vector(std::string, bool is_sync = false) = 0;
        virtual saga::task attribute_is_extended(std::string, bool is_sync = false) = 0;

        // Additional set of functions allowing to optimize the attribute 
        // implementation under certain circumstances. The fall back 
        // implementation emulates the required functionality.
        virtual std::string get_attribute_sync(std::string const& key)
        {
            saga::task t = this->get_attribute(key, true);
            return t.get_result<std::string>();
        }
        virtual void set_attribute_sync(std::string const& key, 
            std::string const& val)
        {
            saga::task t = this->set_attribute(key, val, true);
            t.rethrow();
        }
        virtual void get_vector_attribute_sync(std::string const& key, 
            std::vector<std::string>& result)
        {
            saga::task t = this->get_vector_attribute(key, true);
            result = t.get_result<std::vector<std::string> >();
        }
        virtual void set_vector_attribute_sync(std::string const& key, 
            strvec_type const& val)
        {
            saga::task t = this->set_vector_attribute(key, val, true);
            t.rethrow();
        }
        virtual void remove_attribute_sync(std::string const& key)
        {
            saga::task t = this->remove_attribute(key, true);
            t.rethrow();
        }
        virtual void list_attributes_sync(std::vector<std::string>& result)
        {
            saga::task t = this->list_attributes(true);
            result = t.get_result<std::vector<std::string> >();
        }
        virtual void find_attributes_sync(std::string const& pattern, 
            std::vector<std::string>& result)
        {
            saga::task t = this->find_attributes(pattern, true);
            result = t.get_result<std::vector<std::string> >();
        }
        virtual bool attribute_exists_sync(std::string const& key)
        {
            saga::task t = this->attribute_exists(key, true);
            return t.get_result<bool>();
        }
        virtual bool attribute_is_readonly_sync(std::string const& key)
        {
            saga::task t = this->attribute_is_readonly(key, true);
            return t.get_result<bool>();
        }
        virtual bool attribute_is_writable_sync(std::string const& key)
        {
            saga::task t = this->attribute_is_writable(key, true);
            return t.get_result<bool>();
        }
        virtual bool attribute_is_vector_sync(std::string const& key)
        {
            saga::task t = this->attribute_is_vector(key, true);
            return t.get_result<bool>();
        }
        virtual bool attribute_is_extended_sync(std::string const& key)
        {
            saga::task t = this->attribute_is_extended(key, true);
            return t.get_result<bool>();
        }
    };

}}  // namespace saga::impl

#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

#endif // SAGA_IMPL_ENGINE_ATTRIBUTE_INTERFACE_HPP


