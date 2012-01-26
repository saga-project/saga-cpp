#ifndef SAGA_PACKAGES_JOB_JOB_HPP
#define SAGA_PACKAGES_JOB_JOB_HPP

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/job.hpp")
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
#include <iosfwd>

// include dependent spec sections
#include <saga/saga/util.hpp>
#include <saga/saga/base.hpp>
#include <saga/saga/types.hpp>
#include <saga/saga/session.hpp>
#include <saga/saga/call.hpp>

#include <saga/saga/packages/job/config.hpp>
#include <saga/saga/packages/job/job_description.hpp>
#include <saga/saga/packages/job/istream.hpp>
#include <saga/saga/packages/job/ostream.hpp>

#include <saga/saga/detail/attribute.hpp>
#include <saga/saga/detail/permissions.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable: 4251 4231 4275 4660)
#endif

#ifdef SAGA_DEBUG
#include <saga/saga/packages/job/preprocessed/job.hpp>
#else

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/job.hpp")
#endif
///////////////////////////////////////////////////////////////////////////////
namespace saga
{
    /*! \brief The <I>Job Management</I> API package
     *
     *
     */
    namespace job {

    /*! \brief  Available %attributes defined for the Job Management API package
     *
     *   <b>NOTE:</b> Attributes with a <i>description_</i>prefix are read/write
     *   %attributes and belong to a saga::job::description instance. The rest are
     *   read-only %attributes and available for saga::job::job instances.<br><br>
     *   Although JSDL based %attribute names are used for the %job description
     *   %attributes, the API uses no explicit representation of JSDL (i.e. JSDL
     *   compliant XML).<br>
     *   <br><b>Usage example:</b><br><br>
     *   <code>saga::job::service js("localhost");<br><br>
     *   saga::job::description jd();<br>
     *   jd.set_attribute("description_executable", "/bin/uname");<br><br>
     *   saga::job::job j1 = js.create_job(jd);<br>
     *   std::cout << j1.get_attribute("executionhosts") << std::endl;</code>
     *
     */
    namespace attributes
    {
        //  read only attributes
        /*! \brief <b>[r]</b> SAGA representation of the %job identifier
         *         (format: [backend %url]-[native id])
         */
        char const* const jobid = "JobID";
        /*! \brief <b>[r]</b> List of host names or IP addresses allocated to run this %job
         */
        char const* const execution_hosts = "ExecutionHosts";
        /*! \brief <b>[r]</b> Time stamp of the %job creation in the resource manager
         */
        char const* const created = "Created";
        /*! \brief <b>[r]</b> Time stamp indicating when the %job started running
         */
        char const* const started = "Started";
        /*! \brief <b>[r]</b> Time stamp indicating when the %job completed
         */
        char const* const finished = "Finished";
        /*! \brief <b>[r]</b> Working directory on the execution host
         */
        char const* const working_directory = "WorkingDirectory";
        /*! \brief <b>[r]</b> Process exit code as collected by the wait(2) series of
         *         system calls
         */
        char const* const exitcode = "ExitCode";
        /*! \brief <b>[r]</b> Signal number which caused the %job to exit <br><br>
         */
        char const* const termsig = "Termsig";
        /*! \brief <b>[r]</b> url of job service instance which started this job <br><br>
         */
        char const* const job_service = "JobService";
    }

    /*! \brief   Available %metrics defined for the Job Management API package
     *
     *   Several metrics on the saga::job::job class (the class implements the
     *   saga::monitorable interface) reflect %attributes from the %job
     *   description. This redundancy is intentional, and aims at providing
     *   information about (a) %attributes which may change at runtime, and (b)
     *   %attributes for jobs for which no %job description is available (e.g.
     *   saga::job::job instances obtained by calling saga::job::service::get_job().<br>
     *   <br><b>Usage example:</b><br><br>
     *   <code>//TODO</code>
     */
    namespace metrics
    {
        /*! \brief Fires on state changes of the job, and has the literal
         *         value of the job state enum
         */
        char const* const state = saga::metrics::task_state;
        /*! \brief Fires on state changes of the job, and has the 
         *         value of any platform specific substate the underlying
         *         middleware exposes
         */
        char const* const state_detail = "job.state_detail";
        /*! \brief Fires as a job receives a signal, and has a value
         *         indicating the signal number
         */
        char const* const signal = "job.signal";
        /*! \brief Number of CPU seconds consumed by the job
         */
        char const* const cpu_time = "job.cpu_time";
        /*! \brief Current aggregate memory usage
         */
        char const* const memory_use = "job.memory_use";
        /*! \brief Current aggregate virtual memory usage
         */
        char const* const vmemory_use = "job.vmemory_use";
        /*! \brief Current performance (FLOPS)
         */
        char const* const performance = "job.performance";
    }

    namespace attributes
    {
        // textual representations of job states
        char const* const job_state_unknown   = saga::attributes::task_state_unknown;
        char const* const job_state_new       = saga::attributes::task_state_new;
        char const* const job_state_running   = saga::attributes::task_state_running;
        char const* const job_state_done      = saga::attributes::task_state_done;
        char const* const job_state_canceled  = saga::attributes::task_state_canceled;
        char const* const job_state_failed    = saga::attributes::task_state_failed;
        char const* const job_state_suspended = "Suspended";
    }

    /*! \brief The state is equivalent to the inherited
     * %saga::task::state, but adds the <i>Suspended</i> state.
     *
     */
    enum state
    {
        /*! \brief Not part of the SPEC...
         */
        Unknown   =  saga::task_base::Unknown,   // -1

        /*! \brief This state identies a newly constructed %job instance which
         * has not yet run. This state corresponds to the BES state <i>Pending</i>.
         * This state is initial.
         */
        New       =  saga::task_base::New,       //  1

        /*! \brief The run() method has been invoked on the %job, either explicitly
         * or implicitly. This state corresponds to the BES state <i>Running</i>.
         * This state is initial.
         */
        Running   =  saga::task_base::Running,   //  2

        /*! \brief The synchronous or asynchronous operation has finished
         * successfully. It corresponds to the BES state <i>Finished</i>. This
         * state is final.
         */
        Done      =  saga::task_base::Done,      //  3

        /*! \brief The asynchronous operation has been canceled, i.e. cancel()
         * has been called on the %job instance. It corresponds to the BES state
         * <i>Canceled</i>. This state is final.
         */
        Canceled  =  saga::task_base::Canceled,  //  4

        /*! \brief The synchronous or asynchronous operation has finished
         * unsuccessfully. It corresponds to the BES state <i>Failed</i>. This
         * state is final.
         */
        Failed    =  saga::task_base::Failed,    //  5

        /*! \brief Suspended identifies a %job instance which has been suspended.
         * This state corresponds to the BES state <i>Suspend</i>.
         */
        Suspended =  6
    };

  ///@cond
    namespace detail
    {
        SAGA_JOB_PACKAGE_EXPORT std::string get_state_name(state s);
    }
  ///@endcond

    /*! \brief The job provides the manageability interface to a job instance
     * submitted to a resource manager.
     *
     *   There are two general types of methods: those for
     * retrieving job state and information, and those for manipulating the job.
     * The methods intended to manipulate jobs cannot make any guarantees about how
     * the resource manager will affect an action to be taken. The API implementation
     * is designed to be agnostic of the backend implementation, such that any backend
     * could be implemented to perform an action. For example, the checkpoint routine
     * might cause an application level checkpoint, or might use the services of GridCPR.
     */
    class SAGA_JOB_PACKAGE_EXPORT job
        : public saga::task,
          public saga::detail::attribute<job>,
          public saga::detail::permissions<job>
    {
    private:
        friend struct saga::detail::attribute<job>;  // needs to access get_impl()
        friend struct saga::detail::permissions<job>;

        // job inspection
        SAGA_CALL_PRIV_0(get_job_id)
        SAGA_CALL_PRIV_0(get_state)
        SAGA_CALL_PRIV_0(get_description)

        // job task interface
        SAGA_CALL_PRIV_0(run)
        SAGA_CALL_PRIV_1(cancel, double)
        SAGA_CALL_PRIV_1(wait, double)

        // The following functions are a bit special because they do not provide
        // the usual plain sync interface function returning the result. We have
        // the task based variants available only.
        SAGA_CALL_PRIV_0(get_stdin)
        SAGA_CALL_PRIV_0(get_stdout)
        SAGA_CALL_PRIV_0(get_stderr)

        // job management
        SAGA_CALL_PRIV_0(suspend)
        SAGA_CALL_PRIV_0(resume)
        SAGA_CALL_PRIV_0(checkpoint)
        SAGA_CALL_PRIV_1(migrate, description)
        SAGA_CALL_PRIV_1(signal, int)

    protected:
        /// @cond
        /** These methods are not within API scope */
        typedef saga::detail::attribute<job> attribute_base;

        TR1::shared_ptr <saga::impl::job> get_impl_sp (void) const;
        saga::impl::job * get_impl (void) const;
        bool is_impl_valid() const;

        explicit job (saga::impl::job *);
        /// @endcond

    public:
        /*! \brief Creates the object.
         *
         *
         */
        job();

        /*! \brief Creates the object.
         *
         *
         */
        explicit job (saga::object const& o);

        /*! \brief Destroys the object.
         *
         *
         */
        ~job();

        /*! \brief Brief %description starts here
         *
         *
         */
        job &operator= (saga::object const& o);

        /*! \brief Brief %description starts here
         *
         *
         */
        std::string get_job_id()
        {
            saga::task t = get_job_idpriv(saga::task_base::Sync());
            return t.get_result<std::string>();
        }
        SAGA_CALL_PUB_0_DEF_0(get_job_id)

        /*! \brief Start the job
         *
         *
         */
        void run()
        {
            saga::task t = runpriv(saga::task_base::Sync());
            t.get_result ();
        }
        SAGA_CALL_PUB_0_DEF_0(run)

        /*! \brief Wait for the job to complete
         *
         *
         */
        bool wait(double timeout = -1.0)
        {
            saga::task t = waitpriv(timeout, saga::task_base::Sync());
            return t.get_result<bool>();
        }
        SAGA_CALL_PUB_1_DEF_1(wait, double, -1.0)

        /*! \brief Cancel the job
         *
         *
         */
        void cancel(double timeout = 0.0)
        {
            saga::task t = cancelpriv(timeout, saga::task_base::Sync());
            t.get_result ();
        }
        SAGA_CALL_PUB_1_DEF_1(cancel, double, 0.0)

        /*! \brief Get the state of the job.
         *  \return State of the job
         *
         *
         */
        saga::job::state get_state()
        {
            saga::task t = get_statepriv(saga::task_base::Sync());
            return t.get_result<saga::job::state>();
        }
        SAGA_CALL_PUB_0_DEF_0(get_state)

        /*! \brief Retrieve the description which was used to submit this
         *         job instance.
         *  \return A job_description object
         *
         *
         */
        saga::job::description get_description()
        {
            saga::task t = get_descriptionpriv(saga::task_base::Sync());
            return t.get_result<saga::job::description>();
        }
        SAGA_CALL_PUB_0_DEF_0(get_description)

        /*! \brief Retrieve input stream for a job
         *  \return standard input stream for the job
         *
         *
         */
        saga::job::ostream get_stdin()
        {
            saga::task t = get_stdinpriv(saga::task_base::Sync());
            return t.get_result<saga::job::ostream>();
        }
        SAGA_CALL_PUB_0_DEF_0(get_stdin)

        /*! \brief Retrieve output stream of job
         *         
         *  \return Standard output stream for the job
         *
         *
         */
        saga::job::istream get_stdout()
        {
            saga::task t = get_stdoutpriv(saga::task_base::Sync());
            return t.get_result<saga::job::istream>();
        }
        SAGA_CALL_PUB_0_DEF_0(get_stdout)

        /*! \brief Retrieve error stream of job
         *         
         *  \return Standard error stream for the job
         *
         *
         */
        saga::job::istream get_stderr()
        {
            saga::task t = get_stderrpriv(saga::task_base::Sync());
            return t.get_result<saga::job::istream>();
        }
        SAGA_CALL_PUB_0_DEF_0(get_stderr)

        /*! \brief Ask the resource manager to suspend this job
         *         
         *
         *
         */
        void suspend()
        {
            saga::task t = suspendpriv(saga::task_base::Sync());
            t.get_result ();
        }
        SAGA_CALL_PUB_0_DEF_0(suspend)

        /*! \brief Ask the resource manager to resume this job
         *         
         *
         *
         */
        void resume()
        {
            saga::task t = resumepriv(saga::task_base::Sync());
            t.get_result ();
        }
        SAGA_CALL_PUB_0_DEF_0(resume)

        /*! \brief Ask the resource manager to checkpoint this job
         *         
         *
         *
         */
        void checkpoint()
        {
            saga::task t = checkpointpriv(saga::task_base::Sync());
            t.get_result ();
        }
        SAGA_CALL_PUB_0_DEF_0(checkpoint)

        /*! \brief Ask the resource manager to migrate this job
         *         
         *  \param job_desc New job parameters to apply when the job is migrated
         *
         *
         */
        void migrate(description job_desc)
        {
            saga::task t = migratepriv(job_desc, saga::task_base::Sync());
            t.get_result ();
        }
        SAGA_CALL_PUB_1_DEF_0(migrate, description)

        /*! \brief Ask the resource manager to deliver an arbitrary signal to
         *         this job
         *         
         *  \param signum Signal number to be delivered
         *
         *
         */
        void (signal)(int signal)
        {
            saga::task t = signalpriv(signal, saga::task_base::Sync());
            t.get_result ();
        }
        SAGA_CALL_PUB_1_DEF_0(signal, int)
    };

    }   // namespace job

  ///@cond
    namespace adaptors
    {
        saga::job::state SAGA_JOB_PACKAGE_EXPORT 
            job_state_value_to_enum(std::string const& val);
        std::string SAGA_JOB_PACKAGE_EXPORT 
            job_state_enum_to_value(int s);
    }
  ///@endcond
}   // namespace saga

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif

#endif // !defined(SAGA_DEBUG)

// re-enable warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

#endif // !defined(SAGA_PACKAGES_JOB_JOB_HPP)

