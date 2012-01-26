//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_IMPL_ENGINE_PERMISSIONS_CPI_HPP
#define SAGA_IMPL_ENGINE_PERMISSIONS_CPI_HPP

#include <string>

#include <saga/saga/util.hpp>
#include <saga/saga/types.hpp>
#include <saga/saga/exception.hpp>

#include <saga/impl/call.hpp>
#include <saga/impl/config.hpp>
#include <saga/impl/engine/cpi.hpp>
#include <saga/impl/engine/register_members.hpp>
#include <saga/saga/adaptors/attribute_cpi.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl { namespace v1_0 {

    ///////////////////////////////////////////////////////////////////////////
    class permissions_cpi : public attribute_cpi
    {
    protected:
        typedef saga::impl::proxy proxy;
        typedef saga::impl::v1_0::cpi cpi;
        typedef saga::impl::v1_0::cpi_info cpi_info;
        typedef saga::impl::v1_0::preference_type preference_type;

    public:
        permissions_cpi(proxy* p, cpi_info const& info, 
                TR1::shared_ptr<saga::adaptor> adaptor, cpi::flags flags) 
            : attribute_cpi (p, info, adaptor, flags) 
        { 
        }
         
        ~permissions_cpi(void) 
        {
        }

        SAGA_CALL_CPI_DECL_VIRT_2(permissions_cpi, saga::impl::void_t, permissions_allow, std::string, int)
        SAGA_CALL_CPI_DECL_VIRT_2(permissions_cpi, saga::impl::void_t, permissions_deny, std::string, int)
        SAGA_CALL_CPI_DECL_VIRT_2(permissions_cpi, bool, permissions_check, std::string, int)
        SAGA_CALL_CPI_DECL_VIRT_0(permissions_cpi, std::string, get_owner)
        SAGA_CALL_CPI_DECL_VIRT_0(permissions_cpi, std::string, get_group)

    }; // class permissions_cpi

///////////////////////////////////////////////////////////////////////////////
}}} // namespace saga::impl::v1_0

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace adaptors { namespace v1_0 {

///////////////////////////////////////////////////////////////////////////////
// register permissions CPI functions
    template <typename Derived>
    inline bool
    register_permissions_functions(
        std::vector<saga::impl::v1_0::cpi_info>& infos, 
        saga::impl::v1_0::cpi::maker_type maker, 
        saga::impl::v1_0::preference_type const& prefs, 
        saga::uuid const& cpi_uuid, saga::uuid const& adaptor_uuid, 
        std::string const& cpi_name)
    {
        bool retval = false;
        saga::impl::v1_0::cpi_info info(saga::adaptors::permissions_cpi,
            cpi_name, maker, prefs, cpi_uuid, adaptor_uuid);
          
        SAGA_LOG(SAGA_VERBOSE_LEVEL_BLURB)
            << "begin register_permissions_functions: " 
            << adaptor_uuid.string() << ":";

        typedef saga::impl::v1_0::permissions_cpi base_cpi;

        typedef void (base_cpi::*base_sync_permissions_allow_type)(saga::impl::void_t&, std::string, int);
        typedef saga::task (base_cpi::*base_async_permissions_allow_type)(std::string, int);
        typedef void (Derived::*sync_permissions_allow_type)(saga::impl::void_t&, std::string, int);
        typedef saga::task (Derived::*async_permissions_allow_type)(std::string, int);

        SAGA_REGISTER_MEMBER_EX(retval, info, base_cpi, Derived, permissions_allow, prefs,
            base_sync_permissions_allow_type, base_async_permissions_allow_type,
            sync_permissions_allow_type, async_permissions_allow_type);

        typedef void (base_cpi::*base_sync_permissions_deny_type)(saga::impl::void_t&, std::string, int);
        typedef saga::task (base_cpi::*base_async_permissions_deny_type)(std::string, int);
        typedef void (Derived::*sync_permissions_deny_type)(saga::impl::void_t&, std::string, int);
        typedef saga::task (Derived::*async_permissions_deny_type)(std::string, int);

        SAGA_REGISTER_MEMBER_EX(retval, info, base_cpi, Derived, permissions_deny, prefs,
            base_sync_permissions_deny_type, base_async_permissions_deny_type,
            sync_permissions_deny_type, async_permissions_deny_type);

        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, permissions_check, prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, get_owner, prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, get_group, prefs);

        infos.push_back(info);

        SAGA_LOG(SAGA_VERBOSE_LEVEL_BLURB)
            << "end register_permissions_functions";

        return retval;    // is true if at least one function got registered
    }

    /////////////////////////////////////////////////////////////////////////////
    template <typename Derived>
    class permissions_cpi 
      : public saga::impl::v1_0::permissions_cpi
    {
    protected:
        typedef saga::impl::proxy proxy;
        typedef saga::impl::v1_0::cpi cpi;
        typedef saga::impl::v1_0::cpi_info cpi_info;
        typedef saga::impl::v1_0::preference_type preference_type;
        typedef saga::impl::v1_0::permissions_cpi base_type;
        
    public:
        permissions_cpi (proxy* p, cpi_info const& info, 
              TR1::shared_ptr<saga::adaptor> adaptor, cpi::flags flags) 
            : base_type(p, info, adaptor, flags) 
        { 
        }
         
        ~permissions_cpi (void) 
        {
        }

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
            saga::uuid adaptor_uuid)
        {
            // register attribute CPI functions
            saga::uuid cpi_uuid;
            register_permissions_functions<Derived>(infos, 
                &saga::adaptors::v1_0::permissions_cpi<Derived>::cpi_maker, 
                prefs, cpi_uuid, adaptor_uuid, saga::adaptors::permissions_cpi);
        }

        ///////////////////////////////////////////////////////////////////////
        // generic up-cast of the impl pointer       
        TR1::shared_ptr<Derived> shared_from_this()
        {
            return TR1::static_pointer_cast<Derived>(
                this->base_type::shared_from_this());
        }

    }; // class permissions_cpi

///////////////////////////////////////////////////////////////////////////////
}}}   // namespace saga::adaptors::v1_0

#endif 

