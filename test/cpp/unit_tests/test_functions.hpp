//  Copyright (c) 2005-2007 Chris Miceli (cmicel1@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(SAGA_TEST_FUNCTIONS_HPP)
#define SAGA_TEST_FUNCTIONS_HPP

#include "macros.hpp"

///////////////////////////////////////////////////////////////////////////////
//  verification of first state
inline void verify_created_state(saga::task t, saga::task_base::Sync)
{
    SAGA_CHECK(t.get_state() == saga::task_base::Done ||
               t.get_state() == saga::task_base::Failed);
}
inline void verify_created_state(saga::task t, saga::task_base::Async)
{
    SAGA_CHECK(t.get_state() == saga::task_base::Running || 
               t.get_state() == saga::task_base::Done    || 
               t.get_state() == saga::task_base::Failed);
}
inline void verify_created_state(saga::task t, saga::task_base::Task)
{
    SAGA_CHECK_EQUAL(t.get_state(), saga::task_base::New);
}
template <typename Tag>
inline void verify_created_state(saga::task t)
{
    verify_created_state(t, Tag());
}

///////////////////////////////////////////////////////////////////////////////
//  run process
inline void run_check_state(saga::task t, saga::task_base::Sync)
{
    SAGA_CHECK(t.get_state() == saga::task_base::Done || 
               t.get_state() == saga::task_base::Failed);
}
inline void run_check_state(saga::task t, saga::task_base::Async)
{
    SAGA_CHECK(t.get_state() == saga::task_base::Running || 
               t.get_state() == saga::task_base::Done    || 
               t.get_state() == saga::task_base::Failed);
}
inline void run_check_state(saga::task t, saga::task_base::Task)
{
    t.run();
    SAGA_CHECK(t.get_state() == saga::task_base::Running || 
               t.get_state() == saga::task_base::Done    || 
               t.get_state() == saga::task_base::Failed);
}
template <typename Tag>
inline void run_check_state(saga::task t)
{
    run_check_state(t, Tag());
}

///////////////////////////////////////////////////////////////////////////////
//  wait for all
inline void wait_check_state(saga::task t, saga::task_base::Sync)
{
    SAGA_CHECK(t.get_state() == saga::task_base::Done || 
               t.get_state() == saga::task_base::Failed);
}
inline void wait_check_state(saga::task t, saga::task_base::Async)
{
    t.wait();
    SAGA_CHECK(t.get_state() == saga::task_base::Done || 
               t.get_state() == saga::task_base::Failed);
}
inline void wait_check_state(saga::task t, saga::task_base::Task)
{
    t.wait();
    SAGA_CHECK(t.get_state() == saga::task_base::Done || 
               t.get_state() == saga::task_base::Failed);
}
template <typename Tag>
inline void wait_check_state(saga::task t)
{
    wait_check_state(t, Tag());
}

///////////////////////////////////////////////////////////////////////////////
// Call rethrow if expected result is a exception
inline void rethrow_on_error(saga::task t)
{
    t.rethrow();
}

#endif // SAGA_TEST_FUNCTIONS_HPP
