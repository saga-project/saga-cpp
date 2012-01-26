#ifndef SAGA_IMPL_PACKAGES_JOB_JOB_SERVICE_CPI_HPP
#define SAGA_IMPL_PACKAGES_JOB_JOB_SERVICE_CPI_HPP

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/job_service_cpi.hpp")
#endif
//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif

#include <string>
#include <iosfwd>

#include <saga/saga/util.hpp>
#include <saga/saga/types.hpp>
#include <saga/saga/exception.hpp>
#include <saga/saga/job.hpp>

#include <saga/saga/adaptors/instance_data.hpp>
#include <saga/saga/adaptors/packages/job_service_cpi_instance_data.hpp>
#include <saga/saga/adaptors/permissions_cpi.hpp>

#include <saga/impl/call.hpp>
#include <saga/impl/config.hpp>
#include <saga/impl/engine/cpi.hpp>
#include <saga/impl/engine/register_members.hpp>

#ifdef SAGA_DEBUG
#include <saga/saga/packages/job/preprocessed/job_service_cpi_instance_data.hpp>
#else

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: "preprocessed/job_service_cpi.hpp")
#endif
///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl { namespace v1_0 {

    /**
     * This class declares all functions of "service" defined in the SAGA API.
     * It extends the base class saga::cpi
     *
     * @see For more information, read the GGF SAGA API Specification
     */
    class job_service_cpi 
        : public permissions_cpi
    {
    public:
        job_service_cpi (proxy * p, cpi_info const& info,
                TR1::shared_ptr<saga::adaptor> adaptor, cpi::flags flags) 
            : permissions_cpi (p, info, adaptor, flags) 
        { 
        }
         
        virtual ~job_service_cpi (void) 
        { 
        }

        cpi::type get_type() const { return cpi::JobService; }

        SAGA_CALL_CPI_DECL_VIRT_1(job_service_cpi, saga::job::job, create_job, 
            saga::job::description)
        SAGA_CALL_CPI_DECL_VIRT_0(job_service_cpi, saga::url, get_url)
        SAGA_CALL_CPI_DECL_VIRT_5(job_service_cpi, saga::job::job, run_job, 
            std::string, std::string, saga::job::ostream&, saga::job::istream&, 
            saga::job::istream&)
        SAGA_CALL_CPI_DECL_VIRT_2(job_service_cpi, saga::job::job, run_job_noio, 
            std::string, std::string)
        SAGA_CALL_CPI_DECL_VIRT_0(job_service_cpi, std::vector<std::string>, list)
        SAGA_CALL_CPI_DECL_VIRT_1(job_service_cpi, saga::job::job, get_job, std::string)
        SAGA_CALL_CPI_DECL_VIRT_0(job_service_cpi, saga::job::self, get_self)

    }; // class job_service_cpi
    
///////////////////////////////////////////////////////////////////////////////
}}} // namespace saga::impl::v1_0

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace adaptors { namespace v1_0 {

///////////////////////////////////////////////////////////////////////////////
// register service CPI functions
    template <typename Derived>
    inline bool
    register_job_service_functions(
        std::vector<saga::impl::v1_0::cpi_info>& infos, 
        saga::impl::v1_0::cpi::maker_type maker, 
        saga::impl::v1_0::preference_type const& prefs, 
        saga::uuid const& cpi_uuid, saga::uuid const& adaptor_uuid, 
        std::string const& cpi_name)
    {
        bool retval = false;
        saga::impl::v1_0::cpi_info info(saga::adaptors::job_service_cpi, 
            cpi_name, maker, prefs, cpi_uuid, adaptor_uuid);
          
        SAGA_LOG(SAGA_VERBOSE_LEVEL_BLURB)
            << "begin register_job_service_functions (" << cpi_name << "): " 
            << adaptor_uuid.string() << ":";

        typedef saga::impl::v1_0::job_service_cpi base_cpi;

        typedef void (base_cpi::*base_sync_create_job_type)(saga::job::job&, saga::job::description);
        typedef saga::task (base_cpi::*base_async_create_job_type)(saga::job::description);
        typedef void (Derived::*sync_create_job_type)(saga::job::job&, saga::job::description);
        typedef saga::task (Derived::*async_create_job_type)(saga::job::description);

        SAGA_REGISTER_MEMBER_EX(retval, info, base_cpi, Derived, create_job, prefs,
            base_sync_create_job_type, base_async_create_job_type,
            sync_create_job_type, async_create_job_type);

        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, get_url, prefs);

        typedef void (base_cpi::*base_sync_run_job_type)(saga::job::job&, 
            std::string, std::string, saga::job::ostream&, saga::job::istream&, saga::job::istream&);
        typedef saga::task (base_cpi::*base_async_run_job_type)(
            std::string, std::string, saga::job::ostream&, saga::job::istream&, saga::job::istream&);
        typedef void (Derived::*sync_run_job_type)(saga::job::job&, 
            std::string, std::string, saga::job::ostream&, saga::job::istream&, saga::job::istream&);
        typedef saga::task (Derived::*async_run_job_type)(
            std::string, std::string, saga::job::ostream&, saga::job::istream&, saga::job::istream&);

        SAGA_REGISTER_MEMBER_EX(retval, info, base_cpi, Derived, run_job, prefs,
            base_sync_run_job_type, base_async_run_job_type,
            sync_run_job_type, async_run_job_type);

        typedef void (base_cpi::*base_sync_run_job_noio_type)(saga::job::job&, std::string, std::string);
        typedef saga::task (base_cpi::*base_async_run_job_noio_type)(std::string, std::string);
        typedef void (Derived::*sync_run_job_noio_type)(saga::job::job&, std::string, std::string);
        typedef saga::task (Derived::*async_run_job_noio_type)(std::string, std::string);

        SAGA_REGISTER_MEMBER_EX(retval, info, base_cpi, Derived, run_job_noio, prefs,
            base_sync_run_job_noio_type, base_async_run_job_noio_type,
            sync_run_job_noio_type, async_run_job_noio_type);

        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, list, prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, get_job, prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, get_self, prefs);

        infos.push_back(info);

        SAGA_LOG(SAGA_VERBOSE_LEVEL_BLURB)
            << "end register_job_service_functions";

        return retval;    // is true if at least one function got registered
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename Derived, typename Mutex = boost::recursive_mutex>
    class job_service_cpi 
        : public saga::impl::v1_0::job_service_cpi
    {
    protected:
        typedef saga::impl::v1_0::job_service_cpi base_type;
        typedef saga::impl::proxy proxy;
        typedef saga::impl::v1_0::cpi cpi;
        typedef saga::impl::v1_0::cpi_info cpi_info;
        typedef saga::impl::v1_0::preference_type preference_type;
        typedef Mutex mutex_type;

        mutex_type mtx_;     // secure access to cpi_instance_data

    public:
        job_service_cpi (proxy * p, cpi_info const& info,
                TR1::shared_ptr<saga::adaptor> adaptor, cpi::flags flags) 
            : saga::impl::v1_0::job_service_cpi (p, info, adaptor, flags) 
        { 
        }
         
        virtual ~job_service_cpi (void) 
        { 
        }

        ///////////////////////////////////////////////////////////////////////
        //  instance data
        //    data associated with the API object instance, i.e. visible to all 
        //    CPI object instances associated with the API object instance 
        //    regardless of the shared library (adaptor) they are implemented in.
        typedef saga::adaptors::v1_0::job_service_cpi_instance_data 
            job_service_cpi_instance_data;
        friend class saga::adaptors::instance_data<job_service_cpi_instance_data>;
        typedef      saga::adaptors::instance_data<job_service_cpi_instance_data> 
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
            // register service CPI functions
            saga::uuid cpi_uuid;
            register_job_service_functions<Derived>(infos,
                &saga::adaptors::v1_0::job_service_cpi<Derived>::cpi_maker, 
                prefs, cpi_uuid, adaptor_uuid, saga::adaptors::job_service_cpi);

            // register permissions CPI functions
            register_permissions_functions<Derived>(infos, 
                &saga::adaptors::v1_0::permissions_cpi<Derived>::cpi_maker, 
                prefs, cpi_uuid, adaptor_uuid, saga::adaptors::job_service_cpi);
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

#endif // SAGA_IMPL_PACKAGES_JOB_JOB_SERVICE_CPI_HPP

