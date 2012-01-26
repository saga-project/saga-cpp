#ifndef SAGA_IMPL_PACKAGES_CPR_CPR_JOB_SERVICE_HPP
#define SAGA_IMPL_PACKAGES_CPR_CPR_JOB_SERVICE_HPP

#if defined(__WAVE__)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/cpr_job_service.hpp")
#endif
//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if defined(__WAVE__)
#pragma wave option(output: null)
#endif
#include <string>

#include <saga/saga/session.hpp>
#include <saga/impl/call.hpp>
#include <saga/impl/engine/proxy.hpp>
#include <saga/impl/packages/cpr/cpr_job_service_cpi.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable: 4251 4231 4275 4660)
#endif

#ifdef SAGA_DEBUG
#include <saga/saga/packages/cpr/preprocessed/cpr_job_service.hpp>
#else

#if defined(__WAVE__)
#pragma wave option(output: "preprocessed/cpr_job_service.hpp")
#endif
///////////////////////////////////////////////////////////////////////////////
namespace saga 
{ 
  namespace impl 
  {
    /**
     * This class acts as a proxy class, hiding all the nasty Saga stuff
     * (like creating and using adaptors) for the user. Users just call
     * the functions of this class like to would on every local object,
     * and don't have to worry about adaptors. It extends saga::cpr_job_service_cpi.
     */
    class cpr_job_service : public saga::impl::job_service
    {
      typedef v1_0::cpr_job_service_cpi cpr_job_service_cpi;
      typedef v1_0::preference_type     preference_type;

      // initialize newly attached CPI instance
      void init();

     public:
      /**
       * Constructor of service, sets the resource manager of the 
       * service, and gets an instance of the Saga engine
       *
       * @param rm  the url of the resource manager to use
       * @param s   the session handle
       */
      cpr_job_service (saga::session const & s, saga::url rm);

      /*! destructor of service, destroys service and the adaptor it is 
       *  bound to */
      ~cpr_job_service (void);

      // factory support
      void create_impl_sync(saga::cpr::service& retval);
      static saga::task create_impl_async(saga::session const& s, saga::url const& rm);

      // API functions of service
      SAGA_CALL_IMPL_DECL_1(create_job,     saga::cpr::description)
      SAGA_CALL_IMPL_DECL_2(create_job_cpr, saga::cpr::description, saga::cpr::description)
      SAGA_CALL_IMPL_DECL_5(run_job,        std::string, std::string, 
                                            saga::job::ostream &, 
                                            saga::job::istream &, 
                                            saga::job::istream &);
      SAGA_CALL_IMPL_DECL_2(run_job_noio,   std::string, std::string)

    };

}}  // namespace saga::impl
///////////////////////////////////////////////////////////////////////////////

#if defined(__WAVE__)
#pragma wave option(output: null)
#endif

#endif // !defined(SAGA_DEBUG)

// re-enable warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

#endif // !defined(SAGA_IMPL_PACKAGES_CPR_CPR_JOB_SERVICE_HPP)

