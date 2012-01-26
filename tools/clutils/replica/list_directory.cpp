//  Copyright (c) 2008 Ole Weidner
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga.hpp>
#include "saga-replica.hpp"

///////////////////////////////////////////////////////////////////////////////
#if defined(SAGA_HAVE_PACKAGE_REPLICA) && defined(SAGA_HAVE_PACKAGE_NAMESPACE)

void replica_list_directory (saga_tools::common & c,
                             std::string          ldn)
{
  // instantiate logical directory
  saga::replica::logical_directory ld (c.session (), saga::url (ldn));

  // read replica entries
  std::vector <saga::url> entries (ld.list ());

  std::vector <saga::url>::const_iterator it;

  for ( it = entries.begin (); it != entries.end (); ++it )
  {
    std::cout << "  " << (*it) << std::endl;
  }
}

#endif
