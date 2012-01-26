//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga/packages/job/version.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga 
{
  // Returns the version of the SAGA job package
  unsigned long get_job_package_version (void)
  {
    return SAGA_VERSION_PACKAGE_JOB_FULL;
  }

  // Returns the implemented SAGA job package API version
  unsigned long get_job_package_api_version (void)
  {
    return SAGA_VERSION_PACKAGE_JOB_API;
  }

} // namespace saga
///////////////////////////////////////////////////////////////////////////////

