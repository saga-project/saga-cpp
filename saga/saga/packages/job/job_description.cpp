//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  this is needed in every file including detail/attribute_impl.hpp and not 
//  belonging to the engine

//#define SAGA_ATTRIBUTE_EXPORTS
#define SAGA_NO_IMPORT_ATTRIBUTE
#define SAGA_EXPORT_ATTRIBUTE       SAGA_JOB_PACKAGE_EXPORT

#include <boost/assign/list_inserter.hpp>
#include <boost/assign/std/vector.hpp>

// include job API and implementation
#include <saga/saga/job.hpp>
#include <saga/saga/adaptors/metric.hpp>
#include <saga/impl/job.hpp>

#include <saga/saga/detail/attribute_impl.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace job {

    description::description ()
        : object (new saga::impl::description())
    {
        using namespace boost::assign;
        std::vector<std::string> valid_keys;
        valid_keys += 
              attributes::description_executable,
              attributes::description_arguments,
              attributes::description_environment,
              attributes::description_working_directory, 
              attributes::description_interactive,
              attributes::description_input,
              attributes::description_output,
              attributes::description_error,
              attributes::description_file_transfer,
              attributes::description_cleanup,
              attributes::description_job_start_time,
              attributes::description_total_cpu_time,
              attributes::description_wall_time_limit,
              attributes::description_total_physical_memory,
              attributes::description_cpu_architecture, 
              attributes::description_operating_system_type,
              attributes::description_candidate_hosts,
              attributes::description_queue,
              attributes::description_job_contact,
              attributes::description_job_project,
              attributes::description_spmd_variation,
              attributes::description_total_cpu_count,
              attributes::description_number_of_processes,
              attributes::description_processes_per_host,
              attributes::description_threads_per_process
        ;

      // initialize list of valid keys          
      this->init_keynames(valid_keys);

      strmap_type attributes_scalar_rw;
      insert(attributes_scalar_rw)
              (attributes::description_executable, "")
              (attributes::description_operating_system_type, "")
              (attributes::description_cpu_architecture, "")
              (attributes::description_total_cpu_count, "")
              (attributes::description_spmd_variation, "")
              (attributes::description_number_of_processes, "")
              (attributes::description_processes_per_host, "")
              (attributes::description_threads_per_process, "")
              (attributes::description_working_directory, "")
              (attributes::description_interactive, "")
              (attributes::description_input, "")
              (attributes::description_output, "")
              (attributes::description_error, "")
              (attributes::description_cleanup, "")
              (attributes::description_job_start_time, "")
              (attributes::description_total_cpu_time, "")
              (attributes::description_wall_time_limit, "")
              (attributes::description_total_physical_memory, "")
              (attributes::description_queue, "")
              (attributes::description_job_contact, "")
          ;

      strmap_type attributes_vector_rw;
      insert(attributes_vector_rw)
              (attributes::description_candidate_hosts, "")
              (attributes::description_file_transfer, "")
              (attributes::description_environment, "")
              (attributes::description_arguments, "")
              (attributes::description_candidate_hosts, "")
              (attributes::description_job_project, "")
          ;

      // initialize attribute implementation
      this->init (strmap_type(), attributes_scalar_rw, 
          strmap_type(), attributes_vector_rw);
      this->init (false, true);   // cache only implementation
  }

  description::description(saga::impl::description *impl)
    : object (impl)
  {
  }

  description::~description (void)
  {
  }

  /////////////////////////////////////////////////////////////////////////////
  description::description (saga::object const& o)
    : saga::object (o)
  {
      if (this->get_type() != saga::object::JobDescription)
      {
          SAGA_THROW("Bad type conversion.", saga::BadParameter);
      }
  }

  description &description::operator= (saga::object const& o)
  {
      return saga::object::operator=(o), *this;
  }

  } // namespace job

  namespace detail
  {
    /////////////////////////////////////////////////////////////////////////////
    //  implement the attribute functions (we need to explicitly specialize 
    //  the template because the functions are not implemented inline)
    template struct SAGA_JOB_PACKAGE_EXPORT_REPEAT attribute<job::description>;

    template struct SAGA_JOB_PACKAGE_EXPORT attribute_priv<job::description, task_base::Async>;
    template struct SAGA_JOB_PACKAGE_EXPORT attribute_priv<job::description, task_base::Task>;
    template struct SAGA_JOB_PACKAGE_EXPORT attribute_priv<job::description, task_base::Sync>;

    template struct SAGA_JOB_PACKAGE_EXPORT attribute_sync<job::description>;
  }

///////////////////////////////////////////////////////////////////////////////////
} // namespace saga


