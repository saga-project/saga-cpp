#ifndef SAGA_PACKAGES_JOB_JOB_SERVICE_HPP
#define SAGA_PACKAGES_JOB_JOB_SERVICE_HPP

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/job_service.hpp")
#endif
//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2007 Ole Weidner (oweidner@cct.lsu.edu)
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

#include <saga/saga/packages/job/config.hpp>
#include <saga/saga/job.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable: 4251 4231 4275 4660)
#endif

#ifdef SAGA_DEBUG
#include <saga/saga/packages/job/preprocessed/job_service.hpp>
#else

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/job_service.hpp")
#endif
///////////////////////////////////////////////////////////////////////////////
namespace saga
{
    namespace job {

    /*! \brief The job_service represents a resource management backend, and as 
     *such allows to create and submit jobs, and to discover jobs. 
     *
     *   The job management methods are on the job object itself - this probably 
     *implies that implementations need to internally track what resource manager 
     *(or job_service instance) created the job. 
     */
    class SAGA_JOB_PACKAGE_EXPORT service 
        : public saga::object
    {
    private:
        /// @cond
        /** These methods are not within API scope */
        // factory
        SAGA_CALL_CREATE_PRIV_2(session const&, saga::url)

        SAGA_CALL_PRIV_1(create_job, description)
        SAGA_CALL_CONST_PRIV_0(get_url)
        SAGA_CALL_PRIV_5(run_job, std::string, std::string,
            saga::job::ostream&, saga::job::istream&, saga::job::istream&)
        SAGA_CALL_PRIV_2(run_job, std::string, std::string)
        SAGA_CALL_PRIV_0(list)
        SAGA_CALL_PRIV_1(get_job, std::string)
        SAGA_CALL_PRIV_0(get_self)
        /// @endcond

    protected:
        /// @cond
        /** These methods are not within API scope */
        friend class saga::impl::job_service;
        friend struct saga::detail::create_default<service>;

        TR1::shared_ptr <saga::impl::job_service> get_impl_sp(void) const;
        saga::impl::job_service* get_impl (void) const;
        explicit service(saga::impl::job_service *impl);
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
            return create<Tag>(saga::detail::get_the_session(), rm);
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
        job create_job(description job_desc) 
        {
            saga::task t = create_jobpriv(job_desc, saga::task_base::Sync()); 
            return t.get_result<saga::job::job>();
        }
        SAGA_CALL_PUB_1_DEF_0(create_job, description)

        /*! \brief Create a job instance     
         *  \return The resource manager url used to create this job_service 
         *          instance
         */
        saga::url get_url() const
        {
            saga::task t = get_urlpriv(saga::task_base::Sync()); 
            return t.get_result<saga::url>();
        }
        SAGA_CALL_CONST_PUB_0_DEF_0(get_url)

        /*! \brief Run a command synchronously    
         *         
         *  \param hostname Hostname to be used by rm for submission 
         *  \param commandline The command and arguments to be run 
         *
         *
         */
        job run_job(std::string commandline, std::string hostname,
            saga::job::ostream& stdin_stream, saga::job::istream& stdout_stream, 
            saga::job::istream& stderr_stream) 
        {
            saga::task t = run_jobpriv(commandline, hostname, stdin_stream, 
                stdout_stream, stderr_stream, saga::task_base::Sync()); 
            return t.get_result<saga::job::job>();
        }
        /// @cond
        SAGA_CALL_PUB_5_DEF_0(run_job, std::string, std::string,
            saga::job::ostream&, saga::job::istream&, saga::job::istream&)
        /// @endcond

        job run_job(std::string commandline, std::string hostname = "")
        {
            saga::task t = run_jobpriv(commandline, hostname, saga::task_base::Sync()); 
            return t.get_result<saga::job::job>();
        }
        /// @cond
        SAGA_CALL_PUB_2_DEF_1(run_job, std::string, std::string, "")
        /// @endcond


        /*! \brief Get a list of jobs which are currently known by 
         *         the resource manager    
         *  \return An array of job identifiers 
         *         
         *
         *
         */
        std::vector<std::string> list(void) 
        {
            saga::task t = listpriv(saga::task_base::Sync()); 
            return t.get_result<std::vector<std::string> >();
        }
        SAGA_CALL_PUB_0_DEF_0(list)

        /*! \brief Given a job identifier, this method returns a job object 
         *         representing this job    
         *         
         *  \param job_id Job identifier as returned by the resource manager 
         *  \return A job object representing the job identified by job_id
         *
         *
         */
        job get_job(std::string job_id) 
        {
            saga::task t = get_jobpriv(job_id, saga::task_base::Sync()); 
            return t.get_result<saga::job::job>();
        }
        /// @cond
        SAGA_CALL_PUB_1_DEF_0(get_job, std::string)
        /// @endcond

        /*! \brief This method returns a job object representing _this_ job, 
         *         i.e. the calling application 
         *         
         *  \return A job_self object representing _this_ job.
         *
         *
         */
        saga::job::self get_self() 
        {
            saga::task t = get_selfpriv(saga::task_base::Sync()); 
            return t.get_result<saga::job::self>();
        }
        SAGA_CALL_PUB_0_DEF_0(get_self)

    };

    } // saga::job

    namespace detail
    {
        // we need to specialize the create_default template (used in hold_any) 
        // to be able to create a truly default constructed job_service (empty 
        // facade)
        template<>
        struct create_default<saga::job::service>
        {
            static saga::job::service* call()
            {
                return new saga::job::service(1);    // use dummy constructor
            }
            template <typename T_> static void call(T_* obj) 
            { 
                new (obj) saga::job::service(1); 
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

#endif // !defined(SAGA_PACKAGES_JOB_JOB_SERVICE_HPP)

