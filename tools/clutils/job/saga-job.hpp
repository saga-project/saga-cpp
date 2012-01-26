//  Copyright (c) 2008 Ole Weidner
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_FILE_HPP
#define SAGA_FILE_HPP

#include <iostream>
#include <saga/saga-defs.hpp>

#include <tools_common.hpp>

void job_run_test (saga_tools::common & c, 
                   std::string fileURL);

int  job_run      (saga_tools::common & c, 
                   std::string rm_url, 
                   std::string exe, 
                   std::vector<std::string> args);

void job_list     (saga_tools::common & c, 
                   std::string rm_url);

void job_submit   (saga_tools::common & c, 
                   std::string rm_url, 
                   std::string exe, 
                   std::vector<std::string> args);

void job_state    (saga_tools::common & c, 
                   std::string rm_url, 
                   std::string job_id);

void job_suspend  (saga_tools::common & c, 
                   std::string rm_url, 
                   std::string job_id);

void job_resume   (saga_tools::common & c, 
                   std::string rm_url, 
                   std::string job_id);


#endif

