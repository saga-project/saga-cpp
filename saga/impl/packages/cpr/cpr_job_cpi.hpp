#ifndef SAGA_IMPL_PACKAGES_CPR_CPR_JOB_CPI_HPP
#define SAGA_IMPL_PACKAGES_CPR_CPR_JOB_CPI_HPP

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/cpr_job_cpi.hpp")
#endif
//  Copyright (c) 2005-2009 Hartmut Kaiser
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
#include <saga/saga/cpr.hpp>

#include <saga/saga/adaptors/instance_data.hpp>
#include <saga/saga/adaptors/packages/cpr_job_cpi_instance_data.hpp>
#include <saga/saga/adaptors/attribute_cache_cpi.hpp>
#include <saga/saga/adaptors/attribute_cpi.hpp>

#include <saga/impl/call.hpp>
#include <saga/impl/config.hpp>
#include <saga/impl/engine/register_members.hpp>

#include <saga/impl/packages/job/job_cpi.hpp>

#ifdef SAGA_DEBUG
#include <saga/saga/packages/cpr/preprocessed/cpr_job_cpi.hpp>
#else

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: "preprocessed/cpr_job_cpi.hpp")
#endif
///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl { namespace v1_0 {

    /**
     * This class declares all functions of "service" defined in the SAGA API.
     * It extends the base class saga::cpi
     *
     * @see For more information, read the GGF SAGA API Specification
     */
    class cpr_job_cpi 
        : public job_cpi
    {
    public:
       
        cpr_job_cpi (proxy * p, cpi_info const& info, 
                TR1::shared_ptr<saga::adaptor> adaptor, cpi::flags flags) 
            : job_cpi (p, info, adaptor, flags) 
        { 
        }
         
        virtual ~cpr_job_cpi (void) 
        { 
        }

        cpi::type get_type() const { return cpi::CPRJob; }

        // cpr management methods
        SAGA_CALL_CPI_DECL_VIRT_1 (cpr_job_cpi, saga::impl::void_t,            checkpoint,    saga::url)
        SAGA_CALL_CPI_DECL_VIRT_1 (cpr_job_cpi, saga::impl::void_t,            recover,       saga::url)

        SAGA_CALL_CPI_DECL_VIRT_1 (cpr_job_cpi, saga::impl::void_t,            cpr_stage_in,  saga::url)
        SAGA_CALL_CPI_DECL_VIRT_1 (cpr_job_cpi, saga::impl::void_t,            cpr_stage_out, saga::url)

        SAGA_CALL_CPI_DECL_VIRT_0 (cpr_job_cpi, saga::url,               cpr_last)
        SAGA_CALL_CPI_DECL_VIRT_0 (cpr_job_cpi, std::vector <saga::url>, cpr_list)
        
    }; // class job_cpi

///////////////////////////////////////////////////////////////////////////////
}}} // namespace saga::impl::v1_0

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace adaptors { namespace v1_0 {

///////////////////////////////////////////////////////////////////////////////
// register cpr CPI functions
    template <typename Derived>
    inline bool
    register_cpr_job_functions(
        std::vector<saga::impl::v1_0::cpi_info>& infos, 
        saga::impl::v1_0::cpi::maker_type maker, 
        saga::impl::v1_0::preference_type const& prefs, 
        saga::uuid const& cpi_uuid, saga::uuid const& adaptor_uuid, 
        std::string const& cpi_name)
    {
        bool retval = false;
        saga::impl::v1_0::cpi_info info(saga::adaptors::cpr_job_cpi, cpi_name, 
            maker, prefs, cpi_uuid, adaptor_uuid);

        SAGA_LOG(SAGA_VERBOSE_LEVEL_BLURB)
            << "begin register_cpr_job_functions (" << cpi_name << "): " 
            << adaptor_uuid.string() << ":";

        typedef saga::impl::v1_0::cpr_job_cpi base_cpi;

        typedef void (base_cpi::*base_sync_checkpoint_type)(saga::impl::void_t&, saga::url);
        typedef saga::task (base_cpi::*base_async_checkpoint_type)(saga::url);
        typedef void (Derived::*sync_checkpoint_type)(saga::impl::void_t&, saga::url);
        typedef saga::task (Derived::*async_checkpoint_type)(saga::url);

        SAGA_REGISTER_MEMBER_EX(retval, info, base_cpi, Derived, checkpoint, prefs,
            base_sync_checkpoint_type, base_async_checkpoint_type,
            sync_checkpoint_type, async_checkpoint_type);

        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, recover,       prefs)

        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, cpr_stage_in,  prefs)
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, cpr_stage_out, prefs)

        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, cpr_last,      prefs)
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, cpr_list,      prefs)
      
        infos.push_back(info);

        SAGA_LOG(SAGA_VERBOSE_LEVEL_BLURB)
            << "end register_cpr_job_functions";

        return retval;    // is true if at least one function got registered
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename Derived, typename Mutex = boost::recursive_mutex>
    class cpr_job_cpi 
        : public attribute_cache_cpi<Derived, saga::impl::v1_0::cpr_job_cpi> 
    {
    protected:
        typedef attribute_cache_cpi<Derived, saga::impl::v1_0::cpr_job_cpi> 
            base_type;
        typedef saga::impl::proxy proxy;
        typedef saga::impl::v1_0::cpi cpi;
        typedef saga::impl::v1_0::cpi_info cpi_info;
        typedef saga::impl::v1_0::preference_type preference_type;
        typedef Mutex mutex_type;

        mutex_type mtx_;     // secure access to cpi_instance_data

    public:
        cpr_job_cpi (proxy * p, cpi_info const& info, 
                TR1::shared_ptr<saga::adaptor> adaptor, cpi::flags flags) 
        :   base_type(p, info, adaptor, flags) 
        { 
        }
         
        virtual ~cpr_job_cpi (void) 
        { 
        }

        ///////////////////////////////////////////////////////////////////////
        //  instance data
        //    data associated with the API object instance, i.e. visible to all 
        //    CPI object instances associated with the API object instance 
        //    regardless of the shared library (adaptor) they are implemented in.
        typedef      saga::adaptors::v1_0::cpr_job_cpi_instance_data cpr_job_cpi_instance_data;
        friend class saga::adaptors::instance_data<cpr_job_cpi_instance_data>;
        typedef      saga::adaptors::instance_data<cpr_job_cpi_instance_data> instance_data;

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
            // register cpr job CPI functions
            saga::uuid cpi_uuid;
            register_cpr_job_functions<Derived>(infos,
                &saga::adaptors::v1_0::cpr_job_cpi<Derived>::cpi_maker, 
                prefs, cpi_uuid, adaptor_uuid, saga::adaptors::cpr_job_cpi);

            register_job_functions<Derived>(infos,
                &saga::adaptors::v1_0::cpr_job_cpi<Derived>::cpi_maker, 
                prefs, cpi_uuid, adaptor_uuid, saga::adaptors::cpr_job_cpi);

            // register attribute CPI functions
            if (!register_attribute_functions<Derived>(infos,
                    &saga::adaptors::v1_0::cpr_job_cpi<Derived>::cpi_maker, 
                    prefs, cpi_uuid, adaptor_uuid, saga::adaptors::cpr_job_cpi))
            {
                // register the default (cache based) implementation of the
                // attribute CPI if the derived class does not implement the 
                // attribute interface by itself
                register_attribute_functions<base_type>(infos,
                    &saga::adaptors::v1_0::cpr_job_cpi<Derived>::cpi_maker, 
                    prefs, cpi_uuid, adaptor_uuid, saga::adaptors::cpr_job_cpi);
            }
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

#endif // SAGA_IMPL_PACKAGES_CPR_CPR_JOB_CPI_HPP

