//
//  Copyright (c) 2005-2008 Andre Merzky (andre@merzky.net)
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_DEFS_HPP
#define SAGA_DEFS_HPP

// always include saga-config.hpp, if we have any, to get system defs 
// consistent
#ifndef SAGA_SKIP_CONFIG_HPP
# include <saga/saga-config.hpp>
#endif

// boost knows about windows.  Note that the other platform defines are defined
// in saga-config.hpp
#include <boost/config.hpp>
#ifdef BOOST_WINDOWS
# define SAGA_WINDOWS
#endif

#if defined(SAGA_WINDOWS)   // avoid problems with boost asio
#include <WinSock2.h>
#endif

// export definitions
#include <saga/saga/export_definitions.hpp>
  
// version defines
#include <saga/saga/version.hpp>
  
// get default defines, e.g. paths etc.
#include <saga/saga-defaults.hpp>

#endif // SAGA_DEFS_HPP

