//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga-defs.hpp>

#ifdef SAGA_WINDOWS

#include <cerrno>
#include <cstring>
#include <ctime>

#include <set>
#include <map>
#include <vector>

#include <boost/assert.hpp>
#include <boost/version.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/process.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>
  
#include <saga/saga/util.hpp>
#include <saga/saga/exception.hpp>
#include <saga/saga/url.hpp>
#include <saga/saga/filesystem.hpp>

#include <saga/saga/adaptors/task.hpp>
#include <saga/saga/adaptors/metric.hpp>
#include <saga/saga/adaptors/attribute.hpp>
#include <saga/saga/adaptors/file_transfer_spec.hpp>
#include <saga/saga/adaptors/manage_threads.hpp>
#include <saga/saga/packages/job/adaptors/job_self.hpp>
#include <saga/saga/adaptors/utils/is_local_address.hpp>

#include <saga/impl/config.hpp>

#include <saga/saga/packages/job/job_description.hpp>

#include "default_job.hpp"
#include "default_job_istream.hpp"
#include "default_job_ostream.hpp"
#include "process_helpers.hpp"
#include "execution_environment.hpp"

#ifndef  MAX_PATH
# define MAX_PATH _POSIX_PATH_MAX
#endif


///////////////////////////////////////////////////////////////////////////////
//  this is a hack to make boost::process work (avoid multiple definitions)
//  (this will go away as soon as it is fixed in Boost.Process)
#if defined(BOOST_PROCESS_WIN32_API)
const boost::process::detail::file_handle::handle_type 
    boost::process::detail::file_handle::INVALID_VALUE = INVALID_HANDLE_VALUE;
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

    ///////////////////////////////////////////////////////////////////////////
    // ensure to get a non-empty resource manager name
    inline std::string ensure_resourcemanager(saga::url rm_url)
    {
        if ( rm_url.get_host ().empty () ) 
             rm_url.set_host (get_hostname ());
        if ( rm_url.get_scheme ().empty () ) 
             rm_url.set_scheme("fork");
        return rm_url.get_url();
    }

    ///////////////////////////////////////////////////////////////////////////
    inline std::string trim_whitespace(std::string const &s)
    {
        typedef std::string::size_type size_type;
        
        size_type first = s.find_first_not_of(" \t");
        if (std::string::npos == first)
            return std::string();
        size_type last = s.find_last_not_of(" \t");
        return s.substr(first, last-first+1);
    }

    inline bool split_environment(std::string const& env, std::string& key, 
        std::string & value)
    {
        std::string::size_type pos = env.find_first_of("=");
        if (std::string::npos == pos)
            return false;
            
        key = env.substr(0, pos);
        value = env.substr(pos+1);

        return true;
    }

///////////////////////////////////////////////////////////////////////////////
}   // namespace 

///////////////////////////////////////////////////////////////////////////
class state_setter 
{
public:        
    state_setter(job_cpi_impl *cpi, bool commit_state = true)
      : commit_state_(commit_state), state_(saga::job::Unknown), cpi_(cpi),
        cookie_(0) 
    {
        job_cpi_impl::mutex_type::scoped_lock lock(cpi_->mtx_);
        saga::monitorable monitor (cpi->proxy_);
        cookie_ = monitor.add_callback(saga::metrics::task_state, 
            boost::bind(&state_setter::update_state, this, _1, _2, _3));
        state_ = cpi_->get_state();
    }
    ~state_setter()
    {
        // don't invoke anything if inside the proxy destructor
#if BOOST_VERSION >= 103900
        if (NULL == cpi_->proxy_)
            return;

        TR1::weak_ptr<saga::impl::object> wp(cpi_->proxy_->shared_from_this());
        if (!wp.expired())
#else
        if (0 != cpi_->proxy_->_internal_weak_this.use_count())
#endif
        {
            saga::monitorable monitor (cpi_->proxy_);
            monitor.remove_callback(saga::metrics::task_state, cookie_);

            if (commit_state_)
                cpi_->update_state(state_);
        }
    }

    bool update_state(saga::object, saga::metric, saga::context)
    {
        job_cpi_impl::mutex_type::scoped_lock lock(cpi_->mtx_);
        state_ = cpi_->get_state();
        return true;
    }
    
    bool commit_state_;
    saga::job::state state_;

private:
    job_cpi_impl *cpi_;
    unsigned int cookie_;
};

/*
 * constructor
 */
job_cpi_impl::job_cpi_impl (proxy                * p, 
                            cpi_info       const & info,
                            saga::ini::ini const & glob_ini, 
                            saga::ini::ini const & adap_ini,
                            TR1::shared_ptr<saga::adaptor> adaptor)
    : base_cpi (p, info, adaptor, cpi::Noflags), 
      thrd_(NULL), 
      cond_(NULL), 
      thread_alive_(false),
      isinitialized_(false),
      staging_cookie_(0)
{
    boost::process::child::handle_type handle = get_invalid_handlevalue();

    // initialize instance data
    bool job_is_self = false;
    bool init_from_jobid = false;
    std::string jobid;      // valid if init_from_jobid_

    {
        // check if we can handle this request
        instance_data inst_data(this);
        if (!inst_data->rm_.get_url().empty())
        {
            saga::url rm(inst_data->rm_);
            std::string host(rm.get_host());
            if (!saga::adaptors::utils::is_local_address(host))
            {
                SAGA_ADAPTOR_THROW("Can't use remote hosts for job submission: " + rm.get_url(),
                    saga::adaptors::AdaptorDeclined);
            }

            std::string scheme(rm.get_scheme());
            if (!scheme.empty() && scheme != "fork" && scheme != "any")
            {
                SAGA_ADAPTOR_THROW("Can't use schemes others from 'any' or 'fork' " 
                    "for job submission.", saga::adaptors::AdaptorDeclined);
            }
        }

        if (inst_data->init_from_jobid_) {
        // This cpi instance has to be attached to a running resource manager 
        // job. The difference to the code below is, that SAGA does not know
        // anything about this job except its jobid. We need to fill the 
        // job description from the data we may be able to get from the 
        // resource manager.
            init_from_jobid = true;
            jobid = inst_data->jobid_;
            if (jobid.empty()) {
                SAGA_ADAPTOR_THROW(
                    "Cannot attach to a job without a given jobid",
                    saga::adaptors::Unexpected);
            }

            // extract the process id from the given handle
            handle = extract_jobid(jobid);
            if (!is_valid_handle(handle)) {
                SAGA_ADAPTOR_THROW("Not existing job: " + jobid, 
                    saga::adaptors::Unexpected);
            }

            // initialize the jobid attribute(
            saga::adaptors::attribute attr (this);
            jobid_ = jobid;
            attr.set_attribute(saga::job::attributes::jobid, jobid);

        // if this is the job_self instance, do not start a monitoring thread
            if (handle == getpid()) 
                job_is_self = true;

            if (!fill_jobdescription(jobid, inst_data->jd_)) {
                if (job_is_self) {
                // this is the job_self instance, fill the job description 
                // from execution environment
                    inst_data->jd_is_valid_ = 
                        fill_self_jobdescription(inst_data->jd_);
                }
                else {
                    inst_data->jd_is_valid_ = false;
                }
            }
            else {
                inst_data->jd_is_valid_ = true;
            }
        }
    }

    // initialize adaptor instance inst_data
    state_setter setstate (this);
    bool startup_thread = false;
    
    {
        mutex_type::scoped_lock lock(mtx_);
        
        // the first assumption is that we're done
        if (init_from_jobid) 
            setstate.state_ = saga::job::Done;

        if (is_valid_handle(handle)) {
        // this cpi instance has to be attached to a running child, SAGA
        // doesn't know anything about this child yet
            using namespace boost::process::detail;
            child_ = factories::create_child(handle);
            setstate.state_ = state_from_process(child_.get_handle(), setstate.state_);
            if (saga::job::Running == setstate.state_) {
            // start the monitoring thread for this child process
                if (!job_is_self)
                    startup_thread = true;
                register_jobid(true);   // register the jobid with this cpi
            }
            else if (saga::job::Unknown == setstate.state_) {
                SAGA_ADAPTOR_THROW("Nothing known about job: " + jobid, 
                    saga::DoesNotExist);
            }
        } 
//         else if (!isinitialized_) {
//         // initialize state metric to have the value saga::job::New
//         // we have no 'Unknown' state since we are 'New' from the beginning :-P
//         // (all operations are executed locally)
//             isinitialized_ = true;
//             setstate.state_ = saga::job::New;
//         }
//         else if (is_valid_handle(child_.get_handle())) {
//         // this cpi instance has to be attached to a running child, SAGA
//         // already knows about this child
//             setstate.state_ = state_from_process(child_.get_handle(), setstate.state_);
//             if (saga::job::Running == setstate.state_)
//                 startup_thread = true;
//         }
        else {
        // child has not been started yet
            setstate.state_ = saga::job::New;
        }
    }

    // start up new waiting thread, if needed
    if (startup_thread)
    {
        setup_wait_thread();
    }

    // if the job is (was) running, we need to have a jobid as well
    BOOST_ASSERT(setstate.state_ == saga::job::New || !jobid_.empty());
}

/*
 * destructor
 */
job_cpi_impl::~job_cpi_impl (void)
{
    mutex_type::scoped_lock lock(mtx_);
    
    // break thread and wait for it to finish
    /* saga::job::state state = */ state_from_process(child_.get_handle(), saga::job::Failed);

    if (NULL != thrd_) {
        if (thread_alive_) {
            cancel_thrd_.notify_one();    // break thread
            lock.unlock();
            thrd_->join();
        }

        // saga::adaptors::remove_managed_thread(proxy_->get_session(), thrd_);
        delete thrd_;
        thrd_ = NULL;
    }
}

///////////////////////////////////////////////////////////////////////////////
void job_cpi_impl::update_state(saga::job::state newstate)
{
    // update state attribute and promote state change to the metric callbacks
    saga::monitorable monitor (this->proxy_);
    saga::adaptors::metric m (monitor.get_metric(saga::metrics::task_state));
    m.set_attribute(saga::attributes::metric_value, 
        saga::adaptors::job_state_enum_to_value(newstate));
    m.fire();
}

///////////////////////////////////////////////////////////////////////////////
saga::job::state job_cpi_impl::get_state()
{
    // get state attribute 
    saga::monitorable monitor (this->proxy_);
    saga::metric m (monitor.get_metric(saga::metrics::task_state));
    return saga::adaptors::job_state_value_to_enum(
        m.get_attribute(saga::attributes::metric_value));
}

///////////////////////////////////////////////////////////////////////////////
void job_cpi_impl::register_jobid(bool mayfail)
{
    BOOST_ASSERT(!jobid_.empty());

    saga::job::description jd;
    {
        instance_data data(this);
        if (data->jd_is_valid_)
            jd = data->jd_.clone();
        else if (mayfail)
            return;
        else {
            SAGA_ADAPTOR_THROW("Can't register job: " + jobid_, 
                saga::adaptors::Unexpected);
        }
    }

    adaptor_data_type data(this);
    if (!data->register_job(jobid_, jd) && !mayfail) {
        SAGA_ADAPTOR_THROW("Can't register job: " + jobid_, 
            saga::adaptors::Unexpected);
    }
}

void job_cpi_impl::unregister_jobid()
{
    if (!jobid_.empty()) {
        adaptor_data_type data(this);
        data->unregister_job(jobid_);
    }
}

///////////////////////////////////////////////////////////////////////////////
//  SAGA API functions
void job_cpi_impl::sync_get_state (saga::job::state& state)
{
    state = get_state();
}

void job_cpi_impl::sync_get_description(saga::job::description& jd)
{
    instance_data data(this);
    if (data->jd_is_valid_)
        jd = data->jd_.clone();     // return a deep copy of the job description
    else {
        SAGA_ADAPTOR_THROW("Job description cannot be retrieved.", 
            saga::DoesNotExist);
    }
}

void job_cpi_impl::sync_get_job_id (std::string& jobid)
{
    // verify this job is running already (not New)
    saga::job::state state = saga::job::Unknown;
    sync_get_state(state);
    if (saga::job::New == state) {
        jobid.clear();
        return;
    }

    saga::attribute attr (this->proxy_);
    jobid = attr.get_attribute(saga::job::attributes::jobid);
    BOOST_ASSERT(jobid == jobid_);
}

///////////////////////////////////////////////////////////////////////////////
// inherited from the task interface
void job_cpi_impl::sync_run (saga::impl::void_t&)
{
    // verify this job is not running yet (has to be New)
    saga::job::state state = saga::job::Unknown;
    sync_get_state(state);
    if (saga::job::New != state) {
        SAGA_ADAPTOR_THROW("Job has been started already!", 
            saga::IncorrectState);
    }

    saga::job::description jd;
    std::string rm;    
    {
        instance_data data(this);
        if (!data->jd_is_valid_) {
            SAGA_ADAPTOR_THROW("Job description cannot be retrieved.", 
                saga::NotImplemented);
        }
        jd = data->jd_;
        rm = ensure_resourcemanager(data->rm_);
    }

    std::string exename(
        jd.get_attribute(saga::job::attributes::description_executable));

    try {
        // initialize command line, add arguments
        boost::process::command_line cl(exename);

        if (jd.attribute_exists(saga::job::attributes::description_arguments))
        {
            std::vector<std::string> arguments = 
                jd.get_vector_attribute(saga::job::attributes::description_arguments);
            std::vector<std::string>::iterator end = arguments.end();
            for (std::vector<std::string>::iterator it = arguments.begin(); 
                it != end; ++it)
            {
                std::string s = trim_whitespace(*it);
                if ( (s[0] == '"' || s[0] == '\'') && s[0] == s[s.size()-1] )
                {
                    // original argument is quoted
                    cl.argument(*it);   // add this as an argument as is
                }
                else if (std::string::npos == s.find_first_of(" \t"))
                {
                    // argument doesn't contain whitespace
                    cl.argument(*it);   // add this as an argument as is
                }
                else
                {
                    // add this as an argument with additional quotes
                    cl.argument(std::string("\"") + *it + "\"");
                }
            }
        }

        // execute pre-staging directives
        register_post_staging();
        do_pre_staging(jd);

        // initialize launcher (I/O redirection, environment)
        boost::process::launcher l;

        // set up environment
        l.clear_environment();

        std::vector<std::string> env;
        if (jd.attribute_exists(saga::job::attributes::description_environment))
        {
            env = jd.get_vector_attribute(saga::job::attributes::description_environment);
        }
        else {
        // inherit environment of the current job
            env = get_self_env();
        }

        std::vector<std::string>::iterator end = env.end();
        for (std::vector<std::string>::iterator it = env.begin(); 
            it != end; ++it)
        {
            std::string key, value;
            if (!split_environment(*it, key, value)) 
            {
                SAGA_ADAPTOR_THROW(
                    "Bogus formatting of a environment entry: ':" + *it + "'",
                    saga::adaptors::Unexpected);
            }
            l.set_environment(key, value);
        }

        // set up I/O redirection
        if (jd.attribute_exists(saga::job::attributes::description_interactive) &&
            jd.get_attribute(saga::job::attributes::description_interactive) ==
                saga::attributes::common_true)
        {
            namespace bp = boost::process;
            l.set_stdin_behavior(bp::redirect_stream);
            l.set_stdout_behavior(bp::redirect_stream);
            l.set_stderr_behavior(bp::redirect_stream);
        }
        else {
            namespace bp = boost::process;
            if (jd.attribute_exists(saga::job::attributes::description_input)) {
            // set up input file (used as stdin)
                l.set_stdin_behavior(bp::redirect_file);
                l.set_stdin_file(
                    jd.get_attribute(saga::job::attributes::description_input));
            }
            if (jd.attribute_exists(saga::job::attributes::description_output)) {
            // set up output file (used as stdout)
                l.set_stdout_behavior(bp::redirect_file);
                l.set_stdout_file(
                    jd.get_attribute(saga::job::attributes::description_output));
            }
            if (jd.attribute_exists(saga::job::attributes::description_error)) {
            // set up output file (used as stderr)
                l.set_stderr_behavior(bp::redirect_file);
                l.set_stderr_file(
                    jd.get_attribute(saga::job::attributes::description_error));
            }
        }

        saga::adaptors::attribute attr (this);

        // working directory
        if (jd.attribute_exists(saga::job::attributes::description_working_directory))
        {
            std::string wd (jd.get_attribute
                            (saga::job::attributes::description_working_directory));
            l.set_work_directory(wd);
            attr.set_attribute(saga::job::attributes::working_directory, wd);
        }
        else 
        {
            namespace fs = boost::filesystem;
            fs::path cwd (fs::initial_path());
            l.set_work_directory(cwd.string());
            attr.set_attribute(saga::job::attributes::working_directory, cwd.string());
        }

    // execute the new process
        state_setter setstate(this);
        mutex_type::scoped_lock lock(mtx_);
        int exit_code = 0;

        child_ = l.start(cl);
        setstate.state_ = state_from_process(child_.get_handle(), 
                                             saga::job::Running, 
                                             &exit_code);

        if ( setstate.state_ != saga::job::Running &&
             setstate.state_ != saga::job::Done    )
        {
            SAGA_ADAPTOR_THROW(
                "Could not start process: " + cl.get_executable(),
                saga::NoSuccess);
        }

        if (setstate.state_ == saga::job::Done)
        {
            if (exit_code != 0)
            {
              setstate.state_ = saga::job::Failed;
            }
            attr.set_attribute(saga::job::attributes::exitcode, 
                boost::lexical_cast<std::string>(exit_code));
        }

        // set the job id of the newly created process
        SAGA_OSSTREAM jobid;
        jobid << "[" << ensure_resourcemanager(rm) << "]-[" 
              << get_hostname() << ":" << child_.get_handle() << "]";
        jobid_ = SAGA_OSSTREAM_GETSTRING(jobid);
        attr.set_attribute(saga::job::attributes::jobid, jobid_);

        // set other required attributes of the new job
        std::vector<std::string> hosts;
        hosts.push_back(get_hostname());
        attr.set_vector_attribute(saga::job::attributes::execution_hosts, hosts);

        std::time_t started = 0;
        std::time(&started);
        attr.set_attribute(saga::job::attributes::started, ctime(&started));

        // register this job with the adaptor data
        register_jobid();
    }
    catch (saga::adaptors::exception const&) {
        throw;      // just rethrow our adaptor exceptions
    }
    catch (std::exception const& e) {
        SAGA_ADAPTOR_THROW(
            "Problem in starting process: " + exename + 
                " (std::exception caught: " + e.what() + ")",
            saga::BadParameter);
    }

    // set up separate thread watching for job termination
    // this gets called for jobs in Done or Running state.  
    // For done, the thread will return immediately.
    setup_wait_thread();
}

///////////////////////////////////////////////////////////////////////////////
// pre-staging
void job_cpi_impl::do_pre_staging (saga::job::description jd)
{
    if (!jd.attribute_exists(saga::job::attributes::description_file_transfer))
        return;

    // get the staging specifications
    std::vector<std::string> specs (
        jd.get_vector_attribute(saga::job::attributes::description_file_transfer));

    std::vector<std::string>::iterator end = specs.end();
    for (std::vector<std::string>::iterator it = specs.begin(); it != end; ++it)
    {
        using namespace saga::adaptors;
        std::string left_url, right_url;
        file_transfer_operator mode;
        if (!parse_file_transfer_specification(*it, left_url, mode, right_url))
        {
            SAGA_ADAPTOR_THROW(
                "job_cpi_impl::do_pre_staging: "
                    "ill formatted file transfer specification: " + *it,
                saga::BadParameter);
        }
        
        if (copy_local_remote == mode) {
            // copy the file
            saga::filesystem::file f(left_url);
            f.copy(right_url, saga::filesystem::Overwrite);
        }
        else if (append_local_remote == mode) {
            // append to remote file
            saga::filesystem::file in(left_url, saga::filesystem::Read|saga::filesystem::Binary);
            saga::filesystem::file out(right_url, 
                saga::filesystem::ReadWrite|saga::filesystem::Append|saga::filesystem::Binary);

            saga::off_t size = in.get_size();
            if (size > 10000) size = 10000;
            std::auto_ptr<char> buffer (new char[size]);
            saga::ssize_t bytes = in.read(saga::buffer(buffer.get(), size));
            while (bytes != 0) {
                out.write(saga::buffer(buffer.get(), bytes));
                bytes = in.read(saga::buffer(buffer.get(), size));
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// post-staging
void job_cpi_impl::register_post_staging()
{
    mutex_type::scoped_lock l(mtx_);
    saga::monitorable monitor (this->proxy_);
    staging_cookie_ = monitor.add_callback(saga::metrics::task_state, 
        boost::bind(&job_cpi_impl::do_post_staging, this, _1, _2, _3));
}

bool job_cpi_impl::do_post_staging (saga::object, saga::metric, saga::context)
{
    // make sure post staging is done in Done state only
    {
        saga::job::state state = get_state();
        if (saga::job::Failed == state) {
            mutex_type::scoped_lock l(mtx_);
            staging_cookie_ = 0;
            return false;   // we are done, don't call callback anymore
        }
        if (saga::job::Done != state)
            return true;    // continue calling this callback
    }

    // get the job description of this job    
    saga::job::description jd;
    {
        instance_data data(this);
        jd = data->jd_;
    }

    if (!jd.attribute_exists(saga::job::attributes::description_file_transfer)) {
        mutex_type::scoped_lock l(mtx_);
        staging_cookie_ = 0;
        return false;           // unregister callback
    }

    // get the staging specifications
    std::vector<std::string> specs (
        jd.get_vector_attribute(saga::job::attributes::description_file_transfer));

    std::vector<std::string>::iterator end = specs.end();
    for (std::vector<std::string>::iterator it = specs.begin(); it != end; ++it)
    {
        using namespace saga::adaptors;
        std::string left_url, right_url;
        file_transfer_operator mode;
        if (!parse_file_transfer_specification(*it, left_url, mode, right_url))
        {
            SAGA_ADAPTOR_THROW(
                "job_cpi_impl::do_post_staging: "
                    "ill formatted file transfer specification: " + *it,
                saga::BadParameter);
        }

        if (copy_remote_local == mode) {
            // copy the file
            saga::filesystem::file f(right_url);
            f.copy(left_url, saga::filesystem::Overwrite);
        }
        else if (append_remote_local == mode) {
            // append to local file
            saga::filesystem::file in(right_url, saga::filesystem::Read|saga::filesystem::Binary);
            saga::filesystem::file out(left_url, 
                saga::filesystem::ReadWrite|saga::filesystem::Append|saga::filesystem::Binary);

            saga::off_t size = in.get_size();
            if (size > 10000) size = 10000;
            std::auto_ptr<char> buffer (new char[size]);
            saga::ssize_t bytes = in.read(saga::buffer(buffer.get(), size));
            while (bytes != 0) {
                out.write(saga::buffer(buffer.get(), bytes));
                bytes = in.read(saga::buffer(buffer.get(), size));
            }
        }
    }

    mutex_type::scoped_lock l(mtx_);
    staging_cookie_ = 0;
    return false;         // un-register this metric callback
}

///////////////////////////////////////////////////////////////////////////////
void job_cpi_impl::setup_wait_thread()
{
    mutex_type::scoped_lock lock(mtx_);

    BOOST_ASSERT(NULL == cond_ && NULL == thrd_);    // should be called only once
    BOOST_ASSERT(!jobid_.empty());  // jobid should have been generated

    // set up separate thread watching for job termination
    thread_alive_ = true;
    try {
        cond_ = new boost::condition();
        thrd_ = new boost::thread(TR1::bind(&job_cpi_impl::wait_for_child, this));
        //saga::adaptors::add_managed_thread(proxy_->get_session(), thrd_);
        cond_->wait(lock);
    }
    catch (...) {
        thread_alive_ = false;

        saga::adaptors::remove_managed_thread(proxy_->get_session(), thrd_);
        delete thrd_;
        thrd_ = NULL;
    }

    delete cond_;
    cond_ = NULL;
}

namespace 
{
    struct handle_flag
    {
        handle_flag(job_cpi_impl::mutex_type& mtx, bool& flag) 
          : mtx_(mtx), flag_(flag)
        {
        }
        ~handle_flag()
        {
            job_cpi_impl::mutex_type::scoped_lock l(mtx_);
            flag_ = false;
        }
        
        job_cpi_impl::mutex_type& mtx_;
        bool& flag_;
    };
}

// thread routine waiting for the child to terminate
void job_cpi_impl::wait_for_child()
{
    handle_flag handler(mtx_, thread_alive_);   // 'thread_alive_ = false' on exit    
    bool first = true;
    
    try {
        // wait for child to terminate
        state_setter setstate(this, false);
        int exit_code = 0;
        boost::process::child::handle_type pid = get_invalid_handlevalue();

        do {
            mutex_type::scoped_lock lock(mtx_);
            if (first) {
                first = false;
                pid = child_.get_handle();
                if (NULL != cond_) 
                    cond_->notify_one();        // initialization is complete
            }
            else {
                boost::xtime xt;
                boost::xtime_get(&xt, boost::TIME_UTC);
                xt.nsec += 5000000;             // sleep max. 5ms
                if (cancel_thrd_.timed_wait(lock, xt))
                    return;                     // thread has been canceled
            }
            setstate.state_ = state_from_process (pid, setstate.state_, &exit_code);
        } while (setstate.state_ == saga::job::Running);
        
#if BOOST_VERSION >= 103900
        TR1::weak_ptr<job_cpi_impl> wp(this->shared_from_this());
        if (setstate.state_ != saga::job::Running && !wp.expired()) 
#else
        if (setstate.state_ != saga::job::Running && 
            0 != _internal_weak_this.use_count()) 
#endif
        {
        //  set exit code in our attributes  
            saga::adaptors::attribute attr (this);
            attr.set_attribute(saga::job::attributes::exitcode, 
                boost::lexical_cast<std::string>(exit_code));

            std::time_t finished = 0;
            std::time(&finished);
            attr.set_attribute(saga::job::attributes::finished, ctime(&finished));
        }
#if BOOST_VERSION >= 103900
        setstate.commit_state_ = wp.expired() ? false : true;
#else
        setstate.commit_state_ = (0 != _internal_weak_this.use_count()) ? true : false;
#endif
    }
    catch (...) {
        // state_setter may throw an exception if cancel_thread_ has been set 
        // before this thread function has been entered
        mutex_type::scoped_lock l(mtx_);
        if (NULL != cond_) 
            cond_->notify_one();        // initialization is complete
    }
}

//  terminate the child process (if any)
void job_cpi_impl::sync_cancel (saga::impl::void_t&, double timeout)
{
    state_setter setstate(this);
    mutex_type::scoped_lock lock(mtx_);
    setstate.state_ = terminate_process(this, child_);

    saga::adaptors::attribute attr(this);
    std::time_t finished = 0;
    std::time(&finished);
    attr.set_attribute(saga::job::attributes::finished, ctime(&finished));
}

//  wait for the child process to terminate
void job_cpi_impl::sync_wait (bool& retval, double timeout)
{
    boost::process::child::handle_type pid = get_invalid_handlevalue();
    {
        mutex_type::scoped_lock lock(mtx_);
        pid = child_.get_handle();
    }

    int exit_code = 0;

    {
        state_setter setstate(this, false);

    // check state consistency
        saga::job::state state = state_from_process(pid, setstate.state_);
        if (saga::job::Canceled == state) {
            SAGA_ADAPTOR_THROW("job not running: job was canceled!",
                saga::IncorrectState);
        }
        if (saga::job::New == state) {
            SAGA_ADAPTOR_THROW("job not running, yet: is still pending!",
                saga::IncorrectState);
        }
        if (saga::job::Unknown == state) {
            SAGA_ADAPTOR_THROW("Nothing known about job: " + 
                    boost::lexical_cast<std::string>(pid), saga::DoesNotExist);
        }
        if (saga::job::Done == state) {
        // mark this as successful
            retval = true;

        // wait for the status thread to finish
            if (NULL != thrd_) {
                thrd_->join();
                mutex_type::scoped_lock lock(mtx_);

                saga::adaptors::remove_managed_thread(proxy_->get_session(), thrd_);
                delete thrd_;
                thrd_ = NULL;
            }
            return;
        }

    // perform the actual wait
        if (timeout < 0.0) {
        // blocking wait
            wait_for_process(pid, &exit_code);
            setstate.state_ = saga::job::Done;
            setstate.commit_state_ = true;
        }
        else if (timeout > 0.0) {
        // wait for the given amount of time
            std::time_t start_time = time (0);
            while ( (timeout - (std::difftime (std::time(0), start_time))) > 0 )
            {
                setstate.state_ = state_from_process(pid, setstate.state_);
                if (saga::job::Running != setstate.state_)
                    break;
                saga::impl::task_base::sleep (50);
            }
            setstate.state_ = state_from_process(pid, setstate.state_, &exit_code);
        }
        else {
        // just query the current state
            setstate.state_ = state_from_process(pid, setstate.state_, &exit_code);
        }
        
        retval = setstate.state_ != saga::job::Running;
    }   // update state here
    
    if (retval) {
        mutex_type::scoped_lock lock(mtx_);
        if (NULL != thrd_) {
        // wait for the status thread to finish
            if (thread_alive_) {
                lock.unlock();
                thrd_->join();
                lock.lock();
            }
            
            saga::adaptors::remove_managed_thread(proxy_->get_session(), thrd_);
            delete thrd_;
            thrd_ = NULL;
        }

    // set exit code in our attributes 
        saga::adaptors::attribute attr (this);
        attr.set_attribute(saga::job::attributes::exitcode, 
            boost::lexical_cast<std::string>(exit_code));

        std::time_t finished = 0;
        std::time(&finished);
        attr.set_attribute(saga::job::attributes::finished, ctime(&finished));
    }
}

//  suspend the child process 
void job_cpi_impl::sync_suspend (saga::impl::void_t&)
{
    state_setter setstate(this);
    mutex_type::scoped_lock lock(mtx_);
    setstate.state_ = suspend_process(this, child_);
}

//  suspend the child process 
void job_cpi_impl::sync_resume (saga::impl::void_t&)
{
    state_setter setstate(this);
    mutex_type::scoped_lock lock(mtx_);
    setstate.state_ = resume_process(this, child_);
}

// access streams for communication with the child
void job_cpi_impl::sync_get_stdin(saga::job::ostream& ostrm)
{
    // saga::job::state state = get_state();
    // if (saga::job::Running   != state && 
    //     saga::job::New       != state && 
    //     saga::job::Suspended != state) 
    // {
    //     SAGA_OSSTREAM strm;
    //     strm << "Attempt to retrieve stdin stream from a job in bad job state (" 
    //          << state << ")";
    //     SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::IncorrectState);
    // }    
    
    {
        // make sure this job has been started with the interactive flag
        adaptor_data_type data (this);
        saga::job::description jd = data->get_job(this, jobid_);
        if (!jd.attribute_exists(saga::job::attributes::description_interactive) ||
            jd.get_attribute(saga::job::attributes::description_interactive) !=
                saga::attributes::common_true)
        {
            SAGA_ADAPTOR_THROW(
                "This job was not started with its interactive attribute set, "
                "input/output redirection is not available.",
                saga::IncorrectState);
        }
    }

    mutex_type::scoped_lock lock(mtx_);
    default_job_ostream strm(this, child_.get_stdin());
    ostrm = strm;
}

void job_cpi_impl::sync_get_stdout(saga::job::istream& istrm)
{
    // saga::job::state state = get_state();
    // if (saga::job::Running != state && saga::job::Suspended != state) 
    // {
    //     SAGA_OSSTREAM strm;
    //     strm << "Attempt to retrieve stdout stream from a job in bad job state (" 
    //          << state << ")";
    //     SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::IncorrectState);
    // }    
    
    {
        // make sure this job has been started with the interactive flag
        adaptor_data_type data (this);
        saga::job::description jd = data->get_job(this, jobid_);
        if (!jd.attribute_exists(saga::job::attributes::description_interactive) ||
            jd.get_attribute(saga::job::attributes::description_interactive) !=
                saga::attributes::common_true)
        {
            SAGA_ADAPTOR_THROW(
                "This job was not started with its interactive attribute set, "
                "input/output redirection is not available.",
                saga::IncorrectState);
        }
    }

    mutex_type::scoped_lock lock(mtx_);
    default_job_istream strm(this, child_.get_stdout());
    istrm = strm;
}

void job_cpi_impl::sync_get_stderr(saga::job::istream& errstrm)
{
    // saga::job::state state = get_state();
    // if (saga::job::Running != state && saga::job::Suspended != state) 
    // {
    //     SAGA_OSSTREAM strm;
    //     strm << "Attempt to retrieve stderr stream from a job in bad job state (" 
    //          << state << ")";
    //     SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::IncorrectState);
    // }    
    
    {
        // make sure this job has been started with the interactive flag
        adaptor_data_type data (this);
        saga::job::description jd = data->get_job(this, jobid_);
        if (!jd.attribute_exists(saga::job::attributes::description_interactive) ||
            jd.get_attribute(saga::job::attributes::description_interactive) !=
                saga::attributes::common_true)
        {
            SAGA_ADAPTOR_THROW(
                "This job was not started with its interactive attribute set, "
                "input/output redirection is not available.",
                saga::IncorrectState);
        }
    }

    mutex_type::scoped_lock lock(mtx_);
    default_job_istream strm(this, child_.get_stderr());
    errstrm = strm;
}
    
void job_cpi_impl::sync_migrate(saga::impl::void_t&, saga::job::description jd)
{
    saga::monitorable monitor (this->proxy_);

//     saga::job::state state = get_state();
//     if (saga::job::Running != state && saga::job::Suspended != state) {
//         SAGA_OSSTREAM strm;
//         strm << "Attempt to migrate a job in bad job state (" << state << ")";
//         SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::IncorrectState);
//     }    

    // get resource manager string
    std::string target_rm;
    {
        instance_data data(this);
        target_rm = ensure_resourcemanager(data->rm_);
    }

    // create new job_service and move job over
    saga::job::service js (this->proxy_->get_session(), target_rm);
    saga::job::job j = js.create_job(jd);
    
    // set the created attribute
    saga::adaptors::attribute jobattr (j);
    std::time_t current = 0;
    std::time(&current);
    jobattr.set_attribute(saga::job::attributes::created, ctime(&current));

    // start the newly created job
    j.run();

// FIXME: migrate should return new job
//    ret = job;
}

///////////////////////////////////////////////////////////////////////////////
#if !defined (SAGA_DEFAULT_JOB_ADAPTOR_NO_ASYNCS)
// This adaptor implements the async functions based on its own synchronous
// functions for testing purposes only. Since there is no principal need
// to do so, we allow these to be 'switched off'.

saga::task 
job_cpi_impl::async_get_state ()
{
    return saga::adaptors::task("job_cpi_impl::async_get_state",
        shared_from_this(), &job_cpi_impl::sync_get_state);
}

saga::task 
job_cpi_impl::async_get_description()
{
    return saga::adaptors::task("job_cpi_impl::async_get_description",
        shared_from_this(), &job_cpi_impl::sync_get_description);
}

saga::task 
job_cpi_impl::async_get_job_id ()
{
    return saga::adaptors::task("job_cpi_impl::async_get_job_id",
        shared_from_this(), &job_cpi_impl::sync_get_job_id);
}

saga::task 
job_cpi_impl::async_run ()
{
    return saga::adaptors::task("job_cpi_impl::async_run",
        shared_from_this(), &job_cpi_impl::sync_run);
}

saga::task 
job_cpi_impl::async_wait (double timeout)
{
    return saga::adaptors::task("job_cpi_impl::async_wait",
        shared_from_this(), &job_cpi_impl::sync_wait, timeout);
}

saga::task 
job_cpi_impl::async_cancel (double timeout)
{
    return saga::adaptors::task("job_cpi_impl::async_cancel",
        shared_from_this(), &job_cpi_impl::sync_cancel, timeout);
}

saga::task 
job_cpi_impl::async_suspend ()
{
    return saga::adaptors::task("job_cpi_impl::async_suspend",
        shared_from_this(), &job_cpi_impl::sync_suspend);
}

saga::task 
job_cpi_impl::async_resume ()
{
    return saga::adaptors::task("job_cpi_impl::async_resume",
        shared_from_this(), &job_cpi_impl::sync_resume);
}

saga::task 
job_cpi_impl::async_get_stdin()
{
    return saga::adaptors::task("job_cpi_impl::async_get_stdin",
        shared_from_this(), &job_cpi_impl::sync_get_stdin);
}

saga::task  
job_cpi_impl::async_get_stdout()
{
    return saga::adaptors::task("job_cpi_impl::async_get_stdout",
        shared_from_this(), &job_cpi_impl::sync_get_stdout);
}

saga::task  
job_cpi_impl::async_get_stderr()
{
    return saga::adaptors::task("job_cpi_impl::async_get_stderr",
        shared_from_this(), &job_cpi_impl::sync_get_stderr);
}

saga::task 
job_cpi_impl::async_migrate(saga::job::description jd)
{
    return saga::adaptors::task("job_cpi_impl::async_migrate",
        shared_from_this(), &job_cpi_impl::sync_migrate, jd);
}

#endif  // !defined (SAGA_DEFAULT_JOB_ADAPTOR_NO_ASYNCS)

///////////////////////////////////////////////////////////////////////////////
// initialize the job description from a given jobid
bool job_cpi_impl::fill_jobdescription(std::string jobid, 
    saga::job::description& jd)
{
    adaptor_data_type data (this);
    if (!data->knows_job(jobid))
        return false;
    jd = data->get_job(this, jobid);
    return true;
}

///////////////////////////////////////////////////////////////////////////
bool job_cpi_impl::fill_self_jobdescription(saga::job::description& jd)
{
    saga::adaptors::attribute attr(jd);
    
    // set the attributes for the job description we are able to deduce 
    // from the execution environment
    attr.set_attribute(saga::job::attributes::description_executable,
        get_self_exepath());
    attr.set_attribute(saga::job::attributes::description_working_directory,
        boost::filesystem::initial_path().string());
    attr.set_vector_attribute(saga::job::attributes::description_arguments, 
        get_self_args());
    attr.set_vector_attribute(saga::job::attributes::description_environment, 
        get_self_env());
    
    return true;
}

#endif // SAGA_WINDOWS

