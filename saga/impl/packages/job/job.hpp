#ifndef SAGA_IMPL_PACKAGES_JOB_JOB_HPP
#define SAGA_IMPL_PACKAGES_JOB_JOB_HPP

#if defined(__WAVE__)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/job.hpp")
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
#include <saga/impl/packages/job/job_cpi.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable: 4251 4231 4275 4660)
#endif

#ifdef SAGA_DEBUG
#include <saga/saga/packages/job/preprocessed/job.hpp>
#else

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: "preprocessed/job.hpp")
#endif
///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl 
{
    // implement the task_interface for a job object
    template <typename Derived>
    struct job_task_interface 
      : public saga::impl::task_interface
    {
        Derived& derived() 
            { return static_cast<Derived&>(*this); }
        Derived const& derived() const 
            { return static_cast<Derived const&>(*this); }
            
        std::string get_func_name(void) const
        {
            SAGA_THROW("Do not call the 'get_func_name()' function on a "
                       "saga::impl::job object instance", saga::NotImplemented);
            return std::string();
        }

        saga::session& get_session() 
        {
            return derived().saga::impl::proxy::get_session();
        }
        saga::session const& get_session() const
        {
            return derived().saga::impl::proxy::get_session();
        }

        v1_0::cpi* 
        get_bulk_adaptor(std::string const&, std::string const&, 
            v1_0::preference_type const&)
        {
            SAGA_THROW("Do not call the 'get_bulk_adaptor()' function on a "
                       "saga::impl::job object instance", saga::NotImplemented);
            return NULL;
        }

        void visit_args(v1_0::cpi*)
        {
            SAGA_THROW("Do not call the 'visit_args()' function on a "
                       "saga::impl::job object instance", saga::NotImplemented);
        }
        
        void rethrow() const
        {
            SAGA_THROW("Do not call the 'rethrow()' function on a "
                       "saga::impl::job object instance", saga::NotImplemented);
        }
        
        saga::object get_object() const
        {
            SAGA_THROW("Do not call the 'get_object()' function on a "
                       "saga::impl::job object instance", saga::NotImplemented);
            return saga::object();
        }

        saga::detail::hold_any& get_result()
        {
            SAGA_THROW("Do not call the 'get_result()' function on a "
                       "saga::impl::job object instance", saga::NotImplemented);
            static saga::detail::hold_any any_;
            return any_;
        }

        void set_selector_state(TR1::shared_ptr<impl::adaptor_selector_state>)
        {
            SAGA_THROW("Do not call the 'set_selector_state()' function on a "
                       "saga::impl::job object instance", saga::NotImplemented);
        }

        void set_task_exception(saga::impl::object const *obj,
            saga::impl::exception_list const&, saga::error)
        {
            SAGA_THROW("Do not call the 'set_task_exception()' function on a "
                       "saga::impl::job object instance", saga::NotImplemented);
        }

        bool restart()
        {
            SAGA_THROW("Do not call the 'restart()' function on a "
                       "saga::impl::job object instance", saga::NotImplemented);
            return false;
        }

        void set_state(saga::task_base::state)
        {
            SAGA_THROW("Do not call the 'set_state()' function on a "
                       "saga::impl::job object instance", saga::NotImplemented);
        }

        void set_external_treatment (bool = true)
        {
            SAGA_THROW("Do not call the 'set_external_treatment()' function on a "
                       "saga::impl::job object instance", saga::NotImplemented);
        }

        void set_bulk_treatment (bulk_treatment_state = actual_bulk_treated)
        {
            SAGA_THROW("Do not call the 'set_bulk_treatment()' function on a "
                       "saga::impl::job object instance", saga::NotImplemented);
        }

        void set_external_bulk_async(
            bulk_async_in_adaptor_state = will_async_in_adaptor)
        {
            SAGA_THROW("Do not call the 'set_external_bulk_async()' function on a "
                       "saga::impl::job object instance", saga::NotImplemented);
        }
    };

    /**
     * This class acts as a proxy class, hiding all the nasty Saga stuff
     * (like creating and using adaptors) for the user. Users just call
     * the functions of this class like to would on every local object,
     * and don't have to worry about adaptors. It extends saga::job_cpi.
     *
     * Note, the impl::job class is somewhat specific as it represents a
     * task (wrapping the service::create_job() function) and at the same 
     * time it is a proxy for job CPI's.
     */
    class SAGA_JOB_PACKAGE_EXPORT job 
      : public saga::impl::job_task_interface<job>,
        public saga::impl::proxy,
        public saga::impl::attribute,
        public saga::impl::monitorable,
        public saga::impl::permissions
    {
      typedef v1_0::job_cpi job_cpi;
      typedef v1_0::preference_type preference_type;

     private:
     // implement task interface by the means of the job_cpi
        int run() { job_run(true); return 1; }
        void cancel() { job_cancel(true); }
        bool wait(double timeout) 
        {
            saga::task t = job_wait(timeout, true); 
            return t.get_result<bool>(); 
        }
        saga::task_base::state get_state (void) const
        {
            saga::task t = const_cast<job*>(this)->get_state(true); 
            return (saga::task_base::state)t.get_result<saga::job::state>(); 
        }

     // initialize newly attached CPI instance
        void init();

     public:
      /**
       * Constructor of job
       */
      job (saga::url rm, saga::job::description jd, saga::session const& s, 
           saga::object::type t = saga::object::Job);
      job (saga::url rm, std::string jobid, saga::session const& s, 
           saga::object::type t = saga::object::Job);

      /*! destructor of job, destroys job and the adaptor it is 
       *  bound to */
      ~job (void);

      // task interface
      SAGA_CALL_IMPL_DECL_0(job_run)
      SAGA_CALL_IMPL_DECL_1(job_cancel, double)
      SAGA_CALL_IMPL_DECL_1(job_wait, double)

      // job inspection
      SAGA_CALL_IMPL_DECL_0(get_job_id)
      SAGA_CALL_IMPL_DECL_0(get_state)
      SAGA_CALL_IMPL_DECL_0(get_description)
      SAGA_CALL_IMPL_DECL_0(get_stdin)
      SAGA_CALL_IMPL_DECL_0(get_stdout)
      SAGA_CALL_IMPL_DECL_0(get_stderr)

      // job management
      SAGA_CALL_IMPL_DECL_0(suspend)
      SAGA_CALL_IMPL_DECL_0(resume)
      SAGA_CALL_IMPL_DECL_0(checkpoint)
      SAGA_CALL_IMPL_DECL_1(migrate, saga::job::description)
      SAGA_CALL_IMPL_DECL_1(signal, int)

      ///////////////////////////////////////////////////////////////////////
      //  return the task_interface to the facade 
      saga::impl::task_interface* get_task_interface() { return this; }
      saga::impl::task_interface const* get_task_interface() const { return this; }

      ///////////////////////////////////////////////////////////////////////
      //  return the monitorable interface to the facade 
      saga::impl::monitorable* get_monitorable() { return this; }
      saga::impl::monitorable const* get_monitorable() const { return this; }

      ///////////////////////////////////////////////////////////////////////
      //  return the attribute interface to the facade 
      saga::impl::attribute* get_attributes() { return this; }
      saga::impl::attribute const* get_attributes() const { return this; }

      ///////////////////////////////////////////////////////////////////////
      //  return the permissions interface to the facade 
      saga::impl::permissions* get_permissions() { return this; }
      saga::impl::permissions const* get_permissions() const { return this; }

      ///////////////////////////////////////////////////////////////////////
      //  return the object interface to the facade 
      saga::impl::proxy* get_proxy() { return this; }
      saga::impl::proxy const* get_proxy() const { return this; }

      virtual saga::uuid get_id() const { return this->get_uuid(); }

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

#endif // !defined(SAGA_IMPL_PACKAGES_JOB_JOB_HPP)

