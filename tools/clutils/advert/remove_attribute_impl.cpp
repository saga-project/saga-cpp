//  Copyright (c) 2008-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga.hpp>
#include "saga-advert.hpp"

///////////////////////////////////////////////////////////////////////////////
#if defined(SAGA_HAVE_PACKAGE_ADVERT) && defined(SAGA_HAVE_PACKAGE_NAMESPACE)

void advert_remove_attribute (saga_tools::common & c,
                              std::string          aname, 
                              std::string          key)
{
    // instantiate advert entry
    saga::url ad(aname);
    saga::advert::entry e(ad, saga::advert::ReadWrite);

    // remove attributes
    e.remove_attribute(key);
}

#endif
