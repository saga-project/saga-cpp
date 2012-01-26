//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_IMPL_TASK_FWD_HPP
#define SAGA_IMPL_TASK_FWD_HPP

#include <boost/version.hpp>
#if BOOST_VERSION >= 103800
#include <boost/spirit/include/phoenix1_tuples.hpp>
#else
#include <boost/spirit/phoenix/tuples.hpp>
#endif

namespace saga { namespace impl
{
    /**
     * Generic task class, for encapsulating asynchronous function calls.
     *
     * This class is not part of the saga facade, but of the implementation.
     * Function calls are stored here and executed within a thread.
     *
     * The stored function pointer is executed together with the
     * stored function parameter within a thread as soon as run is called.
     */
    template <typename BaseCpi, typename Base, typename RetVal,
              typename FuncArg0 = phoenix::nil_t,
              typename Arg0     = phoenix::nil_t, 
              typename FuncArg1 = phoenix::nil_t,
              typename Arg1     = phoenix::nil_t,
              typename FuncArg2 = phoenix::nil_t, 
              typename Arg2     = phoenix::nil_t,
              typename FuncArg3 = phoenix::nil_t, 
              typename Arg3     = phoenix::nil_t,
              typename FuncArg4 = phoenix::nil_t, 
              typename Arg4     = phoenix::nil_t,
              typename FuncArg5 = phoenix::nil_t, 
              typename Arg5     = phoenix::nil_t,
              typename FuncArg6 = phoenix::nil_t,
              typename Arg6     = phoenix::nil_t >
    class task;

    /**
     * Generic task class, for encapsulating asynchronous function calls.
     *
     * Almost the same as saga::impl::task, with the slight difference
     * that here, the function to execute, takes a further parameter
     * more precisely a saga::uuid.
     * This is needed to enable monitoring of tasks, where the adaptor
     * implements the asynchronity.
     *
     * @see the saga::impl::task for a detailed description.
     */
    template <typename BaseCpi, typename Base, typename RetVal,
              typename FuncArg0 = phoenix::nil_t,
              typename Arg0     = phoenix::nil_t, 
              typename FuncArg1 = phoenix::nil_t,
              typename Arg1     = phoenix::nil_t,
              typename FuncArg2 = phoenix::nil_t, 
              typename Arg2     = phoenix::nil_t,
              typename FuncArg3 = phoenix::nil_t, 
              typename Arg3     = phoenix::nil_t,
              typename FuncArg4 = phoenix::nil_t, 
              typename Arg4     = phoenix::nil_t,
              typename FuncArg5 = phoenix::nil_t, 
              typename Arg5     = phoenix::nil_t,
              typename FuncArg6 = phoenix::nil_t,
              typename Arg6     = phoenix::nil_t>
    class wrapper_task;

}}

#endif
