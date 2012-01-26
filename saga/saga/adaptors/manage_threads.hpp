//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(SAGA_MANAGE_THREADS_JUL_14_2007_0828PM)
#define SAGA_MANAGE_THREADS_JUL_14_2007_0828PM

#include <saga/saga/util.hpp>

#include <boost/thread.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace adaptors 
{
    ///////////////////////////////////////////////////////////////////////////
    //  saga supports managing external threads, which essentially means it 
    //  will join with all managed threads before the main thread terminates
    SAGA_EXPORT void add_managed_thread (saga::session s, boost::thread* thrd);
    SAGA_EXPORT void remove_managed_thread (saga::session s, boost::thread* thrd);

///////////////////////////////////////////////////////////////////////////////
}}

#endif


