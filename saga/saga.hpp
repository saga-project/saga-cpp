//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_HPP
#define SAGA_HPP

/*******************************
 * SAGA defines                * 
 *******************************/
#include <saga/saga-defs.hpp>

/*******************************
 * SAGA Look and Feel Packages * 
 *******************************/
# include <saga/saga/core.hpp>
  
/*******************************
 * SAGA Functional Packages    * 
 *******************************/
# ifdef SAGA_HAVE_PACKAGE_NAMESPACE
#  include <saga/saga/namespace.hpp>
# endif

# ifdef SAGA_HAVE_PACKAGE_FILESYSTEM
#  include <saga/saga/filesystem.hpp>
# endif

# ifdef SAGA_HAVE_PACKAGE_ADVERT
#  include <saga/saga/advert.hpp>
# endif

# ifdef SAGA_HAVE_PACKAGE_REPLICA
#  include <saga/saga/replica.hpp>
# endif

# ifdef SAGA_HAVE_PACKAGE_STREAM
#  include <saga/saga/stream.hpp>
# endif

# ifdef SAGA_HAVE_PACKAGE_RPC
#  include <saga/saga/rpc.hpp>
# endif

# ifdef SAGA_HAVE_PACKAGE_JOB
#  include <saga/saga/job.hpp>
# endif

# ifdef SAGA_HAVE_PACKAGE_SD
#  include <saga/saga/sd.hpp>
# endif

# ifdef SAGA_HAVE_PACKAGE_CPR
#  include <saga/saga/cpr.hpp>
# endif

#endif // SAGA_HPP

