#ifndef SAGA_IMPL_PACKAGES_DATA_REPLICA_LOGICAL_FILE_CPI_HPP
#define SAGA_IMPL_PACKAGES_DATA_REPLICA_LOGICAL_FILE_CPI_HPP

#if defined(__WAVE__)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/file_cpi.hpp")
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
#include <saga/saga/replica.hpp>

#include <saga/saga/adaptors/instance_data.hpp>
#include <saga/saga/adaptors/packages/logical_file_cpi_instance_data.hpp>
#include <saga/saga/adaptors/attribute_cache_cpi.hpp>

#include <saga/impl/call.hpp>
#include <saga/impl/config.hpp>
#include <saga/impl/engine/register_members.hpp>

#include <saga/impl/packages/namespace/namespace_entry_cpi.hpp>

#ifdef SAGA_DEBUG
#include <saga/impl/packages/namespace/preprocessed/locical_file_cpi.hpp>
#else

#if defined(__WAVE__)
#pragma wave option(output: "preprocessed/locical_file_cpi.hpp")
#endif
///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl { namespace v1_0 {

    /**
     * This class declares all functions of "locical_file" defined in the SAGA API.
     * It extends the base class saga::cpi
     *
     * @see For more information, read the GGF SAGA API Specification
     */
    class logical_file_cpi 
    :   public namespace_entry_cpi
    {
    public:
       
        logical_file_cpi (proxy * p, cpi_info const& info, 
                TR1::shared_ptr<saga::adaptor> adaptor, cpi::flags flags) 
            : namespace_entry_cpi (p, info, adaptor, flags) 
        { 
        }
         
        virtual ~logical_file_cpi (void) 
        { 
        }

        cpi::type get_type() const { return cpi::LogicalFile; }

        // logical file CPI
        SAGA_CALL_CPI_DECL_VIRT_1(logical_file_cpi, saga::impl::void_t, add_location, saga::url)
        SAGA_CALL_CPI_DECL_VIRT_1(logical_file_cpi, saga::impl::void_t, remove_location, saga::url)
        SAGA_CALL_CPI_DECL_VIRT_2(logical_file_cpi, saga::impl::void_t, update_location, saga::url, saga::url)
        SAGA_CALL_CPI_DECL_VIRT_0(logical_file_cpi, std::vector<saga::url>, list_locations)
        SAGA_CALL_CPI_DECL_VIRT_2(logical_file_cpi, saga::impl::void_t, replicate, saga::url, int)

    }; // class logical_file_cpi

///////////////////////////////////////////////////////////////////////////////
}}} // namespace saga::impl::v1_0

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace adaptors { namespace v1_0 {

///////////////////////////////////////////////////////////////////////////////
// register logical_file CPI functions
    template <typename Derived>
    inline bool
    register_logical_file_functions(
        std::vector<saga::impl::v1_0::cpi_info>& infos, 
        saga::impl::v1_0::cpi::maker_type maker, 
        saga::impl::v1_0::preference_type const& prefs, 
        saga::uuid const& cpi_uuid, saga::uuid const& adaptor_uuid, 
        std::string const& cpi_name)
    {
        bool retval = false;
        saga::impl::v1_0::cpi_info info(saga::adaptors::logical_file_cpi, 
            cpi_name, maker, prefs, cpi_uuid, adaptor_uuid);
          
        SAGA_LOG(SAGA_VERBOSE_LEVEL_BLURB)
            << "begin register_logical_file_functions: " 
            << adaptor_uuid.string() << ":";

        typedef saga::impl::v1_0::logical_file_cpi base_cpi;
        
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, add_location, prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, remove_location, prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, update_location, prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, list_locations, prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, replicate, prefs);
      
        infos.push_back(info);

        SAGA_LOG(SAGA_VERBOSE_LEVEL_BLURB)
            << "end register_logical_file_functions";

        return retval;    // is true if at least one function got registered
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename Derived, typename Mutex = boost::recursive_mutex>
    class logical_file_cpi
        : public attribute_cache_cpi<Derived, saga::impl::v1_0::logical_file_cpi>
    {
    protected:
        typedef attribute_cache_cpi<Derived, saga::impl::v1_0::logical_file_cpi>
            base_type;
        typedef saga::impl::proxy proxy;
        typedef saga::impl::v1_0::cpi cpi;
        typedef saga::impl::v1_0::cpi_info cpi_info;
        typedef saga::impl::v1_0::preference_type preference_type;
        typedef Mutex mutex_type;

        mutex_type mtx_;     // secure access to cpi_instance_data

    public:
        logical_file_cpi (proxy * p, cpi_info const& info, 
                TR1::shared_ptr<saga::adaptor> adaptor, cpi::flags flags) 
            : base_type(p, info, adaptor, flags) 
        { 
        }
         
        virtual ~logical_file_cpi (void) 
        { 
        }

        ///////////////////////////////////////////////////////////////////////
        //  instance data
        //    data associated with the API object instance, i.e. visible to all 
        //    CPI object instances associated with the API object instance 
        //    regardless of the shared library (adaptor) they are implemented in.
        typedef saga::adaptors::v1_0::logical_file_cpi_instance_data 
            logical_file_instance_data;
        friend class saga::adaptors::instance_data<logical_file_instance_data>;
        typedef      saga::adaptors::instance_data<logical_file_instance_data> 
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
                &saga::adaptors::v1_0::logical_file_cpi<Derived>::cpi_maker, 
                prefs, cpi_uuid, adaptor_uuid, saga::adaptors::logical_file_cpi);

            // register logical_file CPI functions
            register_logical_file_functions<Derived>(infos,
                &saga::adaptors::v1_0::logical_file_cpi<Derived>::cpi_maker, 
                prefs, cpi_uuid, adaptor_uuid, saga::adaptors::logical_file_cpi);
              
            // register attribute CPI functions
            register_attribute_functions<Derived>(infos,
                &saga::adaptors::v1_0::logical_file_cpi<Derived>::cpi_maker, 
                prefs, cpi_uuid, adaptor_uuid, saga::adaptors::logical_file_cpi);

            // register permissions CPI functions
            register_permissions_functions<Derived>(infos, 
                &saga::adaptors::v1_0::permissions_cpi<Derived>::cpi_maker, 
                prefs, cpi_uuid, adaptor_uuid, saga::adaptors::logical_file_cpi);
        }

        ///////////////////////////////////////////////////////////////////////
        // generic up-cast of the impl pointer       
        TR1::shared_ptr<Derived> shared_from_this()
        {
            return TR1::static_pointer_cast<Derived>(
                this->base_type::shared_from_this());
        }

    }; // class logical_file_cpi
    
///////////////////////////////////////////////////////////////////////////////
}}} // namespace saga::adaptors::v1_0

#if defined(__WAVE__)
#pragma wave option(output: null)
#endif

#endif // !defined(SAGA_DEBUG)

#endif // SAGA_IMPL_PACKAGES_DATA_REPLICA_LOGICAL_FILE_CPI_HPP

