//  Copyright (c) 2005-2009 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga-defs.hpp>

#ifdef SAGA_WINDOWS 

#  include <map>
#  include <vector>
#  include <algorithm>

#  include <boost/function_output_iterator.hpp>

#  include <saga/saga/adaptors/config.hpp>
#  include <saga/saga/adaptors/task.hpp>
#  include <saga/saga/adaptors/adaptor.hpp>

#  include "default_job_adaptor.hpp"

#  include "default_job_service.hpp"
#  include "default_job.hpp"



SAGA_ADAPTOR_REGISTER (default_job_adaptor);

/*
 * register function for the SAGA engine
 */

saga::impl::adaptor_selector::adaptor_info_list_type
default_job_adaptor::adaptor_register(saga::impl::session *s)
{
  // list of implemented cpi's
  saga::impl::adaptor_selector::adaptor_info_list_type list;
  
  // create preferences
  preference_type prefs; // (std::string ("security"), std::string ("none"));
  
  // create file adaptor infos (each adaptor instance gets its own uuid)
  // and add cpi_infos to list
  job_service_cpi_impl::register_cpi(list, prefs, adaptor_uuid_);
  job_cpi_impl::register_cpi(list, prefs, adaptor_uuid_);
  
  // and return list
  return (list);
}

///////////////////////////////////////////////////////////////////////////////
bool default_job_adaptor::register_job(std::string jobid, saga::job::description jd)
{
  std::pair<known_jobs_type::iterator, bool> p =
  known_jobs_.insert(known_jobs_type::value_type(jobid, jd));
  return p.second;
}

bool default_job_adaptor::unregister_job(std::string jobid)
{
  known_jobs_type::iterator it = known_jobs_.find(jobid);
  if (it == known_jobs_.end())
    return false;
  
  known_jobs_.erase(it);
  return true;
}

saga::job::description 
default_job_adaptor::get_job(job_cpi_impl const* job, std::string jobid) const
{
  known_jobs_type::const_iterator it = known_jobs_.find(jobid);
  if (it == known_jobs_.end()) {
    SAGA_ADAPTOR_THROW_VERBATIM(job, "Nothing known about job: " + jobid, 
                                saga::BadParameter);
  }
  return (*it).second;
}

///////////////////////////////////////////////////////////////////////////////
namespace {
  
  struct string_appender
  {
    typedef default_job_adaptor::known_jobs_type::value_type value_type;
    
    string_appender(std::vector<std::string>& v) : v_(v) {}
    void operator()(value_type v) { v_.push_back(v.first); }
    std::vector<std::string>& v_;
  };
}

std::vector<std::string> default_job_adaptor::list_jobs() const
{
  std::vector <std::string> jobids;
  std::copy(known_jobs_.begin(), known_jobs_.end(),
            boost::make_function_output_iterator(string_appender(jobids)));
  return jobids;
}

bool default_job_adaptor::knows_job(std::string jobid) const
{
  return known_jobs_.find(jobid) != known_jobs_.end();
}



#else // SAGA_WINDOWS



# include <saga/saga.hpp>

// saga adaptor includes
# include <saga/saga/adaptors/config.hpp>
# include <saga/saga/adaptors/task.hpp>
# include <saga/saga/adaptors/adaptor.hpp>

// adaptor includes
# include "default_job_adaptor.hpp"
# include "posix_job_service.hpp"
# include "posix_job.hpp"

SAGA_ADAPTOR_REGISTER (default_job_adaptor);

// register function for the SAGA engine
saga::impl::adaptor_selector::adaptor_info_list_type
default_job_adaptor::adaptor_register (saga::impl::session * s)
{
  // list of implemented cpi's
  saga::impl::adaptor_selector::adaptor_info_list_type list;
  
  // create empty preference list
  // these list should be filled with properties of the adaptor, 
  // which can be used to select adaptors with specific preferences.
  // Example:
  //   'security' -> 'gsi'
  //   'logging'  -> 'yes'
  //   'auditing' -> 'no'
  preference_type prefs; 
  
  // create file adaptor infos (each adaptor instance gets its own uuid)
  // and add cpi_infos to list
  job_service_cpi_impl::register_cpi (list, prefs, adaptor_uuid_);
  job_cpi_impl::register_cpi         (list, prefs, adaptor_uuid_);
  
  // and return list
  return (list);
}


void default_job_adaptor::known_job_refresh (void)
{
  // refresh list
  impl::tools::known_job_map_t new_known_jobs = impl::tools::get_known_jobs ();

  // fold job infos we gatherered earlier into the new known job map, but mark
  // them as done.  This relies on pid'd being unique, which is not strictly
  // true, but 'true enough'...
  impl::tools::known_job_map_t::iterator old_it;

  {
    // impl::tools::known_job_map_t::iterator new_it;
    // 
    // std::cout << " --------------------------------------------\n";
    // for ( new_it = new_known_jobs.begin () ; new_it != new_known_jobs.end (); new_it++ )
    // {
    //   new_it->second.dump ();
    // }
    // std::cout << " --------------------------------------------\n";
  }


  // lets see if the old ids we stored are actually part of the new list
  for ( old_it = known_jobs_.begin () ; old_it != known_jobs_.end (); old_it++ )
  {
    pid_t old_pid = old_it->first;

    impl::tools::known_job_map_t::iterator new_it;

    // if this job is not yet in the new list...
    bool found = false;
    for ( new_it = new_known_jobs.begin () ; new_it != new_known_jobs.end (); new_it++ )
    {
      pid_t new_pid = new_it->first;
      
      if ( new_pid == old_pid )
      {
        // this old id *is* part of the new list - nothing to do
        found  = true;
        break;
      }
    }

    if ( ! found ) 
    {
      // this old id is not in the new list - so we add it.
      struct impl::tools::known_job kj;
      
      kj.pid    = old_it->second.pid;
      kj.uid    = old_it->second.uid;
      kj.exe    = old_it->second.exe;
      kj.args   = old_it->second.args;
      kj.state  = saga::job::Done;

      // std::cout << " ============================================\n";
      // kj.dump ();
      // std::cout << " ============================================\n";
      
      new_known_jobs[old_pid] = kj;
    }
  }

  // replace old list with new one
  known_jobs_ = new_known_jobs;
}


impl::tools::known_job default_job_adaptor::known_job_find (pid_t pid)
{
  // try to find the job from ps (list)
  known_job_refresh ();
  
  impl::tools::known_job_map_t::iterator it = known_jobs_.find (pid);
  
  if ( it == known_jobs_.end ())
  {
    SAGA_OSSTREAM strm;
    strm << "Could not find local job " << pid2jobid (pid) << ". ";
    
    SAGA_ADAPTOR_THROW_NO_CONTEXT (SAGA_OSSTREAM_GETSTRING(strm),
                                   saga::DoesNotExist);
  }
  
  return it->second;
}

void default_job_adaptor::known_job_register (TR1::shared_ptr <impl::posix_job> impl)
{
  if ( ! impl )
  {
    return;
  }
  
  pid_t                  pid = impl->get_jobid ();
  saga::job::description jd  = impl->get_jd ();
  
  impl::tools::known_job kj;
  
  kj.spawned = true;
  kj.uid     = ::getuid ();
  kj.pid     = pid;
  kj.state   = impl->get_state ();
  
  if ( jd.attribute_exists (saga::job::attributes::description_executable) )
  { 
    kj.exe  = jd.get_attribute (saga::job::attributes::description_executable);
  }
  
  if ( jd.attribute_exists (saga::job::attributes::description_arguments) )
  {
    kj.args = jd.get_vector_attribute (saga::job::attributes::description_arguments);
  }

  // kj.dump ();

  known_jobs_[pid] = kj;
}


void default_job_adaptor::dump_known_jobs (void)
{
  std::cout << " ==============================================" << std::endl;

  impl::tools::known_job_map_t::iterator begin = known_jobs_.begin ();
  impl::tools::known_job_map_t::iterator end   = known_jobs_.end ();
  impl::tools::known_job_map_t::iterator it;

  for ( it = begin; it != end; it++ )
  {
    std::cout << " ------------------------" << std::endl;
    it->second.dump ();
  }
  
  std::cout << " ------------------------" << std::endl;
  std::cout << " ==============================================" << std::endl;
}


//////////////////////////////////////////////////////////////////////
std::string default_job_adaptor::pid2jobid (pid_t pid, std::string h)
{
  std::stringstream pid_string;
  std::string       out = "";
  
  pid_string << pid;
  
  out += "[fork://";
  
  if ( h.empty () ) { out += localhost (); }
  else              { out += h;            }

  out += "]-[";
  out += pid_string.str ();
  out += "]";
  
  return (out);
}

//////////////////////////////////////////////////////////////////////
pid_t default_job_adaptor::jobid2pid (std::string jobid)
{
  std::vector <std::string> elems  = saga::adaptors::utils::split (jobid,     '-');
  if ( elems.size () < 2 ) return -1;
  
//std::vector <std::string> elems1 = saga::adaptors::utils::split (elems[0],  ']');
//if ( elems1.size () < 1 ) return -1;
 
//std::vector <std::string> elems2 = saga::adaptors::utils::split (elems1[0], '[');
//if ( elems2.size () < 1 ) return -1;
  
  std::vector <std::string> elems3 = saga::adaptors::utils::split (elems[1],  ']');
  if ( elems3.size () < 1 ) return -1;
  
  std::vector <std::string> elems4 = saga::adaptors::utils::split (elems3[0], '[');
  if ( elems4.size () < 1 ) return -1;
  
  return (::atoi (elems4[0].c_str ()));
}

//////////////////////////////////////////////////////////////////////
saga::url default_job_adaptor::jobid2url (std::string jobid)
{
  std::string url;
  std::string::size_type pos = jobid.find("]-[");
  
  if(pos == std::string::npos)
    throw("malformed url");
  
  if(jobid.find("[")== 0)
    url = jobid.substr(1,pos-1); // cut trailing '['
  else
    url = jobid.substr(0,pos-1); // probably malformes url. don't mess with it
 
  return saga::url(url);
}


//////////////////////////////////////////////////////////////////////
std::string default_job_adaptor::localhost (void)
{
  if ( localhost_ == "" )
  {
    char hostname_c[PATH_MAX + 1];
    
    if ( -1 == gethostname (hostname_c, PATH_MAX) )
    {
      // could not get hostname, assume 'localhost'
      snprintf (hostname_c, PATH_MAX, "localhost");
    }
    
    for ( unsigned int i = 0; i < strlen (hostname_c); i++ )
    {
      hostname_c[i] = ::tolower (hostname_c[i]);
    }
    
    localhost_ = hostname_c;
  }
  
  return (localhost_);
}


#endif // SAGA_WINDOWS

