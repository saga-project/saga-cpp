//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 
///////////////////////////////////////////////////////////////////////////////
// This adaptor implements the async functions based on its own synchronous
// functions for testing purposes only. Since there is no principal need
// to do so, we allow these to be 'switched off'.
#if !defined (SAGA_DEFAULT_REPLICA_ADAPTOR_NO_ASYNCS)

#include <map>
#include <vector>

#include <boost/assert.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/tokenizer.hpp>

#include <saga/saga/exception.hpp>
#include <saga/saga/url.hpp>
#include <saga/saga/adaptors/task.hpp>

#include <saga/impl/config.hpp>

#include "config.hpp"
#include "logicalfile.hpp"
#include "common_helpers.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace replica
{

///////////////////////////////////////////////////////////////////////////////
// attribute functions
saga::task 
logical_file_cpi_impl::async_attribute_exists(std::string key)
{
    return saga::adaptors::task("logical_file_cpi_impl::async_attribute_exists",
        shared_from_this(), &logical_file_cpi_impl::sync_attribute_exists,
        key); 
}

saga::task 
logical_file_cpi_impl::async_attribute_is_readonly(std::string key)
{
    return saga::adaptors::task("logical_file_cpi_impl::async_attribute_is_readonly",
        shared_from_this(), &logical_file_cpi_impl::sync_attribute_is_readonly,
        key); 
}

saga::task 
logical_file_cpi_impl::async_attribute_is_writable(std::string key)
{
    return saga::adaptors::task("logical_file_cpi_impl::async_attribute_is_writable",
        shared_from_this(), &logical_file_cpi_impl::sync_attribute_is_writable,
        key); 
}

saga::task 
logical_file_cpi_impl::async_attribute_is_vector(std::string key)
{
    return saga::adaptors::task("logical_file_cpi_impl::async_attribute_is_vector",
        shared_from_this(), &logical_file_cpi_impl::sync_attribute_is_vector,
        key); 
}

saga::task 
logical_file_cpi_impl::async_attribute_is_extended(std::string key)
{
    return saga::adaptors::task("logical_file_cpi_impl::async_attribute_is_extended",
        shared_from_this(), &logical_file_cpi_impl::sync_attribute_is_extended,
        key); 
}

saga::task 
logical_file_cpi_impl::async_get_attribute(std::string key)
{
    return saga::adaptors::task("logical_file_cpi_impl::async_get_attribute",
        shared_from_this(), &logical_file_cpi_impl::sync_get_attribute,
        key); 
}

saga::task 
logical_file_cpi_impl::async_set_attribute(std::string key, std::string val)
{
    return saga::adaptors::task("logical_file_cpi_impl::async_set_attribute",
        shared_from_this(), &logical_file_cpi_impl::sync_set_attribute,
        key, val); 
}

saga::task 
logical_file_cpi_impl::async_get_vector_attribute(std::string key)
{
    return saga::adaptors::task("logical_file_cpi_impl::async_get_vector_attribute",
        shared_from_this(), &logical_file_cpi_impl::sync_get_vector_attribute,
        key); 
}

saga::task 
logical_file_cpi_impl::async_set_vector_attribute(std::string key, 
    std::vector<std::string> val)
{
    return saga::adaptors::task("logical_file_cpi_impl::async_set_vector_attribute",
        shared_from_this(), &logical_file_cpi_impl::sync_set_vector_attribute,
        key, val); 
}

saga::task 
logical_file_cpi_impl::async_remove_attribute(std::string key)
{
    return saga::adaptors::task("logical_file_cpi_impl::async_remove_attribute",
        shared_from_this(), &logical_file_cpi_impl::sync_remove_attribute,
        key); 
}

saga::task 
logical_file_cpi_impl::async_list_attributes()
{
    return saga::adaptors::task("logical_file_cpi_impl::async_list_attributes",
        shared_from_this(), &logical_file_cpi_impl::sync_list_attributes); 
}

saga::task 
logical_file_cpi_impl::async_find_attributes(std::string pattern)
{
    return saga::adaptors::task("logical_file_cpi_impl::async_find_attributes",
        shared_from_this(), &logical_file_cpi_impl::sync_find_attributes,
        pattern); 
}

///////////////////////////////////////////////////////////////////////////////
// namespace_entry functions
saga::task 
logical_file_cpi_impl::async_get_url()
{
    return saga::adaptors::task("logical_file_cpi_impl::async_get_url",
        shared_from_this(), &logical_file_cpi_impl::sync_get_url); 
}

saga::task 
logical_file_cpi_impl::async_get_cwd()
{
    return saga::adaptors::task("logical_file_cpi_impl::async_get_cwd",
        shared_from_this(), &logical_file_cpi_impl::sync_get_cwd); 
}

saga::task 
logical_file_cpi_impl::async_get_name()
{
    return saga::adaptors::task("logical_file_cpi_impl::async_get_name",
        shared_from_this(), &logical_file_cpi_impl::sync_get_name); 
}

saga::task logical_file_cpi_impl::async_is_dir()
{
    return saga::adaptors::task("logical_file_cpi_impl::async_is_dir",
        shared_from_this(), &logical_file_cpi_impl::sync_is_dir); 
}

saga::task logical_file_cpi_impl::async_is_entry()
{
    return saga::adaptors::task("logical_file_cpi_impl::async_is_entry",
        shared_from_this(), &logical_file_cpi_impl::sync_is_entry); 
}

saga::task logical_file_cpi_impl::async_is_link()
{
    return saga::adaptors::task("logical_file_cpi_impl::async_is_link",
        shared_from_this(), &logical_file_cpi_impl::sync_is_link); 
}

saga::task logical_file_cpi_impl::async_remove(int flags)
{
    return saga::adaptors::task("logical_file_cpi_impl::async_remove",
        shared_from_this(), &logical_file_cpi_impl::sync_remove,
        flags); 
}

saga::task logical_file_cpi_impl::async_close(double timeout)
{
    return saga::adaptors::task("logical_file_cpi_impl::async_close",
        shared_from_this(), &logical_file_cpi_impl::sync_close,
        timeout); 
}

///////////////////////////////////////////////////////////////////////////////
// logical_file functions
saga::task 
logical_file_cpi_impl::async_list_locations()
{
    return saga::adaptors::task("logical_file_cpi_impl::async_list_locations",
        shared_from_this(), &logical_file_cpi_impl::sync_list_locations); 
}

saga::task 
logical_file_cpi_impl::async_add_location(saga::url location)
{
    return saga::adaptors::task("logical_file_cpi_impl::async_add_location",
        shared_from_this(), &logical_file_cpi_impl::sync_add_location,
        location); 
}

saga::task 
logical_file_cpi_impl::async_remove_location(saga::url location)
{
    return saga::adaptors::task("logical_file_cpi_impl::async_remove_location",
        shared_from_this(), &logical_file_cpi_impl::sync_remove_location,
        location); 
}

saga::task 
logical_file_cpi_impl::async_update_location(saga::url oldlocation, 
    saga::url newlocation)
{
    return saga::adaptors::task("logical_file_cpi_impl::async_update_location",
        shared_from_this(), &logical_file_cpi_impl::sync_update_location,
        oldlocation, newlocation); 
}

saga::task 
logical_file_cpi_impl::async_replicate(saga::url location, int mode)
{
    return saga::adaptors::task("logical_file_cpi_impl::async_replicate",
        shared_from_this(), &logical_file_cpi_impl::sync_replicate,
        location, mode); 
}

///////////////////////////////////////////////////////////////////////////////
}   // namespace replica

#endif // SAGA_DEFAULT_FILE_ADAPTOR_NO_ASYNCS

