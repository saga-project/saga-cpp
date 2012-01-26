//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2007 Ole Weidner (oweidner@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_PACKAGES_JOB_JOB_DESCRIPTION_HPP
#define SAGA_PACKAGES_JOB_JOB_DESCRIPTION_HPP

#include <string>
#include <vector>

// include dependent spec sections
#include <saga/saga/util.hpp>
#include <saga/saga/base.hpp>
#include <saga/saga/types.hpp>
#include <saga/saga/session.hpp>
#include <saga/saga/call.hpp>
#include <saga/saga/detail/attribute.hpp>

#include <saga/saga/packages/job/config.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable: 4251 4231 4275 4660)
#endif

///////////////////////////////////////////////////////////////////////////////
namespace saga
{
    namespace job {

    namespace attributes 
    {
        /*! \brief <b>[rw]</b> Command to execute (this is the only required %attribute for 
         *         a %job %description %object) 
         */
        char const* const description_executable          = "Executable";
        /*! \brief <b>[rw]</b> Positional parameters for the command
         */
        char const* const description_arguments           = "Arguments";
        /*! \brief <b>[rw]</b> Set of environment variables for the %job (format: 'key=value').
         */
        char const* const description_environment         = "Environment";
        /*! \brief <b>[rw]</b> Working directory for the %job
         */
        char const* const description_working_directory   = "WorkingDirectory";
        /*! \brief <b>[rw]</b> Run the %job in interactive mode
         */
        char const* const description_interactive         = "Interactive";
        /*! \brief <b>[rw]</b> Pathname of the standard input %file 
         */
        char const* const description_input               = "Input";
        /*! \brief <b>[rw]</b> Pathname of the standard output %file 
         */
        char const* const description_output              = "Output";
        /*! \brief <b>[rw]</b> Pathname of the standard error %file 
         */
        char const* const description_error               = "Error";
        /*! \brief <b>[rw]</b> A list of %file transfer directives 
         */
        char const* const description_file_transfer        = "FileTransfer";
        /*! \brief <b>[rw]</b> Defines if output files get removed after the %job finishes 
         */
        char const* const description_cleanup             = "Cleanup";
        /*! \brief <b>[rw]</b> Time at which a %job should be scheduled
         */
        char const* const description_job_start_time      = "JobStartTime";
        /*! \brief <b>[rw]</b> ?!?
         */
        char const* const description_total_cpu_time      = "TotalCPUTime";
        /*! \brief <b>[rw]</b> ?!?
         */
        char const* const description_wall_time_limit     = "WallTimeLimit";
        /*! \brief <b>[rw]</b> Estimated amount of memory the %job requires
         */
        char const* const description_total_physical_memory = "TotalPhysicalMemory";
        /*! \brief <b>[rw]</b> Compatible processor for %job submission
         */
        char const* const description_cpu_architecture     = "CPUArchitecture";
        
        /*! \brief <b>[rw]</b> Compatible operating system for %job submission 
         */
        char const* const description_operating_system_type = "OperatingSystemType";
        /*! \brief <b>[rw]</b> List of host names which are to be considered by the 
         *         resource manager as candidate targets 
         */
        char const* const description_candidate_hosts      = "CandidateHosts";
        /*! \brief <b>[rw]</b> Name of a queue to place the %job into
         */
        char const* const description_queue               = "Queue";
        /*! \brief <b>[rw]</b> Set of endpoints describing where to report %job state transitions
         */
        char const* const description_job_contact          = "JobContact";
        /*! \brief <b>[rw]</b>
         */
        char const* const description_job_project          = "JobProject";
        /*! \brief <b>[rw]</b> SPMD %job type and startup mechanism 
         */
        char const* const description_spmd_variation       = "SPMDVariation";
        /*! \brief <b>[rw]</b> Total number of cpus requested for this %job 
         */
        char const* const description_total_cpu_count       = "TotalCPUCount";
        /*! \brief <b>[rw]</b> Number of process instances to start 
         */
        char const* const description_number_of_processes   = "NumberOfProcesses";
        /*! \brief <b>[rw]</b> Number of processes to start per host 
         */
        char const* const description_processes_per_host    = "ProcessesPerHost";
        /*! \brief <b>[rw]</b> Expected number of threads per process<br>&nbsp;<br> 
         */
        char const* const description_threads_per_process   = "ThreadsPerProcess";

        /*! \brief <b>[r]</b> SPARC CPU type */
        char const* const description_cpuarchitecture_sparc = "sparc";
        /*! \brief <b>[r]</b> PowerPC CPU type */
        char const* const description_cpuarchitecture_powerpc = "powerpc";
        /*! \brief <b>[r]</b> x86 CPU type */
        char const* const description_cpuarchitecture_x86 = "x86";
        /*! \brief <b>[r]</b> x86 (32bit) CPU type */
        char const* const description_cpuarchitecture_x86_32 = "x86_32";
        /*! \brief <b>[r]</b> x86 (64bit) CPU type */
        char const* const description_cpuarchitecture_x86_64 = "x86_64";
        /*! \brief <b>[r]</b> PARISC CPU type */
        char const* const description_cpuarchitecture_parisc = "parisc";
        /*! \brief <b>[r]</b> MIPS CPU type */
        char const* const description_cpuarchitecture_mips = "mips";
        /*! \brief <b>[r]</b> IA64 CPU type */
        char const* const description_cpuarchitecture_ia64 = "ia64";
        /*! \brief <b>[r]</b> ARM CPU type */
        char const* const description_cpuarchitecture_arm = "arm";
        /*! \brief <b>[r]</b> Other CPU type */
        char const* const description_cpuarchitecture_other = "other";

        
        /*! \brief <b>[r]</b> None */
        char const* const description_spmdvariation_none      = "None";
        
        /*! \brief <b>[r]</b> Generic MPI */
        char const* const description_spmdvariation_mpi       = "MPI";
        
        /*! \brief <b>[r]</b> GridMPI */
        char const* const description_spmdvariation_gridmpi   = "GridMPI";
        
        /*! \brief <b>[r]</b> Intel's MPI implementation */
        char const* const description_spmdvariation_intelmpi  = "IntelMPI";

        /*! \brief <b>[r]</b> LAN MPI */
        char const* const description_spmdvariation_lammpi    = "LAM-MPI";

        /*! \brief <b>[r]</b> MPICH 1 */
        char const* const description_spmdvariation_mpich1    = "MPICH1";

        /*! \brief <b>[r]</b> MPICH 2 */
        char const* const description_spmdvariation_mpich2    = "MPICH2";

        /*! \brief <b>[r]</b> MPICH GM */
        char const* const description_spmdvariation_mpichgm   = "MPICH-GM";

        /*! \brief <b>[r]</b> MPICH MX */
        char const* const description_spmdvariation_mpichmx   = "MPICH-MX";

        /*! \brief <b>[r]</b> MVAPICH */
        char const* const description_spmdvariation_mvapich   = "MVAPICH";

        /*! \brief <b>[r]</b> MVAPICH 2 */
        char const* const description_spmdvariation_mvapich2  = "MVAPICH2";

        /*! \brief <b>[r]</b> OpenMP */
        char const* const description_spmdvariation_openmp    = "OpenMP";

        /*! \brief <b>[r]</b> POE */
        char const* const description_spmdvariation_poe       = "POE";

        /*! \brief <b>[r]</b> PVM */
        char const* const description_spmdvariation_pvm       = "PVM";

        
        
        ///@cond
        namespace detail
        {
            char const* const description_operating_system_unknown = "Unknown";
            char const* const description_operating_system_macos = "MACOS";
            char const* const description_operating_system_attunix = "ATTUNIX";
            char const* const description_operating_system_dgux = "DGUX";
            char const* const description_operating_system_decnt = "DECNT";
            char const* const description_operating_system_true64_unix = "True64_UNIX";
            char const* const description_operating_system_openvms = "openVMS";
            char const* const description_operating_system_hpux = "HPUX";
            char const* const description_operating_system_aix = "AIX";
            char const* const description_operating_system_mvs = "MVS";
            char const* const description_operating_system_os400 = "OS400";
            char const* const description_operating_system_os_2 = "OS_2";
            char const* const description_operating_system_javavm = "JavaVM";
            char const* const description_operating_system_msdos = "MSDOS";
            char const* const description_operating_system_win3x = "WIN3x";
            char const* const description_operating_system_win95 = "WIN95";
            char const* const description_operating_system_win98 = "WIN98";
            char const* const description_operating_system_winnt = "WINNT";
            char const* const description_operating_system_wince = "WINCE";
            char const* const description_operating_system_ncr3000 = "NCR3000";
            char const* const description_operating_system_netware = "netWare";
            char const* const description_operating_system_osf = "OSF";
            char const* const description_operating_system_dc_os = "DC_OS";
            char const* const description_operating_system_reliant_unix = "Reliant_UNIX";
            char const* const description_operating_system_sco_unixware = "SCO_UnixWare";
            char const* const description_operating_system_sco_openserver = "SCO_OpenServer";
            char const* const description_operating_system_sequent = "Sequent";
            char const* const description_operating_system_irix = "IRIX";
            char const* const description_operating_system_solaris = "Solaris";
            char const* const description_operating_system_sunos = "SunOS";
            char const* const description_operating_system_u6000 = "U6000";
            char const* const description_operating_system_aseries = "ASERIES";
            char const* const description_operating_system_tandemnsk = "TandemNSK";
            char const* const description_operating_system_tandemnt = "TandenNT";
            char const* const description_operating_system_bs2000 = "BS2000";
            char const* const description_operating_system_linux = "LINUX";
            char const* const description_operating_system_lynx = "Lynx";
            char const* const description_operating_system_xenix = "XENIX";
            char const* const description_operating_system_vm = "VM";
            char const* const description_operating_system_interactive_unix = "Interactive_UNIX";
            char const* const description_operating_system_bsdunix = "BSDUNIX";
            char const* const description_operating_system_freebsd = "FreeBSD";
            char const* const description_operating_system_netbsd = "NetBSD";
            char const* const description_operating_system_gnu_hurd = "GNU_Hurd";
            char const* const description_operating_system_os9 = "OS9";
            char const* const description_operating_system_mach_kernel = "MACH_Kernel";
            char const* const description_operating_system_inferno = "Inferno";
            char const* const description_operating_system_qnx = "QNX";
            char const* const description_operating_system_epoc = "EPOC";
            char const* const description_operating_system_ixworks = "IxWorks";
            char const* const description_operating_system_vxworks = "VxWorks";
            char const* const description_operating_system_mint = "MiNT";
            char const* const description_operating_system_beos = "BeOS";
            char const* const description_operating_system_hp_mpe = "HP_MPE";
            char const* const description_operating_system_nextstep = "NextStep";
            char const* const description_operating_system_palmpilot = "PalmPilot";
            char const* const description_operating_system_rhapsody = "Rhapsody";
            char const* const description_operating_system_windows_2000 = "Windows_2000";
            char const* const description_operating_system_dedicated = "Dedicated";
            char const* const description_operating_system_os_390 = "OS_390";
            char const* const description_operating_system_vse = "VSE";
            char const* const description_operating_system_tpf = "TPF";
            char const* const description_operating_system_windows_me = "Windows_Me";
            char const* const description_operating_system_caldera_open_unix = "Caldera_open_UNIX";
            char const* const description_operating_system_openbsd = "OpenBSD";
            char const* const description_operating_system_not_applicable = "Not_Applicable";
            char const* const description_operating_system_windows_xp = "Windows_XP";
            char const* const description_operating_system_z_os = "z_OS";
            char const* const description_operating_system_other = "other";
        }
        ///@endcond
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
    class SAGA_JOB_PACKAGE_EXPORT description 
        : public saga::object,
          public saga::detail::attribute<description>
    {
    protected:
        /// @cond
        /** These methods are not within API scope */
        friend struct saga::detail::attribute<description>;  // needs to access get_impl()
        friend class saga::impl::description;
        explicit description(saga::impl::description *impl);
        /// @endcond

    public:
        /*! \brief Creates the object.     
         *
         *
         */
        description (void);

        /*! \brief Creates the object.     
         *
         *
         */
        explicit description (saga::object const& o);

        /*! \brief Destroys the object.     
         *
         *
         */
        ~description (void);

        /*! \brief Brief %description starts here     
         *
         *
         */
        description &operator= (saga::object const& o);

    }; 

    }   // namespace job
}

// re-enable warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

#endif // !defined(SAGA_PACKAGES_JOB_JOB_DESCRIPTION_HPP)

