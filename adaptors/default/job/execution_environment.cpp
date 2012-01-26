//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga-defs.hpp>

#ifdef SAGA_WINDOWS

#include <cstdlib>
#include <ctime>
#include <algorithm>

#include <boost/config.hpp>
#include <saga/saga/util.hpp>
#include <saga/impl/exception.hpp>

#include "execution_environment.hpp"

#ifdef __APPLE__
#include <stdio.h>
#include <crt_externs.h>
#include <mach-o/dyld.h>
#define environ (*_NSGetEnviron())

#elif defined(linux) || defined(__linux) || defined(__linux__) || defined(__AIX__)
#include <unistd.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <sstream>
extern char **environ;
#endif

#if !defined(MAX_PATH)
#define MAX_PATH _POSIX_PATH_MAX
#endif

#if defined(linux) || defined(__linux) || defined(__linux__) 
        
  bool linux_get_cmd_line (std::vector <std::string>& cmdline)
  {
    std::string   elem;
    SAGA_OSSTREAM procfile;
    
    procfile << "/proc/" << getpid () << "/cmdline";
    std::ifstream proc;
    proc.open (SAGA_OSSTREAM_GETSTRING(procfile).c_str (), std::ios::binary);

    if (!proc.is_open())
        return false;       // proc fs does not exist on this machine
        
    while (!proc.eof ())
    {
      char c = proc.get();
      if (c == '\0')
      {
        cmdline.push_back(elem);
        elem.clear();
      }
      else
      {
        elem += c;
      }
    }
    
    if (!elem.empty())
      cmdline.push_back(elem);

    return true;
  }

  bool linux_get_args (std::vector <std::string>& args)
  {
    std::vector <std::string> cmdline;
    if (!linux_get_cmd_line (cmdline))
      return false;

    if (cmdline.size() > 1) 
      std::copy(cmdline.begin()+1, cmdline.end(), std::back_inserter(args));

    return true;
  }

  bool linux_get_cmd (std::string& result)
  {
    std::vector <std::string> cmdline; 
    if (linux_get_cmd_line(cmdline) && cmdline.size() > 0)
    {
      result = "./" + cmdline[0];
      return true;
    }
    return false;
  }

#endif
///////////////////////////////////////////////////////////////////////////////
execution_environment::execution_environment()
:   started_(0)
{
    initialize();           // initialize argv, env and exepath
    std::time(&started_);
    
    if (NULL != saga::detail::safe_getenv("SAGA_JOBID")) 
        jobid_ = saga::detail::safe_getenv("SAGA_JOBID");
}

execution_environment::~execution_environment()
{
}

#if defined(__APPLE__)
int get_executable_filename(char *exepath, uint32_t len)
{
  /* First appeared in Mac OS X 10.2 */
  return _NSGetExecutablePath(exepath, &len);
}  
  
#elif defined(linux) || defined(__linux) || defined(__linux__) || defined(__AIX__) 
int get_executable_filename(char *exepath, int len, char * argv[])
{
    if (len <= 0)
        return 0;   // buffer is too small
        
    // check for /proc/self/exe which is an symbolic link to the exe
    // if ( 0 > readlink ("/proc/self/exe", exe, MAX_PATH) )
    exepath[0] = '\0';

    if ( argv == NULL )
      return (0); // no chance to find exec path

    if (true) {
    /* some more heuristics:
      *    if argv[0] starts with '/'
      *    then
      *       absolute path - done
      *    else if argv[0] contains '/'
      *       relative path - append to pwd - done
      *    else
      *       run through $PATH looking for the first entry
      *       that yields a file that exists when pre-pended
      *       to argv[0]
      *    fi
      */
        if (strlen (argv[0]) > 0) {
            if (argv[0][0] == '/') {                  // absolute path
              snprintf (exepath, len, "%s", argv[0]);
            }
            else if (NULL != strchr (argv[0], '/')) { // relative path
              char pwd[MAX_PATH + 1] = { '\0' };
              if (NULL != getcwd (pwd, MAX_PATH))
                  snprintf (exepath, len, "%s/%s", pwd, argv[0]);
            }
            else {
            // need to search $PATH 
                char* epath = saga::detail::safe_getenv("PATH");
                if (NULL != epath) {
                // get first path element 
                    char* path_elem = strtok (epath, ":");

                    // try each path element
                    while (NULL != path_elem) {
                        struct stat s;
                        snprintf (exepath, len, "%s/%s", path_elem, argv[0]);

                        if (0 == stat (exepath, &s)) {
                        // exe exists - check if it is executable 
                            if ((s.st_uid == geteuid() && (s.st_mode & S_IXUSR)) ||
                                (s.st_gid == getegid() && (s.st_mode & S_IXGRP )) ||
                                (                          s.st_mode & S_IXOTH ))
                            {
                                break;
                            }
                        }
                        // prepare next try
                        exepath[0] = '\0';
                        path_elem = strtok(NULL, ":");
                    }
                }
            }
        }
    }
    return strlen(exepath);
}
#endif  // apple/linux

///////////////////////////////////////////////////////////////////////////
//  Figure out the size of the given environment
inline int get_arraylen(char **array)
{
    int count = 0;
    if (NULL != array) {
        while(NULL != array[count])
            ++count;   // simply count the environment strings
    }
    return count;
}

///////////////////////////////////////////////////////////////////////////
//  Initialize the execution environment
void execution_environment::initialize()
{
    initialize(0, NULL, NULL, NULL);
}

void 
execution_environment::initialize(int argc, char* argv[], char **env, 
    char const* path)
{
// initialize the execution environment
    env_.clear();
    if (NULL == env) {
#if defined(BOOST_WINDOWS)
        int len = get_arraylen(_environ);
        std::copy(&_environ[0], &_environ[len], std::back_inserter(env_));
#elif defined(linux) || defined(__linux) || defined(__linux__) || defined(__AIX__)  
        int len = get_arraylen(environ);
        std::copy(&environ[0], &environ[len], std::back_inserter(env_));
#elif defined(__APPLE__)
        int len = get_arraylen(environ);
        std::copy(&environ[0], &environ[len], std::back_inserter(env_));
#else
#error "Don't know, how to access the execution environment on this platform"
#endif
    }
    else {
        int len = get_arraylen(env);
        std::copy(&env[0], &env[len], std::back_inserter(env_));
    }
        
// initialize the path to the executable
    exepath_.clear();
    if (NULL == path) {
        char exepath[MAX_PATH + 1] = { '\0' };
        
#if defined(BOOST_WINDOWS)
        GetModuleFileName(NULL, exepath, sizeof(exepath));
        exepath_ = exepath;

#elif defined(linux) || defined(__linux) || defined(__linux__)
        if (!linux_get_cmd(exepath_)) 
        {
            if (NULL == argv || 
                0 == get_executable_filename(exepath, sizeof(exepath), argv))
            {
                SAGA_THROW("Unable to extract executable path for this job "
                    "on this platform.", saga::NotImplemented);
            }
            exepath_ = exepath;
        }
#elif defined(__AIX__)
	exepath_.clear();
#elif defined(__APPLE__)
        get_executable_filename(exepath, sizeof(exepath));
        exepath_ = exepath;
#else
#error "Don't know, how to access the executable path on this platform"
#endif
    }
    else {
        exepath_ = path;
    }
        
// store the arguments
    args_.clear();
    if (NULL == argv) {
#if defined(BOOST_WINDOWS)
        // we need to calculate the size of argv, because Boost.Test just 
        // NULL's out its own command line arguments, leaving us with
        // invalid command line argument entries
        int len = get_arraylen(__argv);
        std::copy(&__argv[0], &__argv[len], std::back_inserter(args_));
      
#elif defined(linux) || defined(__linux) || defined(__linux__)
        // get args from /proc/$pid/cmdline
        if (!linux_get_args (args_)) 
        {
            SAGA_THROW("Unable to extract arguments for this job on this "
                "platform.",  saga::NotImplemented);
        }
#elif defined(__AIX__)
              
#elif defined(__APPLE__)
        // we need to calculate the size of argv, because Boost.Test just 
        // NULL's out its own command line arguments, leaving us with
        // invalid command line argument entries
        char **__argv = *_NSGetArgv();
        int len = get_arraylen(__argv);
        std::copy(&__argv[0], &__argv[len], std::back_inserter(args_));
#else
#error "Don't know, how to access the executable arguments on this platform"
#endif
    }
    else {
        std::copy(&argv[0], &argv[argc], std::back_inserter(args_));
    }
}

///////////////////////////////////////////////////////////////////////////
// This manages and exposes the execution environment of the current job
execution_environment& get_execution_environment()
{
    static execution_environment environment;
    return environment;
}

#if defined(BOOST_WINDOWS)
///////////////////////////////////////////////////////////////////////////////
//  initialize Winsock library
struct init_winsocket
{
    init_winsocket()
    {
        WSADATA data_;
        WSAStartup(MAKEWORD(2, 2), &data_);
    }
    ~init_winsocket()
    {
        WSACleanup();
    }
};
init_winsocket init_winsock_lib;    // instantiate during startup
#endif


#endif // SAGA_WINDOWS

