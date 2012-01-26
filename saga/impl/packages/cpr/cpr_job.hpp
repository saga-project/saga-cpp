#ifndef SAGA_IMPL_PACKAGES_CPR_CPR_JOB_HPP
#define SAGA_IMPL_PACKAGES_CPR_CPR_JOB_HPP

#if defined(__WAVE__)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/cpr_job.hpp")
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

#include <saga/saga/session.hpp>
#include <saga/impl/call.hpp>
#include <saga/impl/engine/task_base.hpp>
#include <saga/impl/engine/proxy.hpp>
#include <saga/impl/engine/permissions.hpp>
#include <saga/impl/job.hpp>

#include <saga/impl/packages/cpr/cpr_job_cpi.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable: 4251 4231 4275 4660)
#endif

#ifdef SAGA_DEBUG
#include <saga/saga/packages/cpr/preprocessed/cpr_job.hpp>
#else

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: "preprocessed/cpr_job.hpp")
#endif
///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl 
{
    /**
     * This class acts as a proxy class, hiding all the nasty Saga stuff
     * (like creating and using adaptors) for the user. Users just call
     * the functions of this class like to would on every local object,
     * and don't have to worry about adaptors. It extends saga::cpr_job_cpi.
     *
     * Note, the impl::cpr_job class is somewhat specific as it represents a
     * task (wrapping the service::create_job() function) and at the same 
     * time it is a proxy for job CPI's.
     */
    class cpr_job 
      : public saga::impl::job
    {
      typedef v1_0::cpr_job_cpi cpr_job_cpi;
      typedef v1_0::preference_type preference_type;

     private:
       // initialize newly attached CPI instance
       void init();

     public:
      /**
       * Constructor of cpr_job
       */
       cpr_job (saga::url              rm, 
                saga::cpr::description jd_start, 
                saga::cpr::description jd_restart, 
                saga::session const&   s,
                saga::object::type     t = saga::object::CPRJob);

       cpr_job (saga::url              rm, 
                saga::cpr::description jd, 
                saga::session const&   s,
                saga::object::type     t = saga::object::CPRJob);

       cpr_job (saga::url              rm, 
                std::string            jobid,         
                saga::session const&   s,
                saga::object::type     t = saga::object::CPRJob);

      /*! destructor of cpr_job, destroys cpr_job and the adaptor it is 
       *  bound to */
      ~cpr_job (void);

      SAGA_CALL_IMPL_DECL_1(checkpoint,    saga::url)
      SAGA_CALL_IMPL_DECL_1(recover,       saga::url)

      SAGA_CALL_IMPL_DECL_1(cpr_stage_in,  saga::url)
      SAGA_CALL_IMPL_DECL_1(cpr_stage_out, saga::url)

      SAGA_CALL_IMPL_DECL_0(cpr_list)
      SAGA_CALL_IMPL_DECL_0(cpr_last)
    };

///////////////////////////////////////////////////////////////////////////////
}}  // namespace saga::impl

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif

#endif // !defined(SAGA_DEBUG)

// re-enable warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

#endif // !defined(SAGA_IMPL_PACKAGES_CPR_CPR_JOB_HPP)

