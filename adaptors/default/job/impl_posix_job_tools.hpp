//  Copyright (c) 2005-2007 Andre Merzky   (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. 
//  (See accompanying file LICENSE or copy at 
//   http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga-defs.hpp>

#ifndef SAGA_WINDOWS

#ifndef SAGA_POSIX_JOB_TOOLS_HPP
#define SAGA_POSIX_JOB_TOOLS_HPP

// C++ includes
#include <vector>
#include <string>
#include <fstream>

// C/posix includes
#include <unistd.h>
#include <sys/types.h>


////////////////////////////////////////////////////////////////////////
namespace impl
{
  namespace tools
  {
    std::vector <std::string> split    (std::string src, 
                                        char        del = ' ');
    std::string               readline (FILE      * fd);

    void                      dump_jd  (saga::job::description jd, 
                                        std::string id = "");
  

    // cache for ps information (known jobs in the system)
    class known_job
    {
      public:
        pid_t                     pid;
        uid_t                     uid;
        std::string               sta;
        std::string               exe;
        std::string               psline;
        std::vector <std::string> args;
        saga::job::state          state;
        bool                      spawned;

        known_job (void)
          : spawned (false)
        {
        }

        void dump (void)
        {
          std::cout << "job entry: \t pid    : " << pid       << std::endl;
          std::cout << "           \t uid    : " << uid       << std::endl;
          std::cout << "           \t sta    : " << sta       << std::endl;
          std::cout << "           \t state  : " << state2str (state) << std::endl;
          std::cout << "           \t spawned: " << spawned   << std::endl;
          std::cout << "           \t exe    : " << exe       << std::endl;

          for ( unsigned int i = 0; i < args.size (); i++ )
          {
            std::cout << args[i] << " ";
          }

          std::cout << "           \t ps line: " << psline  << std::endl;
          std::cout << "\n------------------------" << std::endl;
        }

        std::string state2str (saga::job::state s)
        {
          switch ( s )
          {
            case saga::job::Unknown:
                    return "Unknown";
                    break;
            case saga::job::New:
                    return "New";
                    break;
            case saga::job::Running:
                    return "Running";
                    break;
            case saga::job::Suspended:
                    return "Suspended";
                    break;
            case saga::job::Done:
                    return "Done";
                    break;
            case saga::job::Failed:
                    return "Failed";
                    break;
            case saga::job::Canceled:
                    return "Canceled";
                    break;
            default:
                    return "???";
                    break;
          }
        }
    };

    typedef std::map <pid_t, known_job>  known_job_map_t; 

    known_job_map_t get_known_jobs (void);
    known_job       get_known_job  (pid_t pid);

    class logger 
    {
      private:
        std::string  log_;
# ifdef USE_LOGGER
        std::fstream fs_;
#endif

      public:
        logger (std::string log = "/tmp/log")
          : log_ (log)
# ifdef USE_LOGGER
            , fs_  (log_.c_str (), std::fstream::out | std::fstream::app)
#endif
        {
# ifdef USE_LOGGER
          if ( fs_.fail () ) 
          {
            std::cerr << "cannot open log\n";
            SAGA_ADAPTOR_THROW_NO_CONTEXT ("cannot open log file", saga::NoSuccess);
          } 
# endif
        }


        ~logger (void)
        {
# ifdef USE_LOGGER
          fs_.close ();
# endif
        }

        void log (std::string msg) 
        {
# ifdef USE_LOGGER
          logn (msg);
          fs_ << std::endl;
# endif
        }

        void logn (std::string msg) 
        {
# ifdef USE_LOGGER
          fs_ << msg << std::flush;
# endif
        }

        template <typename T>
          void log (std::string msg, T t) 
          {
# ifdef USE_LOGGER
            logn (msg, t);
            fs_ << std::endl;
# endif
          }

        template <typename T>
          void logn (std::string msg, T t) 
          {
# ifdef USE_LOGGER
            char buf[256] = "";
            ::snprintf (buf, 255, "%s: ", msg.c_str ());
            fs_ << buf << t << std::flush;
# endif
          }
    };
  }

} // namespace impl
////////////////////////////////////////////////////////////////////////

#endif // SAGA_POSIX_JOB_TOOLS_HPP

#endif // SAGA_WINDOWS

