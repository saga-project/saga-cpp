//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2008 Andre Merzky (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  this is needed in every file including detail/attribute_impl.hpp and not 
//  belonging to the engine
//#define SAGA_ATTRIBUTE_EXPORTS
#define SAGA_NO_IMPORT_ATTRIBUTE
#define SAGA_EXPORT_ATTRIBUTE       SAGA_CPR_PACKAGE_EXPORT

#include <boost/assign/list_inserter.hpp>
#include <boost/assign/std/vector.hpp>

// include cpr API and implementation
#include <saga/saga/cpr.hpp>
#include <saga/saga/adaptors/metric.hpp>
#include <saga/impl/cpr.hpp>

#include <saga/saga/detail/attribute_impl.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace cpr {

    description::description ()
        : saga::job::description (new saga::impl::cpr_job_description ())
    {
        using namespace boost::assign;
        std::vector<std::string> valid_keys;
        valid_keys += 
              saga::job::attributes::description_executable,
              saga::job::attributes::description_arguments,
              saga::job::attributes::description_environment,
              saga::job::attributes::description_working_directory, 
              saga::job::attributes::description_interactive,
              saga::job::attributes::description_input,
              saga::job::attributes::description_output,
              saga::job::attributes::description_error,
              saga::job::attributes::description_file_transfer,
              saga::job::attributes::description_cleanup,
              saga::job::attributes::description_job_start_time,
              saga::job::attributes::description_total_cpu_time,
              saga::job::attributes::description_wall_time_limit,
              saga::job::attributes::description_total_physical_memory,
              saga::job::attributes::description_cpu_architecture, 
              saga::job::attributes::description_operating_system_type,
              saga::job::attributes::description_candidate_hosts,
              saga::job::attributes::description_queue,
              saga::job::attributes::description_job_contact,
              saga::job::attributes::description_job_project,
              saga::job::attributes::description_spmd_variation,
              saga::job::attributes::description_total_cpu_count,
              saga::job::attributes::description_number_of_processes,
              saga::job::attributes::description_processes_per_host,
              saga::job::attributes::description_threads_per_process
        ;

      // initialize list of valid keys          
      this->init_keynames(valid_keys);

      strmap_type attributes_scalar_rw;
      insert(attributes_scalar_rw)
              (saga::job::attributes::description_executable, "")
              (saga::job::attributes::description_operating_system_type, "")
              (saga::job::attributes::description_cpu_architecture, "")
              (saga::job::attributes::description_total_cpu_count, "")
              (saga::job::attributes::description_spmd_variation, "")
              (saga::job::attributes::description_number_of_processes, "")
              (saga::job::attributes::description_processes_per_host, "")
              (saga::job::attributes::description_threads_per_process, "")
              (saga::job::attributes::description_working_directory, "")
              (saga::job::attributes::description_interactive, "")
              (saga::job::attributes::description_input, "")
              (saga::job::attributes::description_output, "")
              (saga::job::attributes::description_error, "")
              (saga::job::attributes::description_cleanup, "")
              (saga::job::attributes::description_job_start_time, "")
              (saga::job::attributes::description_total_cpu_time, "")
              (saga::job::attributes::description_wall_time_limit, "")
              (saga::job::attributes::description_total_physical_memory, "")
              (saga::job::attributes::description_queue, "")
              (saga::job::attributes::description_job_contact, "")
          ;

      strmap_type attributes_vector_rw;
      insert(attributes_vector_rw)
              (saga::job::attributes::description_candidate_hosts, "")
              (saga::job::attributes::description_file_transfer, "")
              (saga::job::attributes::description_environment, "")
              (saga::job::attributes::description_arguments, "")
              (saga::job::attributes::description_candidate_hosts, "")
              (saga::job::attributes::description_job_project, "")
          ;

      // initialize attribute implementation
      this->init (strmap_type(), attributes_scalar_rw, 
          strmap_type(), attributes_vector_rw);
      this->init (false, true);   // cache only implementation
  }

  description::description(saga::impl::cpr_job_description *impl)
    : saga::job::description (impl)
  {
  }

  description::~description (void)
  {
  }

  /////////////////////////////////////////////////////////////////////////////
  description::description (saga::object const& o)
    : saga::job::description (o)
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

  // /////////////////////////////////////////////////////////////////////////////
  // //  implement the attribute functions (we need to explicitly specialize 
  // //  the template because the functions are not implemented inline)
  // template struct SAGA_CPR_PACKAGE_EXPORT saga::detail::attribute<cpr::description>;
  // 
  // template struct SAGA_CPR_PACKAGE_EXPORT saga::detail::attribute_priv<cpr::description, task_base::Async>;
  // template struct SAGA_CPR_PACKAGE_EXPORT saga::detail::attribute_priv<cpr::description, task_base::Task>;
  // template struct SAGA_CPR_PACKAGE_EXPORT saga::detail::attribute_priv<cpr::description, task_base::Sync>;

///////////////////////////////////////////////////////////////////////////////////
} // namespace saga


