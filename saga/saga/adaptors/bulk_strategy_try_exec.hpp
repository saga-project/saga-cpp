//  Copyright (c) 2005 Stephan Hirmer (shirmer@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BULK_STRATEGY_TRY_EXEC_HPP
#define BULK_STRATEGY_TRY_EXEC_HPP

#include <saga/impl/config.hpp>
#include <saga/impl/engine/task.hpp>
#include <saga/impl/engine/bulk_analyser.hpp>

#include <saga/saga/adaptors/bulk_strategy_base.hpp>


#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable: 4251 4231 4275 4660)
#endif

namespace saga
{
    namespace adaptors
    {
        class bulk_strategy_try_exec : public bulk_strategy_base
        {
        private:
            
            /**
              * selects the first adaptor from out a task container.
              *
              * nothing special here, just a re-factored function to
              * keep the apply function as clean as possible.
              */
            saga::task get_first_task(saga::task_container& tc);
            
            /**
              * makes all the tasks pass their parameters to a selected
              * adaptor.
              */
            bool prepare_args_for_adaptor(saga::task_container& tc, 
                                          TR1::shared_ptr<saga::impl::v1_0::cpi> adp);
            
            int bond_apply(sorted_tc_type & analyser_res);
            
            
        protected:
        public:
            /** 
              * constructor
              */
            bulk_strategy_try_exec(void);
            
            /** 
              * destructor
              */
            ~bulk_strategy_try_exec(void) {}
            
            /**
              * applies the try and retry execution strategy.
              *
              * same as simple strategy.
              *
              * selects a adaptor for every task group, submits the task group
              * to it, and controls the proper execution. If tasks remain,
              * which are not executed, the next adaptor is selected and
              * until no more bulk enabled adaptors are available.
              *
              * If tasks or task groups can not be executed within a bulk
              * they are executed one-by-one.
              */
            virtual void apply(sorted_tc_type& analyser_res_);
        };
    }
}

#endif // BULK_STRATEGY_TRY_EXEC_HPP

