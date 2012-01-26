//  Copyright (c) 2005-2008 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_SAGA_REPLICA_HPP
#define SAGA_SAGA_REPLICA_HPP

#include <saga/saga-defs.hpp>

# ifdef SAGA_HAVE_PACKAGE_REPLICA
// logicalfile comes from the data package
#include <saga/saga/packages/replica/version.hpp>
#include <saga/saga/packages/replica/logical_file.hpp>
#include <saga/saga/packages/replica/logical_directory.hpp>

#if !defined(SAGA_REPLICA_PACKAGE_EXPORTS) || defined(SAGA_USE_AUTO_LINKING)
#define SAGA_AUTOLINK_LIB_NAME "replica"
#include <saga/saga/autolink.hpp>
#endif

#else

#if defined(_MSC_VER) || defined(__BORLANDC__) || defined(__DMC__)
#  pragma message ("Warning: The saga-replica package has been disabled at configuration time.")
#elif defined(__GNUC__) || defined(__HP_aCC) || defined(__SUNPRO_CC) || defined(__IBMCPP__)
#  warning "The saga-replica package has been disabled at configuration time."
#endif

#endif

#endif //  SAGA_SAGA_REPLICA_HPP

