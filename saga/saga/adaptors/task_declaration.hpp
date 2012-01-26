//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005-2006 Stephan Hirmer (shirmer@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_ADAPTOR_TASK_V1_HPP
#define SAGA_ADAPTOR_TASK_V1_HPP

#include <saga/saga/util.hpp>
#include <saga/saga/task.hpp>
#include <saga/impl/engine/task_fwd.hpp>
#include <saga/saga/uuid.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga 
{ 
  namespace adaptors 
  {
    
    /**
      * Helper class to create saga::task's.
      *
      * As this task is not a template class but has a template'd constructor
      * the templates are hidden from the adaptor programmer. Further this
      * class is not visible within the saga:: namespace, so templates
      * does not occur within the saga facade.
      */
    class task : public saga::task
    {
    private:
        typedef saga::task base_type;
        
        /**
          * template'd helper function, which creates a appropriate
          * saga::impl::task object
          */
        template<typename Cpi, typename Base, typename RetVal>
        static inline saga::impl::task<Cpi, Base, RetVal> *
        create_task(char const *func_name, TR1::shared_ptr<Cpi> cpi, 
            impl::proxy* prxy, void (Base::*sync)(RetVal&), 
            bool (Base::*prep)(RetVal&, saga::uuid));

        /**
          * templated helper function, which creates a appropriate
          * saga::impl::task object
          *
          * The function to execute takes as a further parameter,
          * more precisely, a uuid of the task, which will be executed in the adaptor
          *
          */
        template<typename Cpi, typename Base, typename RetVal>
        static inline saga::impl::wrapper_task<Cpi, Base, RetVal> *
        create_task(char const* func_name, TR1::shared_ptr<Cpi> cpi, 
            impl::proxy* prxy, void (Base::*sync)(RetVal&, saga::uuid), 
            bool (Base::*prep)(RetVal&, saga::uuid));

    public:
        task(void);
        
        ~task (void) 
        {
        }
        
        /**
          * template'd constructor
          *
          * calls member function create_task and puts the returned task
          * into the saga::task class, which is part of the facade.
          */ 
        template<typename Cpi, typename Base, typename RetVal>
        task (char const* func_name, TR1::shared_ptr<Cpi> cpi, 
              impl::proxy* prxy, void (Base::*sync)(RetVal&), 
              bool (Base::*prep)(RetVal&, saga::uuid) = NULL);

        template<typename Cpi, typename Base, typename RetVal>
        task (char const* func_name, TR1::shared_ptr<Cpi> cpi, 
              void (Base::*sync)(RetVal&), 
              bool (Base::*prep)(RetVal&, saga::uuid) = NULL);

        /**
          * overloaded template'd constructor
          *
          * calls member function create_task and puts the returned task
          * into the saga::task class, which is part of the facade.
          * Differs from the other constructor, because the func takes a 
          * further parameter, a task uuid.
          */ 
        template<typename Cpi, typename Base, typename RetVal>
        task (char const* func_name, TR1::shared_ptr<Cpi> cpi, impl::proxy* prxy, 
              void (Base::*sync)(RetVal, saga::uuid), 
              bool (Base::*prep)(RetVal, saga::uuid) = NULL);

        template<typename Cpi, typename Base, typename RetVal>
        task (char const* func_name, TR1::shared_ptr<Cpi> cpi, 
              void (Base::*sync)(RetVal, saga::uuid), 
              bool (Base::*prep)(RetVal, saga::uuid) = NULL);

        ///////////////////////////////////////////////////////////////////////
        // bring in the generated constructors with 1 to N parameters.
        #include <saga/saga/adaptors/task_declaration_impl.hpp>
    };

  } // adaptors

} // namespace saga
///////////////////////////////////////////////////////////////////////////////

#endif // SAGA_ADAPTOR_TASK_V1_HPP

