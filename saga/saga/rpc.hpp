//  Copyright (c) 2005-2006 Andre Merzky (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_SAGA_RPC_HPP
#define SAGA_SAGA_RPC_HPP

#include <saga/saga-defs.hpp>

# ifdef SAGA_HAVE_PACKAGE_RPC
// rpc comes from the comm package
#include <saga/saga/packages/rpc/version.hpp>
#include <saga/saga/packages/rpc/parameter.hpp>
#include <saga/saga/packages/rpc/rpc.hpp>

// auto linking is disabled while compiling the rpc package
#if !defined(SAGA_RPC_PACKAGE_EXPORTS) || defined(SAGA_USE_AUTO_LINKING)
#define SAGA_AUTOLINK_LIB_NAME "rpc"
#include <saga/saga/autolink.hpp>
#endif

#else

#if defined(_MSC_VER) || defined(__BORLANDC__) || defined(__DMC__)
#  pragma message ("Warning: The saga-rpc package has been disabled at configuration time.")
#elif defined(__GNUC__) || defined(__HP_aCC) || defined(__SUNPRO_CC) || defined(__IBMCPP__)
#  warning "The saga-rpc package has been disabled at configuration time."
#endif

#endif

#endif //  SAGA_SAGA_RPC_HPP

