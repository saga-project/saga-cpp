//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga-defs.hpp>

#ifdef SAGA_WINDOWS

///////////////////////////////////////////////////////////////////////////////
//  This class holds the execution environment of this job
class execution_environment
{
public:
    execution_environment();
    ~execution_environment();
    
    void initialize();
    void initialize(int argc, char* argv[], char **env, char const* path);
    
    std::time_t get_started_time() const { return started_; }
    std::string get_exepath() const { return exepath_; }
    std::string get_jobid() const { return jobid_; }
    std::vector<std::string> const& get_args() const { return args_; }
    std::vector<std::string> const& get_env() const { return env_; }
    
private:
    std::string exepath_;
    std::vector<std::string> args_;
    std::vector<std::string> env_;
    std::time_t started_;
    std::string jobid_;
};

///////////////////////////////////////////////////////////////////////////
// This manages and exposes the execution environment of the current job
execution_environment& get_execution_environment();

///////////////////////////////////////////////////////////////////////////
//  return the time this job was started
inline std::time_t get_self_started_time()
{
    return get_execution_environment().get_started_time();
}

inline std::string get_self_exepath()
{
    return get_execution_environment().get_exepath();
}

inline std::string get_self_jobid()
{
    return get_execution_environment().get_jobid();
}

inline std::vector<std::string> const& get_self_args()
{
    return get_execution_environment().get_args();
}

inline std::vector<std::string> const& get_self_env()
{
    return get_execution_environment().get_env();
}

#endif // SAGA_WINDOWS

