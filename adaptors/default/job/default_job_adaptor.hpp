//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ADAPTORS_DEFAULT_JOB_DEFAULT_ADAPTOR_HPP
#define ADAPTORS_DEFAULT_JOB_DEFAULT_ADAPTOR_HPP

// saga adaptor includes
#include <saga/saga/adaptors/adaptor.hpp>

#include <saga/saga-defs.hpp>

#ifdef SAGA_WINDOWS

#include <map>

///////////////////////////////////////////////////////////////////////////////
//  forward decl only
class job_cpi_impl;

///////////////////////////////////////////////////////////////////////////////
struct default_job_adaptor : public saga::adaptor
{
    typedef saga::impl::v1_0::op_info         op_info;  
    typedef saga::impl::v1_0::cpi_info        cpi_info;
    typedef saga::impl::v1_0::preference_type preference_type;

    /**
    * This functions registers the adaptor with the factory
    *
    * @param factory the factory where the adaptor registers
    *        its maker function and description table
    */
    saga::impl::adaptor_selector::adaptor_info_list_type 
            adaptor_register (saga::impl::session *s);
            
    std::string get_name (void) const
    { 
        return BOOST_PP_STRINGIZE(SAGA_ADAPTOR_NAME);
    }

    // list of job ids of jobs created by this adaptor
    typedef std::map<std::string, saga::job::description> known_jobs_type;
    known_jobs_type known_jobs_;
    
    bool register_job(std::string jobid, saga::job::description jd);
    bool unregister_job(std::string jobid);
    bool knows_job(std::string jobid) const;
    saga::job::description get_job(job_cpi_impl const* job, std::string jobid) const;
    std::vector<std::string> list_jobs() const;
};



# else // SAGA_WINDOWS


  
  // adaptor includes
  # include "impl_posix_job.hpp"
  # include "impl_posix_job_tools.hpp"

  // adaptor includes/predefs
  class job_cpi_impl;
  class job_service_cpi_impl;

  class default_job_adaptor : public saga::adaptor
  {
    private:
      typedef saga::impl::v1_0::op_info         op_info;  
      typedef saga::impl::v1_0::cpi_info        cpi_info;
      typedef saga::impl::v1_0::preference_type preference_type;

      // This function registers the adaptor with the factory
      // @param factory the factory where the adaptor registers
      //        its maker function and description table
      saga::impl::adaptor_selector::adaptor_info_list_type 
        adaptor_register (saga::impl::session * s);
  
      std::string get_name (void) const
      { 
        return BOOST_PP_STRINGIZE (SAGA_ADAPTOR_NAME);
      }
  
      // where to store running jobs?
      std::map <std::string, job_cpi_impl *> jobs_;
  
      std::string  localhost_;


    public:
      default_job_adaptor(void)
        : localhost_ ("")
      {
        // NOTE: Throwing here will mess things up!
      }

      ~default_job_adaptor (void) 
      {
      }
  
  
      impl::tools::known_job_map_t  known_jobs_;

      void                   known_job_refresh  (void);
      impl::tools::known_job known_job_find     (pid_t pid);
      void                   known_job_register (TR1::shared_ptr <impl::posix_job> impl);
      void                   dump_known_jobs    (void);

      std::string            pid2jobid          (pid_t pid, std::string h = "");
      pid_t                  jobid2pid          (std::string jobid);
      saga::url              jobid2url          (std::string jobid);
      std::string            localhost          (void);
  };

#endif // SAGA_WINDOWS

#endif // ADAPTORS_DEFAULT_JOB_DEFAULT_ADAPTOR_HPP

