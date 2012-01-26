//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2007 Ole Weidner (oweidner@cct.lsu.edu)
//  Copyright (c) 2008 Andre Merzky (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


#ifndef SAGA_PACKAGES_CPR_JOB_DESCRIPTION_HPP
#define SAGA_PACKAGES_CPR_JOB_DESCRIPTION_HPP

#include <string>
#include <vector>

// include dependent spec sections
#include <saga/saga/util.hpp>
#include <saga/saga/base.hpp>
#include <saga/saga/types.hpp>
#include <saga/saga/session.hpp>
#include <saga/saga/call.hpp>
#include <saga/saga/job.hpp>
#include <saga/saga/detail/attribute.hpp>

#include <saga/saga/packages/cpr/config.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable: 4251 4231 4275 4660)
#endif

///////////////////////////////////////////////////////////////////////////////
namespace saga
{
  namespace cpr 
  {
    namespace attributes 
    {
    //  /*! \brief <b>[rw]</b> Command to execute (this is the only required %attribute for 
    //   *         a %job description %object) 
    //   */
    //  char const* const description_executable          = "Executable";
    //  /*! \brief <b>[rw]</b> Positional parameters for the command
    //   */
    //  char const* const description_arguments           = "Arguments";
    //  /*! \brief <b>[rw]</b> Set of environment variables for the %job (format: 'key=value').
    //   */
    //  char const* const description_environment         = "Environment";
    //  /*! \brief <b>[rw]</b> Working directory for the %job 
    //   */
    //  char const* const description_working_directory    = "WorkingDirectory";
    //  /*! \brief <b>[rw]</b> Run the %job in interactive mode
    //   */
    //  char const* const description_interactive         = "Interactive";
    //  /*! \brief <b>[rw]</b> Pathname of the standard input %file 
    //   */
    //  char const* const description_input               = "Input";
    //  /*! \brief <b>[rw]</b> Pathname of the standard output %file 
    //   */
    //  char const* const description_output              = "Output";
    //  /*! \brief <b>[rw]</b> Pathname of the standard error %file 
    //   */
    //  char const* const description_error               = "Error";
    //  /*! \brief <b>[rw]</b> A list of %file transfer directives 
    //   */
    //  char const* const description_file_transfer        = "FileTransfer";
    //  /*! \brief <b>[rw]</b> Defines if output files get removed after the %job finishes 
    //   */
    //  char const* const description_cleanup             = "Cleanup";
    //  /*! \brief <b>[rw]</b> Time at which a %job should be scheduled
    //   */
    //  char const* const description_job_start_time        = "JobStartTime";
    //  /*! \brief <b>[rw]</b> ?!?
    //   */
    //  char const* const description_total_cpu_time        = "TotalCPUTime";
    //  /*! \brief <b>[rw]</b> Estimated amount of memory the %job requires
    //   */
    //  char const* const description_total_physical_memory = "TotalPhysicalMemory";
    //  /*! \brief <b>[rw]</b> Compatible processor for %job submission
    //   */
    //  char const* const description_cpu_architecture     = "CPUArchitecture";
    //  
    //      /*! \brief <b>[rw]</b> Compatible processor for %job submission values
    //       */
    //      char const* const cpuarchitecture_sparc = "sparc";
    //      char const* const cpuarchitecture_powerpc = "powerpc";
    //      char const* const cpuarchitecture_x86 = "x86";
    //      char const* const cpuarchitecture_x86_32 = "x86_32";
    //      char const* const cpuarchitecture_x86_64 = "x86_64";
    //      char const* const cpuarchitecture_parisc = "parisc";
    //      char const* const cpuarchitecture_mips = "mips";
    //      char const* const cpuarchitecture_ia64 = "ia64";
    //      char const* const cpuarchitecture_arm = "arm";
    //      char const* const cpuarchitecture_other = "other";
    //  
    //  /*! \brief <b>[rw]</b> Compatible operating system for %job submission 
    //   */
    //  char const* const description_operating_system_type = "OperatingSystemType";
    //  /*! \brief <b>[rw]</b> List of host names which are to be considered by the 
    //   *         resource manager as candidate targets 
    //   */
    //  char const* const description_candidate_hosts      = "CandidateHosts";
    //  /*! \brief <b>[rw]</b> Name of a queue to place the %job into
    //   */
    //  char const* const description_queue               = "Queue";
    //  /*! \brief <b>[rw]</b> Set of endpoints describing where to report %job state transitions
    //   */
    //  char const* const description_job_contact          = "JobContact";
    //  /*! \brief <b>[rw]</b> SPMD %job type and startup mechanism 
    //   */
    //  char const* const description_spmd_variation       = "SPMDVariation";
    //  /*! \brief <b>[rw]</b> Total number of cpus requested for this %job 
    //   */
    //  char const* const description_total_cpu_count       = "TotalCPUCount";
    //  /*! \brief <b>[rw]</b> Number of process instances to start 
    //   */
    //  char const* const description_number_of_processes   = "NumberOfProcesses";
    //  /*! \brief <b>[rw]</b> Number of processes to start per host 
    //   */
    //  char const* const description_processes_per_host    = "ProcessesPerHost";
    //  /*! \brief <b>[rw]</b> Expected number of threads per process 
    //   */
    //  char const* const description_threads_per_process   = "ThreadsPerProcess";
    }
    
    /*! \brief This object encapsulates all the attributes which define a job to be run.     
     *
     * The %description encapsulates all the attributes which define a job to be run. It has 
     * no methods of its own, but implements the saga::attributes interface in order 
     * to provide access to the job properties, which are expressed as JSDL keywords. 
     * The only required attribute in order to perform a valid job submission is the 
     * Executable. Given the Executable, a job can be instantiated in many existing 
     * backend systems without any further specification. 
     */
    class SAGA_CPR_PACKAGE_EXPORT description 
        : public saga::job::description
    {
    
    protected:
        /// @cond
        /** These methods are not within API scope */
        friend struct saga::detail::attribute<description>;  // needs to access get_impl()
        friend class saga::impl::cpr_job_description;
        explicit description(saga::impl::cpr_job_description *impl);
        /// @endcond
          
    public:
        /*! \brief Creates the object.     
         *
         *   Detailed %description starts here.
         */
        description (void);

        /*! \brief Creates the object.     
         *
         *   Detailed %description starts here.
         */
        explicit description (saga::object const& o);

        /*! \brief Destroys the object.     
         *
         *   Detailed %description starts here.
         */
        ~description (void);

        /*! \brief Brief %description starts here     
         *
         *   Detailed %description starts here.
         */
        description &operator= (saga::object const& o);

    }; 
    
  }   // namespace cpr
}   

// re-enable warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

#endif // !defined(SAGA_PACKAGES_CPR_JOB_DESCRIPTION_HPP)

