//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_DETAIL_CALL_HPP
#define SAGA_DETAIL_CALL_HPP

#include <saga/impl/exception.hpp>

/// @cond

///////////////////////////////////////////////////////////////////////////////
//
// FIXME: document usage extensively
//
//  This file defines the API macros for the private functions and the 
//  function implementation.

///////////////////////////////////////////////////////////////////////////////
// Helper macro to ensure properly initialized objects
///////////////////////////////////////////////////////////////////////////////
#define SAGA_OBJECT_CHECK()                                                   \
    if (!this->is_impl_valid()) {                                             \
        SAGA_THROW("The object has not been properly initialized.",           \
            saga::IncorrectState);                                            \
    }                                                                         \
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
// 0 parameters
//
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
#define SAGA_CALL_IMP_0_EX(ns, name, name_impl)                               \
  saga::task ns::name ## priv (saga::task_base::Sync)                         \
  { SAGA_OBJECT_CHECK(); return get_impl()->name_impl(true); }                \
                                                                              \
  saga::task ns::name ## priv (saga::task_base::Async)                        \
  { SAGA_OBJECT_CHECK(); return saga::detail::run(get_impl()->name_impl()); } \
                                                                              \
  saga::task ns::name ## priv (saga::task_base::Task)                         \
  { SAGA_OBJECT_CHECK(); return get_impl()->name_impl(); }                    \
//-----------------------------------------------------------------------------
#define SAGA_CALL_IMP_0(ns, name)                                             \
    SAGA_CALL_IMP_0_EX(ns, name, name)                                        \
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define SAGA_CALL_CREATE_IMP_0(ns, impl)                                      \
  saga::task ns::createpriv (saga::task_base::Sync)                           \
  {                                                                           \
      saga::task t(saga::task::Done);                                         \
      TR1::shared_ptr<impl> i(new impl());                                    \
      i->create_impl_sync(t.get_result<ns>());                                \
      return t;                                                               \
  }                                                                           \
  saga::task ns::createpriv (saga::task_base::Async)                          \
  {   return saga::detail::run(impl::create_impl_async()); }                  \
                                                                              \
  saga::task ns::createpriv (saga::task_base::Task)                           \
  {   return impl::create_impl_async(); }                                     \
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define SAGA_CALL_CONST_IMP_0_EX(ns, name, name_impl)                         \
  saga::task ns::name ## priv (saga::task_base::Sync) const                   \
  { SAGA_OBJECT_CHECK(); return get_impl()->name_impl(true); }                \
                                                                              \
  saga::task ns::name ## priv (saga::task_base::Async) const                  \
  { SAGA_OBJECT_CHECK(); return saga::detail::run(get_impl()->name_impl()); } \
                                                                              \
  saga::task ns::name ## priv (saga::task_base::Task) const                   \
  { SAGA_OBJECT_CHECK(); return get_impl()->name_impl(); }                    \
//-----------------------------------------------------------------------------
#define SAGA_CALL_CONST_IMP_0(ns, name)                                       \
    SAGA_CALL_CONST_IMP_0_EX(ns, name, name)                                  \
//-----------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////
//
// 1 parameter
//
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
#define SAGA_CALL_IMP_1_EX(ns, name, name_impl, p1)                           \
  saga::task ns::name ## priv (p1 par1, saga::task_base::Sync)                \
  { SAGA_OBJECT_CHECK(); return get_impl()->name_impl(par1, true); }          \
                                                                              \
  saga::task ns::name ## priv (p1 par1, saga::task_base::Async)               \
  { SAGA_OBJECT_CHECK(); return (saga::detail::run(get_impl()->name_impl(     \
                                                   par1))); }                 \
                                                                              \
  saga::task ns::name ## priv (p1 par1, saga::task_base::Task)                \
  { SAGA_OBJECT_CHECK(); return get_impl()->name_impl(par1); }                \
//-----------------------------------------------------------------------------
#define SAGA_CALL_IMP_1(ns, name, p1)                                         \
    SAGA_CALL_IMP_1_EX(ns, name, name, p1)                                    \
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define SAGA_CALL_CREATE_IMP_1(ns, impl, p1)                                  \
  saga::task ns::createpriv (p1 par1, saga::task_base::Sync)                  \
  {                                                                           \
      saga::task t(saga::task::Done);                                         \
      TR1::shared_ptr<impl> i(new impl(par1));                                \
      i->create_impl_sync(t.get_result<ns>());                                \
      return t;                                                               \
  }                                                                           \
  saga::task ns::createpriv (p1 par1, saga::task_base::Async)                 \
  {   return saga::detail::run(impl::create_impl_async(par1)); }              \
                                                                              \
  saga::task ns::createpriv (p1 par1, saga::task_base::Task)                  \
  {   return impl::create_impl_async(par1); }                                 \
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define SAGA_CALL_CONST_IMP_1_EX(ns, name, name_impl, p1)                     \
  saga::task ns::name ## priv (p1 par1, saga::task_base::Sync) const          \
  { SAGA_OBJECT_CHECK(); return get_impl()->name_impl(par1, true); }          \
                                                                              \
  saga::task ns::name ## priv (p1 par1, saga::task_base::Async) const         \
  { SAGA_OBJECT_CHECK(); return (saga::detail::run(get_impl()->name_impl(     \
                                                   par1))); }                 \
                                                                              \
  saga::task ns::name ## priv (p1 par1, saga::task_base::Task) const          \
  { SAGA_OBJECT_CHECK(); return get_impl()->name_impl(par1); }                \
//-----------------------------------------------------------------------------
#define SAGA_CALL_CONST_IMP_1(ns, name, p1)                                   \
    SAGA_CALL_CONST_IMP_1_EX(ns, name, name, p1)                              \
//-----------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////
//
// 2 parameters
//
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
#define SAGA_CALL_IMP_2_EX(ns, name, name_impl, p1, p2)                       \
  saga::task ns::name ## priv (p1 par1, p2 par2, saga::task_base::Sync)       \
  { SAGA_OBJECT_CHECK(); return get_impl()->name_impl(par1, par2, true); }    \
                                                                              \
  saga::task ns::name ## priv (p1 par1, p2 par2, saga::task_base::Async)      \
  { SAGA_OBJECT_CHECK(); return (saga::detail::run(get_impl()->name_impl(par1,\
                                            par2))); }                        \
                                                                              \
  saga::task ns::name ## priv (p1 par1, p2 par2, saga::task_base::Task)       \
  { SAGA_OBJECT_CHECK(); return get_impl()->name_impl(par1, par2); }          \
//-----------------------------------------------------------------------------
#define SAGA_CALL_IMP_2(ns, name, p1, p2)                                     \
    SAGA_CALL_IMP_2_EX(ns, name, name, p1, p2)                                \
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define SAGA_CALL_CREATE_IMP_2(ns, impl, p1, p2)                              \
  saga::task ns::createpriv (p1 par1, p2 par2, saga::task_base::Sync)         \
  {                                                                           \
      saga::task t(saga::task::Done);                                         \
      TR1::shared_ptr<impl> i(new impl(par1, par2));                          \
      i->create_impl_sync(t.get_result<ns>());                                \
      return t;                                                               \
  }                                                                           \
  saga::task ns::createpriv (p1 par1, p2 par2, saga::task_base::Async)        \
  {   return saga::detail::run(impl::create_impl_async(par1, par2)); }        \
                                                                              \
  saga::task ns::createpriv (p1 par1, p2 par2, saga::task_base::Task)         \
  {   return impl::create_impl_async(par1, par2); }                           \
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define SAGA_CALL_CONST_IMP_2_EX(ns, name, name_impl, p1, p2)                 \
  saga::task ns::name ## priv (p1 par1, p2 par2, saga::task_base::Sync) const \
  { SAGA_OBJECT_CHECK(); return get_impl()->name_impl(par1, par2, true); }    \
                                                                              \
  saga::task ns::name ## priv (p1 par1, p2 par2, saga::task_base::Async) const\
  { SAGA_OBJECT_CHECK(); return (saga::detail::run(get_impl()->name_impl(par1,\
                                            par2))); }                        \
                                                                              \
  saga::task ns::name ## priv (p1 par1, p2 par2, saga::task_base::Task) const \
  { SAGA_OBJECT_CHECK(); return get_impl()->name_impl(par1, par2); }          \
//-----------------------------------------------------------------------------
#define SAGA_CALL_CONST_IMP_2(ns, name, p1, p2)                               \
    SAGA_CALL_CONST_IMP_2_EX(ns, name, name, p1, p2)                          \
//-----------------------------------------------------------------------------


///////////////////////////////////////////////////////////////////////////////
//
// 3 parameters
//
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
#define SAGA_CALL_IMP_3_EX(ns, name, name_impl, p1, p2, p3)                   \
  saga::task ns::name ## priv (p1 par1, p2 par2, p3 par3, saga::task_base::Sync) \
  { SAGA_OBJECT_CHECK(); return get_impl()->name_impl(par1, par2, par3, true); } \
                                                                              \
  saga::task ns::name ## priv (p1 par1, p2 par2, p3 par3, saga::task_base::Async)\
  { SAGA_OBJECT_CHECK(); return (saga::detail::run(get_impl()->name_impl(par1,\
                                            par2, par3))); }                  \
                                                                              \
  saga::task ns::name ## priv (p1 par1, p2 par2, p3 par3, saga::task_base::Task) \
  { SAGA_OBJECT_CHECK(); return get_impl()->name_impl(par1, par2, par3); }    \
//-----------------------------------------------------------------------------
#define SAGA_CALL_IMP_3(ns, name, p1, p2, p3)                                 \
    SAGA_CALL_IMP_3_EX(ns, name, name, p1, p2, p3)                            \
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define SAGA_CALL_CREATE_IMP_3(ns, impl, p1, p2, p3)                          \
  saga::task ns::createpriv (p1 par1, p2 par2, p3 par3, saga::task_base::Sync) \
  {                                                                           \
      saga::task t(saga::task::Done);                                         \
      TR1::shared_ptr<impl> i(new impl(par1, par2, par3));                    \
      i->create_impl_sync(t.get_result<ns>());                                \
      return t;                                                               \
  }                                                                           \
  saga::task ns::createpriv (p1 par1, p2 par2, p3 par3, saga::task_base::Async)\
  {   return saga::detail::run(impl::create_impl_async(par1, par2, par3)); }  \
                                                                              \
  saga::task ns::createpriv (p1 par1, p2 par2, p3 par3, saga::task_base::Task) \
  {   return impl::create_impl_async(par1, par2, par3); }                     \
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define SAGA_CALL_CONST_IMP_3_EX(ns, name, name_impl, p1, p2, p3)             \
  saga::task ns::name ## priv (p1 par1, p2 par2, p3 par3, saga::task_base::Sync) const \
  { SAGA_OBJECT_CHECK(); return get_impl()->name_impl(par1, par2, par3, true); } \
                                                                              \
  saga::task ns::name ## priv (p1 par1, p2 par2, p3 par3, saga::task_base::Async) const \
  { SAGA_OBJECT_CHECK(); return (saga::detail::run(get_impl()->name_impl(par1,\
                                            par2, par3))); }                  \
                                                                              \
  saga::task ns::name ## priv (p1 par1, p2 par2, p3 par3, saga::task_base::Task) const \
  { SAGA_OBJECT_CHECK(); return get_impl()->name_impl(par1, par2, par3); }    \
//-----------------------------------------------------------------------------
#define SAGA_CALL_CONST_IMP_3(ns, name, p1, p2, p3)                           \
    SAGA_CALL_CONST_IMP_3_EX(ns, name, name, p1, p2, p3)                      \
//-----------------------------------------------------------------------------


///////////////////////////////////////////////////////////////////////////////
//
// 4 parameters
//
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
#define SAGA_CALL_IMP_4_EX(ns, name, name_impl, p1, p2, p3, p4)               \
  saga::task ns::name ## priv (p1 par1, p2 par2, p3 par3, p4 par4,            \
                               saga::task_base::Sync)                         \
  { SAGA_OBJECT_CHECK(); return get_impl()->name_impl(par1, par2, par3, par4, true); } \
                                                                              \
  saga::task ns::name ## priv (p1 par1, p2 par2, p3 par3, p4 par4,            \
                               saga::task_base::Async)                        \
  { SAGA_OBJECT_CHECK(); return (saga::detail::run(get_impl()->name_impl(par1,\
                                            par2, par3, par4))); }            \
                                                                              \
  saga::task ns::name ## priv (p1 par1, p2 par2, p3 par3, p4 par4,            \
                               saga::task_base::Task)                         \
  { SAGA_OBJECT_CHECK(); return get_impl()->name_impl(par1, par2, par3, par4); } \
//-----------------------------------------------------------------------------
#define SAGA_CALL_IMP_4(ns, name, p1, p2, p3, p4)                             \
    SAGA_CALL_IMP_4_EX(ns, name, name, p1, p2, p3, p4)                        \
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define SAGA_CALL_CREATE_IMP_4(ns, impl, p1, p2, p3, p4)                      \
  saga::task ns::createpriv (p1 par1, p2 par2, p3 par3, p4 par4,              \
      saga::task_base::Sync)                                                  \
  {                                                                           \
      saga::task t(saga::task::Done);                                         \
      TR1::shared_ptr<impl> i(new impl(par1, par2, par3, par4));              \
      i->create_impl_sync(t.get_result<ns>());                                \
      return t;                                                               \
  }                                                                           \
  saga::task ns::createpriv (p1 par1, p2 par2, p3 par3, p4 par4,              \
      saga::task_base::Async)                                                 \
  {   return saga::detail::run(                                               \
      impl::create_impl_async(par1, par2, par3, par4)); }                     \
                                                                              \
  saga::task ns::createpriv (p1 par1, p2 par2, p3 par3, p4 par4,              \
      saga::task_base::Task)                                                  \
  {   return impl::create_impl_async(par1, par2, par3, par4); }               \
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define SAGA_CALL_CONST_IMP_4_EX(ns, name, name_impl, p1, p2, p3, p4)         \
  saga::task ns::name ## priv (p1 par1, p2 par2, p3 par3, p4 par4,            \
                               saga::task_base::Sync) const                   \
  { SAGA_OBJECT_CHECK(); return get_impl()->name_impl(par1, par2, par3, par4, true); } \
                                                                              \
  saga::task ns::name ## priv (p1 par1, p2 par2, p3 par3, p4 par4,            \
                               saga::task_base::Async) const                  \
  { SAGA_OBJECT_CHECK(); return (saga::detail::run(get_impl()->name_impl(par1,\
                                            par2, par3, par4))); }            \
                                                                              \
  saga::task ns::name ## priv (p1 par1, p2 par2, p3 par3, p4 par4,            \
                               saga::task_base::Task) const                   \
  { SAGA_OBJECT_CHECK(); return get_impl()->name_impl(par1, par2, par3, par4); } \
//-----------------------------------------------------------------------------
#define SAGA_CALL_CONST_IMP_4(ns, name, p1, p2, p3, p4)                       \
    SAGA_CALL_CONST_IMP_4_EX(ns, name, name, p1, p2, p3, p4)                  \
//-----------------------------------------------------------------------------


///////////////////////////////////////////////////////////////////////////////
//
// 5 parameters
//
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
#define SAGA_CALL_IMP_5_EX(ns, name, name_impl, p1, p2, p3, p4, p5)           \
  saga::task ns::name ## priv (p1 par1, p2 par2, p3 par3, p4 par4,            \
                               p5 par5, saga::task_base::Sync)                \
  { SAGA_OBJECT_CHECK(); return get_impl()->name_impl(par1, par2, par3, par4, \
                                               par5, true); }                 \
                                                                              \
  saga::task ns::name ## priv (p1 par1, p2 par2, p3 par3, p4 par4,            \
                               p5 par5, saga::task_base::Async)               \
  { SAGA_OBJECT_CHECK(); return (saga::detail::run(get_impl()->name_impl(par1,\
                                            par2, par3, par4, par5))); }      \
                                                                              \
  saga::task ns::name ## priv (p1 par1, p2 par2, p3 par3, p4 par4,            \
                               p5 par5, saga::task_base::Task)                \
  { SAGA_OBJECT_CHECK(); return get_impl()->name_impl(par1, par2, par3, par4, \
                                                      par5); }                \
//-----------------------------------------------------------------------------
#define SAGA_CALL_IMP_5(ns, name, p1, p2, p3, p4, p5)                         \
    SAGA_CALL_IMP_5_EX(ns, name, name, p1, p2, p3, p4, p5)                    \
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define SAGA_CALL_CREATE_IMP_5(ns, impl, p1, p2, p3, p4, p5)                  \
  saga::task ns::createpriv (p1 par1, p2 par2, p3 par3, p4 par4, p5 par5,     \
      saga::task_base::Sync)                                                  \
  {                                                                           \
      saga::task t(saga::task::Done);                                         \
      TR1::shared_ptr<impl> i(new impl(par1, par2, par3, par4, par5));        \
      i->create_impl_sync(t.get_result<ns>());                                \
      return t;                                                               \
  }                                                                           \
  saga::task ns::createpriv (p1 par1, p2 par2, p3 par3, p4 par4, p5 par5,     \
      saga::task_base::Async)                                                 \
  {   return saga::detail::run(                                               \
      impl::create_impl_async(par1, par2, par3, par4, par5)); }               \
                                                                              \
  saga::task ns::createpriv (p1 par1, p2 par2, p3 par3, p4 par4, p5 par5,     \
      saga::task_base::Task)                                                  \
  {   return impl::create_impl_async(par1, par2, par3, par4, par5); }         \
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define SAGA_CALL_CONST_IMP_5_EX(ns, name, name_impl, p1, p2, p3, p4, p5)     \
  saga::task ns::name ## priv (p1 par1, p2 par2, p3 par3, p4 par4,            \
                               p5 par5, saga::task_base::Sync) const          \
  { SAGA_OBJECT_CHECK(); return get_impl()->name_impl(par1, par2, par3, par4, \
                                               par5, true); }                 \
                                                                              \
  saga::task ns::name ## priv (p1 par1, p2 par2, p3 par3, p4 par4,            \
                               p5 par5, saga::task_base::Async) const         \
  { SAGA_OBJECT_CHECK(); return (saga::detail::run(get_impl()->name_impl(par1,\
                                            par2, par3, par4, par5))); }      \
                                                                              \
  saga::task ns::name ## priv (p1 par1, p2 par2, p3 par3, p4 par4,            \
                               p5 par5, saga::task_base::Task) const          \
  { SAGA_OBJECT_CHECK(); return get_impl()->name_impl(par1, par2, par3, par4, \
                                                      par5); }                \
//-----------------------------------------------------------------------------
#define SAGA_CALL_CONST_IMP_5(ns, name, p1, p2, p3, p4, p5)                   \
    SAGA_CALL_CONST_IMP_5_EX(ns, name, name, p1, p2, p3, p4, p5)              \
//-----------------------------------------------------------------------------

/// @endcond

#endif // SAGA_DETAIL_CALL_V1_HPP

