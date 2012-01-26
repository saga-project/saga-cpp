//  Copyright (c) 2008 Ole Weidner
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/lambda/lambda.hpp>

#include <saga/saga.hpp>
#include "saga-replica.hpp"

///////////////////////////////////////////////////////////////////////////////
#if defined(SAGA_HAVE_PACKAGE_REPLICA) && defined(SAGA_HAVE_PACKAGE_NAMESPACE)

namespace 
{
  struct print_attributes
  {
    saga::replica::logical_file lf_;

    print_attributes(saga::replica::logical_file lf) 
      : lf_ (lf) 
    {
    }

    template <typename T>
    void operator() (T const & key) const
    {
      // FIXME: what about vector attribs?
      std::cout << "  " << key << " \t: " << lf_.get_attribute (key) 
                << std::endl;
    }

  };
}

///////////////////////////////////////////////////////////////////////////////
void replica_list_attributes (saga_tools::common & c,
                              std::string          lfn)
{
  // instantiate logical file 
  
  saga::replica::logical_file lf (c.session (), saga::url (lfn));

  // list attributes
  std::vector <std::string> keys (lf.list_attributes ());
  std::for_each (keys.begin (), keys.end (), print_attributes (lf));
}

#endif

