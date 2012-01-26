#ifndef SAGA_IMPL_PACKAGES_CPR_CHECKPOINT_CPI_HPP
#define SAGA_IMPL_PACKAGES_CPR_CHECKPOINT_CPI_HPP

#if defined(__WAVE__)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/cpr_checkpoint_cpi.hpp")
#endif
//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if defined(__WAVE__)
#pragma wave option(output: null)
#endif

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif 

#include <saga/saga/util.hpp>
#include <saga/saga/types.hpp>
#include <saga/saga/exception.hpp>
#include <saga/saga/cpr.hpp>
#include <saga/saga/filesystem.hpp>

#include <saga/saga/adaptors/instance_data.hpp>
#include <saga/saga/adaptors/packages/cpr_checkpoint_cpi_instance_data.hpp>
#include <saga/saga/adaptors/attribute_cache_cpi.hpp>

#include <saga/impl/call.hpp>
#include <saga/impl/config.hpp>
#include <saga/impl/engine/register_members.hpp>

#include <saga/impl/packages/namespace/namespace_entry_cpi.hpp>

#ifdef SAGA_DEBUG
#include <saga/impl/packages/namespace/preprocessed/cpr_checkpoint_cpi.hpp>
#else

#if defined(__WAVE__)
#pragma wave option(output: "preprocessed/cpr_checkpoint_cpi.hpp")
#endif
///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl { namespace v1_0 {

    /**
     * This class declares all functions of "cpr_checkpoint" defined in the SAGA API.
     * It extends the base class saga::cpi
     *
     * @see For more information, read the GGF SAGA API Specification
     */
    class cpr_checkpoint_cpi 
    :   public namespace_entry_cpi
    {
    public:
       
        cpr_checkpoint_cpi (proxy * p, cpi_info const& info, 
                TR1::shared_ptr<saga::adaptor> adaptor, cpi::flags flags) 
            : namespace_entry_cpi (p, info, adaptor, flags) 
        { 
        }
         
        virtual ~cpr_checkpoint_cpi (void) 
        { 
        }

        cpi::type get_type() const { return cpi::CPRCheckpoint; }

        // cpr CPI
        SAGA_CALL_CPI_DECL_VIRT_2(cpr_checkpoint_cpi, saga::impl::void_t    , set_parent,      saga::url, int)
        SAGA_CALL_CPI_DECL_VIRT_1(cpr_checkpoint_cpi, saga::url             , get_parent,      int)
        SAGA_CALL_CPI_DECL_VIRT_0(cpr_checkpoint_cpi, int                   , get_file_num)
        SAGA_CALL_CPI_DECL_VIRT_0(cpr_checkpoint_cpi, std::vector<saga::url>, list_files)
        SAGA_CALL_CPI_DECL_VIRT_1(cpr_checkpoint_cpi, int                   , add_file,        saga::url)
        SAGA_CALL_CPI_DECL_VIRT_1(cpr_checkpoint_cpi, saga::url             , get_file,        int)
        SAGA_CALL_CPI_DECL_VIRT_2(cpr_checkpoint_cpi, saga::filesystem::file, open_file,       saga::url, int)
        SAGA_CALL_CPI_DECL_VIRT_2(cpr_checkpoint_cpi, saga::filesystem::file, open_file_idx,   int,       int)
        SAGA_CALL_CPI_DECL_VIRT_1(cpr_checkpoint_cpi, saga::impl::void_t    , remove_file,     saga::url)
        SAGA_CALL_CPI_DECL_VIRT_1(cpr_checkpoint_cpi, saga::impl::void_t    , remove_file_idx, int)
        SAGA_CALL_CPI_DECL_VIRT_2(cpr_checkpoint_cpi, saga::impl::void_t    , update_file,     saga::url, saga::url)
        SAGA_CALL_CPI_DECL_VIRT_2(cpr_checkpoint_cpi, saga::impl::void_t    , update_file_idx, int,       saga::url)
        SAGA_CALL_CPI_DECL_VIRT_2(cpr_checkpoint_cpi, saga::impl::void_t    , stage_file,      saga::url, saga::url)
        SAGA_CALL_CPI_DECL_VIRT_2(cpr_checkpoint_cpi, saga::impl::void_t    , stage_file_idx,  int,       saga::url)
        SAGA_CALL_CPI_DECL_VIRT_1(cpr_checkpoint_cpi, saga::impl::void_t    , stage_file_all,  saga::url)

    }; // class cpr_checkpoint_cpi

///////////////////////////////////////////////////////////////////////////////
}}} // namespace saga::impl::v1_0

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace adaptors { namespace v1_0 {

///////////////////////////////////////////////////////////////////////////////
// register cpr_checkpoint CPI functions
    template <typename Derived>
    inline bool
    register_cpr_checkpoint_functions(
        std::vector<saga::impl::v1_0::cpi_info>& infos, 
        saga::impl::v1_0::cpi::maker_type maker, 
        saga::impl::v1_0::preference_type const& prefs, 
        saga::uuid const& cpi_uuid, saga::uuid const& adaptor_uuid, 
        std::string const& cpi_name)
    {
        bool retval = false;
        saga::impl::v1_0::cpi_info info(saga::adaptors::cpr_checkpoint_cpi, 
            cpi_name, maker, prefs, cpi_uuid, adaptor_uuid);
          
        SAGA_LOG(SAGA_VERBOSE_LEVEL_BLURB)
            << "begin register_cpr_checkpoint_functions (" << cpi_name << "): " 
            << adaptor_uuid.string() << ":";

        typedef saga::impl::v1_0::cpr_checkpoint_cpi base_cpi;
        
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, set_parent     , prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, get_parent     , prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, get_file_num   , prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, list_files     , prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, add_file       , prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, get_file       , prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, open_file      , prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, open_file_idx  , prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, remove_file    , prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, remove_file_idx, prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, update_file    , prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, update_file_idx, prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, stage_file     , prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, stage_file_idx , prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, stage_file_all , prefs);
      
        infos.push_back(info);

        SAGA_LOG(SAGA_VERBOSE_LEVEL_BLURB)
            << "end register_cpr_checkpoint_functions";

        return retval;    // is true if at least one function got registered
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename Derived, typename Mutex = boost::recursive_mutex>
    class cpr_checkpoint_cpi
        : public attribute_cache_cpi<Derived, saga::impl::v1_0::cpr_checkpoint_cpi>
    {
    protected:
        typedef attribute_cache_cpi<Derived, saga::impl::v1_0::cpr_checkpoint_cpi>
            base_type;
        typedef saga::impl::proxy proxy;
        typedef saga::impl::v1_0::cpi cpi;
        typedef saga::impl::v1_0::cpi_info cpi_info;
        typedef saga::impl::v1_0::preference_type preference_type;
        typedef Mutex mutex_type;

        mutex_type mtx_;     // secure access to cpi_instance_data

    public:
        cpr_checkpoint_cpi (proxy * p, cpi_info const& info, 
                TR1::shared_ptr<saga::adaptor> adaptor, cpi::flags flags) 
            : base_type(p, info, adaptor, flags) 
        { 
        }
         
        virtual ~cpr_checkpoint_cpi (void) 
        { 
        }

        ///////////////////////////////////////////////////////////////////////
        //  instance data
        //    data associated with the API object instance, i.e. visible to all 
        //    CPI object instances associated with the API object instance 
        //    regardless of the shared library (adaptor) they are implemented in.
        typedef saga::adaptors::v1_0::cpr_checkpoint_cpi_instance_data 
            cpr_checkpoint_instance_data;
        friend class saga::adaptors::instance_data<cpr_checkpoint_instance_data>;
        typedef      saga::adaptors::instance_data<cpr_checkpoint_instance_data> 
            instance_data;

        ///////////////////////////////////////////////////////////////////////
        // generic factory function
        static cpi* cpi_maker (proxy                * p, 
                               cpi_info       const & info,
                               saga::ini::ini const & glob_ini, 
                               saga::ini::ini const & adap_ini,
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
            // register namespace_entry CPI functions
            saga::uuid cpi_uuid;
            register_namespace_entry_functions<Derived>(infos, 
                &saga::adaptors::v1_0::cpr_checkpoint_cpi<Derived>::cpi_maker, 
                prefs, cpi_uuid, adaptor_uuid, saga::adaptors::cpr_checkpoint_cpi);

            // register cpr_checkpoint CPI functions
            register_cpr_checkpoint_functions<Derived>(infos,
                &saga::adaptors::v1_0::cpr_checkpoint_cpi<Derived>::cpi_maker, 
                prefs, cpi_uuid, adaptor_uuid, saga::adaptors::cpr_checkpoint_cpi);
              
            // register attribute CPI functions
            register_attribute_functions<Derived>(infos,
                &saga::adaptors::v1_0::cpr_checkpoint_cpi<Derived>::cpi_maker, 
                prefs, cpi_uuid, adaptor_uuid, saga::adaptors::cpr_checkpoint_cpi);

            // register permissions CPI functions
            register_permissions_functions<Derived>(infos, 
                &saga::adaptors::v1_0::permissions_cpi<Derived>::cpi_maker, 
                prefs, cpi_uuid, adaptor_uuid, saga::adaptors::cpr_checkpoint_cpi);
        }

        ///////////////////////////////////////////////////////////////////////
        // generic up-cast of the impl pointer       
        TR1::shared_ptr<Derived> shared_from_this()
        {
            return TR1::static_pointer_cast<Derived>(
                this->base_type::shared_from_this());
        }

    }; // class cpr_checkpoint_cpi
    
///////////////////////////////////////////////////////////////////////////////
}}} // namespace saga::adaptors::v1_0

#if defined(__WAVE__)
#pragma wave option(output: null)
#endif

#endif // !defined(SAGA_DEBUG)

#endif // SAGA_IMPL_PACKAGES_CPR_CHECKPOINT_CPI_HPP

