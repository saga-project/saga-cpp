//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_IMPL_CALL_HPP
#define SAGA_IMPL_CALL_HPP

// include some boost magic
#include <saga/saga/util.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl {

    ///////////////////////////////////////////////////////////////////////////
    template <typename T, bool IsRef>
    struct make_reference
    {
      typedef typename TR1::remove_reference<T>::type noref_type;
      typedef          TR1::reference_wrapper<noref_type> type;
      static type eval (noref_type & t)
      {
        return TR1::reference_wrapper <noref_type> (t);
      }
    };

    template <typename T>
    struct make_reference <T, false>
    {
      typedef T type;
      static type& eval (T& t)
      {
        return (t);
      }
    };

///////////////////////////////////////////////////////////////////////////////
}} // namespace saga::impl

///////////////////////////////////////////////////////////////////////////////
//
// 0 parameters
//
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
#define SAGA_CALL_CPI_DECL_VIRT_0(ns, rt, name)                               \
                                                                              \
      virtual void sync_ ## name (rt & ret)                                   \
      { SAGA_THROW("call_wrapper: sync " #ns "::" #name                       \
                            " is not implemented", saga::NotImplemented); }   \
                                                                              \
      virtual saga::task async_ ## name()                                     \
      { SAGA_THROW("call_wrapper: async " #ns "::" #name                      \
                            " is not implemented", saga::NotImplemented);     \
                   return saga::task(saga::task::Done); }                     \
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define SAGA_CALL_CPI_DECL_0(ns, rt, name)                                    \
                                                                              \
      void sync_ ## name (rt & ret)                                           \
      { SAGA_THROW("call_wrapper: sync " #ns "::" #name                       \
                            " is not implemented", saga::NotImplemented); }   \
                                                                              \
      saga::task async_ ## name()                                             \
      { SAGA_THROW("call_wrapper: async " #ns "::" #name                      \
                            " is not implemented", saga::NotImplemented);     \
                   return saga::task(saga::task::Done); }                     \
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define SAGA_CALL_IMPL_DECL_0(name)                                           \
      saga::task name (bool is_sync = false);                                 \
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define SAGA_CALL_IMPL_IMPL_0_EX(ns, cpi, name_impl, name)                    \
    saga::task ns::name_impl (bool is_sync)                                   \
    {                                                                         \
        return saga::impl::execute_sync_async(this,                           \
            #cpi, #name, #ns "::" #name, is_sync,                             \
            &cpi::sync_  ## name, &cpi::async_ ## name);                      \
    }                                                                         \
//-----------------------------------------------------------------------------
#define SAGA_CALL_IMPL_IMPL_0(ns, cpi, name)                                  \
    SAGA_CALL_IMPL_IMPL_0_EX(ns, cpi, name, name)                             \
//-----------------------------------------------------------------------------


///////////////////////////////////////////////////////////////////////////////
//
// 1 parameter
//
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
#define SAGA_CALL_CPI_DECL_VIRT_1(ns, rt, name, p1)                           \
                                                                              \
      virtual void sync_ ## name (rt & ret, p1 par1)                          \
      { SAGA_THROW("call_wrapper: sync " #ns "::" #name                       \
                            " is not implemented", saga::NotImplemented); }   \
                                                                              \
      virtual saga::task async_ ## name (p1 par1)                             \
      { SAGA_THROW("call_wrapper: async " #ns "::" #name                      \
                            " is not implemented", saga::NotImplemented);     \
                   return saga::task(saga::task::Done); }                     \
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define SAGA_CALL_CPI_DECL_1(ns, rt, name, p1)                                \
                                                                              \
      void sync_ ## name (rt & ret, p1 par1)                                  \
      { SAGA_THROW("call_wrapper: sync " #ns "::" #name                       \
                            " is not implemented", saga::NotImplemented); }   \
                                                                              \
      saga::task async_ ## name (p1 par1)                                     \
      { SAGA_THROW("call_wrapper: async " #ns "::" #name                      \
                            " is not implemented", saga::NotImplemented);     \
                   return saga::task(saga::task::Done); }                     \
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define SAGA_CALL_IMPL_DECL_1(name, p1)                                       \
      saga::task name (p1 par1, bool is_sync = false);                        \
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define SAGA_CALL_IMPL_IMPL_1_EX(ns, cpi, name_impl, name, p1)                \
    saga::task ns::name_impl (p1 par1, bool is_sync)                          \
    {                                                                         \
        return saga::impl::execute_sync_async(this,                           \
            #cpi, #name, #ns "::" #name, is_sync,                             \
            &cpi::sync_  ## name, &cpi::async_ ## name,                       \
            make_reference <p1, TR1::is_reference<p1 >::value>::eval(par1));  \
    }                                                                         \
//-----------------------------------------------------------------------------
#define SAGA_CALL_IMPL_IMPL_1(ns, cpi, name, p1)                              \
    SAGA_CALL_IMPL_IMPL_1_EX(ns, cpi, name, name, p1)                         \
//-----------------------------------------------------------------------------


///////////////////////////////////////////////////////////////////////////////
//
// 2 parameters
//
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
#define SAGA_CALL_CPI_DECL_VIRT_2(ns, rt, name, p1, p2)                       \
                                                                              \
      virtual void sync_ ## name (rt & ret, p1 par1, p2 par2)                 \
      { SAGA_THROW("call_wrapper: sync " #ns "::" #name                       \
                            " is not implemented", saga::NotImplemented); }   \
                                                                              \
      virtual saga::task async_ ## name (p1 par1, p2 par2)                    \
      { SAGA_THROW("call_wrapper: async " #ns "::" #name                      \
                            " is not implemented", saga::NotImplemented);     \
                   return saga::task(saga::task::Done); }                     \
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define SAGA_CALL_CPI_DECL_2(ns, rt, name, p1, p2)                            \
                                                                              \
      void sync_ ## name (rt & ret, p1 par1, p2 par2)                         \
      { SAGA_THROW("call_wrapper: sync " #ns "::" #name                       \
                            " is not implemented", saga::NotImplemented); }   \
                                                                              \
      saga::task async_ ## name (p1 par1, p2 par2)                            \
      { SAGA_THROW("call_wrapper: async " #ns "::" #name                      \
                            " is not implemented", saga::NotImplemented);     \
                   return saga::task(saga::task::Done); }                     \
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define SAGA_CALL_IMPL_DECL_2(name, p1, p2)                                   \
      saga::task name (p1 par1, p2 par2,                                      \
                       bool is_sync = false);                                 \
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define SAGA_CALL_IMPL_IMPL_2_EX(ns, cpi, name_impl, name, p1, p2)            \
    saga::task ns::name_impl (p1 par1, p2 par2, bool is_sync)                 \
    {                                                                         \
        return saga::impl::execute_sync_async(this,                           \
            #cpi, #name, #ns "::" #name, is_sync,                             \
            &cpi::sync_  ## name, &cpi::async_ ## name,                       \
            make_reference <p1, TR1::is_reference<p1 >::value>::eval(par1),   \
            make_reference <p2, TR1::is_reference<p2 >::value>::eval(par2));  \
    }                                                                         \
//-----------------------------------------------------------------------------
#define SAGA_CALL_IMPL_IMPL_2(ns, cpi, name, p1, p2)                          \
    SAGA_CALL_IMPL_IMPL_2_EX(ns, cpi, name, name, p1, p2)                     \
//-----------------------------------------------------------------------------



///////////////////////////////////////////////////////////////////////////////
//
// 3 parameters
//
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
#define SAGA_CALL_CPI_DECL_VIRT_3(ns, rt, name, p1, p2, p3)                   \
                                                                              \
      virtual void sync_ ## name (rt & ret, p1 par1, p2 par2, p3 par3)        \
      { SAGA_THROW("call_wrapper: sync " #ns "::" #name                       \
                            " is not implemented", saga::NotImplemented); }   \
                                                                              \
      virtual saga::task async_ ## name (p1 par1, p2 par2, p3 par3)           \
      { SAGA_THROW("call_wrapper: async " #ns "::" #name                      \
                            " is not implemented", saga::NotImplemented);     \
                   return saga::task(saga::task::Done); }                     \
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define SAGA_CALL_CPI_DECL_3(ns, rt, name, p1, p2, p3)                        \
                                                                              \
      void sync_ ## name (rt & ret, p1 par1, p2 par2, p3 par3)                \
      { SAGA_THROW("call_wrapper: sync " #ns "::" #name                       \
                            " is not implemented", saga::NotImplemented); }   \
                                                                              \
      saga::task async_ ## name (p1 par1, p2 par2, p3 par3)                   \
      { SAGA_THROW("call_wrapper: async " #ns "::" #name                      \
                            " is not implemented", saga::NotImplemented);     \
                   return saga::task(saga::task::Done); }                     \
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define SAGA_CALL_IMPL_DECL_3(name, p1, p2, p3)                               \
      saga::task name (p1 par1, p2 par2, p3 par3,                             \
                       bool is_sync = false);                                 \
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define SAGA_CALL_IMPL_IMPL_3_EX(ns, cpi, name_impl, name, p1, p2, p3)        \
    saga::task ns::name_impl (p1 par1, p2 par2, p3 par3, bool is_sync)        \
    {                                                                         \
        return saga::impl::execute_sync_async(this,                           \
            #cpi, #name, #ns "::" #name, is_sync,                             \
            &cpi::sync_  ## name, &cpi::async_ ## name,                       \
            make_reference <p1, TR1::is_reference<p1 >::value>::eval(par1),   \
            make_reference <p2, TR1::is_reference<p2 >::value>::eval(par2),   \
            make_reference <p3, TR1::is_reference<p3 >::value>::eval(par3));  \
    }                                                                         \
//-----------------------------------------------------------------------------
#define SAGA_CALL_IMPL_IMPL_3(ns, cpi, name, p1, p2, p3)                      \
    SAGA_CALL_IMPL_IMPL_3_EX(ns, cpi, name, name, p1, p2, p3)                 \
//-----------------------------------------------------------------------------


///////////////////////////////////////////////////////////////////////////////
//
// 4 parameters
//
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
#define SAGA_CALL_CPI_DECL_VIRT_4(ns, rt, name, p1, p2, p3, p4)               \
                                                                              \
    virtual void sync_ ## name (rt & ret, p1 par1, p2 par2, p3 par3, p4 par4) \
      { SAGA_THROW("call_wrapper: sync " #ns "::" #name                       \
                            " is not implemented", saga::NotImplemented); }   \
                                                                              \
    virtual saga::task async_ ## name (p1 par1, p2 par2, p3 par3, p4 par4)    \
      { SAGA_THROW("call_wrapper: async " #ns "::" #name                      \
                            " is not implemented", saga::NotImplemented);     \
                   return saga::task(saga::task::Done); }                     \
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define SAGA_CALL_CPI_DECL_4(ns, rt, name, p1, p2, p3, p4)                    \
                                                                              \
      void sync_ ## name (rt & ret, p1 par1, p2 par2, p3 par3, p4 par4)       \
      { SAGA_THROW("call_wrapper: sync " #ns "::" #name                       \
                            " is not implemented", saga::NotImplemented); }   \
                                                                              \
      saga::task async_ ## name (p1 par1, p2 par2, p3 par3, p4 par4)          \
      { SAGA_THROW("call_wrapper: async " #ns "::" #name                      \
                            " is not implemented", saga::NotImplemented);     \
                   return saga::task(saga::task::Done); }                     \
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define SAGA_CALL_IMPL_DECL_4(name, p1, p2, p3, p4)                           \
      saga::task name (p1 par1, p2 par2, p3 par3, p4 par4,                    \
                       bool is_sync = false);                                 \
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define SAGA_CALL_IMPL_IMPL_4_EX(ns, cpi, name_impl, name, p1, p2, p3, p4)    \
    saga::task ns::name_impl (p1 par1, p2 par2, p3 par3, p4 par4,             \
                         bool is_sync)                                        \
    {                                                                         \
        return saga::impl::execute_sync_async(this,                           \
            #cpi, #name, #ns "::" #name, is_sync,                             \
            &cpi::sync_  ## name, &cpi::async_ ## name,                       \
            make_reference <p1, TR1::is_reference<p1 >::value>::eval(par1),   \
            make_reference <p2, TR1::is_reference<p2 >::value>::eval(par2),   \
            make_reference <p3, TR1::is_reference<p3 >::value>::eval(par3),   \
            make_reference <p4, TR1::is_reference<p4 >::value>::eval(par4));  \
    }                                                                         \
//-----------------------------------------------------------------------------
#define SAGA_CALL_IMPL_IMPL_4(ns, cpi, name, p1, p2, p3, p4)                  \
    SAGA_CALL_IMPL_IMPL_4_EX(ns, cpi, name, name, p1, p2, p3, p4)             \
//-----------------------------------------------------------------------------


///////////////////////////////////////////////////////////////////////////////
//
// 5 parameters
//
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
#define SAGA_CALL_CPI_DECL_VIRT_5(ns, rt, name, p1, p2, p3, p4, p5)           \
                                                                              \
    virtual void sync_ ## name (rt & ret, p1 par1, p2 par2, p3 par3, p4 par4, \
                                p5 par5)                                      \
      { SAGA_THROW("call_wrapper: sync " #ns "::" #name                       \
                            " is not implemented", saga::NotImplemented); }   \
                                                                              \
    virtual saga::task async_ ## name (p1 par1, p2 par2, p3 par3,             \
                                       p4 par4, p5 par5)                      \
      { SAGA_THROW("call_wrapper: async " #ns "::" #name                      \
                            " is not implemented", saga::NotImplemented);     \
                   return saga::task(saga::task::Done); }                     \
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define SAGA_CALL_CPI_DECL_5(ns, rt, name, p1, p2, p3, p4, p5)                \
                                                                              \
      void sync_ ## name (rt & ret, p1 par1, p2 par2, p3 par3, p4 par4,       \
                          p5 par5)                                            \
      { SAGA_THROW("call_wrapper: sync " #ns "::" #name                       \
                            " is not implemented", saga::NotImplemented); }   \
                                                                              \
      saga::task async_ ## name (p1 par1, p2 par2, p3 par3, p4 par4,          \
                                 p5 par5)                                     \
      { SAGA_THROW("call_wrapper: async " #ns "::" #name                      \
                            " is not implemented", saga::NotImplemented);     \
                   return saga::task(saga::task::Done); }                     \
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define SAGA_CALL_IMPL_DECL_5(name, p1, p2, p3, p4, p5)                       \
      saga::task name (p1 par1, p2 par2, p3 par3, p4 par4, p5 par5,           \
                       bool is_sync = false);                                 \
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define SAGA_CALL_IMPL_IMPL_5_EX(ns, cpi, name_impl, name, p1, p2, p3, p4, p5)\
    saga::task ns::name_impl (p1 par1, p2 par2, p3 par3, p4 par4,             \
                         p5 par5, bool is_sync)                               \
    {                                                                         \
        return saga::impl::execute_sync_async(this,                           \
            #cpi, #name, #ns "::" #name, is_sync,                             \
            &cpi::sync_  ## name, &cpi::async_ ## name,                       \
            make_reference <p1, TR1::is_reference<p1 >::value>::eval(par1),   \
            make_reference <p2, TR1::is_reference<p2 >::value>::eval(par2),   \
            make_reference <p3, TR1::is_reference<p3 >::value>::eval(par3),   \
            make_reference <p4, TR1::is_reference<p4 >::value>::eval(par4),   \
            make_reference <p5, TR1::is_reference<p5 >::value>::eval(par5));  \
    }                                                                         \
//-----------------------------------------------------------------------------
#define SAGA_CALL_IMPL_IMPL_5(ns, cpi, name, p1, p2, p3, p4, p5)              \
    SAGA_CALL_IMPL_IMPL_5_EX(ns, cpi, name, name, p1, p2, p3, p4, p5)         \
//-----------------------------------------------------------------------------

#endif // SAGA_IMPL_CALL_HPP
