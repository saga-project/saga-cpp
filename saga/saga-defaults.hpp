//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Note that this file relies on a number of defines to be set, usually by
// configure or the build environment used.  These defines are:
//
// SAGA_LOCATION: the target dir for this SAGA installation.

#ifndef SAGA_DEFAULTS_HPP
#define SAGA_DEFAULTS_HPP

#if !defined(SAGA_LOCATION)
#error "Do not include this file without defining SAGA_LOCATION (see saga-config.hpp)."
#endif

// the saga engine needs to know where to look for the saga ini 
// files if no ini path is specified by the user.  That by default
// in $SAGA_LOCATION/share/saga/.  Also, the default adaptor path 
// is set within the same prefix
#if !defined(SAGA_DEFAULT_INI_PATH)
#define SAGA_DEFAULT_INI_PATH      SAGA_LOCATION "/share/saga"
#endif
#if !defined(SAGA_DEFAULT_INI_FILE)
#define SAGA_DEFAULT_INI_FILE      SAGA_LOCATION "/share/saga/saga.ini"
#endif
#if !defined(SAGA_DEFAULT_ADAPTOR_PATH)
#define SAGA_DEFAULT_ADAPTOR_PATH  SAGA_LOCATION "/lib"
#endif

#endif // SAGA_DEFAULTS_HPP

