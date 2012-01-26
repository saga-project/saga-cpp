#ifndef SAGA_IMPL_PACKAGES_JOB_JOB_CPI_HPP
#define SAGA_IMPL_PACKAGES_JOB_JOB_CPI_HPP

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/job_cpi.hpp")
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
#include <saga/saga/job.hpp>

#include <saga/saga/adaptors/instance_data.hpp>
#include <saga/saga/adaptors/packages/job_cpi_instance_data.hpp>
#include <saga/saga/adaptors/attribute_cache_cpi.hpp>
#include <saga/saga/adaptors/attribute_cpi.hpp>

#include <saga/impl/call.hpp>
#include <saga/impl/config.hpp>
#include <saga/impl/engine/register_members.hpp>

#ifdef SAGA_DEBUG
#include <saga/saga/packages/job/preprocessed/job_cpi.hpp>
#else

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: "preprocessed/job_cpi.hpp")
#endif
///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl { namespace v1_0 {

    /**
     * This class declares all functions of "service" defined in the SAGA API.
     * It extends the base class saga::cpi
     *
     * @see For more information, read the GGF SAGA API Specification
     */
    class job_cpi 
        : public attribute_cpi
    {
    public:
       
        job_cpi (proxy * p, cpi_info const& info, 
                TR1::shared_ptr<saga::adaptor> adaptor, cpi::flags flags) 
            : attribute_cpi (p, info, adaptor, flags) 
        { 
        }
         
        virtual ~job_cpi (void) 
        { 
        }

        cpi::type get_type() const { return cpi::Job; }

        // job inspection
        SAGA_CALL_CPI_DECL_VIRT_0 (job_cpi, std::string, get_job_id)
        SAGA_CALL_CPI_DECL_VIRT_0 (job_cpi, saga::job::state, get_state)
        SAGA_CALL_CPI_DECL_VIRT_0 (job_cpi, saga::job::description, get_description)
        SAGA_CALL_CPI_DECL_VIRT_0 (job_cpi, saga::job::ostream, get_stdin)
        SAGA_CALL_CPI_DECL_VIRT_0 (job_cpi, saga::job::istream, get_stdout)
        SAGA_CALL_CPI_DECL_VIRT_0 (job_cpi, saga::job::istream, get_stderr)

        // job management
        SAGA_CALL_CPI_DECL_VIRT_0 (job_cpi, saga::impl::void_t, suspend)
        SAGA_CALL_CPI_DECL_VIRT_0 (job_cpi, saga::impl::void_t, resume)
        SAGA_CALL_CPI_DECL_VIRT_0 (job_cpi, saga::impl::void_t, checkpoint)
        SAGA_CALL_CPI_DECL_VIRT_1 (job_cpi, saga::impl::void_t, migrate, saga::job::description)
        SAGA_CALL_CPI_DECL_VIRT_1 (job_cpi, saga::impl::void_t, signal, int)

        // functions inherited from the task interface
        SAGA_CALL_CPI_DECL_VIRT_0 (job_cpi, saga::impl::void_t, run)
        SAGA_CALL_CPI_DECL_VIRT_1 (job_cpi, saga::impl::void_t, cancel, double)
        SAGA_CALL_CPI_DECL_VIRT_1 (job_cpi, bool, wait, double)
        
    }; // class job_cpi

///////////////////////////////////////////////////////////////////////////////
}}} // namespace saga::impl::v1_0

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace adaptors { namespace v1_0 {

///////////////////////////////////////////////////////////////////////////////
// register job CPI functions
    template <typename Derived>
    inline bool
    register_job_functions(
        std::vector<saga::impl::v1_0::cpi_info>& infos, 
        saga::impl::v1_0::cpi::maker_type maker, 
        saga::impl::v1_0::preference_type const& prefs, 
        saga::uuid const& cpi_uuid, saga::uuid const& adaptor_uuid, 
        std::string const& cpi_name)
    {
        bool retval = false;
        saga::impl::v1_0::cpi_info info(saga::adaptors::job_cpi, cpi_name, 
            maker, prefs, cpi_uuid, adaptor_uuid);
          
        SAGA_LOG(SAGA_VERBOSE_LEVEL_BLURB)
            << "begin register_job_functions (" << cpi_name << "): " 
            << adaptor_uuid.string() << ":";

        typedef saga::impl::v1_0::job_cpi base_cpi;
        
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, get_job_id, prefs)
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, get_state, prefs)
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, get_description, prefs)
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, get_stdin, prefs)
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, get_stdout, prefs)
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, get_stderr, prefs)
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, suspend, prefs)
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, resume, prefs)

        typedef void (base_cpi::*base_sync_checkpoint_type)(saga::impl::void_t&);
        typedef saga::task (base_cpi::*base_async_checkpoint_type)();
        typedef void (Derived::*sync_checkpoint_type)(saga::impl::void_t&);
        typedef saga::task (Derived::*async_checkpoint_type)();

        SAGA_REGISTER_MEMBER_EX(retval, info, base_cpi, Derived, checkpoint, prefs,
            base_sync_checkpoint_type, base_async_checkpoint_type,
            sync_checkpoint_type, async_checkpoint_type);

        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, migrate, prefs)
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, signal, prefs)
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, run, prefs)
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, cancel, prefs)
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, wait, prefs)
      
        infos.push_back(info);

        SAGA_LOG(SAGA_VERBOSE_LEVEL_BLURB)
            << "end register_job_functions";

        return retval;    // is true if at least one function got registered
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename Derived, typename Mutex = boost::recursive_mutex>
    class job_cpi 
        : public attribute_cache_cpi<Derived, saga::impl::v1_0::job_cpi>
    {
    protected:
        ///@cond
        typedef attribute_cache_cpi<Derived, saga::impl::v1_0::job_cpi> 
            base_type;
        typedef saga::impl::v1_0::preference_type preference_type;
        ///@endcond
        typedef saga::impl::proxy proxy;
        typedef saga::impl::v1_0::cpi cpi;
        typedef saga::impl::v1_0::cpi_info cpi_info;
        typedef Mutex mutex_type;

        mutex_type mtx_;     // secure access to cpi_instance_data

    public:
        job_cpi (proxy * p, cpi_info const& info, 
                TR1::shared_ptr<saga::adaptor> adaptor, cpi::flags flags) 
        :   base_type(p, info, adaptor, flags) 
        { 
        }
         
        virtual ~job_cpi (void) 
        { 
        }

#ifdef DOXYGEN_ONLY
        virtual void sync_get_job_id (std::string & ret);
#endif
        
        ///////////////////////////////////////////////////////////////////////
        //  instance data
        //    data associated with the API object instance, i.e. visible to all 
        //    CPI object instances associated with the API object instance 
        //    regardless of the shared library (adaptor) they are implemented in.
        typedef saga::adaptors::v1_0::job_cpi_instance_data 
            job_cpi_instance_data;
        friend class saga::adaptors::instance_data<job_cpi_instance_data>;
        typedef      saga::adaptors::instance_data<job_cpi_instance_data> 
            instance_data;

        ///////////////////////////////////////////////////////////////////////
        // generic factory function
        ///@cond
        static cpi* cpi_maker (proxy                * p, 
                               cpi_info       const & info,
                               saga::ini::ini const & glob_ini, 
                               saga::ini::ini const & adap_ini,
                               TR1::shared_ptr<saga::adaptor> adaptor)
        {
            return new Derived (p, info, glob_ini, adap_ini, adaptor);
        }
        ///@endcond 
        
        ///////////////////////////////////////////////////////////////////////
        // register implemented functions
        ///@cond
        static void 
        register_cpi(std::vector<cpi_info>& infos, preference_type prefs, 
            saga::uuid adaptor_uuid)
        {
            // register job CPI functions
            saga::uuid cpi_uuid;
            register_job_functions<Derived>(infos,
                &saga::adaptors::v1_0::job_cpi<Derived>::cpi_maker, 
                prefs, cpi_uuid, adaptor_uuid, saga::adaptors::job_cpi);

            // register attribute CPI functions
            if (!register_attribute_functions<Derived>(infos,
                    &saga::adaptors::v1_0::job_cpi<Derived>::cpi_maker, 
                    prefs, cpi_uuid, adaptor_uuid, saga::adaptors::job_cpi))
            {
                // register the default (cache based) implementation of the
                // attribute CPI if the derived class does not implement the 
                // attribute interface by itself
                register_attribute_functions<base_type>(infos,
                    &saga::adaptors::v1_0::job_cpi<Derived>::cpi_maker, 
                    prefs, cpi_uuid, adaptor_uuid, saga::adaptors::job_cpi);
            }
        }
        ///@endcond


        ///////////////////////////////////////////////////////////////////////
        // generic up-cast of the impl pointer       
        ///@cond
        TR1::shared_ptr<Derived> shared_from_this()
        {
            return TR1::static_pointer_cast<Derived>(
                this->base_type::shared_from_this());
        }
        ///@endcond

        
    }; 

///////////////////////////////////////////////////////////////////////////////
}}} // namespace saga::adaptors::v1_0

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif

#endif // !defined(SAGA_DEBUG)

#endif // SAGA_IMPL_PACKAGES_JOB_JOB_CPI_HPP

