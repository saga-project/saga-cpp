//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_SAGA_JOB_HPP
#define SAGA_SAGA_JOB_HPP

#include <saga/saga-defs.hpp>

# ifdef SAGA_HAVE_PACKAGE_JOB
// file comes from the data package
#include <saga/saga/task.hpp>
#include <saga/saga/packages/job/version.hpp>
#include <saga/saga/packages/job/job_description.hpp>
#include <saga/saga/packages/job/istream.hpp>
#include <saga/saga/packages/job/ostream.hpp>
#include <saga/saga/packages/job/job.hpp>
#include <saga/saga/packages/job/job_self.hpp>
#include <saga/saga/packages/job/job_service.hpp>

// auto linking is disabled while compiling the job package
#if !defined(SAGA_JOB_PACKAGE_EXPORTS) || defined(SAGA_USE_AUTO_LINKING)
#define SAGA_AUTOLINK_LIB_NAME "job"
#include <saga/saga/autolink.hpp>
#endif

#else

#if defined(_MSC_VER) || defined(__BORLANDC__) || defined(__DMC__)
#  pragma message ("Warning: The saga-job package has been disabled at configuration time.")
#elif defined(__GNUC__) || defined(__HP_aCC) || defined(__SUNPRO_CC) || defined(__IBMCPP__)
#  warning "The saga-job package has been disabled at configuration time."
#endif

#endif

#endif //  SAGA_SAGA_JOB_HPP

