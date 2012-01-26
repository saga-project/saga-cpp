#ifndef SAGA_IMPL_PACKAGES_DATA_NAMESPACE_NAMESPACEDIR_CPI_HPP
#define SAGA_IMPL_PACKAGES_DATA_NAMESPACE_NAMESPACEDIR_CPI_HPP

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/namespace_dir_cpi.hpp")
#endif
//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005 Michel Zandstra [michelzandstra@gmail.com]
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif
#include <string>

#include <saga/saga/util.hpp>
#include <saga/saga/types.hpp>
#include <saga/saga/exception.hpp>

#include <saga/saga/namespace.hpp>

#include <saga/saga/adaptors/instance_data.hpp>
#include <saga/saga/adaptors/packages/namespace_dir_cpi_instance_data.hpp>

#include <saga/impl/call.hpp>
#include <saga/impl/config.hpp>
#include <saga/impl/engine/cpi.hpp>
#include <saga/impl/engine/register_members.hpp>

#include <saga/impl/packages/namespace/namespace_entry_cpi.hpp>

#ifdef SAGA_DEBUG
#include <saga/impl/packages/namespace/preprocessed/namespace_dir_cpi.hpp>
#else

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: "preprocessed/namespace_dir_cpi.hpp")
#endif
///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl { namespace v1_0 {

    ///////////////////////////////////////////////////////////////////////////
    class namespace_dir_cpi 
        : public namespace_entry_cpi
    {
    public:
        namespace_dir_cpi   (proxy * p, cpi_info const& info, 
                TR1::shared_ptr<saga::adaptor> adaptor, cpi::flags flags) 
            : namespace_entry_cpi (p, info, adaptor, flags) 
        { 
        }
         
        ~namespace_dir_cpi (void) 
        {
        }

        cpi::type get_type() const { return cpi::NSDirectory; }

        // navigation/query methods
        SAGA_CALL_CPI_DECL_VIRT_1 (namespace_dir_cpi, saga::impl::void_t, change_dir, saga::url)
        SAGA_CALL_CPI_DECL_VIRT_2 (namespace_dir_cpi, std::vector <saga::url>, list, std::string, int)
        SAGA_CALL_CPI_DECL_VIRT_2 (namespace_dir_cpi, std::vector <saga::url>, find, std::string, int)

        SAGA_CALL_CPI_DECL_VIRT_1 (namespace_dir_cpi, saga::url, read_link, saga::url)
        SAGA_CALL_CPI_DECL_VIRT_1 (namespace_dir_cpi, bool, exists, saga::url)
        SAGA_CALL_CPI_DECL_VIRT_1 (namespace_dir_cpi, bool, is_dir, saga::url)
        SAGA_CALL_CPI_DECL_VIRT_1 (namespace_dir_cpi, bool, is_entry, saga::url)
        SAGA_CALL_CPI_DECL_VIRT_1 (namespace_dir_cpi, bool, is_link, saga::url)

        // Deal with entries by entry number_cpi
        SAGA_CALL_CPI_DECL_VIRT_0 (namespace_dir_cpi, std::size_t, get_num_entries)
        SAGA_CALL_CPI_DECL_VIRT_1 (namespace_dir_cpi, saga::url, get_entry, std::size_t)

        // entry management methods
        SAGA_CALL_CPI_DECL_VIRT_3 (namespace_dir_cpi, saga::impl::void_t, copy, saga::url, saga::url, int)
        SAGA_CALL_CPI_DECL_VIRT_3 (namespace_dir_cpi, saga::impl::void_t, link, saga::url, saga::url, int)
        SAGA_CALL_CPI_DECL_VIRT_3 (namespace_dir_cpi, saga::impl::void_t, move, saga::url, saga::url, int)
        SAGA_CALL_CPI_DECL_VIRT_2 (namespace_dir_cpi, saga::impl::void_t, remove, saga::url, int)
        SAGA_CALL_CPI_DECL_VIRT_2 (namespace_dir_cpi, saga::impl::void_t, make_dir, saga::url, int)

        // open methods
        SAGA_CALL_CPI_DECL_VIRT_2 (namespace_dir_cpi, saga::name_space::entry, open, saga::url, int)
        SAGA_CALL_CPI_DECL_VIRT_2 (namespace_dir_cpi, saga::name_space::directory, open_dir, saga::url, int)

        // entry management methods - wild card versions
        SAGA_CALL_CPI_DECL_VIRT_3 (namespace_dir_cpi, saga::impl::void_t, copy_wildcard, std::string, saga::url, int)
        SAGA_CALL_CPI_DECL_VIRT_3 (namespace_dir_cpi, saga::impl::void_t, link_wildcard, std::string, saga::url, int)
        SAGA_CALL_CPI_DECL_VIRT_3 (namespace_dir_cpi, saga::impl::void_t, move_wildcard, std::string, saga::url, int)
        SAGA_CALL_CPI_DECL_VIRT_2 (namespace_dir_cpi, saga::impl::void_t, remove_wildcard, std::string, int)
        
        // permissions with flags
        SAGA_CALL_CPI_DECL_VIRT_4(namespace_dir_cpi, saga::impl::void_t, permissions_allow, saga::url, std::string, int, int)
        SAGA_CALL_CPI_DECL_VIRT_4(namespace_dir_cpi, saga::impl::void_t, permissions_deny, saga::url, std::string, int, int)
        SAGA_CALL_CPI_DECL_VIRT_4(namespace_dir_cpi, saga::impl::void_t, permissions_allow_wildcard, std::string, std::string, int, int)
        SAGA_CALL_CPI_DECL_VIRT_4(namespace_dir_cpi, saga::impl::void_t, permissions_deny_wildcard, std::string, std::string, int, int)

    }; // class namespace_dir_cpi

///////////////////////////////////////////////////////////////////////////////
}}} // namespace saga::impl::v1_0

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace adaptors { namespace v1_0 {

///////////////////////////////////////////////////////////////////////////////
//  register namespace_dir CPI functions
    template <typename Derived>
    inline bool
    register_namespace_dir_functions(
        std::vector<saga::impl::v1_0::cpi_info>& infos, 
        saga::impl::v1_0::cpi::maker_type maker, 
        saga::impl::v1_0::preference_type const& prefs, 
        saga::uuid const& cpi_uuid, saga::uuid const& adaptor_uuid, 
        std::string const& cpi_name)
    {
        bool retval = false;
        saga::impl::v1_0::cpi_info info(adaptors::namespace_dir_cpi, 
            cpi_name, maker, prefs, cpi_uuid, adaptor_uuid);
              
        SAGA_LOG(SAGA_VERBOSE_LEVEL_BLURB)
            << "begin register_namespace_dir_functions (" << cpi_name << "): " 
            << adaptor_uuid.string() << ":";

        typedef saga::impl::v1_0::namespace_dir_cpi base_cpi;
        
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, change_dir, prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, list, prefs);     
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, exists, prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, get_num_entries, prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, get_entry, prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, make_dir, prefs);

        // register ambiguous cpi functions        
        typedef void (base_cpi::*base_sync_read_link_type)(saga::url&, saga::url);
        typedef saga::task (base_cpi::*base_async_read_link_type)(saga::url);
        typedef void (Derived::*sync_read_link_type)(saga::url&, saga::url);
        typedef saga::task (Derived::*async_read_link_type)(saga::url);

        SAGA_REGISTER_MEMBER_EX(retval, info, base_cpi, Derived, read_link, prefs,
            base_sync_read_link_type, base_async_read_link_type,
            sync_read_link_type, async_read_link_type);

        typedef void (base_cpi::*base_sync_is_dir_type)(bool&, saga::url);
        typedef saga::task (base_cpi::*base_async_is_dir_type)(saga::url);
        typedef void (Derived::*sync_is_dir_type)(bool&, saga::url);
        typedef saga::task (Derived::*async_is_dir_type)(saga::url);

        SAGA_REGISTER_MEMBER_EX(retval, info, base_cpi, Derived, is_dir, prefs,
            base_sync_is_dir_type, base_async_is_dir_type,
            sync_is_dir_type, async_is_dir_type);

        typedef void (base_cpi::*base_sync_is_entry_type)(bool&, saga::url);
        typedef saga::task (base_cpi::*base_async_is_entry_type)(saga::url);
        typedef void (Derived::*sync_is_entry_type)(bool&, saga::url);
        typedef saga::task (Derived::*async_is_entry_type)(saga::url);

        SAGA_REGISTER_MEMBER_EX(retval, info, base_cpi, Derived, is_entry, prefs,
            base_sync_is_entry_type, base_async_is_entry_type,
            sync_is_entry_type, async_is_entry_type);

        typedef void (base_cpi::*base_sync_is_link_type)(bool&, saga::url);
        typedef saga::task (base_cpi::*base_async_is_link_type)(saga::url);
        typedef void (Derived::*sync_is_link_type)(bool&, saga::url);
        typedef saga::task (Derived::*async_is_link_type)(saga::url);

        SAGA_REGISTER_MEMBER_EX(retval, info, base_cpi, Derived, is_link, prefs,
            base_sync_is_link_type, base_async_is_link_type,
            sync_is_link_type, async_is_link_type);

        typedef void (base_cpi::*base_sync_copy_type)(saga::impl::void_t&, saga::url, saga::url, int);
        typedef saga::task (base_cpi::*base_async_copy_type)(saga::url, saga::url, int);
        typedef void (Derived::*sync_copy_type)(saga::impl::void_t&, saga::url, saga::url, int);
        typedef saga::task (Derived::*async_copy_type)(saga::url, saga::url, int);

        SAGA_REGISTER_MEMBER_EX(retval, info, base_cpi, Derived, copy, prefs,
            base_sync_copy_type, base_async_copy_type,
            sync_copy_type, async_copy_type);

        typedef void (base_cpi::*base_sync_link_type)(saga::impl::void_t&, saga::url, saga::url, int);
        typedef saga::task (base_cpi::*base_async_link_type)(saga::url, saga::url, int);
        typedef void (Derived::*sync_link_type)(saga::impl::void_t&, saga::url, saga::url, int);
        typedef saga::task (Derived::*async_link_type)(saga::url, saga::url, int);

        SAGA_REGISTER_MEMBER_EX(retval, info, base_cpi, Derived, link, prefs,
            base_sync_link_type, base_async_link_type,
            sync_link_type, async_link_type);

        typedef void (base_cpi::*base_sync_move_type)(saga::impl::void_t&, saga::url, saga::url, int);
        typedef saga::task (base_cpi::*base_async_move_type)(saga::url, saga::url, int);
        typedef void (Derived::*sync_move_type)(saga::impl::void_t&, saga::url, saga::url, int);
        typedef saga::task (Derived::*async_move_type)(saga::url, saga::url, int);

        SAGA_REGISTER_MEMBER_EX(retval, info, base_cpi, Derived, move, prefs,
            base_sync_move_type, base_async_move_type,
            sync_move_type, async_move_type);

        typedef void (base_cpi::*base_sync_remove_type)(saga::impl::void_t&, saga::url, int);
        typedef saga::task (base_cpi::*base_async_remove_type)(saga::url, int);
        typedef void (Derived::*sync_remove_type)(saga::impl::void_t&, saga::url, int);
        typedef saga::task (Derived::*async_remove_type)(saga::url, int);

        SAGA_REGISTER_MEMBER_EX(retval, info, base_cpi, Derived, remove, prefs,
            base_sync_remove_type, base_async_remove_type,
            sync_remove_type, async_remove_type);

        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, copy_wildcard, prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, link_wildcard, prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, move_wildcard, prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, remove_wildcard, prefs);

        typedef void (base_cpi::*base_sync_open_type)(saga::name_space::entry&, saga::url, int);
        typedef saga::task (base_cpi::*base_async_open_type)(saga::url, int);
        typedef void (Derived::*sync_open_type)(saga::name_space::entry&, saga::url, int);
        typedef saga::task (Derived::*async_open_type)(saga::url, int);

        SAGA_REGISTER_MEMBER_EX(retval, info, base_cpi, Derived, open, prefs,
            base_sync_open_type, base_async_open_type,
            sync_open_type, async_open_type);

        typedef void (base_cpi::*base_sync_open_dir_type)(saga::name_space::directory&, saga::url, int);
        typedef saga::task (base_cpi::*base_async_open_dir_type)(saga::url, int);
        typedef void (Derived::*sync_open_dir_type)(saga::name_space::directory&, saga::url, int);
        typedef saga::task (Derived::*async_open_dir_type)(saga::url, int);

        SAGA_REGISTER_MEMBER_EX(retval, info, base_cpi, Derived, open_dir, prefs,
            base_sync_open_dir_type, base_async_open_dir_type,
            sync_open_dir_type, async_open_dir_type);

        typedef void (base_cpi::*base_sync_find_type)(std::vector<saga::url>&, std::string, int);
        typedef saga::task (base_cpi::*base_async_find_type)(std::string, int);
        typedef void (Derived::*sync_find_type)(std::vector<saga::url>&, std::string, int);
        typedef saga::task (Derived::*async_find_type)(std::string, int);

        SAGA_REGISTER_MEMBER_EX(retval, info, base_cpi, Derived, find, prefs,
            base_sync_find_type, base_async_find_type,
            sync_find_type, async_find_type);

        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, permissions_allow, prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, permissions_deny, prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, permissions_allow_wildcard, prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, permissions_deny_wildcard, prefs);

        infos.push_back(info);

        SAGA_LOG(SAGA_VERBOSE_LEVEL_BLURB)
            << "end register_namespace_dir_functions";

        return retval;    // is true if at least one function got registered
    }

    /////////////////////////////////////////////////////////////////////////////
    template <typename Derived, typename Mutex = boost::recursive_mutex>
    class namespace_dir_cpi 
        : public saga::impl::v1_0::namespace_dir_cpi
    {
    protected:
        typedef saga::impl::v1_0::namespace_dir_cpi base_type;
        typedef saga::impl::proxy proxy;
        typedef saga::impl::v1_0::cpi cpi;
        typedef saga::impl::v1_0::cpi_info cpi_info;
        typedef saga::impl::v1_0::preference_type preference_type;
        typedef Mutex mutex_type;
        
        mutex_type mtx_;     // secure access to cpi_instance_data

    public:
        namespace_dir_cpi (proxy * p, cpi_info const& info, 
              TR1::shared_ptr<saga::adaptor> adaptor, cpi::flags flags) 
            : saga::impl::v1_0::namespace_dir_cpi (p, info, adaptor, flags) 
        { 
        }
         
        ~namespace_dir_cpi (void) 
        {
        }

        ///////////////////////////////////////////////////////////////////////
        //  instance data
        //    data associated with the API object instance, i.e. visible to all 
        //    CPI object instances associated with the API object instance 
        //    regardless of the shared library (adaptor) they are implemented in.
        typedef saga::adaptors::v1_0::namespace_dir_cpi_instance_data 
            ns_dir_instance_data;
        friend class saga::adaptors::instance_data<ns_dir_instance_data>;
        typedef      saga::adaptors::instance_data<ns_dir_instance_data> 
            instance_data;

        ///////////////////////////////////////////////////////////////////////
        // generic factory function
        static cpi* cpi_maker (proxy* p, cpi_info const& info,
            saga::ini::ini const& glob_ini, saga::ini::ini const& adap_ini,
            TR1::shared_ptr<saga::adaptor> adaptor)
        {
            return new Derived (p, info, glob_ini, adap_ini, adaptor);
        }

        ///////////////////////////////////////////////////////////////////////
        // register implemented functions
        static void 
        register_cpi(std::vector<cpi_info>& infos, preference_type prefs, 
            saga::uuid adaptor_uuid, 
            std::string const& cpi_name = saga::adaptors::namespace_dir_cpi)
        {
            // register namespace_entry CPI functions
            saga::uuid cpi_uuid;
            register_namespace_entry_functions<Derived>(infos, 
                &saga::adaptors::v1_0::namespace_dir_cpi<Derived>::cpi_maker, 
                prefs, cpi_uuid, adaptor_uuid, cpi_name);

            // register namespace_dir CPI functions
            register_namespace_dir_functions<Derived>(infos,
                &saga::adaptors::v1_0::namespace_dir_cpi<Derived>::cpi_maker, 
                prefs, cpi_uuid, adaptor_uuid, cpi_name);

            // register attribute CPI functions
            register_attribute_functions<Derived>(infos, 
                &saga::adaptors::v1_0::namespace_dir_cpi<Derived>::cpi_maker, 
                prefs, cpi_uuid, adaptor_uuid, cpi_name);

            // register permissions CPI functions
            register_permissions_functions<Derived>(infos, 
                &saga::adaptors::v1_0::permissions_cpi<Derived>::cpi_maker, 
                prefs, cpi_uuid, adaptor_uuid, cpi_name);
        }

        ///////////////////////////////////////////////////////////////////////
        // generic up-cast of the impl pointer       
        TR1::shared_ptr<Derived> shared_from_this()
        {
            return TR1::static_pointer_cast<Derived>(
                this->base_type::shared_from_this());
        }

    }; // class namespace_dir_cpi

///////////////////////////////////////////////////////////////////////////////
}}} // namespace saga::adaptors::v1_0

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif
#endif // !defined(SAGA_DEBUG)

#endif // SAGA_IMPL_PACKAGES_DATA_NAMESPACE_NAMESPACEDIR_CPI_HPP

