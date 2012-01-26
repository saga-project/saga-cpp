#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/job.cpp")
#endif
//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif

//  this is needed in every file including detail/attribute_impl.hpp and not 
//  belonging to the engine
#define SAGA_NO_IMPORT_ATTRIBUTE
#define SAGA_EXPORT_ATTRIBUTE       SAGA_JOB_PACKAGE_EXPORT

#define SAGA_NO_IMPORT_PERMISSIONS
#define SAGA_EXPORT_PERMISSIONS     SAGA_JOB_PACKAGE_EXPORT

#include <boost/assign/list_inserter.hpp>
#include <boost/assign/std/vector.hpp>

// include job API and implementation
#include <saga/saga/job.hpp>
#include <saga/saga/adaptors/metric.hpp>
#include <saga/impl/job.hpp>
#include <saga/saga/detail/call.hpp>

#include <saga/saga/detail/attribute_impl.hpp>
#include <saga/saga/detail/permissions_impl.hpp>

#ifdef SAGA_DEBUG
#include <saga/saga/packages/job/preprocessed/job.cpp>
#else

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/job.cpp")
#endif
///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace job {

    namespace metrics
    {
        /// @cond
        saga::metrics::init_data const job_metric_data[] = 
        {
          // task specific metrics
            {
                saga::metrics::task_state,
                "Metric to monitor the current state of the job, "
                    "e.g. New, Running, Canceled, Done, or Failed",
                saga::attributes::metric_mode_readonly,
                "1", 
                saga::attributes::metric_type_enum,
                "New"   // saga::job::New
            },

            // job specific metrics
            {
                saga::job::metrics::state_detail,
                "Metric fires on state detail changes of the job.",
                saga::attributes::metric_mode_readonly,
                "1", 
                saga::attributes::metric_type_string,
                ""
            },
            {
                saga::job::metrics::signal,
                "Metric fires as a job receives a signal, and has a "
                    "value indicating the signal number.",
                saga::attributes::metric_mode_readonly,
                "1", 
                saga::attributes::metric_type_int,
                ""
            },
            {
                saga::job::metrics::cpu_time,
                "Metric gives access to number of cpu seconds consumed by the job.",
                saga::attributes::metric_mode_readonly,
                "seconds", 
                saga::attributes::metric_type_int,
                ""
            },
            {
                saga::job::metrics::memory_use,
                "Metric gives access to the current aggregate memory usage "
                    "of the job.",
                saga::attributes::metric_mode_readonly,
                "megabyte", 
                saga::attributes::metric_type_float,
                "0.0"
            },
            {
                saga::job::metrics::vmemory_use,
                "Metric gives access to the current aggregate virtual memory "
                    "usage of the job.",
                saga::attributes::metric_mode_readonly,
                "megabyte", 
                saga::attributes::metric_type_float,
                "0.0"
            },
            {
                saga::job::metrics::performance,
                "Metric gives access to the current performance of the job.",
                saga::attributes::metric_mode_readonly,
                "FLOPS", 
                saga::attributes::metric_type_float,
                "0.0"
            },
        };
    /// @endcond
    }

    namespace detail 
    {

    SAGA_JOB_PACKAGE_EXPORT std::string get_state_name(state s)
    {
        switch (s) {
        case saga::job::New: return "New";
        case saga::job::Running:  return "Running";
        case saga::job::Done:  return "Done";
        case saga::job::Canceled: return "Canceled";
        case saga::job::Failed: return "Failed";
        case saga::job::Suspended: return "Suspended";
        default:
        case saga::job::Unknown:
            return "Unknown";
        }
    }

    }

  /////////////////////////////////////////////////////////////////////////////
  job::job()
  {
  }
  
  job::job(saga::impl::job* impl)
    : task(impl)
  {
      // initialize attributes
      using namespace boost::assign;
      std::vector<std::string> valid_keys;
      valid_keys += 
              attributes::jobid,
              attributes::execution_hosts,
              attributes::created,
              attributes::started,
              attributes::finished,
              attributes::working_directory,
              attributes::exitcode,
              attributes::termsig,
              attributes::job_service
          ;

      // initialize list of valid keys
      this->attribute_base::init_keynames(valid_keys);

      strmap_type attributes_scalar_ro;
      insert(attributes_scalar_ro)
              (attributes::jobid, "")
          ;

      strmap_type attributes_scalar_rw;
      insert(attributes_scalar_rw)
              (attributes::created, "")
              (attributes::started, "")
              (attributes::finished, "")
              (attributes::working_directory, "")
              (attributes::exitcode, "")
              (attributes::termsig, "")
              (attributes::job_service, "")
          ;

      strmap_type attributes_vector_rw;
      insert(attributes_vector_rw)
              (attributes::execution_hosts, "")
          ;

      // initialize attribute implementation
      this->attribute_base::init (attributes_scalar_ro, attributes_scalar_rw,
          strmap_type(), attributes_vector_rw);
      
      // initialize metrics
      std::vector<saga::metric> metrics;
      for (unsigned int i = 0; 
           i < sizeof(saga::job::metrics::job_metric_data)/sizeof(saga::metrics::init_data);
           ++i)
      {
          saga::metrics::init_data const* p = &saga::job::metrics::job_metric_data[i];
          saga::metric m(*this, p->name, p->description, p->mode, p->unit, 
              p->type, p->value);
          metrics.push_back(m);
      }
      this->monitorable_base::init (metrics);
  }
  
  job::~job (void)
  {
  }

  /////////////////////////////////////////////////////////////////////////////
  job::job (saga::object const& o)
    : saga::task (o)
  {
      if (this->get_type() != saga::object::Job)
      {
          SAGA_THROW("Bad type conversion.", saga::BadParameter);
      }
  }

  job &job::operator= (saga::object const& o)
  {
      return saga::task::operator=(o), *this;
  }

  /////////////////////////////////////////////////////////////////////////////
  saga::impl::job* job::get_impl() const
  { 
      typedef saga::object base_type;
      return static_cast<saga::impl::job*>(this->base_type::get_impl()); 
  }

  TR1::shared_ptr <saga::impl::job> job::get_impl_sp(void) const
  { 
      // FIXME: this needs documentation
      typedef saga::object base_type;
      return TR1::static_pointer_cast <saga::impl::job> (
          this->base_type::get_impl_sp());
  }

  bool job::is_impl_valid() const
  {
      typedef saga::object base_type;
      if (!this->base_type::is_impl_valid()) 
          return false;

      if (TR1::dynamic_pointer_cast<saga::impl::job>(
              this->base_type::get_impl_sp()))
      {
          return true;
      }
      return false;
  }

  // job task interface
  SAGA_CALL_IMP_0_EX(job, run, job_run)
  SAGA_CALL_IMP_1_EX(job, cancel, job_cancel, double)
  SAGA_CALL_IMP_1_EX(job, wait, job_wait, double)

  // job inspection
  SAGA_CALL_IMP_0 (job, get_job_id)
  SAGA_CALL_IMP_0 (job, get_state)
  SAGA_CALL_IMP_0 (job, get_description)
  SAGA_CALL_IMP_0 (job, get_stdin)
  SAGA_CALL_IMP_0 (job, get_stdout)
  SAGA_CALL_IMP_0 (job, get_stderr)

  // job management
  SAGA_CALL_IMP_0 (job, suspend)
  SAGA_CALL_IMP_0 (job, resume)
  SAGA_CALL_IMP_0 (job, checkpoint)
  SAGA_CALL_IMP_1 (job, migrate, description)
  SAGA_CALL_IMP_1 (job, signal, int)

  }   // namspace job

  namespace detail
  {
    /////////////////////////////////////////////////////////////////////////////
    //  implement the attribute functions (we need to explicitly specialize 
    //  the template because the functions are not implemented inline)
    template struct SAGA_JOB_PACKAGE_EXPORT_REPEAT attribute<job::job>;

    template struct SAGA_JOB_PACKAGE_EXPORT attribute_priv<job::job, task_base::Sync>;
    template struct SAGA_JOB_PACKAGE_EXPORT attribute_priv<job::job, task_base::Async>;
    template struct SAGA_JOB_PACKAGE_EXPORT attribute_priv<job::job, task_base::Task>;

    template struct SAGA_JOB_PACKAGE_EXPORT attribute_sync<job::job>;

    /////////////////////////////////////////////////////////////////////////////
    //  implement the permissions functions (we need to explicitly specialize 
    //  the template because the functions are not implemented inline)
    template struct SAGA_JOB_PACKAGE_EXPORT_REPEAT permissions<job::job>;

    template struct SAGA_JOB_PACKAGE_EXPORT permissions_priv<job::job, task_base::Sync>;
    template struct SAGA_JOB_PACKAGE_EXPORT permissions_priv<job::job, task_base::Async>;
    template struct SAGA_JOB_PACKAGE_EXPORT permissions_priv<job::job, task_base::Task>;
  }

  namespace adaptors
  {
    saga::job::state job_state_value_to_enum(std::string const& val)
    {
      if (val == saga::job::attributes::job_state_new) 
        return saga::job::New;

      if (val == saga::job::attributes::job_state_done) 
        return saga::job::Done;

      if (val == saga::job::attributes::job_state_running) 
        return saga::job::Running;

      if (val == saga::job::attributes::job_state_failed) 
        return saga::job::Failed;

      if (val == saga::job::attributes::job_state_canceled) 
        return saga::job::Canceled;

      if (val == saga::job::attributes::job_state_suspended) 
        return saga::job::Suspended;

      return saga::job::Unknown;
    }

    std::string job_state_enum_to_value(int s)
    {
      return saga::job::detail::get_state_name((saga::job::state)s);
    }
  }

///////////////////////////////////////////////////////////////////////////////////
} // namespace saga

#endif

