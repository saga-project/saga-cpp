//  Copyright (c) 2009 STFC
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef SAGA_SAGA_SD_HPP
#define SAGA_SAGA_SD_HPP

#include <saga/saga-defs.hpp>

# ifdef SAGA_HAVE_PACKAGE_SD
#include <saga/saga/task.hpp>
#include <saga/saga/packages/sd/version.hpp>
#include <saga/saga/packages/sd/service_data.hpp>
#include <saga/saga/packages/sd/service_description.hpp>
#include <saga/saga/packages/sd/discoverer.hpp>

// auto linking is disabled while compiling the sd package and while 
// compiling the SAGA engine
#if !defined(SAGA_SD_PACKAGE_EXPORTS) || defined(SAGA_USE_AUTO_LINKING)
#define SAGA_AUTOLINK_LIB_NAME "sd"
#include <saga/saga/autolink.hpp>
#endif

#else

#if defined(_MSC_VER) || defined(__BORLANDC__) || defined(__DMC__)
#  pragma message ("Warning: The saga-sd package has been disabled at configuration time.")
#elif defined(__GNUC__) || defined(__HP_aCC) || defined(__SUNPRO_CC) || defined(__IBMCPP__)
#  warning "The saga-sd package has been disabled at configuration time."
#endif

#endif

#endif
