//  Copyright (c) 2005-2008 Andre Merzky
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <list>
#include <vector>
#include <string>

// helper class which maintains our process table
class joblist
{
  private:
    // the information we keep for all jobs.  We should, 
    // actually, be able to all info from the jobid alone...
    typedef struct 
    {
      int             pid;
      std::string     jobid;
      std::string     command;
      saga::job::job  job;
    } entry_t;

    typedef std::list <entry_t>           list_t;
    typedef std::list <entry_t>::iterator list_it_t;

    list_t list_;
    int    last_pid_;


  public:
    joblist (void);
   ~joblist (void) { };

    // a couple of methods to manipulate the process table
    int         add (std::string jobid, 
                     std::string command, 
                     saga::job::job   job);
    void        del (std::string jobid);
    void        del (int         pid);

    std::string get_jobid   (int pid);
    std::string get_command (int pid);
    saga::job::job  get_job     (int pid);

    std::vector <int> list (void);
};

