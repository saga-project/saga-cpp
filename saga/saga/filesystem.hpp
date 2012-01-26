//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_SAGA_FILESYSTEM_HPP
#define SAGA_SAGA_FILESYSTEM_HPP

#include <saga/saga-defs.hpp>

# ifdef SAGA_HAVE_PACKAGE_FILESYSTEM
// file comes from the data package
#include <saga/saga/packages/filesystem/version.hpp>
#include <saga/saga/packages/filesystem/iovec.hpp>
#include <saga/saga/packages/filesystem/file.hpp>
#include <saga/saga/packages/filesystem/directory.hpp>

// auto linking is disabled while compiling the file package
#if !defined(SAGA_FILESYSTEM_PACKAGE_EXPORTS) || defined(SAGA_USE_AUTO_LINKING)
#define SAGA_AUTOLINK_LIB_NAME "filesystem"
#include <saga/saga/autolink.hpp>
#endif

#else

#if defined(_MSC_VER) || defined(__BORLANDC__) || defined(__DMC__)
#  pragma message ("Warning: The saga-filesystem package has been disabled at configuration time.")
#elif defined(__GNUC__) || defined(__HP_aCC) || defined(__SUNPRO_CC) || defined(__IBMCPP__)
#  warning "The saga-filesystem package has been disabled at configuration time."
#endif

#endif

#endif //  SAGA_SAGA_FILESYSTEM_HPP

