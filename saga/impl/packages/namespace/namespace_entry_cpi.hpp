#ifndef SAGA_IMPL_PACKAGES_DATA_NAMESPACE_NAMESPACEENTRY_CPI_HPP
#define SAGA_IMPL_PACKAGES_DATA_NAMESPACE_NAMESPACEENTRY_CPI_HPP

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/namespace_entry_cpi.hpp")
#endif
//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
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

#include <saga/saga/adaptors/instance_data.hpp>
#include <saga/saga/adaptors/packages/namespace_entry_cpi_instance_data.hpp>
#include <saga/saga/adaptors/permissions_cpi.hpp>

#include <saga/impl/call.hpp>
#include <saga/impl/config.hpp>
#include <saga/impl/engine/register_members.hpp>

#ifdef SAGA_DEBUG
#include <saga/impl/packages/namespace/preprocessed/namespace_entry_cpi.hpp>
#else

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: "preprocessed/namespace_entry_cpi.hpp")
#endif
///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl { namespace v1_0 {

    ///////////////////////////////////////////////////////////////////////////
    class namespace_entry_cpi
        : public permissions_cpi
    {
    public:
      namespace_entry_cpi (proxy * p, cpi_info const& info,
            TR1::shared_ptr <saga::adaptor> adaptor, cpi::flags flags)
        : permissions_cpi (p, info, adaptor, flags)
      {
      }

      ~namespace_entry_cpi (void)
      {
      }

      cpi::type get_type() const { return cpi::NSEntry; }

      // basic properties: get url
      SAGA_CALL_CPI_DECL_VIRT_0(namespace_entry_cpi, saga::url, get_url)
      SAGA_CALL_CPI_DECL_VIRT_0(namespace_entry_cpi, saga::url, get_cwd)
      SAGA_CALL_CPI_DECL_VIRT_0(namespace_entry_cpi, saga::url, get_name)

      // navigation/query methods
      SAGA_CALL_CPI_DECL_VIRT_0(namespace_entry_cpi, saga::url, read_link)
      SAGA_CALL_CPI_DECL_VIRT_0(namespace_entry_cpi, bool, is_dir)
      SAGA_CALL_CPI_DECL_VIRT_0(namespace_entry_cpi, bool, is_entry)
      SAGA_CALL_CPI_DECL_VIRT_0(namespace_entry_cpi, bool, is_link)

      // entry management methods
      SAGA_CALL_CPI_DECL_VIRT_2(namespace_entry_cpi, saga::impl::void_t, copy, saga::url, int)
      SAGA_CALL_CPI_DECL_VIRT_2(namespace_entry_cpi, saga::impl::void_t, link, saga::url, int)
      SAGA_CALL_CPI_DECL_VIRT_2(namespace_entry_cpi, saga::impl::void_t, move, saga::url, int)
      SAGA_CALL_CPI_DECL_VIRT_1(namespace_entry_cpi, saga::impl::void_t, remove, int)
      SAGA_CALL_CPI_DECL_VIRT_1(namespace_entry_cpi, saga::impl::void_t, close, double)

      // permissions with flags
      SAGA_CALL_CPI_DECL_VIRT_3(namespace_entry_cpi, saga::impl::void_t, permissions_allow, std::string, int, int)
      SAGA_CALL_CPI_DECL_VIRT_3(namespace_entry_cpi, saga::impl::void_t, permissions_deny, std::string, int, int)

    }; // class namespace_entry_cpi

///////////////////////////////////////////////////////////////////////////////
}}} // namespace saga::impl::v1_0

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace adaptors { namespace v1_0 {

///////////////////////////////////////////////////////////////////////////////
//  register namespace_entry CPI functions
    template <typename Derived>
    inline bool
    register_namespace_entry_functions(
        std::vector<saga::impl::v1_0::cpi_info>& infos,
        saga::impl::v1_0::cpi::maker_type maker,
        saga::impl::v1_0::preference_type const& prefs,
        saga::uuid const& cpi_uuid, saga::uuid const& adaptor_uuid,
        std::string const& cpi_name)
    {
        bool retval = false;
        saga::impl::v1_0::cpi_info info(adaptors::namespace_entry_cpi,
            cpi_name, maker, prefs, cpi_uuid, adaptor_uuid);

        SAGA_LOG(SAGA_VERBOSE_LEVEL_BLURB)
            << "begin register_namespace_entry_functions (" << cpi_name << "): "
            << adaptor_uuid.string() << ":";

        typedef saga::impl::v1_0::namespace_entry_cpi base_cpi;

        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, get_url, prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, get_cwd, prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, get_name, prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, close, prefs);

        // register ambiguous cpi functions
        typedef void (base_cpi::*base_sync_read_link_type)(saga::url&);
        typedef saga::task (base_cpi::*base_async_read_link_type)();
        typedef void (Derived::*sync_read_link_type)(saga::url&);
        typedef saga::task (Derived::*async_read_link_type)();

        SAGA_REGISTER_MEMBER_EX(retval, info, base_cpi, Derived, read_link, prefs,
            base_sync_read_link_type, base_async_read_link_type,
            sync_read_link_type, async_read_link_type);

        typedef void (base_cpi::*base_sync_is_dir_type)(bool&);
        typedef saga::task (base_cpi::*base_async_is_dir_type)();
        typedef void (Derived::*sync_is_dir_type)(bool&);
        typedef saga::task (Derived::*async_is_dir_type)();

        SAGA_REGISTER_MEMBER_EX(retval, info, base_cpi, Derived, is_dir, prefs,
            base_sync_is_dir_type, base_async_is_dir_type,
            sync_is_dir_type, async_is_dir_type);

        typedef void (base_cpi::*base_sync_is_entry_type)(bool&);
        typedef saga::task (base_cpi::*base_async_is_entry_type)();
        typedef void (Derived::*sync_is_entry_type)(bool&);
        typedef saga::task (Derived::*async_is_entry_type)();

        SAGA_REGISTER_MEMBER_EX(retval, info, base_cpi, Derived, is_entry, prefs,
            base_sync_is_entry_type, base_async_is_entry_type,
            sync_is_entry_type, async_is_entry_type);

        typedef void (base_cpi::*base_sync_is_link_type)(bool&);
        typedef saga::task (base_cpi::*base_async_is_link_type)();
        typedef void (Derived::*sync_is_link_type)(bool&);
        typedef saga::task (Derived::*async_is_link_type)();

        SAGA_REGISTER_MEMBER_EX(retval, info, base_cpi, Derived, is_link, prefs,
            base_sync_is_link_type, base_async_is_link_type,
            sync_is_link_type, async_is_link_type);

        typedef void (base_cpi::*base_sync_copy_type)(saga::impl::void_t&, saga::url, int);
        typedef saga::task (base_cpi::*base_async_copy_type)(saga::url, int);
        typedef void (Derived::*sync_copy_type)(saga::impl::void_t&, saga::url, int);
        typedef saga::task (Derived::*async_copy_type)(saga::url, int);

        SAGA_REGISTER_MEMBER_EX(retval, info, base_cpi, Derived, copy, prefs,
            base_sync_copy_type, base_async_copy_type,
            sync_copy_type, async_copy_type);

        typedef void (base_cpi::*base_sync_link_type)(saga::impl::void_t&, saga::url, int);
        typedef saga::task (base_cpi::*base_async_link_type)(saga::url, int);
        typedef void (Derived::*sync_link_type)(saga::impl::void_t&, saga::url, int);
        typedef saga::task (Derived::*async_link_type)(saga::url, int);

        SAGA_REGISTER_MEMBER_EX(retval, info, base_cpi, Derived, link, prefs,
            base_sync_link_type, base_async_link_type,
            sync_link_type, async_link_type);

        typedef void (base_cpi::*base_sync_move_type)(saga::impl::void_t&, saga::url, int);
        typedef saga::task (base_cpi::*base_async_move_type)(saga::url, int);
        typedef void (Derived::*sync_move_type)(saga::impl::void_t&, saga::url, int);
        typedef saga::task (Derived::*async_move_type)(saga::url, int);

        SAGA_REGISTER_MEMBER_EX(retval, info, base_cpi, Derived, move, prefs,
            base_sync_move_type, base_async_move_type,
            sync_move_type, async_move_type);

        typedef void (base_cpi::*base_sync_remove_type)(saga::impl::void_t&, int);
        typedef saga::task (base_cpi::*base_async_remove_type)(int);
        typedef void (Derived::*sync_remove_type)(saga::impl::void_t&, int);
        typedef saga::task (Derived::*async_remove_type)(int);

        SAGA_REGISTER_MEMBER_EX(retval, info, base_cpi, Derived, remove, prefs,
            base_sync_remove_type, base_async_remove_type,
            sync_remove_type, async_remove_type);

        typedef void (base_cpi::*base_sync_permissions_allow_type)(saga::impl::void_t&, std::string, int, int);
        typedef saga::task (base_cpi::*base_async_permissions_allow_type)(std::string, int, int);
        typedef void (Derived::*sync_permissions_allow_type)(saga::impl::void_t&, std::string, int, int);
        typedef saga::task (Derived::*async_permissions_allow_type)(std::string, int, int);

        SAGA_REGISTER_MEMBER_EX(retval, info, base_cpi, Derived, permissions_allow, prefs,
            base_sync_permissions_allow_type, base_async_permissions_allow_type,
            sync_permissions_allow_type, async_permissions_allow_type);

        typedef void (base_cpi::*base_sync_permissions_deny_type)(saga::impl::void_t&, std::string, int, int);
        typedef saga::task (base_cpi::*base_async_permissions_deny_type)(std::string, int, int);
        typedef void (Derived::*sync_permissions_deny_type)(saga::impl::void_t&, std::string, int, int);
        typedef saga::task (Derived::*async_permissions_deny_type)(std::string, int, int);

        SAGA_REGISTER_MEMBER_EX(retval, info, base_cpi, Derived, permissions_deny, prefs,
            base_sync_permissions_deny_type, base_async_permissions_deny_type,
            sync_permissions_deny_type, async_permissions_deny_type);

        infos.push_back(info);

        SAGA_LOG(SAGA_VERBOSE_LEVEL_BLURB)
            << "end register_namespace_entry_functions";

        return retval;    // is true if at least one function got registered
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename Derived, typename Mutex = boost::recursive_mutex>
    class namespace_entry_cpi
        : public saga::impl::v1_0::namespace_entry_cpi
    {
    protected:
        typedef saga::impl::v1_0::namespace_entry_cpi base_type;
        typedef saga::impl::proxy proxy;
        typedef saga::impl::v1_0::cpi cpi;
        typedef saga::impl::v1_0::cpi_info cpi_info;
        typedef saga::impl::v1_0::preference_type preference_type;
        typedef Mutex mutex_type;

        mutex_type mtx_;     // secure access to cpi_instance_data

    public:
        namespace_entry_cpi (proxy* p, cpi_info const& info,
                TR1::shared_ptr <saga::adaptor> adaptor, cpi::flags flags)
            : saga::impl::v1_0::namespace_entry_cpi (p, info, adaptor, flags)
        {
        }

        ~namespace_entry_cpi (void)
        {
        }

        ///////////////////////////////////////////////////////////////////////
        //  instance data
        //    data associated with the API object instance, i.e. visible to all
        //    CPI object instances associated with the API object instance
        //    regardless of the shared library (adaptor) they are implemented in.
        typedef saga::adaptors::v1_0::namespace_entry_cpi_instance_data
            ns_instance_data;
        friend class saga::adaptors::instance_data<ns_instance_data>;
        typedef      saga::adaptors::instance_data<ns_instance_data>
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
            std::string const& cpi_name = saga::adaptors::namespace_entry_cpi)
        {
            // register namespace_entry CPI functions
            saga::uuid cpi_uuid;
            register_namespace_entry_functions<Derived>(infos,
                &saga::adaptors::v1_0::namespace_entry_cpi<Derived>::cpi_maker,
                prefs, cpi_uuid, adaptor_uuid, cpi_name);

            // register attribute CPI functions
            register_attribute_functions<Derived>(infos,
                &saga::adaptors::v1_0::namespace_entry_cpi<Derived>::cpi_maker,
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
    };

///////////////////////////////////////////////////////////////////////////////
}}} // namespace saga::adaptors::v1_0

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif
#endif // !defined(SAGA_DEBUG)

#endif // SAGA_IMPL_PACKAGES_DATA_NAMESPACE_NAMESPACEENTRY_CPI_HPP

