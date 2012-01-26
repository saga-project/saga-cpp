//  Copyright (c) 2008 Ole Weidner
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <vector>

#include <saga/saga.hpp>
#include "saga-job.hpp"

///////////////////////////////////////////////////////////////////////////////
#if defined(SAGA_HAVE_PACKAGE_JOB)

void job_run_test (saga_tools::common & c, 
                   std::string service_url)
{
   using namespace saga::job;
   
   saga::url js_url("gram://qb1.loni.org");
   
   saga::job::description jd;
   jd.set_attribute("Executable", "/bin/date");
   
   //std::vector <std::string> transfers;
   //transfers.push_back ("gsiftp://mO/tmp/files_on_storage_server1 > myfFile1");
   //transfers.push_back ("file://tmp/test.rsl > myfFile2");
   //transfers.push_back ("file://tmp/test.rsl < myfFile2");
   //transfers.push_back ("file://tmp/test.rsl < adasd");

   //jd.set_vector_attribute("FileTransfer", transfers);
   
   saga::job::service js (c.session (), js_url);
   saga::job::job j = js.create_job(jd);
   
   j.run();
   std::cout << "ID: " << j.get_job_id() << std::endl;

   std::cout << "STATUS: " << j.get_state() << std::endl;
   
   saga::job::job j1 = js.get_job(j.get_job_id());
   std::cout << "STATUS: " << j1.get_state() << std::endl;

   saga::job::service js2 (c.session (), js_url);
   saga::job::job j2 = js2.get_job(j.get_job_id());
   std::cout << "STATUS: " << j2.get_state() << std::endl;
   
}

#endif

