//
//  Copyright (c) 2008 João Abecasis
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <saga/saga.hpp>

#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <boost/program_options.hpp>
#include <boost/thread.hpp>
#include <boost/timer.hpp>

#include <list>
#include <string>
#include <vector>

typedef std::list<saga::job::job> job_list;
typedef job_list::iterator job_iterator;

////////////////////////////////////////////////////////////////////////////////
//  Global data

// Shared state
volatile bool all_jobs_started = false;

job_list jobs;

boost::mutex jobs_end_mutex;
job_iterator jobs_end = jobs.begin();

job_iterator get_jobs_end()
{
    boost::mutex::scoped_lock lock(jobs_end_mutex);
    return jobs_end;
}

//  Configuration
std::string resource_manager;
std::string candidate_host;
std::string command_line;
std::size_t repetitions;

//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Command line processing
bool handle_options(int ac, char ** av)
{
    using boost::program_options::value;

    boost::program_options::options_description
        required("Required options"),
        optional("Optional options");
    boost::program_options::positional_options_description
        positional;

    required.add_options()
        ("executable",          value<std::string>(),
            "program to run")
        ("argument",            value<std::vector<std::string> >(),
            "arguments to executable")
        ;

    positional.add("executable", 1);
    positional.add("argument", -1);

    optional.add_options()
        ("help", "show this help message")
        ("repetitions,n",       value<int>()        ->default_value(10),
            "number of jobs to start")
        ("candidate-host,h",     value<std::string>()->default_value(""))
        ("resource-manager,rm",    value<std::string>()->default_value(""))
        ;

    boost::program_options::options_description options;

    options
        .add(required)
        .add(optional)
        ;

    boost::program_options::variables_map vars;
    boost::program_options::store(
        boost::program_options::command_line_parser(ac, av)
            .options(options)
            .positional(positional)
            .run(), vars);
    boost::program_options::notify(vars);

    if (vars.count("help")
            || !vars.count("executable"))
    {
        std::cout << "Usage: " << av[0]
            << " [options] [--executable] exe [[--argument] arg]*\n";
        std::cout << options << std::endl;
        return false;
    }

    resource_manager    = vars["resource-manager"].as<std::string>();
    candidate_host      = vars["candidate-host"].as<std::string>();
    repetitions         = vars["repetitions"].as<int>();

    command_line        = vars["executable"].as<std::string>();

    if (vars.count("argument")) 
    {
        std::vector<std::string> const& args (
            vars["argument"].as<std::vector<std::string> >());
        std::vector<std::string>::const_iterator end = args.end();
        for (std::vector<std::string>::const_iterator it = args.begin(); 
             it != end; ++it)
        {
            // TODO: Deal with quoting
            command_line += " " + *it;
        }
    }
    return true;
}
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Jobs are started in a background thread
void start_jobs(saga::job::service js)
{
    for (std::size_t i = 1; i <= repetitions; ++i)
    {
        saga::job::job job = js.run_job(command_line, candidate_host);
        jobs.push_back(job);

        std::cout << boost::str(boost::format("Started (%1%): %|20t|%2%\n")
            % i % job.get_job_id());

        if (!(i % 7))
        {
            boost::mutex::scoped_lock lock(jobs_end_mutex);
            jobs_end = jobs.end();
        }
    }

    {
        boost::mutex::scoped_lock lock(jobs_end_mutex);
        jobs_end = jobs.end();
    }

    all_jobs_started = true;

    std::clog << "==> All jobs started!\n" << std::flush;
}
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Foreground thread waits for jobs to finish
int main(int argc, char ** argv)
{
    if (!handle_options(argc, argv))
        return 1;

    saga::job::service js(resource_manager); 

    // Start jobs in background thread
    boost::thread init(boost::bind(start_jobs, js));

    // Wait on jobs
    int finished_jobs = 0;

    boost::xtime t;
    boost::xtime_get(&t, boost::TIME_UTC);

    while (true)
    {
        std::string output;

        for (job_iterator job = jobs.begin(), end = get_jobs_end(); job != end;
                ++job)
        {
            try
            {
                saga::job::job job_copy = js.get_job(job->get_job_id());
            }
            catch (saga::exception const &)
            {
                // We should only allow saga::does_not_exist here, however,
                // given the landscape of Adaptor land, we must go with the
                // generic version.
            }

            if (job->wait(0.0))
            {
                output += boost::str(
                    boost::format("Finished (%1%): %|20t|%2%\n")
                        % ++finished_jobs % job->get_job_id());

                job = jobs.erase(job);

                if (!(finished_jobs % 10))
                {
                    std::cout << output << std::flush;
                    output.clear();
                }
            }
        }

        std::cout << output << std::flush;

        // Once all jobs have been started we can (thread-) safely check if jobs
        // is empty.
        if (all_jobs_started && jobs.empty())
            break;

        // Although sleeping here is not really needed, it seems to avoid a
        // race condition on Mac OS.
        //
        // I couldn't find the race elsewhere in the code, so I'm hoping the
        // race is due to compiler re-ordering of instructions and a race
        // condition with exit(3) and multi-threads.
        t.sec += 1;
        boost::thread::sleep(t);
    }

    std::clog << "==> All jobs finished!\n" << std::flush;

    init.join();
}
//
////////////////////////////////////////////////////////////////////////////////
