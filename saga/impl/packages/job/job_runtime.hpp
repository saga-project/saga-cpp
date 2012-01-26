//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(SAGA_JOB_IMPL_RUNTIME_APR_01_2008_0735PM)
#define SAGA_JOB_IMPL_RUNTIME_APR_01_2008_0735PM

#include <saga/saga/packages/job/config.hpp>
#include <saga/saga/packages/job/istream.hpp>
#include <saga/saga/packages/job/ostream.hpp>

namespace saga { namespace impl 
{
    struct job_runtime
    {
#if defined(BOOST_WINDOWS)
        typedef void* handle_type;
#else
        typedef int handle_type;
#endif

        static handle_type get_handle(saga::job::istream const& s, bool detach);
        static handle_type get_handle(saga::job::ostream const& s, bool detach);
    };

}}

#endif


