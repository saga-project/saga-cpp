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

void file_remove (saga_tools::common & c, 
                  std::string fileURL)
{
    saga::filesystem::file file (c.session (), 
                                 saga::url (fileURL), 
                                 saga::filesystem::ReadWrite);
    file.remove();
}

#endif

