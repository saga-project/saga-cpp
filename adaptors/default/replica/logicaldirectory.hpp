//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ADAPTORS_DEFAULT_REPLICA_LOGICALDIRECTORY_HPP
#define ADAPTORS_DEFAULT_REPLICA_LOGICALDIRECTORY_HPP

#include <fstream>
#include <string>

#include <saga/saga/util.hpp>
#include <saga/saga/types.hpp>
#include <saga/saga/adaptors/adaptor.hpp>
#include <saga/saga/adaptors/packages/logical_file_cpi_instance_data.hpp>

#include <saga/impl/engine/proxy.hpp>
#include <saga/impl/packages/replica/logical_directory_cpi.hpp>

#include "replica_database.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace replica
{
    ///////////////////////////////////////////////////////////////////////////////
    //  This adaptor implements the functionality of the Saga API "replica".
    //  It defines the functions declared in its base class, logicaldirectory_cpi.
    // 
    //  @note This adaptor is written for test purposes only; it is possible that
    //        it does not function exactly like the Saga API dictates.
    //  @see The documentation of the Saga API
    class logical_directory_cpi_impl 
    :   public saga::adaptors::v1_0::logical_directory_cpi<logical_directory_cpi_impl>
    {
    private:
        typedef saga::adaptors::v1_0::logical_directory_cpi<logical_directory_cpi_impl> 
            base_cpi;

        void check_permissions(saga::replica::flags flags,
            char const* name, std::string const& lfn);

    public:    
        // constructor of the file adaptor 
        logical_directory_cpi_impl (proxy* p, cpi_info const& info, 
            saga::ini::ini const& glob_ini, saga::ini::ini const& adap_ini,
            TR1::shared_ptr<saga::adaptor> adaptor);

        // destructor of the file adaptor 
        ~logical_directory_cpi_impl  (void);

        // attribute functions
        void sync_attribute_exists(bool&, std::string key);
        void sync_attribute_is_readonly(bool&, std::string key);
        void sync_attribute_is_writable(bool&, std::string key);
        void sync_attribute_is_vector(bool&, std::string key);
        void sync_attribute_is_extended(bool&, std::string key);
        void sync_get_attribute(std::string&, std::string key);
        void sync_set_attribute(saga::impl::void_t&, std::string key, 
            std::string val);
        void sync_get_vector_attribute(std::vector<std::string>&, 
            std::string key);
        void sync_set_vector_attribute(saga::impl::void_t&, std::string, 
            std::vector<std::string>);
        void sync_remove_attribute(saga::impl::void_t&, std::string key);
        void sync_list_attributes(std::vector<std::string>& keys);
        void sync_find_attributes(std::vector<std::string>&, std::string);

        // namespace_entry functions
        void sync_get_url(saga::url& url);
        void sync_get_cwd(saga::url&);
        void sync_get_name(saga::url&);
//         void sync_read_link(saga::url&);
        void sync_is_dir(bool&);
        void sync_is_entry(bool&);
        void sync_is_link(bool&);
//         void sync_copy(saga::impl::void_t&, saga::url, int);
//         void sync_link(saga::impl::void_t&, saga::url, int);
//         void sync_move(saga::impl::void_t&, saga::url, int);
        void sync_remove(saga::impl::void_t&, int);
        void sync_close(saga::impl::void_t&, double);

        // namespace_dir functions
        void sync_change_dir(saga::impl::void_t&, saga::url);
        void sync_list(std::vector<saga::url>&, std::string, int);
        void sync_find(std::vector<saga::url>&, std::string, int);
//         void sync_read_link(saga::url&, saga::url);
        void sync_exists(bool&, saga::url);
        void sync_is_dir(bool&, saga::url);
        void sync_is_entry(bool&, saga::url);
        void sync_is_link(bool&, saga::url);
        void sync_get_num_entries(std::size_t&);
        void sync_get_entry(saga::url&, std::size_t);
//         void sync_copy(saga::impl::void_t&, saga::url, saga::url, int);
//         void sync_link(saga::impl::void_t&, saga::url, saga::url, int);
//         void sync_move(saga::impl::void_t&, saga::url, saga::url, int);
        void sync_remove(saga::impl::void_t&, saga::url, int);
        void sync_make_dir(saga::impl::void_t&, saga::url, int);
        void sync_open(saga::name_space::entry&, saga::url, int);
        void sync_open_dir(saga::name_space::directory&, saga::url, int);

        // logical_directory functions
        void sync_is_file(bool&, saga::url);
        void sync_open(saga::replica::logical_file&, saga::url, int);
        void sync_open_dir(saga::replica::logical_directory&, saga::url, int);
        void sync_find(std::vector<saga::url>&, std::string, 
            std::vector<std::string>, int);
        
    // This adaptor implements the async functions based on its own synchronous
    // functions for testing purposes only. Since there is no principal need
    // to do so, we allow these to be 'switched off'.
    #if !defined(SAGA_DEFAULT_REPLICA_ADAPTOR_NO_ASYNCS)
        // attribute functions
        saga::task async_attribute_exists(std::string key);
        saga::task async_attribute_is_readonly(std::string key);
        saga::task async_attribute_is_writable(std::string key);
        saga::task async_attribute_is_vector(std::string key);
        saga::task async_attribute_is_extended(std::string key);
        saga::task async_get_attribute(std::string key);
        saga::task async_set_attribute(std::string key, std::string val);
        saga::task async_get_vector_attribute(std::string key);
        saga::task async_set_vector_attribute(std::string, std::vector<std::string>);
        saga::task async_remove_attribute(std::string key);
        saga::task async_list_attributes();
        saga::task async_find_attributes(std::string);
        
        // namespace_entry functions
        saga::task async_get_url();
        saga::task async_get_cwd();
        saga::task async_get_name();
//         saga::task async_read_link(&);
        saga::task async_is_dir();
        saga::task async_is_entry();
        saga::task async_is_link();
//         saga::task async_copy(saga::url, int);
//         saga::task async_link(saga::url, int);
//         saga::task async_move(saga::url, int);
        saga::task async_remove(int);
        saga::task async_close(double);

        // namespace_dir functions
        saga::task async_change_dir(saga::url);
        saga::task async_list(std::string, int);
        saga::task async_find(std::string, int);
//         saga::task async_read_link(saga::url);
        saga::task async_exists(saga::url);
        saga::task async_is_dir(saga::url);
        saga::task async_is_entry(saga::url);
        saga::task async_is_link(saga::url);
        saga::task async_get_num_entries();
        saga::task async_get_entry(std::size_t);
//         saga::task async_copy(saga::url, saga::url, int);
//         saga::task async_link(saga::url, saga::url, int);
//         saga::task async_move(saga::url, saga::url, int);
        saga::task async_remove(saga::url, int);
        saga::task async_make_dir(saga::url, int);

        // logical_directory functions
        saga::task async_is_file(saga::url);
        saga::task async_open(saga::url, int);
        saga::task async_open_dir(saga::url, int);
        saga::task async_find(std::string, std::vector<std::string>, int);
    #endif // !SAGA_DEFAULT_REPLICA_ADAPTOR_NO_ASYNCS

    private:
        database db_;
        bool opened_;

    protected:
        void check_if_open (std::string const& functionname, 
            saga::url const& location);
    };  // class logical_file_cpi_impl

///////////////////////////////////////////////////////////////////////////////
}   // namespace replica

#endif // ADAPTORS_DEFAULT_REPLICA_LOGICALDIRECTORY_HPP

