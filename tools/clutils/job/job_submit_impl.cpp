//  Copyright (c) 2008 Ole Weidner
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga.hpp>
#include "saga-job.hpp"

///////////////////////////////////////////////////////////////////////////////
#if defined(SAGA_HAVE_PACKAGE_JOB)

void job_submit (saga_tools::common & c, 
                 std::string          rm, 
                 std::string          exe, 
                 std::vector <std::string> args)
{
    saga::job::description jd;
    jd.set_attribute (saga::job::attributes::description_executable, exe);
    jd.set_attribute (saga::job::attributes::description_interactive, saga::attributes::common_false);

    jd.set_vector_attribute (saga::job::attributes::description_arguments, args);

    saga::job::service js (c.session (), saga::url (rm));
    saga::job::job     j = js.create_job (jd);

    j.run ();

    std::cout << "Job ID: " << j.get_job_id () << std::endl; 
}

#endif

