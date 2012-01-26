//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. 
//  See accompanying file LICENSE 
//  or copy at http://www.boost.org/LICENSE_1_0.txt

#ifndef SAGA_ADAPTORS_DEFAULT_REPLICA_CONFIG_HPP
#define SAGA_ADAPTORS_DEFAULT_REPLICA_CONFIG_HPP

#ifndef SAGA_SKIP_CONFIG_HPP
#include "saga/saga-defs.hpp"
#endif

#ifdef SAGA_HAVE_CONFIG

# ifdef   SAGA_DEFAULT_ADAPTORS_USE_SQLITE3
#  define SAGA_DEFAULT_REPLICA_ADAPTOR_USE_SQLITE3
# endif

# ifdef   SAGA_DEFAULT_ADAPTORS_USE_POSTGRESQL
#  define SAGA_DEFAULT_REPLICA_ADAPTOR_USE_POSTGRESQL
# endif

# ifdef   SAGA_DEFAULT_ADAPTORS_USE_MYSQL
#  define SAGA_DEFAULT_REPLICA_ADAPTOR_USE_MYSQL
# endif

#else // SAGA_HAVE_CONFIG

// make sure at least SQLite3 will be compiled into the adaptor
# if !defined(SAGA_DEFAULT_REPLICA_ADAPTOR_USE_SQLITE3) &&  \
     !defined(SAGA_DEFAULT_REPLICA_ADAPTOR_USE_POSTGRESQL) && \
     !defined(SAGA_DEFAULT_REPLICA_ADAPTOR_USE_MYSQL)
#  define SAGA_DEFAULT_REPLICA_ADAPTOR_USE_SQLITE3
# endif

#endif // SAGA_HAVE_CONFIG

#endif // SAGA_ADAPTORS_DEFAULT_REPLICA_CONFIG_HPP

