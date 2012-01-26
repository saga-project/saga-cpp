//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ADAPTORS_DEFAULT_ADVERT_ADVERT_HPP
#define ADAPTORS_DEFAULT_ADVERT_ADVERT_HPP

#include <fstream>
#include <string>

#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>

#include <saga/saga/util.hpp>
#include <saga/saga/types.hpp>
#include <saga/saga/adaptors/adaptor.hpp>
#include <saga/saga/adaptors/task.hpp>
#include <saga/saga/adaptors/packages/advert_cpi_instance_data.hpp>

#include <saga/impl/engine/proxy.hpp>
#include <saga/impl/packages/advert/advert_cpi.hpp>

#include "advert_database.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace advert
{
    ///////////////////////////////////////////////////////////////////////////////
    //  This adaptor implements the functionality of the Saga API "advert".
    //  It defines the functions declared in its base class, advert_file_cpi.
    class advert_cpi_impl 
        : public saga::adaptors::v1_0::advert_cpi<advert_cpi_impl>
    {
    private:
        typedef saga::adaptors::v1_0::advert_cpi<advert_cpi_impl> 
            base_cpi;
        
        void check_permissions(saga::advert::flags flags,
            char const* name, std::string const& lfn);

    public:    
        typedef base_cpi::mutex_type mutex_type;

        // constructor of the file adaptor 
        advert_cpi_impl (proxy* p, cpi_info const& info, 
            saga::ini::ini const& glob_ini, saga::ini::ini const& adap_ini,
            TR1::shared_ptr<saga::adaptor> adaptor);

        // destructor of the file adaptor 
        ~advert_cpi_impl  (void);

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

        // advert functions
        void sync_store_object(saga::impl::void_t&, saga::object obj);
        void sync_retrieve_object(saga::object&, saga::session s);
        void sync_store_string(saga::impl::void_t&, std::string str);
        void sync_retrieve_string(std::string&);
        
    // This adaptor implements the async functions based on its own synchronous
    // functions for testing purposes only. Since there is no principal need
    // to do so, we allow these to be 'switched off'.
    #if !defined(SAGA_DEFAULT_ADVERT_ADAPTOR_NO_ASYNCS)
        // attribute functions
        saga::task async_attribute_exists(std::string key);
        saga::task async_attribute_is_readonly(std::string key);
        saga::task async_attribute_is_writable(std::string key);
        saga::task async_attribute_is_vector(std::string key);
        saga::task async_attribute_is_extended(std::string key);
        saga::task async_get_attribute(std::string key);
        saga::task async_set_attribute(std::string key, std::string val);
        saga::task async_get_vector_attribute(std::string key);
        saga::task async_set_vector_attribute(std::string, 
            std::vector<std::string>);
        saga::task async_remove_attribute(std::string key);
        saga::task async_list_attributes();
        saga::task async_find_attributes(std::string);

        // namespace_entry functions
        saga::task async_get_url();
        saga::task async_get_cwd();
        saga::task async_get_name();
//         saga::task async_read_link();
        saga::task async_is_dir();
        saga::task async_is_entry();
        saga::task async_is_link();
//         saga::task async_copy(saga::url, int);
//         saga::task async_link(saga::url, int);
//         saga::task async_move(saga::url, int);
        saga::task async_remove(int);
        saga::task async_close(double);

        // advert functions
        saga::task async_store_object(saga::object obj);
        saga::task async_retrieve_object(saga::session s);
        saga::task async_store_string(std::string str);
        saga::task async_retrieve_string();

    #endif // !SAGA_DEFAULT_ADVERT_ADAPTOR_NO_ASYNCS

    private:
    // monitoring
        saga::adaptors::workitem_cookie_handle monitor_handle_;
        std::string last_modification_time_;
        saga::session session_;

    //
        bool opened_;
        database db_;

        std::string clean_url (const saga::url & u);

    protected:
        void setup_monitoring();
        void monitor_entry(boost::system::error_code const&);

        void check_if_open (std::string const& functionname, 
            saga::url const& location);
    };  // class advert_cpi_impl

///////////////////////////////////////////////////////////////////////////////
}   // namespace advert

#endif // ADAPTORS_DEFAULT_ADVERT_ADVERT_HPP

