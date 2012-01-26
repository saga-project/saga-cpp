//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga-defs.hpp>

#ifdef SAGA_WINDOWS

#include <map>
#include <vector>
#include <ctime>

#include <boost/algorithm/string/predicate.hpp>

#include <saga/saga/util.hpp>
#include <saga/saga/exception.hpp>
#include <saga/saga/url.hpp>
#include <saga/saga/adaptors/task.hpp>
#include <saga/saga/packages/job/adaptors/job.hpp>
#include <saga/saga/packages/job/adaptors/job_self.hpp>
#include <saga/saga/adaptors/attribute.hpp>
#include <saga/saga/adaptors/utils/is_local_address.hpp>
#include <saga/saga/adaptors/utils/utils.hpp>

#include <saga/impl/config.hpp>

#include "default_job_service.hpp"

#ifndef  MAX_PATH
# define MAX_PATH _POSIX_PATH_MAX
#endif

///////////////////////////////////////////////////////////////////////////////
namespace {

    // wrapper for gethostname()
    inline std::string get_hostname()
    {
        char buffer[MAX_PATH] = { '\0' };
        gethostname(buffer, sizeof(buffer));
        return std::string(buffer);
    }

    // ensure to get a non-empty resource manager name
    inline std::string ensure_resourcemanager(saga::url rm_url)
    {
        if ( rm_url.get_host ().empty () ) 
             rm_url.set_host (get_hostname ());
        if ( rm_url.get_scheme ().empty () ) 
             rm_url.set_scheme("fork");
        return rm_url.get_url();
    }
    
} // namespace

/*
 * constructor
 */
job_service_cpi_impl::job_service_cpi_impl (proxy                * p, 
                                            cpi_info       const & info,
                                            saga::ini::ini const & glob_ini, 
                                            saga::ini::ini const & adap_ini,
                                            TR1::shared_ptr<saga::adaptor> adaptor)
    : base_cpi (p, info, adaptor, cpi::Noflags)
{
    // check if we can handle this request
    instance_data data(this);
    if (!data->rm_.get_url().empty())
    {
        saga::url rm(data->rm_);
        std::string host(rm.get_host());
        if (!saga::adaptors::utils::is_local_address(host))
        {
            SAGA_ADAPTOR_THROW("Can't use remote hosts for job submission.",
                saga::adaptors::AdaptorDeclined);
        }

        std::string scheme(rm.get_scheme());
        if (!scheme.empty() && scheme != "fork" && scheme != "any")
        {
            SAGA_ADAPTOR_THROW("Can't use schemes others from 'any' or 'fork' " 
                "for job submission.", saga::adaptors::AdaptorDeclined);
        }
    }
}

/*
 * destructor
 */
job_service_cpi_impl::~job_service_cpi_impl (void)
{
}

/*
 * SAGA API functions
 */

///////////////////////////////////////////////////////////////////////////////
void 
job_service_cpi_impl::sync_create_job(saga::job::job& ret, saga::job::description jd)
{
    // make sure the executable path is given
    saga::attribute attr (jd);
    if (!attr.attribute_exists(saga::job::attributes::description_executable) ||
        attr.get_attribute(saga::job::attributes::description_executable).empty())
    {
        SAGA_ADAPTOR_THROW("Missing 'Executable' attribute on job description.",
            saga::BadParameter);
    }
    
    // get resource manager string
    saga::url rm;
    { 
        instance_data data(this);
        rm = data->rm_;             // use resource manager as provided
    }

    saga::job::job job = saga::adaptors::job(rm, jd, proxy_->get_session());

    // set the created attribute
    saga::adaptors::attribute jobattr (job);
    std::time_t current = 0;
    std::time(&current);
    jobattr.set_attribute(saga::job::attributes::created, ctime(&current));

    ret = job;
}

///////////////////////////////////////////////////////////////////////////////
void 
job_service_cpi_impl::sync_run_job_noio(saga::job::job& ret, std::string commandline, 
    std::string host)
{
    std::vector<std::string> cmdline = saga::adaptors::utils::split_commandline (commandline);
    std::string executable = cmdline[0];

    saga::job::description jd;

    std::vector <std::string> chosts;
    chosts.push_back (host);
    jd.set_vector_attribute(saga::job::attributes::description_candidate_hosts, chosts);

    jd.set_attribute(saga::job::attributes::description_executable, executable);
    if (cmdline.size() > 1)
    {
        cmdline.erase(cmdline.begin());
        jd.set_vector_attribute(saga::job::attributes::description_arguments, 
                                cmdline);
    }

    // get resource manager string
    saga::url rm;
    { 
        instance_data data(this);
        rm = data->rm_;             // use resource manager as provided
    }

    // create a new job instance
    saga::job::job job = saga::adaptors::job(rm, jd, proxy_->get_session());

    // set the created attribute 
    saga::adaptors::attribute attr (job);
    std::time_t current = 0;
    std::time(&current);
    attr.set_attribute(saga::job::attributes::created, ctime(&current));

    job.run();    // start this job

    ret = job;
}

void 
job_service_cpi_impl::sync_run_job(saga::job::job& ret, 
                                   std::string commandline, 
                                   std::string host, 
                                   saga::job::ostream& in, 
                                   saga::job::istream& out, 
                                   saga::job::istream& err)
{
    std::vector<std::string> cmdline = saga::adaptors::utils::split_commandline (commandline);
    std::string executable = cmdline[0];
    
    saga::job::description jd;

    std::vector <std::string> chosts;
    chosts.push_back (host);
    jd.set_vector_attribute(saga::job::attributes::description_candidate_hosts, chosts);

    jd.set_attribute(saga::job::attributes::description_executable, executable);
    if (cmdline.size() > 1)
    {
        cmdline.erase(cmdline.begin());
        jd.set_vector_attribute(saga::job::attributes::description_arguments, 
                                cmdline);
    }
    jd.set_attribute(saga::job::attributes::description_interactive, 
        saga::attributes::common_true);

    // get resource manager string
    saga::url rm;
    { 
        instance_data data(this);
        rm = data->rm_;             // use resource manager as provided
    }

    // create a new job instance
    saga::job::job job = saga::adaptors::job(rm, jd, proxy_->get_session());

    // set the created attribute 
    saga::adaptors::attribute attr (job);
    std::time_t current = 0;
    std::time(&current);
    attr.set_attribute(saga::job::attributes::created, ctime(&current));

    job.run();    // start this job

    in = job.get_stdin();
    out = job.get_stdout();
    err = job.get_stderr();

    ret = job;
}

///////////////////////////////////////////////////////////////////////////////
//  return the list of jobs created by this job_service
void job_service_cpi_impl::sync_list(std::vector<std::string>& list_of_jobids)
{
    adaptor_data_type data(this);
    list_of_jobids = data->list_jobs();
}

void job_service_cpi_impl::sync_get_job(saga::job::job & ret, std::string jobid)
{
    // get resource manager string
    saga::url rm;
    { 
        instance_data data(this);
        rm = data->rm_;             // use resource manager as provided
    }
    saga::job::job job = saga::adaptors::job(rm, jobid, proxy_->get_session());

    // set initial attribute if this is a new job
    if (saga::job::New == job.get_state()) {
        saga::adaptors::attribute attr (job);
        std::time_t current = 0;
        std::time(&current);
        attr.set_attribute(saga::job::attributes::created, ctime(&current));
    }

    ret = job;
}

///////////////////////////////////////////////////////////////////////////////
namespace 
{
    // keep track of the time this adaptor was loaded
    struct started_time
    {
        started_time()
        :   started_(0)
        {
            std::time(&started_);
        }
        
        std::time_t started_;
    };
    static started_time started;
}
 
void job_service_cpi_impl::sync_get_self(saga::job::self& ret)
{
    // get resource manager string
    saga::url rm;
    { 
        instance_data data(this);
        rm = data->rm_;     // use resource manager as provided
    }

    // get at our jobid
    SAGA_OSSTREAM strm;
    strm << "[" << ensure_resourcemanager(rm) << "]-[" 
         << get_hostname() << ":" << getpid() << "]";
    std::string jobid = SAGA_OSSTREAM_GETSTRING(strm);

    // create the job_self instance
    saga::job::self job = saga::adaptors::self(rm, jobid, proxy_->get_session());

    // set/modify some of the job attributes
    saga::adaptors::attribute jobattr (job);
    std::time_t current = started.started_;
    jobattr.set_attribute(saga::job::attributes::created, ctime(&current));

    std::vector<std::string> hosts;
    hosts.push_back(get_hostname());
    jobattr.set_vector_attribute(saga::job::attributes::execution_hosts, hosts);

    ret = job;
}

///////////////////////////////////////////////////////////////////////////////
#if !defined (SAGA_DEFAULT_JOB_ADAPTOR_NO_ASYNCS)
// This adaptor implements the async functions based on its own synchronous
// functions for testing purposes only. Since there is no principal need
// to do so, we allow these to be 'switched off'.

saga::task 
job_service_cpi_impl::async_create_job(saga::job::description jd)
{
    return saga::adaptors::task("job_service_cpi_impl::async_create_job",
        shared_from_this(), &job_service_cpi_impl::sync_create_job, jd);
}

saga::task 
job_service_cpi_impl::async_run_job(std::string commandline, 
                                    std::string host, 
                                    saga::job::ostream& in, 
                                    saga::job::istream& out, 
                                    saga::job::istream& err)
{
    return saga::adaptors::task("job_service_cpi_impl::async_run_job",
        shared_from_this(), &job_service_cpi_impl::sync_run_job, 
        commandline, host, TR1::ref(in), TR1::ref(out), TR1::ref(err));
}

saga::task 
job_service_cpi_impl::async_list()
{
    return saga::adaptors::task("job_service_cpi_impl::async_list",
        shared_from_this(), &job_service_cpi_impl::sync_list);
}

saga::task 
job_service_cpi_impl::async_get_job(std::string jobid)
{
    return saga::adaptors::task("job_service_cpi_impl::async_get_job",
        shared_from_this(), &job_service_cpi_impl::sync_get_job, jobid);
}

saga::task 
job_service_cpi_impl::async_get_self()
{
    return saga::adaptors::task("job_service_cpi_impl::async_get_self",
        shared_from_this(), &job_service_cpi_impl::sync_get_self);
}

#endif  // !defined (SAGA_DEFAULT_JOB_ADAPTOR_NO_ASYNCS)

#endif // SAGA_WINDOWS

