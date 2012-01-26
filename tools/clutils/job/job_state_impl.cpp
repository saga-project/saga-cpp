//  Copyright (c) 2008 Ole Weidner
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga.hpp>
#include "saga-job.hpp"

///////////////////////////////////////////////////////////////////////////////
#if defined(SAGA_HAVE_PACKAGE_JOB)

void job_state (saga_tools::common & c, 
                std::string          rm, 
                std::string          id)
{
  saga::job::service js (c.session (), saga::url (rm));
  saga::job::job     j = js.get_job (id);

  switch ( j.get_state () )
  {
    case saga::job::New:       std::cout << "New"       << std::endl ; break;
    case saga::job::Running:   std::cout << "Running"   << std::endl ; break;
    case saga::job::Suspended: std::cout << "Suspended" << std::endl ; break;
    case saga::job::Done:      std::cout << "Done"      << std::endl ; break;
    case saga::job::Failed:    std::cout << "Failed"    << std::endl ; break;
    case saga::job::Canceled:  std::cout << "Canceled"  << std::endl ; break;
    default:                   std::cout << "Unknown"   << std::endl ; break;
  }
}

#endif

