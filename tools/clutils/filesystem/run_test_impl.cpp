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

void file_run_test (saga_tools::common & c, 
                    std::string sourceURL)
{
  using namespace saga::attributes;

  saga::url u1 (sourceURL);
  saga::filesystem::file f1 (c.session(), u1);

  for(int i=0; i < 100000; i++)
  {
    bool a = f1.is_entry();
    a = !a; // avoid 'unused var' warning
  }
}

#endif

