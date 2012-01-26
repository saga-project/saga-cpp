//  Copyright (c) 2008 Ole Weidner
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_REPLICA_HPP
#define SAGA_REPLICA_HPP

#include <iostream>
#include <saga/saga-defs.hpp>

#include <tools_common.hpp>

///////////////////////////////////////////////////////////////////////////////

void replica_add_lfn          (saga_tools::common & c,
                               std::string lfn);


void replica_remove_lfn       (saga_tools::common & c,
                               std::string lfn);

void replica_list_directory   (saga_tools::common & c,
                               std::string lfn);

void replica_add_directory    (saga_tools::common & c,
                               std::string lfn);

void replica_remove_directory (saga_tools::common & c,
                               std::string lfn);

void replica_list_pfns        (saga_tools::common & c,
                               std::string lfn);

void replica_list_attributes  (saga_tools::common & c,
                               std::string lfn);

void replica_set_attribute    (saga_tools::common & c,
                               std::string lfn, 
                               std::string key, 
                               std::string val);

void replica_remove_attribute (saga_tools::common & c,
                               std::string lfn, 
                               std::string key);

void replica_add_pfn          (saga_tools::common & c,
                               std::string lfn, 
                               std::string pfn);

void replica_remove_pfn       (saga_tools::common & c,
                               std::string lfn, 
                               std::string pfn);

#endif

