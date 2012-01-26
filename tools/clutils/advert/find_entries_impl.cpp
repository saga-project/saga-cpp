//  Copyright (c) 2008-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga.hpp>
#include "saga-advert.hpp"

///////////////////////////////////////////////////////////////////////////////
#if defined(SAGA_HAVE_PACKAGE_ADVERT) && defined(SAGA_HAVE_PACKAGE_NAMESPACE)

void advert_find_entries (saga_tools::common & c, 
                          std::string          ad, 
                          std::string          pat)
{
  saga::advert::directory d (c.session (), saga::url (ad));

  std::vector <std::string> patterns;
  patterns.push_back (pat);

  std::vector <saga::url> matches = d.find ("*", patterns, saga::advert::Recursive);

  for ( std::size_t n = 0; n < matches.size (); ++n)
  {
    std::cout << matches[n] << std::endl;
  }
}

#endif

