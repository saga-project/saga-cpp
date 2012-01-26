#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/job_service.cpp")
#endif
//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 0, output: null)
#endif
#define SAGA_NO_IMPORT_ATTRIBUTE

#include <saga/saga/adaptors/task.hpp>
#include <saga/impl/packages/job/job.hpp>
#include <saga/impl/packages/job/job_service.hpp>
#include <saga/saga/adaptors/utils/utils.hpp>
#include <saga/saga/adaptors/instance_data.hpp>
#include <saga/saga/adaptors/packages/job_service_cpi_instance_data.hpp>

#include <saga/saga/attribute.hpp>
#include <saga/saga/detail/attribute_impl.hpp>

#ifdef SAGA_DEBUG
#include <saga/impl/packages/data/job_service/preprocessed/job_service.cpp>
#else

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, output: "preprocessed/job_service.cpp")
#endif
///////////////////////////////////////////////////////////////////////////////
namespace saga 
{ 
  namespace impl 
  {
    job_service::job_service (saga::session const& s, saga::url rm)
        : proxy (saga::object::JobService, s)
    {
        // initialize the instance data
        typedef adaptors::v1_0::job_service_cpi_instance_data instance_data_type;
        typedef adaptors::instance_data <instance_data_type> job_service_instance_data;

        {
            job_service_instance_data data;
            data.init_data (this, TR1::shared_ptr <instance_data_type> (
                new instance_data_type (rm)));
        } // job_service_instance_data lock goes out of scope
    }
    
    
    job_service::job_service (saga::object::type t) 
      : proxy (t, saga::detail::get_the_session()), 
        need_to_free_instance_data(false)
    {
        // no need to init instance data here, this is done in the derived class
    }

    job_service::job_service (saga::object::type t, 
                              saga::session const & s) 
    :   proxy (t, s), need_to_free_instance_data(false)
    {
        // no need to init instance data here, this is done in the derived class
    }

    void job_service::init()
    {
        try {
            // initialize a CPI instance and execute required functions
            this->initcpi(saga::adaptors::job_service_cpi);
        }
        catch (saga::exception const& e) {
            // map special exceptions
            switch (e.get_error()) {
            case saga::adaptors::AdaptorDeclined:
            case saga::adaptors::NoAdaptor:
            case saga::adaptors::NoAdaptorInfo:
            case saga::adaptors::Unexpected:
                SAGA_THROW_PLAIN(e.get_object(), e.what(), saga::NoSuccess);
            // just rethrow otherwise
            default:
                throw;
            }
        }
    }
    
    job_service::~job_service (void) 
    {
      if (need_to_free_instance_data) {
        // free the instance data
        typedef adaptors::v1_0::job_service_cpi_instance_data instance_data_type;
        typedef adaptors::instance_data <instance_data_type> job_service_instance_data;

        job_service_instance_data data;
        data.release_data (this);
      }
    }

    ///////////////////////////////////////////////////////////////////////////
    // factory support
    void job_service::create_impl_sync(saga::job::service& retval)
    {
        this->init();
        retval = saga::job::service(this);
    }

    saga::task job_service::create_impl_async(saga::session const& s, 
        saga::url const& rm)
    {
        TR1::shared_ptr<job_service> impl(new job_service(s, rm));

        bool (job_service::*prep)(saga::job::service&, saga::uuid) = NULL;
        return saga::adaptors::task("__init__", TR1::shared_ptr<v1_0::cpi>(), 
            impl.get(), &job_service::create_impl_sync, prep);
    }

    // API methods of job_service
    SAGA_CALL_IMPL_IMPL_1(job_service, job_service_cpi, create_job, saga::job::description)
#if 0
    SAGA_CALL_IMPL_IMPL_0(job_service, job_service_cpi, get_url)
#else
    saga::task job_service::get_url (bool is_sync)
    {
        SAGA_LOG_INFO("Using default fallback for get_url");
        saga::task t;
        try {
            t = saga::impl::execute_sync_async (this, "job_service_cpi", "get_url", 
                "job_service_cpi::get_url", is_sync, 
                &job_service_cpi::sync_get_url, &job_service_cpi::async_get_url);

            if ( is_sync )
            {
              t.wait ();
            }

            if ( t.get_state () == saga::task::Failed )
            {
              t.rethrow ();
            }

            return t;
        } 
        catch (saga::exception const& /*e*/) {
            SAGA_LOG_INFO("Using fallback for get_url after catching exception from default fallback");

            // the fall back is to just return the value stored in the instance 
            // data
            typedef adaptors::v1_0::job_service_cpi_instance_data instance_data_type;
            typedef adaptors::instance_data <instance_data_type> job_service_instance_data;

            job_service_instance_data data ((proxy*)this);
            saga::task t(saga::task::Done);
            t.get_result<saga::url>() = data->rm_;
            return t;
        }
    }
#endif

#if 0
    SAGA_CALL_IMPL_IMPL_5(job_service, job_service_cpi, run_job, 
                          std::string, std::string, saga::job::ostream&, 
                          saga::job::istream&, saga::job::istream&)
#else
    saga::task 
    job_service::run_job (std::string cmd, std::string host, 
        saga::job::ostream& in, saga::job::istream& out, 
        saga::job::istream& err, bool is_sync)
    {
      saga::task t;
      try {
        t = saga::impl::execute_sync_async (this, 
                                            "job_service_cpi", 
                                            "run_job", 
                                            "job_service_cpi::run_job", 
                                            is_sync, 
                                            &job_service_cpi::sync_run_job,
                                            &job_service_cpi::async_run_job, 
                                            cmd, 
                                            host, 
                                            TR1::ref (in), 
                                            TR1::ref (out), 
                                            TR1::ref (err));

        if ( is_sync )
        {
          t.wait ();
        }

        if ( t.get_state () == saga::task::Failed )
        {
          t.rethrow ();
        }

        return t;
      } 
      catch (saga::exception const& /*e*/) 
 //   catch (saga::not_implemented const& /*e*/) 
      {
        SAGA_LOG_INFO("Using fallback for run_job");
        std::vector<std::string> elems = saga::adaptors::utils::split_commandline(cmd);
        std::string executable = elems[0];

        // we need at least the executable
        if (elems.empty())
        {
            SAGA_THROW_PLAIN(this, "Could not parse command", saga::BadParameter);
        }

        saga::job::description jd;

        if (!host.empty())
        {
            std::vector <std::string> chosts;
            chosts.push_back (host);

            jd.set_vector_attribute (saga::job::attributes::description_candidate_hosts, chosts);
        }

        jd.set_attribute (saga::job::attributes::description_executable, elems[0]);
        jd.set_attribute (saga::job::attributes::description_interactive,
                          saga::attributes::common_true);
        if ( elems.size() > 1 )
        {
            elems.erase (elems.begin());
            jd.set_vector_attribute(saga::job::attributes::description_arguments, elems);
        }

        saga::job::service js (runtime::get_object(
            TR1::static_pointer_cast<saga::impl::object>(
                this->shared_from_this())));

        saga::task create_task = js.create_job <saga::task_base::Async> (jd); 
        create_task.wait ();
        saga::job::job j = create_task.get_result <saga::job::job> ();

        in  = j.get_stdin  ();
        out = j.get_stdout ();
        err = j.get_stderr ();

        j.run ();

        // ATTENTION: the little bugger we return is actually
        // a saga::job_service::create_job task, not
        // a saga::job_service::run_job task.  But as both have the same type,
        // the user should not see any difference.  
        // Also, get_result has been called on this task above, so its not
        // exactly virgin anymore.  Anyway, at the moment we don't care if
        // get_result is called twice...
        return create_task;
      }
    }
#endif

#if 0
    SAGA_CALL_IMPL_IMPL_2(job_service, job_service_cpi, run_job_noio, 
                          std::string, std::string)
#else
    saga::task 
    job_service::run_job_noio (std::string cmd, std::string host, bool is_sync)
    {
      SAGA_LOG_INFO("trying fallback for run_job_noio");
      saga::task t;
      try {
        t = saga::impl::execute_sync_async (this, "job_service_cpi", "run_job_noio", 
            "job_service_cpi::run_job_noio", is_sync, 
            &job_service_cpi::sync_run_job_noio, &job_service_cpi::async_run_job_noio, 
            cmd, host);
        
        if ( is_sync )
        {
          t.wait ();
        }

        if ( t.get_state () == saga::task::Failed )
        {
          t.rethrow ();
        }

        return t;
      } 
      catch (saga::exception const& /*e*/) 
 //   catch (saga::not_implemented const& /*e*/) 
      {
        SAGA_LOG_INFO("using fallback for run_job_noio");
        std::vector<std::string> elems = saga::adaptors::utils::split_commandline(cmd);
        std::string executable = elems[0];

        // we need at least the executable
        if (elems.empty())
        {
            SAGA_THROW_PLAIN(this, "Could not parse command", saga::BadParameter);
        }

        saga::job::description jd;

        if (!host.empty())
        {
            std::vector <std::string> chosts;
            chosts.push_back (host);

            jd.set_vector_attribute (saga::job::attributes::description_candidate_hosts, chosts);
        }

        jd.set_attribute (saga::job::attributes::description_executable, elems[0]);
        jd.set_attribute (saga::job::attributes::description_interactive,
                          saga::attributes::common_false);
        if ( elems.size() > 1 )
        {
            elems.erase (elems.begin());
            jd.set_vector_attribute(saga::job::attributes::description_arguments, elems);
        }

        saga::job::service js (runtime::get_object(
            TR1::static_pointer_cast<saga::impl::object>(
                this->shared_from_this())));

        saga::task create_task = js.create_job <saga::task_base::Async> (jd); 
        create_task.wait ();
        saga::job::job j = create_task.get_result <saga::job::job> ();

        j.run ();

        // ATTENTION: the little bugger we return is actually
        // a saga::job_service::create_job task, not
        // a saga::job_service::run_job task.  But as both have the same type,
        // the user should not see any difference.  
        // Also, get_result has been called on this task above, so its not
        // exactly virgin anymore.  Anyway, at the moment we don't care if
        // get_result is called twice...
        return create_task;
      }
    }
#endif

    SAGA_CALL_IMPL_IMPL_0(job_service, job_service_cpi, list)
    SAGA_CALL_IMPL_IMPL_1(job_service, job_service_cpi, get_job, std::string)
    SAGA_CALL_IMPL_IMPL_0(job_service, job_service_cpi, get_self)

}} // namespace saga::impl
///////////////////////////////////////////////////////////////////////////////


#endif

