#ifndef SAGA_IMPL_PACKAGES_JOB_JOB_SERVICE_HPP
#define SAGA_IMPL_PACKAGES_JOB_JOB_SERVICE_HPP

#if defined(__WAVE__)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/job_service.hpp")
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
#include <saga/saga/packages/job/config.hpp>
#include <saga/impl/call.hpp>
#include <saga/impl/engine/proxy.hpp>
#include <saga/impl/packages/job/job_service_cpi.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable: 4251 4231 4275 4660)
#endif

#ifdef SAGA_DEBUG
#include <saga/saga/packages/job/preprocessed/job_service.hpp>
#else

#if defined(__WAVE__)
#pragma wave option(output: "preprocessed/job_service.hpp")
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
     * and don't have to worry about adaptors. It extends saga::job_service_cpi.
     */
    class SAGA_JOB_PACKAGE_EXPORT job_service : public proxy
    {
      typedef v1_0::job_service_cpi job_service_cpi;
      typedef v1_0::preference_type preference_type;

      // initialize newly attached CPI instance
      void init();

      bool need_to_free_instance_data;

    protected:
      job_service (saga::object::type t, saga::session const & s);
      explicit job_service (saga::object::type t);

     public:
      /**
       * Constructor of service, sets the resource manager of the 
       * service, and gets an instance of the Saga engine
       *
       * @param rm  the url of the resource manager to use
       * @param s   the session handle
       */
      job_service (saga::session const & s, saga::url rm);

      /*! destructor of service, destroys service and the adaptor it is 
       *  bound to */
      ~job_service (void);

      // factory support
      void create_impl_sync(saga::job::service& retval);
      static saga::task create_impl_async(saga::session const& s, saga::url const& rm);

      // API functions of service
      SAGA_CALL_IMPL_DECL_1(create_job, saga::job::description)
      SAGA_CALL_IMPL_DECL_0(get_url)
      SAGA_CALL_IMPL_DECL_5(run_job, std::string, std::string,
            saga::job::ostream&, saga::job::istream&, saga::job::istream&)
      SAGA_CALL_IMPL_DECL_2(run_job_noio, std::string, std::string)
      SAGA_CALL_IMPL_DECL_0(list)
      SAGA_CALL_IMPL_DECL_1(get_job, std::string)
      SAGA_CALL_IMPL_DECL_0(get_self)

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

#endif // !defined(SAGA_IMPL_PACKAGES_JOB_JOB_SERVICE_HPP)

