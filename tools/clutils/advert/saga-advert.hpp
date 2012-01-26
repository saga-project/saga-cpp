//  Copyright (c) 2008-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_REPLICA_HPP
#define SAGA_REPLICA_HPP

#include <iostream>
#include <saga/saga-defs.hpp>
#include <saga/saga.hpp>

#include <tools_common.hpp>

///////////////////////////////////////////////////////////////////////////////

void advert_list_directory   (saga_tools::common & c,
                              std::string          aname, 
                              std::string          pattern = "*");

void advert_dump_directory   (saga_tools::common & c,
                              std::string          aname, 
                              std::string          indent = "  ");

void advert_add_directory    (saga_tools::common & c,
                              std::string          aname);

void advert_add_entry        (saga_tools::common & c,
                              std::string          aname);

void advert_remove_entry     (saga_tools::common & c,
                              std::string          aname);

void advert_remove_directory (saga_tools::common & c,
                              std::string          aname);

void advert_find_entries     (saga_tools::common & c, 
                              std::string          ad, 
                              std::string          pat);

void advert_list_attributes  (saga_tools::common & c,
                              std::string          aname);

void advert_set_attribute    (saga_tools::common & c,
                              std::string          aname, 
                              std::string          key, 
                              std::string          val);

void advert_get_attribute    (saga_tools::common & c,
                              std::string          aname, 
                              std::string          key);

void advert_remove_attribute (saga_tools::common & c,
                              std::string          aname, 
                              std::string          key);

void advert_store_string     (saga_tools::common & c,
                              std::string          aname, 
                              std::string          data);

void advert_retrieve_string  (saga_tools::common & c,
                              std::string          aname);

#endif
