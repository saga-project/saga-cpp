#ifndef SAGA_IMPL_PACKAGES_DATA_ADVERT_ADVERT_CPI_HPP
#define SAGA_IMPL_PACKAGES_DATA_ADVERT_ADVERT_CPI_HPP

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/advert_cpi.hpp")
#endif
//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif 

#include <saga/saga/util.hpp>
#include <saga/saga/types.hpp>
#include <saga/saga/exception.hpp>
#include <saga/saga/advert.hpp>

#include <saga/saga/adaptors/instance_data.hpp>
#include <saga/saga/adaptors/packages/advert_cpi_instance_data.hpp>
#include <saga/saga/adaptors/attribute_cache_cpi.hpp>

#include <saga/impl/call.hpp>
#include <saga/impl/config.hpp>
#include <saga/impl/engine/register_members.hpp>

#include <saga/impl/packages/namespace/namespace_entry_cpi.hpp>

#ifdef SAGA_DEBUG
#include <saga/impl/packages/namespace/preprocessed/advert_cpi.hpp>
#else

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: "preprocessed/advert_cpi.hpp")
#endif
///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl { namespace v1_0 {

    /**
     * This class declares all functions of "advert" defined in the SAGA API.
     * It extends the base class saga::cpi
     *
     * @see For more information, read the OGF SAGA API Specification
     */
    class advert_cpi 
    :   public namespace_entry_cpi
    {
    public:
       
        advert_cpi (proxy * p, cpi_info const& info, 
                TR1::shared_ptr<saga::adaptor> adaptor, cpi::flags flags) 
            : namespace_entry_cpi (p, info, adaptor, flags) 
        { 
        }
         
        virtual ~advert_cpi (void) 
        { 
        }

        cpi::type get_type() const { return cpi::Advert; }

        // logical file CPI
        SAGA_CALL_CPI_DECL_VIRT_1(advert_cpi, saga::impl::void_t, store_object, saga::object)
        SAGA_CALL_CPI_DECL_VIRT_1(advert_cpi, saga::object, retrieve_object, saga::session)
        SAGA_CALL_CPI_DECL_VIRT_1(advert_cpi, saga::impl::void_t, store_string, std::string)
        SAGA_CALL_CPI_DECL_VIRT_0(advert_cpi, std::string, retrieve_string)

    }; // class advert_cpi

///////////////////////////////////////////////////////////////////////////////
}}} // namespace saga::impl::v1_0

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace adaptors { namespace v1_0 {

///////////////////////////////////////////////////////////////////////////////
// register advert CPI functions
    template <typename Derived>
    inline bool
    register_advert_functions(
        std::vector<saga::impl::v1_0::cpi_info>& infos, 
        saga::impl::v1_0::cpi::maker_type maker, 
        saga::impl::v1_0::preference_type const& prefs, 
        saga::uuid const& cpi_uuid, saga::uuid const& adaptor_uuid, 
        std::string const& cpi_name)
    {
        bool retval = false;
        saga::impl::v1_0::cpi_info info(saga::adaptors::advert_cpi, 
            cpi_name, maker, prefs, cpi_uuid, adaptor_uuid);
          
        SAGA_LOG(SAGA_VERBOSE_LEVEL_BLURB)
            << "begin register_advert_functions (" << cpi_name << "): " 
            << adaptor_uuid.string() << ":";

        typedef saga::impl::v1_0::advert_cpi base_cpi;
        
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, store_object, prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, retrieve_object, prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, store_string, prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, retrieve_string, prefs);
      
        infos.push_back(info);

        SAGA_LOG(SAGA_VERBOSE_LEVEL_BLURB)
            << "end register_advert_functions";

        return retval;    // is true if at least one function got registered
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename Derived, typename Mutex = boost::recursive_mutex>
    class advert_cpi
        : public attribute_cache_cpi<Derived, saga::impl::v1_0::advert_cpi>
    {
    protected:
        typedef attribute_cache_cpi<Derived, saga::impl::v1_0::advert_cpi>
            base_type;
        typedef saga::impl::proxy proxy;
        typedef saga::impl::v1_0::cpi cpi;
        typedef saga::impl::v1_0::cpi_info cpi_info;
        typedef saga::impl::v1_0::preference_type preference_type;
        typedef Mutex mutex_type;

        mutex_type mtx_;     // secure access to cpi_instance_data

    public:
        advert_cpi (proxy * p, cpi_info const& info, 
                TR1::shared_ptr<saga::adaptor> adaptor, cpi::flags flags) 
            : base_type(p, info, adaptor, flags) 
        { 
        }
         
        virtual ~advert_cpi (void) 
        { 
        }

        ///////////////////////////////////////////////////////////////////////
        //  instance data
        //    data associated with the API object instance, i.e. visible to all 
        //    CPI object instances associated with the API object instance 
        //    regardless of the shared library (adaptor) they are implemented in.
        typedef saga::adaptors::v1_0::advert_cpi_instance_data 
            advert_instance_data;
        friend class saga::adaptors::instance_data<advert_instance_data>;
        typedef      saga::adaptors::instance_data<advert_instance_data> 
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
                &saga::adaptors::v1_0::advert_cpi<Derived>::cpi_maker, 
                prefs, cpi_uuid, adaptor_uuid, saga::adaptors::advert_cpi);

            // register advert CPI functions
            register_advert_functions<Derived>(infos,
                &saga::adaptors::v1_0::advert_cpi<Derived>::cpi_maker, 
                prefs, cpi_uuid, adaptor_uuid, saga::adaptors::advert_cpi);
              
            // register attribute CPI functions
            register_attribute_functions<Derived>(infos,
                &saga::adaptors::v1_0::advert_cpi<Derived>::cpi_maker, 
                prefs, cpi_uuid, adaptor_uuid, saga::adaptors::advert_cpi);

            // register permissions CPI functions
            register_permissions_functions<Derived>(infos, 
                &saga::adaptors::v1_0::permissions_cpi<Derived>::cpi_maker, 
                prefs, cpi_uuid, adaptor_uuid, saga::adaptors::advert_cpi);
        }

        ///////////////////////////////////////////////////////////////////////
        // generic up-cast of the impl pointer       
        TR1::shared_ptr<Derived> shared_from_this()
        {
            return TR1::static_pointer_cast<Derived>(
                this->base_type::shared_from_this());
        }

    }; // class advert_cpi
    
///////////////////////////////////////////////////////////////////////////////
}}} // namespace saga::adaptors::v1_0

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif
#endif // !defined(SAGA_DEBUG)

#endif // SAGA_IMPL_PACKAGES_DATA_ADVERT_ADVERT_CPI_HPP

