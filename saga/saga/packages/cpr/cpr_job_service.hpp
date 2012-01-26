#ifndef SAGA_PACKAGES_CPR_SERVICE_HPP
#define SAGA_PACKAGES_CPR_SERVICE_HPP

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/cpr_job_service.hpp")
#endif
//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2007 Ole Weidner (oweidner@cct.lsu.edu)
//  Copyright (c) 2008 Andre Merzky (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif

#include <string>
#include <vector>

// include dependent spec sections
#include <saga/saga/util.hpp>
#include <saga/saga/base.hpp>
#include <saga/saga/types.hpp>
#include <saga/saga/session.hpp>
#include <saga/saga/call.hpp>
#include <saga/saga/url.hpp>
#include <saga/saga/job.hpp>

#include <saga/saga/cpr.hpp>
#include <saga/saga/packages/cpr/config.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable: 4251 4231 4275 4660)
#endif

#ifdef SAGA_DEBUG
#include <saga/saga/packages/cpr/preprocessed/cpr_job_service.hpp>
#else

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/cpr_job_service.hpp")
#endif
///////////////////////////////////////////////////////////////////////////////
namespace saga
{
      namespace cpr 
    {
        /*! \brief The cpr::service, as a job::service, represents a resource management 
         * backend, which additionally offers checkpoint management capabilities.
         *
         */
        class SAGA_CPR_PACKAGE_EXPORT service 
            : public saga::job::service
        {
        private:
            // factory
            SAGA_CALL_CREATE_PRIV_2(session const&, saga::url)

            SAGA_CALL_PRIV_1 (create_job, saga::cpr::description)
            SAGA_CALL_PRIV_2 (create_job, saga::cpr::description, saga::cpr::description)
            SAGA_CALL_PRIV_5 (run_job,    std::string, std::string,
                                          saga::job::ostream &, 
                                          saga::job::istream &,
                                          saga::job::istream &);
            SAGA_CALL_PRIV_2 (run_job,    std::string, std::string)

        protected:
            /// @cond
            /** These methods are not within API scope */
            friend class saga::impl::cpr_job_service;
            friend struct saga::detail::create_default<service>;

            TR1::shared_ptr <saga::impl::cpr_job_service> get_impl_sp(void) const;
            saga::impl::cpr_job_service* get_impl (void) const;
            explicit service(saga::impl::cpr_job_service *impl);
            explicit service(int);    // dummy constructor to create empty facade
            /// @endcond

        public:
            /*! \brief Creates the object.     
             *
             *
             */
            explicit service (session const& s, saga::url rm = saga::url());

            /*! \brief Creates the object.     
             *
             *
             */
            explicit service (saga::url rm = saga::url());

            /*! \brief Creates the object.     
             *
             *
             */     
            explicit service (saga::object const& o);

            /*! \brief Destroys the object.     
             *
             *
             */
            ~service (void);

            /*! \brief Factory function resembling service(session, url) 
             *         constructor
             */
            static service create(session const& s, saga::url rm = saga::url())
            {
                return service(s, rm);
            }
            /// @cond
            SAGA_CALL_CREATE_2_DEF_1(session const&, saga::url, saga::url())
            /// @endcond

            /*! \brief Factory function resembling service(url, int) constructor
             */
            static service create(saga::url rm = saga::url())
            {
                return service(rm);
            }
            template <typename Tag>
            static saga::task create(saga::url rm = saga::url())
            {
                return create<Tag>(detail::get_the_session(), rm);
            }

            /*! \brief Brief %description starts here     
             *
             *
             */
            service& operator= (saga::object const& o);

            /*! \brief Create a job instance     
             *         
             *  \param job_desc Description of job to be submitted 
             *  \return A job object representing the submitted job instance 
             *
             *
             */
            saga::cpr::job create_job(saga::cpr::description job_desc) 
            {
                saga::task t = create_jobpriv(job_desc, saga::task_base::Sync()); 
                return t.get_result<saga::cpr::job>();
            }
            /// @cond
            SAGA_CALL_PUB_1_DEF_0 (create_job, saga::cpr::description)
            /// @endcond

            /*! \brief Create a job instance     
             *         
             *  \param job_desc Description of job to be submitted 
             *         
             *  \param restart_desc Description of job to be used on recovery
             *  \return A job object representing the submitted job instance 
             *
             *
             */
            saga::cpr::job create_job(saga::cpr::description job_desc, saga::cpr::description restart_desc) 
            {
                saga::task t = create_jobpriv(job_desc, restart_desc, saga::task_base::Sync()); 
                return t.get_result<saga::cpr::job>();
            }
            /// @cond
            SAGA_CALL_PUB_2_DEF_0 (create_job, saga::cpr::description, saga::cpr::description)
            /// @endcond

            saga::cpr::job run_job(std::string cmd,
                                   std::string host, 
                                   saga::job::ostream & in, 
                                   saga::job::istream & out, 
                                   saga::job::istream & err) 

            {
                saga::task t = run_jobpriv(cmd, host, in, out, err, saga::task_base::Sync()); 
                return t.get_result<saga::cpr::job>();
            }
            /// @cond
            SAGA_CALL_PUB_5_DEF_0 (run_job, 
                                   std::string, std::string, saga::job::ostream, 
                                   saga::job::istream, saga::job::istream);
            /// @endcond

            saga::cpr::job run_job(std::string cmd, std::string host) 

            {
                saga::task t = run_jobpriv(cmd, host, saga::task_base::Sync()); 
                return t.get_result<saga::cpr::job>();
            }
            /// @cond
            SAGA_CALL_PUB_2_DEF_0 (run_job, 
                                   std::string, std::string);
            /// @endcond

        };

    }

    namespace detail
    {
        // we need to specialize the create_default template (used in hold_any) 
        // to be able to create a truly default constructed service (empty 
        // facade)
        template<>
        struct create_default<saga::cpr::service>
        {
            static saga::cpr::service* call()
            {
                return new saga::cpr::service(1);    // use dummy constructor
            }
            template <typename T_> static void call(T_* obj) 
            { 
                new (obj) saga::cpr::service(1); 
            }
        };
    }

}   // namespace saga

///////////////////////////////////////////////////////////////////////////////
#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif

#endif // !defined(SAGA_DEBUG)

// re-enable warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

#endif // !defined(SAGA_PACKAGES_CPR_SERVICE_HPP)

