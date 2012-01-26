//  Copyright (c) 2008 Ole Weidner
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_FILE_HPP
#define SAGA_FILE_HPP

#include <iostream>
#include <saga/saga-defs.hpp>

#include <tools_common.hpp>

void file_run_test (saga_tools::common & c, 
                    std::string fileURL);

void file_list_dir (saga_tools::common & c, 
                    std::string directoryURL);

void file_find_dir (saga_tools::common & c, 
                    std::string          dir, 
                    std::string          pat);

void file_copy     (saga_tools::common & c, 
                    std::string sourceURL, 
                    std::string targetURL);

void file_link     (saga_tools::common & c, 
                    std::string sourceURL, 
                    std::string targetURL);

void file_move     (saga_tools::common & c, 
                    std::string sourceURL, 
                    std::string targetURL);

void file_remove   (saga_tools::common & c, 
                    std::string fileURL);

void file_cat      (saga_tools::common & c, 
                    std::string fileURL);

void file_get_size (saga_tools::common & c, 
                    std::string directoryURL);

void file_is_link  (saga_tools::common & c, 
                    std::string directoryURL);

#endif

