//
//  Copyright (c) 2008 Ole Weidner
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga.hpp>
#include "saga-job.hpp"

///////////////////////////////////////////////////////////////////////////////
#if defined(SAGA_HAVE_PACKAGE_JOB)

void job_list (saga_tools::common & c, 
                std::string          service_url)
{
  saga::job::service js (c.session (), saga::url (service_url));

  std::vector <std::string> ids = js.list ();

  for ( unsigned int i = 0; i < ids.size (); i++ )
  {
    std::cout << ids[i] << " \t ";

    try 
    {
      saga::job::job         j  = js.get_job (ids[i]);
      saga::job::description jd = j.get_description ();

      switch ( j.get_state () )
      {
        case saga::job::New:       std::cout << "(New)      : " ; break;
        case saga::job::Running:   std::cout << "(Running)  : " ; break;
        case saga::job::Suspended: std::cout << "(Suspended): " ; break;
        case saga::job::Done:      std::cout << "(Done)     : " ; break;
        case saga::job::Failed:    std::cout << "(Failed)   : " ; break;
        case saga::job::Canceled:  std::cout << "(Canceled) : " ; break;
        default:                   std::cout << "(Unknown)  : " ; break;
      }
      std::cout << jd.get_attribute (saga::job::attributes::description_executable) << " ";

      if ( jd.attribute_exists (saga::job::attributes::description_arguments) )
      {
        std::vector <std::string> args = jd.get_vector_attribute (saga::job::attributes::description_arguments);

        for ( unsigned int j = 0; j < args.size (); j++ )
        {
          std::cout << args[j] << " ";
        }
      }
    }
    catch ( const saga::exception & e )
    {
      std::cout << " disappeared";
    }

    std::cout << std::endl;
  }
}

#endif

