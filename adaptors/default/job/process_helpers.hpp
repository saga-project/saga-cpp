//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga-defs.hpp>

#ifdef SAGA_WINDOWS

#include <boost/process.hpp>

#if defined(BOOST_PROCESS_POSIX_API)
  #include <signal.h>
  #include <sys/wait.h>
  #include <sys/types.h>
#elif defined(BOOST_PROCESS_WIN32_API)
  #include <tlhelp32.h>
#endif

#if !defined(ADAPTORS_DEFAULT_PROCESS_HELPERS_HPP)
#define ADAPTORS_DEFAULT_PROCESS_HELPERS_HPP

///////////////////////////////////////////////////////////////////////////////
#if defined(BOOST_PROCESS_WIN32_API)
// generate meaningful error message
inline std::string get_errormessage(DWORD errcode)
{
    char *str = NULL;
    if (::FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, 
            NULL, errcode, 0, (char *)&str, 0, NULL)
        )
    {
    // unknown error code
        return std::string("Unknown error: ") + 
            boost::lexical_cast<std::string>(errcode);
    }
    std::string result(str);
    ::LocalFree(str);
    return result;
}

struct handle_wrapper
{
    handle_wrapper(HANDLE h) : h_(h) {}
    ~handle_wrapper()
    {
        ::CloseHandle(h_);
    }
    operator HANDLE() { return h_; }
    HANDLE h_;
};
#endif

// retrieve current status of the child process
inline saga::job::state 
state_from_process (boost::process::child::handle_type pid, 
                    saga::job::state old_state = saga::job::Unknown,
                    int *pstatus = NULL)
{
#if defined(BOOST_PROCESS_POSIX_API)

  // Waits until the process exits and parses its termination status.
  // Note that we receive a posix_status object even when the wait()
  // method returns a status one.

  int status  = 0;
  int ret_pid = 0;
  int options = WNOHANG | WUNTRACED;
  
#ifdef WCONTINUED
  options |= WCONTINUED;
#endif

  ret_pid = ::waitpid (pid, &status, options);

  if ( ret_pid == -1 )
  {
    // here, we SHOULD actually return unknown, as we cannot get
    // state for this pid.  However, if waitpid was called for
    // a finished job twice, we fail as well, and that actually
    // then means that the state did not change (job is
    // finished, remember?).  So, we just return the old state.
    // In order to work correctly, we assume that the old_state
    // for an unknown job is Unknown to begin with... -- AM
    SAGA_LOG_DEBUG("check: do not know this job\n");
    return (old_state);
  }
  else if ( ret_pid == 0 )
  {
    // SAGA_LOG_DEBUG("check: nothing changed\n");
    return (old_state);
    // return saga::job::Running;
  }
  else 
  {
    // make sure we are working on the correct job id
    BOOST_ASSERT (pid == ret_pid);

    // task changed state!
    SAGA_LOG_DEBUG("check: changed\n");


    saga::job::state state_to_return = old_state;

    if ( WIFEXITED(status) ) 
    {
#ifdef WEXITSTATUS
      if (NULL != pstatus) 
        *pstatus = WEXITSTATUS(status);
#endif
      SAGA_LOG_DEBUG("check:   exit\n");

      // FIXME: should remember signaled:    WIFSIGNALED(status)
      // FIXME: should remember signal:      WTERMSIG(status)
#ifdef WCOREDUMP
      // FIXME: should remember core dumped: WCOREDUMP(signal)
#endif

      state_to_return = saga::job::Done;
    }
    else if ( WIFSIGNALED (status) )
    {
      SAGA_LOG_DEBUG("check:   canceled by some signal\n");
      state_to_return = saga::job::Canceled;
    }
    else if ( WIFCONTINUED (status) )
    {
      SAGA_LOG_DEBUG("check:   continued\n");
      state_to_return = saga::job::Running;
    }
    else if ( WIFSTOPPED(status) )
    {
      SAGA_LOG_DEBUG("check:   suspended\n");
      state_to_return = saga::job::Suspended;
    }
    else
    {
      // unknown state change
      SAGA_LOG_DEBUG("check:   what?\n");
      state_to_return = saga::job::Unknown;
    }

    // something changed!  Well, we cannot be sure that the info
    // we git via waitppid is really the latest available.  For
    // example, a kill could have arrived after a resume - the
    // waitpid gets only the resume.
    //
    // Thus we call ourself again, and return the last found
    // state if that did not change.
    {
      if ( state_to_return == old_state )
      {
        return (state_to_return);
      }
      else
      {
        return (state_from_process (pid, state_to_return, pstatus));
      }
    }
  }

  // should never get here
  return saga::job::Unknown;

#elif defined(BOOST_PROCESS_WIN32_API)

    handle_wrapper h (::OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid));
    if (h == NULL)
    {
        return saga::job::Unknown;
    }
     
    DWORD code = 0;
    ::GetExitCodeProcess (h, &code);
    if ( STILL_ACTIVE == code )
    {
        return saga::job::Running;
    }
    
// return exit code is requested
    if (NULL != pstatus)
        *pstatus = code;
        
// exit code of zero means finished ok
    return (code >= 0) ? saga::job::Done : saga::job::Failed;

#endif
}

///////////////////////////////////////////////////////////////////////////////
inline
bool wait_for_process(boost::process::child::handle_type h, 
    int *pstatus = NULL)
{
#if defined(BOOST_PROCESS_POSIX_API)
  //std::cerr << "waitpid start" << std::endl;
    int s = 0;
    
    if (-1 != ::waitpid(h, &s, 0)) { #ifdef WEXITSTATUS if (NULL != pstatus)
      *pstatus = WEXITSTATUS(s); #endif return WIFEXITED(s); } return false;
    
#elif defined(BOOST_PROCESS_WIN32_API)

    handle_wrapper handle (::OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, h));
    DWORD code = 0;
    // This loop should go away in favour of a passive wait.
    do {
        ::GetExitCodeProcess(handle, &code);
        ::Sleep(50);
    } while (code == STILL_ACTIVE);
    ::WaitForSingleObject(handle, 0);

    if (NULL != pstatus) 
        *pstatus = code;
        
    return true;
    
#endif
}

///////////////////////////////////////////////////////////////////////////////
// terminate the given process
inline saga::job::state 
terminate_process (job_cpi_impl const* job, boost::process::child c)
{
#if defined(BOOST_PROCESS_POSIX_API)

    pid_t pid = c.get_handle ();
    if ( 0 != kill (pid, SIGKILL) )
    {
        SAGA_ADAPTOR_THROW_VERBATIM(job, std::string ("Couldn't cancel job: ")
                              + strerror (errno),
                              saga::NoSuccess);
    }

#elif defined(BOOST_PROCESS_WIN32_API)

    handle_wrapper h (::OpenProcess(PROCESS_TERMINATE, FALSE, c.get_handle()));
    if (h == NULL || !::TerminateProcess (h, 0))
    {
        SAGA_ADAPTOR_THROW_VERBATIM(job, "Couldn't cancel job: " +
            get_errormessage(::GetLastError()), saga::NoSuccess);
    }

#endif
    return saga::job::Canceled;
}

///////////////////////////////////////////////////////////////////////////////
// Suspend or resume the given child process
inline saga::job::state 
suspend_process(job_cpi_impl const* job, boost::process::child c) 
{ 
#if defined(BOOST_PROCESS_POSIX_API)

    pid_t pid = c.get_handle ();
    if ( 0 != kill (pid, SIGSTOP) )
    {
        SAGA_ADAPTOR_THROW_VERBATIM(job, std::string ("Couldn't suspend job: ")
                                + strerror (errno),
                                saga::NoSuccess);
    }

#elif defined(BOOST_PROCESS_WIN32_API)

    // we have to walk all threads of this process until we won't find any
    // more
    bool suspended_thread = false;
    std::set<HANDLE> threads;
    do {
        // Take a snapshot of all threads currently in the system. 
        handle_wrapper hThreadSnap (::CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0)); 
        if (INVALID_HANDLE_VALUE == hThreadSnap) {
            SAGA_ADAPTOR_THROW_VERBATIM(job, "Could not suspend/resume process: " +
                get_errormessage(::GetLastError()), saga::NoSuccess);
        }

        THREADENTRY32 te32 = { 0 }; 
        te32.dwSize = sizeof(THREADENTRY32); 
        suspended_thread = false;
      
        // Walk the thread snapshot to find all threads of the process. 
        if (::Thread32First(hThreadSnap, &te32)) { 
            do { 
                if (te32.th32OwnerProcessID == c.get_handle()) {
                            handle_wrapper hThread (::OpenThread(THREAD_SUSPEND_RESUME, 
                                FALSE, te32.th32ThreadID));
                            if (INVALID_HANDLE_VALUE == hThread) 
                        continue;   // thread did exit since snapshot time

                    if (threads.find(hThread) == threads.end()) {
                                  if (::SuspendThread(hThread) != (DWORD)(-1)) {
                            typedef std::set<HANDLE>::iterator iterator;
                            std::pair<iterator, bool> p = threads.insert(hThread);
                            if (p.second)
                                          suspended_thread = true;
                                  }
                    }
                } 
            } while (::Thread32Next(hThreadSnap, &te32));
        } 
        else {
            SAGA_ADAPTOR_THROW_VERBATIM(job, "Could not suspend/resume process: " + 
                get_errormessage(::GetLastError()), saga::NoSuccess);
        }
    } while (suspended_thread);
#endif
    return saga::job::Suspended;
} 

inline saga::job::state 
resume_process(job_cpi_impl const* job, boost::process::child c) 
{ 
#if defined(BOOST_PROCESS_POSIX_API)

    pid_t pid = c.get_handle ();


    if ( 0 != ::kill (pid, SIGCONT) )
    {
        SAGA_ADAPTOR_THROW_VERBATIM(job, std::string ("Couldn't resume job: ")
                                + strerror (errno),
                                saga::NoSuccess);
    }

#elif defined(BOOST_PROCESS_WIN32_API)

    // Take a snapshot of all threads currently in the system. 
    handle_wrapper hThreadSnap (::CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0)); 
    if (INVALID_HANDLE_VALUE == hThreadSnap) {
        SAGA_ADAPTOR_THROW_VERBATIM(job, "Could not suspend/resume process: " + 
            get_errormessage(::GetLastError()), saga::NoSuccess);
    }
          
    THREADENTRY32 te32 = { 0 }; 
    te32.dwSize = sizeof(THREADENTRY32); 
  
    // Walk the thread snapshot to find all threads of the process. 
    if (::Thread32First(hThreadSnap, &te32)) { 
        do { 
            if (te32.th32OwnerProcessID == c.get_handle()) {
                        handle_wrapper hThread (::OpenThread(THREAD_SUSPEND_RESUME, 
                            FALSE, te32.th32ThreadID));
                        if (INVALID_HANDLE_VALUE == hThread || 
                            (DWORD)(-1) == ::ResumeThread(hThread)) 
                        {
                    SAGA_ADAPTOR_THROW_VERBATIM(job, 
                        "Could not suspend/resume process: " + 
                        get_errormessage(::GetLastError()), saga::NoSuccess);
                }
            } 
        } while (::Thread32Next(hThreadSnap, &te32));
    } 
    else {
        SAGA_ADAPTOR_THROW_VERBATIM(job, 
            "Could not suspend/resume process: " + 
            get_errormessage(::GetLastError()), saga::NoSuccess);
    }

#endif
    return saga::job::Running;
} 

///////////////////////////////////////////////////////////////////////////////
// test for validity of child process handle
inline boost::process::child::handle_type get_invalid_handlevalue()
{
    return -1;
}

inline bool is_valid_handle(boost::process::child::handle_type handle)
{
    return handle != get_invalid_handlevalue();
}
    
///////////////////////////////////////////////////////////////////////////
// the jobid must have the format [file://localhost/]-[jobid], where jobid
// must be a integer representing the process id of the running child

#include <boost/version.hpp>
#if BOOST_VERSION >= 103800
#include <boost/spirit/include/classic_core.hpp>
#include <boost/spirit/include/classic_actor.hpp>
#else
#include <boost/spirit/core.hpp>
#include <boost/spirit/actor/assign_actor.hpp>
#endif

inline boost::process::child::handle_type 
extract_jobid(std::string const& jobid)
{
    using namespace boost::spirit;
    
    unsigned int handle = (unsigned int)-1;
    parse_info<> pi = parse(jobid.c_str(), 
            ch_p('[') >> *(anychar_p - ']') >> "]-[" 
        >>  *(anychar_p - ':') >> ':' >> int_p[assign_a(handle)] >> ']');

    return pi.full ?
        (boost::process::child::handle_type)handle :
        get_invalid_handlevalue();
}

///////////////////////////////////////////////////////////////////////////////

#endif // !defined(ADAPTORS_DEFAULT_PROCESS_HELPERS_HPP)

#endif // SAGA_WINDOWS

