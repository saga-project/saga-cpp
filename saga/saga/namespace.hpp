//  Copyright (c) 2005-2006 Andre Merzky (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_SAGA_NAMESPACE_HPP
#define SAGA_SAGA_NAMESPACE_HPP

#include <saga/saga-defs.hpp>

# ifdef SAGA_HAVE_PACKAGE_NAMESPACE
// namespace_entry and namespace_dir come from the data package
#include <saga/saga/packages/namespace/version.hpp>
#include <saga/saga/packages/namespace/namespace_entry.hpp>
#include <saga/saga/packages/namespace/namespace_dir.hpp>

#if !defined(SAGA_NAMESPACE_PACKAGE_EXPORTS) || defined(SAGA_USE_AUTO_LINKING)
#define SAGA_AUTOLINK_LIB_NAME "namespace"
#include <saga/saga/autolink.hpp>
#endif

#else

#if defined(_MSC_VER) || defined(__BORLANDC__) || defined(__DMC__)
#  pragma message ("Warning: The saga-namespace package has been disabled at configuration time.")
#elif defined(__GNUC__) || defined(__HP_aCC) || defined(__SUNPRO_CC) || defined(__IBMCPP__)
#  warning "The saga-namespace package has been disabled at configuration time."
#endif

#endif

#endif //  SAGA_SAGA_NAMESPACE_HPP

