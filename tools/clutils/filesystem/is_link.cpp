//  Copyright (c) 2008 Ole Weidner
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <vector>

#include <saga/saga.hpp>
#include "saga-file.hpp"

///////////////////////////////////////////////////////////////////////////////
#if defined(SAGA_HAVE_PACKAGE_FILESYSTEM) && defined(SAGA_HAVE_PACKAGE_NAMESPACE)

void file_is_link (saga_tools::common & c, 
                   std::string sourceURL)
{
    saga::filesystem::file file (c.session (), 
                                 saga::url (sourceURL), 
                                 saga::filesystem::Read);
    if(file.is_link()) {
      std::cout << sourceURL << ": True" << std::endl;
    }
    else
    {
      std::cout << sourceURL << ": False" << std::endl;
    }
}

#endif

