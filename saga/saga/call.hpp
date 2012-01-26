//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2007 Ole Weidner (oweidner@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_CALL_HPP
#define SAGA_CALL_HPP

#include <saga/saga/task.hpp>

/// @cond

///////////////////////////////////////////////////////////////////////////////
//
// FIXME: document usage extensively
//

///////////////////////////////////////////////////////////////////////////////
//
// 0 parameters
//
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
#define SAGA_CALL_PUB_0_DEF_0(name)                                           \
      template <typename Tag>                                                 \
      saga::task name()                                                       \
      { return name ## priv(Tag()); }                                         \
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define SAGA_CALL_CONST_PUB_0_DEF_0(name)                                     \
      template <typename Tag>                                                 \
      saga::task name() const                                                 \
      { return name ## priv(Tag()); }                                         \
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define SAGA_CALL_CREATE_0_DEF_0()                                            \
      template <typename Tag>                                                 \
      static saga::task create()                                              \
      { return createpriv(Tag()); }                                           \
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define SAGA_CALL_PRIV_0(name)                                                \
      saga::task name ## priv (saga::task_base::Sync);                        \
      saga::task name ## priv (saga::task_base::Async);                       \
      saga::task name ## priv (saga::task_base::Task);                        \
//-----------------------------------------------------------------------------
#define SAGA_CALL_CONST_PRIV_0(name)                                          \
      saga::task name ## priv (saga::task_base::Sync) const;                  \
      saga::task name ## priv (saga::task_base::Async) const;                 \
      saga::task name ## priv (saga::task_base::Task) const;                  \
//-----------------------------------------------------------------------------
#define SAGA_CALL_CREATE_PRIV_0()                                             \
      static saga::task createpriv (saga::task_base::Sync);                   \
      static saga::task createpriv (saga::task_base::Async);                  \
      static saga::task createpriv (saga::task_base::Task);                   \
//-----------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////
//
// 1 parameter
//
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
#define SAGA_CALL_PUB_1_DEF_0(name, p1)                                       \
      template <typename Tag>                                                 \
      saga::task name (p1 par1)                                               \
      { return  (name ## priv (par1, Tag())); }                               \
//-----------------------------------------------------------------------------
#define SAGA_CALL_PUB_1_DEF_1(name, p1, d1)                                   \
      template <typename Tag>                                                 \
      saga::task name (p1 par1 = d1)                                          \
      { return  (name ## priv (par1, Tag())); }                               \
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define SAGA_CALL_CONST_PUB_1_DEF_0(name, p1)                                 \
      template <typename Tag>                                                 \
      saga::task name (p1 par1) const                                         \
      { return  (name ## priv (par1, Tag())); }                               \
//-----------------------------------------------------------------------------
#define SAGA_CALL_CONST_PUB_1_DEF_1(name, p1, d1)                             \
      template <typename Tag>                                                 \
      saga::task name (p1 par1 = d1) const                                    \
      { return  (name ## priv (par1, Tag())); }                               \
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define SAGA_CALL_CREATE_1_DEF_0(p1)                                          \
      template <typename Tag>                                                 \
      static saga::task create (p1 par1)                                      \
      { return  (createpriv (par1, Tag())); }                                 \
//-----------------------------------------------------------------------------
#define SAGA_CALL_CREATE_1_DEF_1(p1, d1)                                      \
      template <typename Tag>                                                 \
      static saga::task create (p1 par1 = d1)                                 \
      { return  (createpriv (par1, Tag())); }                                 \
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define SAGA_CALL_PRIV_1(name, p1)                                            \
      saga::task name ## priv (p1 par1, saga::task_base::Sync);               \
      saga::task name ## priv (p1 par1, saga::task_base::Async);              \
      saga::task name ## priv (p1 par1, saga::task_base::Task);               \
//-----------------------------------------------------------------------------
#define SAGA_CALL_CONST_PRIV_1(name, p1)                                      \
      saga::task name ## priv (p1 par1, saga::task_base::Sync) const;         \
      saga::task name ## priv (p1 par1, saga::task_base::Async) const;        \
      saga::task name ## priv (p1 par1, saga::task_base::Task) const;         \
//-----------------------------------------------------------------------------
#define SAGA_CALL_CREATE_PRIV_1(p1)                                           \
      static saga::task createpriv (p1 par1, saga::task_base::Sync);          \
      static saga::task createpriv (p1 par1, saga::task_base::Async);         \
      static saga::task createpriv (p1 par1, saga::task_base::Task);          \
//-----------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////
//
// 2 parameters
//
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
#define SAGA_CALL_PUB_2_DEF_0(name, p1, p2)                                   \
      template <typename Tag>                                                 \
      saga::task name (p1 par1, p2 par2)                                      \
      { return  (name ## priv (par1, par2, Tag())); }                         \
//-----------------------------------------------------------------------------
#define SAGA_CALL_PUB_2_DEF_1(name, p1, p2, d2)                               \
      template <typename Tag>                                                 \
      saga::task name (p1 par1, p2 par2 = d2)                                 \
      { return  (name ## priv (par1, par2, Tag())); }                         \
//-----------------------------------------------------------------------------
#define SAGA_CALL_PUB_2_DEF_2(name, p1, d1, p2, d2)                           \
      template <typename Tag>                                                 \
      saga::task name (p1 par1 = d1, p2 par2 = d2)                            \
      { return  (name ## priv (par1, par2, Tag())); }                         \
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define SAGA_CALL_CONST_PUB_2_DEF_0(name, p1, p2)                             \
      template <typename Tag>                                                 \
      saga::task name (p1 par1, p2 par2) const                                \
      { return  (name ## priv (par1, par2, Tag())); }                         \
//-----------------------------------------------------------------------------
#define SAGA_CALL_CONST_PUB_2_DEF_1(name, p1, p2, d2)                         \
      template <typename Tag>                                                 \
      saga::task name (p1 par1, p2 par2 = d2) const                           \
      { return  (name ## priv (par1, par2, Tag())); }                         \
//-----------------------------------------------------------------------------
#define SAGA_CALL_CONST_PUB_2_DEF_2(name, p1, d1, p2, d2)                     \
      template <typename Tag>                                                 \
      saga::task name (p1 par1 = d1, p2 par2 = d2) const                      \
      { return  (name ## priv (par1, par2, Tag())); }                         \
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define SAGA_CALL_CREATE_2_DEF_0(p1, p2)                                      \
      template <typename Tag>                                                 \
      static saga::task create (p1 par1, p2 par2)                             \
      { return  (createpriv (par1, par2, Tag())); }                           \
//-----------------------------------------------------------------------------
#define SAGA_CALL_CREATE_2_DEF_1(p1, p2, d2)                                  \
      template <typename Tag>                                                 \
      static saga::task create (p1 par1, p2 par2 = d2)                        \
      { return  (createpriv (par1, par2, Tag())); }                           \
//-----------------------------------------------------------------------------
#define SAGA_CALL_CREATE_2_DEF_2(p1, d1, p2, d2)                              \
      template <typename Tag>                                                 \
      static saga::task create (p1 par1 = d1, p2 par2 = d2)                   \
      { return  (createpriv (par1, par2, Tag())); }                           \
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define SAGA_CALL_PRIV_2(name, p1, p2)                                        \
      saga::task name ## priv (p1, p2, saga::task_base::Sync);                \
      saga::task name ## priv (p1, p2, saga::task_base::Async);               \
      saga::task name ## priv (p1, p2, saga::task_base::Task);                \
//-----------------------------------------------------------------------------
#define SAGA_CALL_CONST_PRIV_2(name, p1, p2)                                  \
      saga::task name ## priv (p1, p2, saga::task_base::Sync)  const;         \
      saga::task name ## priv (p1, p2, saga::task_base::Async) const;         \
      saga::task name ## priv (p1, p2, saga::task_base::Task)  const;         \
//-----------------------------------------------------------------------------
#define SAGA_CALL_CREATE_PRIV_2(p1, p2)                                       \
      static saga::task createpriv (p1, p2, saga::task_base::Sync);           \
      static saga::task createpriv (p1, p2, saga::task_base::Async);          \
      static saga::task createpriv (p1, p2, saga::task_base::Task);           \
//-----------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////
//
// 3 parameters
//
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
#define SAGA_CALL_PUB_3_DEF_0(name, p1, p2, p3)                               \
      template <typename Tag>                                                 \
      saga::task name (p1 par1, p2 par2, p3 par3)                             \
      { return  (name ## priv (par1, par2, par3, Tag())); }                   \
//-----------------------------------------------------------------------------
#define SAGA_CALL_PUB_3_DEF_1(name, p1, p2, p3, d3)                           \
      template <typename Tag>                                                 \
      saga::task name (p1 par1, p2 par2, p3 par3 = d3)                        \
      { return  (name ## priv (par1, par2, par3, Tag())); }                   \
//-----------------------------------------------------------------------------
#define SAGA_CALL_PUB_3_DEF_2(name, p1, p2, d2, p3, d3)                       \
      template <typename Tag>                                                 \
      saga::task name (p1 par1, p2 par2 = d2, p3 par3 = d3)                   \
      { return  (name ## priv (par1, par2, par3, Tag())); }                   \
//-----------------------------------------------------------------------------
#define SAGA_CALL_PUB_3_DEF_3(name, p1, d1, p2, d2, p3, d3)                   \
      template <typename Tag>                                                 \
      saga::task name (p1 par1 = d1, p2 par2 = d2, p3 par3 = d3)              \
      { return  (name ## priv (par1, par2, par3, Tag())); }                   \
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define SAGA_CALL_CONST_PUB_3_DEF_0(name, p1, p2, p3)                         \
      template <typename Tag>                                                 \
      saga::task name (p1 par1, p2 par2, p3 par3)                             \
      { return  (name ## priv (par1, par2, par3, Tag())); }                   \
//-----------------------------------------------------------------------------
#define SAGA_CALL_CONST_PUB_3_DEF_1(name, p1, p2, p3, d3)                     \
      template <typename Tag>                                                 \
      saga::task name (p1 par1, p2 par2, p3 par3 = d3) const                  \
      { return  (name ## priv (par1, par2, par3, Tag())); }                   \
//-----------------------------------------------------------------------------
#define SAGA_CALL_CONST_PUB_3_DEF_2(name, p1, p2, d2, p3, d3)                 \
      template <typename Tag>                                                 \
      saga::task name (p1 par1, p2 par2 = d2, p3 par3 = d3) const             \
      { return  (name ## priv (par1, par2, par3, Tag())); }                   \
//-----------------------------------------------------------------------------
#define SAGA_CALL_CONST_PUB_3_DEF_3(name, p1, d1, p2, d2, p3, d3)             \
      template <typename Tag>                                                 \
      saga::task name (p1 par1 = d1, p2 par2 = d2, p3 par3 = d3) const        \
      { return  (name ## priv (par1, par2, par3, Tag())); }                   \
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define SAGA_CALL_CREATE_3_DEF_0(p1, p2, p3)                                  \
      template <typename Tag>                                                 \
      static saga::task create (p1 par1, p2 par2, p3 par3)                    \
      { return  (createpriv (par1, par2, par3, Tag())); }                     \
//-----------------------------------------------------------------------------
#define SAGA_CALL_CREATE_3_DEF_1(p1, p2, p3, d3)                              \
      template <typename Tag>                                                 \
      static saga::task create (p1 par1, p2 par2, p3 par3 = d3)               \
      { return  (createpriv (par1, par2, par3, Tag())); }                     \
//-----------------------------------------------------------------------------
#define SAGA_CALL_CREATE_3_DEF_2(p1, p2, d2, p3, d3)                          \
      template <typename Tag>                                                 \
      static saga::task create (p1 par1, p2 par2 = d2, p3 par3 = d3)          \
      { return  (createpriv (par1, par2, par3, Tag())); }                     \
//-----------------------------------------------------------------------------
#define SAGA_CALL_CREATE_3_DEF_3(p1, d1, p2, d2, p3, d3)                      \
      template <typename Tag>                                                 \
      static saga::task create (p1 par1 = d1, p2 par2 = d2, p3 par3 = d3)     \
      { return  (createpriv (par1, par2, par3, Tag())); }                     \
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define SAGA_CALL_PRIV_3(name, p1, p2, p3)                                    \
      saga::task name ## priv (p1, p2, p3, saga::task_base::Sync);            \
      saga::task name ## priv (p1, p2, p3, saga::task_base::Async);           \
      saga::task name ## priv (p1, p2, p3, saga::task_base::Task);            \
//-----------------------------------------------------------------------------
#define SAGA_CALL_CONST_PRIV_3(name, p1, p2, p3)                              \
      saga::task name ## priv (p1, p2, p3, saga::task_base::Sync) const;      \
      saga::task name ## priv (p1, p2, p3, saga::task_base::Async) const;     \
      saga::task name ## priv (p1, p2, p3, saga::task_base::Task) const;      \
//-----------------------------------------------------------------------------
#define SAGA_CALL_CREATE_PRIV_3(p1, p2, p3)                                   \
      static saga::task createpriv (p1, p2, p3, saga::task_base::Sync);       \
      static saga::task createpriv (p1, p2, p3, saga::task_base::Async);      \
      static saga::task createpriv (p1, p2, p3, saga::task_base::Task);       \
//-----------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////
//
// 4 parameters
//
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
#define SAGA_CALL_PUB_4_DEF_0(name, p1, p2, p3, p4)                           \
      template <typename Tag>                                                 \
      saga::task name (p1 par1, p2 par2, p3 par3, p4 par4)                    \
      { return  (name ## priv (par1, par2, par3, par4, Tag())); }             \
//-----------------------------------------------------------------------------
#define SAGA_CALL_PUB_4_DEF_1(name, p1, p2, p3, p4, d4)                       \
      template <typename Tag>                                                 \
      saga::task name (p1 par1, p2 par2, p3 par3, p4 par4 = d4)               \
      { return  (name ## priv (par1, par2, par3, par4, Tag())); }             \
//-----------------------------------------------------------------------------
#define SAGA_CALL_PUB_4_DEF_2(name, p1, p2, p3, d3, p4, d4)                   \
      template <typename Tag>                                                 \
      saga::task name (p1 par1, p2 par2, p3 par3 = d3, p4 par4 = d4)          \
      { return  (name ## priv (par1, par2, par3, par4, Tag())); }             \
//-----------------------------------------------------------------------------
#define SAGA_CALL_PUB_4_DEF_3(name, p1, p2, d2, p3, d3, p4, d4)               \
      template <typename Tag>                                                 \
      saga::task name (p1 par1, p2 par2 = d2, p3 par3 = d3,                   \
                       p4 par4 = d4)                                          \
      { return  (name ## priv (par1, par2, par3, par4, Tag())); }             \
//-----------------------------------------------------------------------------
#define SAGA_CALL_PUB_4_DEF_4(name, p1, d1, p2, d2, p3, d3, p4, d4)           \
      template <typename Tag>                                                 \
      saga::task name (p1 par1 = d1, p2 par2 = d2, p3 par3 = d3,              \
                       p4 par4 = d4)                                          \
      { return  (name ## priv (par1, par2, par3, par4, Tag())); }             \
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define SAGA_CALL_CONST_PUB_4_DEF_0(name, p1, p2, p3, p4)                     \
      template <typename Tag>                                                 \
      saga::task name (p1 par1, p2 par2, p3 par3, p4 par4) const              \
      { return  (name ## priv (par1, par2, par3, par4, Tag())); }             \
//-----------------------------------------------------------------------------
#define SAGA_CALL_CONST_PUB_4_DEF_1(name, p1, p2, p3, p4, d4)                 \
      template <typename Tag>                                                 \
      saga::task name (p1 par1, p2 par2, p3 par3, p4 par4 = d4) const         \
      { return  (name ## priv (par1, par2, par3, par4, Tag())); }             \
//-----------------------------------------------------------------------------
#define SAGA_CALL_CONST_PUB_4_DEF_2(name, p1, p2, p3, d3, p4, d4)             \
      template <typename Tag>                                                 \
      saga::task name (p1 par1, p2 par2, p3 par3 = d3, p4 par4 = d4) const    \
      { return  (name ## priv (par1, par2, par3, par4, Tag())); }             \
//-----------------------------------------------------------------------------
#define SAGA_CALL_CONST_PUB_4_DEF_3(name, p1, p2, d2, p3, d3, p4, d4)         \
      template <typename Tag>                                                 \
      saga::task name (p1 par1, p2 par2 = d2, p3 par3 = d3,                   \
                       p4 par4 = d4) const                                    \
      { return  (name ## priv (par1, par2, par3, par4, Tag())); }             \
//-----------------------------------------------------------------------------
#define SAGA_CALL_CONST_PUB_4_DEF_4(name, p1, d1, p2, d2, p3, d3, p4, d4)     \
      template <typename Tag>                                                 \
      saga::task name (p1 par1 = d1, p2 par2 = d2, p3 par3 = d3,              \
                       p4 par4 = d4) const                                    \
      { return  (name ## priv (par1, par2, par3, par4, Tag())); }             \
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define SAGA_CALL_CREATE_4_DEF_0(p1, p2, p3, p4)                              \
      template <typename Tag>                                                 \
      static saga::task create (p1 par1, p2 par2, p3 par3, p4 par4)           \
      { return  (createpriv (par1, par2, par3, par4, Tag())); }               \
//-----------------------------------------------------------------------------
#define SAGA_CALL_CREATE_4_DEF_1(p1, p2, p3, p4, d4)                          \
      template <typename Tag>                                                 \
      static saga::task create (p1 par1, p2 par2, p3 par3, p4 par4 = d4)      \
      { return  (createpriv (par1, par2, par3, par4, Tag())); }               \
//-----------------------------------------------------------------------------
#define SAGA_CALL_CREATE_4_DEF_2(p1, p2, p3, d3, p4, d4)                      \
      template <typename Tag>                                                 \
      static saga::task create (p1 par1, p2 par2, p3 par3 = d3, p4 par4 = d4) \
      { return  (createpriv (par1, par2, par3, par4, Tag())); }               \
//-----------------------------------------------------------------------------
#define SAGA_CALL_CREATE_4_DEF_3(p1, p2, d2, p3, d3, p4, d4)                  \
      template <typename Tag>                                                 \
      static saga::task create (p1 par1, p2 par2 = d2, p3 par3 = d3,          \
                       p4 par4 = d4)                                          \
      { return  (createpriv (par1, par2, par3, par4, Tag())); }               \
//-----------------------------------------------------------------------------
#define SAGA_CALL_CREATE_4_DEF_4(p1, d1, p2, d2, p3, d3, p4, d4)              \
      template <typename Tag>                                                 \
      static saga::task create (p1 par1 = d1, p2 par2 = d2, p3 par3 = d3,     \
                       p4 par4 = d4)                                          \
      { return  (createpriv (par1, par2, par3, par4, Tag())); }               \
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define SAGA_CALL_PRIV_4(name, p1, p2, p3, p4)                                \
      saga::task name ## priv (p1, p2, p3, p4, saga::task_base::Sync);        \
      saga::task name ## priv (p1, p2, p3, p4, saga::task_base::Async);       \
      saga::task name ## priv (p1, p2, p3, p4, saga::task_base::Task);        \
//-----------------------------------------------------------------------------
#define SAGA_CALL_CONST_PRIV_4(name, p1, p2, p3, p4)                          \
      saga::task name ## priv (p1, p2, p3, p4, saga::task_base::Sync) const;  \
      saga::task name ## priv (p1, p2, p3, p4, saga::task_base::Async) const; \
      saga::task name ## priv (p1, p2, p3, p4, saga::task_base::Task) const;  \
//-----------------------------------------------------------------------------
#define SAGA_CALL_CREATE_PRIV_4(p1, p2, p3, p4)                               \
      static saga::task createpriv (p1, p2, p3, p4, saga::task_base::Sync);   \
      static saga::task createpriv (p1, p2, p3, p4, saga::task_base::Async);  \
      static saga::task createpriv (p1, p2, p3, p4, saga::task_base::Task);   \
//-----------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////
//
// 5 parameters
//
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
#define SAGA_CALL_PUB_5_DEF_0(name, p1, p2, p3, p4, p5)                       \
      template <typename Tag>                                                 \
      saga::task name (p1 par1, p2 par2, p3 par3, p4 par4, p5 par5)           \
      { return  (name ## priv (par1, par2, par3, par4, par5, Tag())); }       \
//-----------------------------------------------------------------------------
#define SAGA_CALL_PUB_5_DEF_1(name, p1, p2, p3, p4, p5, d5)                   \
      template <typename Tag>                                                 \
      saga::task name (p1 par1, p2 par2, p3 par3, p4 par4,                    \
                       p5 par5 = d5)                                          \
      { return  (name ## priv (par1, par2, par3, par4, par5, Tag())); }       \
//-----------------------------------------------------------------------------
#define SAGA_CALL_PUB_5_DEF_2(name, p1, p2, p3, p4, d4, p5, d5)               \
      template <typename Tag>                                                 \
      saga::task name (p1 par1, p2 par2, p3 par3, p4 par4 = d4,               \
                       p5 par5 = d5)                                          \
      { return  (name ## priv (par1, par2, par3, par4, par5, Tag())); }       \
//-----------------------------------------------------------------------------
#define SAGA_CALL_PUB_5_DEF_3(name, p1, p2, p3, d3, p4, d4, p5, d5)           \
      template <typename Tag>                                                 \
      saga::task name (p1 par1, p2 par2, p3 par3 = d3,                        \
                       p4 par4 = d4, p5 par5 = d5)                            \
      { return  (name ## priv (par1, par2, par3, par4, par5, Tag())); }       \
//-----------------------------------------------------------------------------
#define SAGA_CALL_PUB_5_DEF_4(name, p1, p2, d2, p3, d3, p4, d4, p5, d5)       \
      template <typename Tag>                                                 \
      saga::task name (p1 par1, p2 par2 = d2, p3 par3 = d3,                   \
                       p4 par4 = d4, p5 par5 = d5)                            \
      { return  (name ## priv (par1, par2, par3, par4, par5, Tag())); }       \
//-----------------------------------------------------------------------------
#define SAGA_CALL_PUB_5_DEF_5(name, p1, d1, p2, d2, p3, d3, p4, d4, p5, d5)   \
      template <typename Tag>                                                 \
      saga::task name (p1 par1 = d1, p2 par2 = d2, p3 par3 = d3,              \
                       p4 par4 = d4, p5 par5 = d5)                            \
      { return  (name ## priv (par1, par2, par3, par4, par5, Tag())); }       \
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define SAGA_CALL_CONST_PUB_5_DEF_0(name, p1, p2, p3, p4, p5)                 \
      template <typename Tag>                                                 \
      saga::task name (p1 par1, p2 par2, p3 par3, p5 par5) const              \
      { return  (name ## priv (par1, par2, par3, par4, par5, Tag())); }       \
//-----------------------------------------------------------------------------
#define SAGA_CALL_CONST_PUB_5_DEF_1(name, p1, p2, p3, p4, p5, d5)             \
      template <typename Tag>                                                 \
      saga::task name (p1 par1, p2 par2, p3 par3, p4 par4,                    \
                       p5 par5 = d5) const                                    \
      { return  (name ## priv (par1, par2, par3, par4, par5, Tag())); }       \
//-----------------------------------------------------------------------------
#define SAGA_CALL_CONST_PUB_5_DEF_2(name, p1, p2, p3, p4, d4, p5, d5)         \
      template <typename Tag>                                                 \
      saga::task name (p1 par1, p2 par2, p3 par3, p4 par4 = d4,               \
                       p5 par5 = d5) const                                    \
      { return  (name ## priv (par1, par2, par3, par4, par5, Tag())); }       \
//-----------------------------------------------------------------------------
#define SAGA_CALL_CONST_PUB_5_DEF_3(name, p1, p2, p3, d3, p4, d4, p5, d5)     \
      template <typename Tag>                                                 \
      saga::task name (p1 par1, p2 par2, p3 par3 = d3, p4 par4 = d4,          \
                       p5 par5 = d5) const                                    \
      { return  (name ## priv (par1, par2, par3, par4, par5, Tag())); }       \
//-----------------------------------------------------------------------------
#define SAGA_CALL_CONST_PUB_5_DEF_4(name, p1, p2, d2, p3, d3, p4, d4, p5, d5) \
      template <typename Tag>                                                 \
      saga::task name (p1 par1, p2 par2 = d2, p3 par3 = d3,                   \
                       p4 par4 = d4, p5 par5 = d5) const                      \
      { return  (name ## priv (par1, par2, par3, par4, par5, Tag())); }       \
//-----------------------------------------------------------------------------
#define SAGA_CALL_CONST_PUB_5_DEF_5(name, p1, d1, p2, d2, p3, d3, p4, d4,     \
          p5, d5)                                                             \
      template <typename Tag>                                                 \
      saga::task name (p1 par1 = d1, p2 par2 = d2, p3 par3 = d3,              \
                       p4 par4 = d4, p5 par5 = d5) const                      \
      { return  (name ## priv (par1, par2, par3, par4, par5, Tag())); }       \
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define SAGA_CALL_CREATE_5_DEF_0(p1, p2, p3, p4, p5)                          \
      template <typename Tag>                                                 \
      static saga::task create (p1 par1, p2 par2, p3 par3, p4 par4, p5 par5)  \
      { return  (createpriv (par1, par2, par3, par4, par5, Tag())); }         \
//-----------------------------------------------------------------------------
#define SAGA_CALL_CREATE_5_DEF_1(p1, p2, p3, p4, p5, d5)                      \
      template <typename Tag>                                                 \
      static saga::task create (p1 par1, p2 par2, p3 par3, p4 par4,           \
                       p5 par5 = d5)                                          \
      { return  (createpriv (par1, par2, par3, par4, par5, Tag())); }         \
//-----------------------------------------------------------------------------
#define SAGA_CALL_CREATE_5_DEF_2(p1, p2, p3, p4, d4, p5, d5)                  \
      template <typename Tag>                                                 \
      static saga::task create (p1 par1, p2 par2, p3 par3, p4 par4 = d4,      \
                       p5 par5 = d5)                                          \
      { return  (createpriv (par1, par2, par3, par4, par5, Tag())); }         \
//-----------------------------------------------------------------------------
#define SAGA_CALL_CREATE_5_DEF_3(p1, p2, p3, d3, p4, d4, p5, d5)              \
      template <typename Tag>                                                 \
      static saga::task create (p1 par1, p2 par2, p3 par3 = d3,               \
                       p4 par4 = d4, p5 par5 = d5)                            \
      { return  (createpriv (par1, par2, par3, par4, par5, Tag())); }         \
//-----------------------------------------------------------------------------
#define SAGA_CALL_CREATE_5_DEF_4(p1, p2, d2, p3, d3, p4, d4, p5, d5)          \
      template <typename Tag>                                                 \
      static saga::task create (p1 par1, p2 par2 = d2, p3 par3 = d3,          \
                       p4 par4 = d4, p5 par5 = d5)                            \
      { return  (createpriv (par1, par2, par3, par4, par5, Tag())); }         \
//-----------------------------------------------------------------------------
#define SAGA_CALL_CREATE_5_DEF_5(p1, d1, p2, d2, p3, d3, p4, d4, p5, d5)      \
      template <typename Tag>                                                 \
      static saga::task create (p1 par1 = d1, p2 par2 = d2, p3 par3 = d3,     \
                       p4 par4 = d4, p5 par5 = d5)                            \
      { return  (createpriv (par1, par2, par3, par4, par5, Tag())); }         \
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define SAGA_CALL_PRIV_5(name, p1, p2, p3, p4, p5)                            \
      saga::task name ## priv (p1, p2, p3, p4, p5, saga::task_base::Sync);    \
      saga::task name ## priv (p1, p2, p3, p4, p5, saga::task_base::Async);   \
      saga::task name ## priv (p1, p2, p3, p4, p5, saga::task_base::Task);    \
//-----------------------------------------------------------------------------
#define SAGA_CALL_CONST_PRIV_5(name, p1, p2, p3, p4, p5)                      \
      saga::task name ## priv (p1, p2, p3, p4, p5, saga::task_base::Sync) const; \
      saga::task name ## priv (p1, p2, p3, p4, p5, saga::task_base::Async) const; \
      saga::task name ## priv (p1, p2, p3, p4, p5, saga::task_base::Task) const; \
//-----------------------------------------------------------------------------
#define SAGA_CALL_CREATE_PRIV_5(p1, p2, p3, p4, p5)                           \
      static saga::task createpriv (p1, p2, p3, p4, p5, saga::task_base::Sync); \
      static saga::task createpriv (p1, p2, p3, p4, p5, saga::task_base::Async); \
      static saga::task createpriv (p1, p2, p3, p4, p5, saga::task_base::Task); \
//-----------------------------------------------------------------------------

/// @endcond

#endif // SAGA_CALL_HPP

