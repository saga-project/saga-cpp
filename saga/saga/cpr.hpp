//  Copyright (c) 2005-2008 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_SAGA_CPR_HPP
#define SAGA_SAGA_CPR_HPP 1

#include <saga/saga/packages/cpr/version.hpp>

// the cpr package, inheriting from namespace and job
#include <saga/saga/job.hpp>
#include <saga/saga/namespace.hpp>

#include <saga/saga/task.hpp>
#include <saga/saga/packages/cpr/cpr_job_description.hpp>
#include <saga/saga/packages/cpr/cpr_job.hpp>
#include <saga/saga/packages/cpr/cpr_job_self.hpp>
#include <saga/saga/packages/cpr/cpr_job_service.hpp>
#include <saga/saga/packages/cpr/cpr_checkpoint.hpp>
#include <saga/saga/packages/cpr/cpr_directory.hpp>

#if !defined(SAGA_CPR_PACKAGE_EXPORTS)
#define SAGA_AUTOLINK_LIB_NAME "cpr"
#include <saga/saga/autolink.hpp>
#endif

#endif //  SAGA_SAGA_CPR_HPP

