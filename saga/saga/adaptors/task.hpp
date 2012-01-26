//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_ADAPTOR_TASK_IMPLEMENTATION_HPP
#define SAGA_ADAPTOR_TASK_IMPLEMENTATION_HPP

#include <saga/saga/adaptors/task_declaration.hpp>
#include <saga/impl/engine/task.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga 
{ 
  namespace adaptors 
  {
    /**
      * template'd helper function, which creates a appropriate
      * saga::impl::task object
      */
    template<typename Cpi, typename Base, typename RetVal>
    inline saga::impl::task<Cpi, Base, RetVal> *
    task::create_task(char const *func_name, TR1::shared_ptr<Cpi> cpi, 
        impl::proxy* prxy, void (Base::*sync)(RetVal&), 
        bool (Base::*prep)(RetVal&, saga::uuid))
    {
        return new saga::impl::task<Cpi, Base, RetVal>(
            func_name, cpi, prxy, sync, prep);
    }

    /**
      * templated helper function, which creates a appropriate
      * saga::impl::task object
      *
      * The function to execute takes as a further parameter,
      * more precisely, a uuid of the task, which will be executed in the adaptor
      *
      */
    template<typename Cpi, typename Base, typename RetVal>
    inline saga::impl::wrapper_task<Cpi, Base, RetVal> *
    task::create_task(char const* func_name, TR1::shared_ptr<Cpi> cpi, 
        impl::proxy* prxy, void (Base::*sync)(RetVal&, saga::uuid), 
        bool (Base::*prep)(RetVal&, saga::uuid))
    {
        return new saga::impl::wrapper_task<Cpi, Base, RetVal>(
            func_name, cpi, prxy, sync, prep);
    }

    /**
      * template'd constructor
      *
      * calls member function create_task and puts the returned task
      * into the saga::task class, which is part of the facade.
      */ 
    template<typename Cpi, typename Base, typename RetVal>
    task::task (char const* func_name, TR1::shared_ptr<Cpi> cpi, 
          impl::proxy* prxy, void (Base::*sync)(RetVal&), 
          bool (Base::*prep)(RetVal&, saga::uuid))
      : base_type (create_task(func_name, cpi, prxy, sync, prep))
    {
    }

    template<typename Cpi, typename Base, typename RetVal>
    task::task (char const* func_name, TR1::shared_ptr<Cpi> cpi, 
          void (Base::*sync)(RetVal&), 
          bool (Base::*prep)(RetVal&, saga::uuid))
      : base_type (create_task(func_name, cpi, cpi->get_proxy(), sync, prep))
    {
    }

    /**
      * overloaded template'd constructor
      *
      * calls member function create_task and puts the returned task
      * into the saga::task class, which is part of the facade.
      * Differs from the other constructor, because the func takes a 
      * further parameter, a task uuid.
      */ 
    template<typename Cpi, typename Base, typename RetVal>
    task::task (char const* func_name, TR1::shared_ptr<Cpi> cpi, 
          impl::proxy* prxy, void (Base::*sync)(RetVal, saga::uuid), 
          bool (Base::*prep)(RetVal, saga::uuid))
      : base_type (create_task (func_name, cpi, prxy, sync, prep)) 
    {
    }

    template<typename Cpi, typename Base, typename RetVal>
    task::task (char const* func_name, TR1::shared_ptr<Cpi> cpi, 
          void (Base::*sync)(RetVal, saga::uuid), 
          bool (Base::*prep)(RetVal, saga::uuid))
      : base_type (create_task (func_name, cpi, cpi->get_proxy(), sync, prep)) 
    {
    }

    ///////////////////////////////////////////////////////////////////////
    // bring in the generated constructors with 1 to N parameters.
    #include <saga/saga/adaptors/task_impl.hpp>

}} // saga::adaptors
///////////////////////////////////////////////////////////////////////////////

#endif 

