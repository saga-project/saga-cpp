//  Copyright (c) 2005 Stephan Hirmer (shirmer@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BULK_STRATEGY_SIMPLE_SORT_HPP
#define BULK_STRATEGY_SIMPLE_SORT_HPP

#include <saga/impl/config.hpp>
#include <saga/impl/engine/task.hpp>
#include <saga/impl/engine/bulk_analyser.hpp>
#include <saga/impl/engine/bulk_analyser_impr.hpp>

#include <saga/saga/adaptors/bulk_strategy_base.hpp>

#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable: 4251 4231 4275 4660)
#endif

namespace saga
{
    namespace adaptors
    {
        class bulk_strategy_simple_sort : public bulk_strategy_base
        {
        private:
        int bond_apply(sorted_tc_type & analyser_res);
        
        TR1::shared_ptr <saga::impl::v1_0::cpi> my_bulk_adaptor;
            
        protected:
        public:
            
            /** 
              * constructor
              */
            bulk_strategy_simple_sort(void);
            
            /** 
              * destructor
              */
            ~bulk_strategy_simple_sort(void){}
            
            /**
              * applies the simple execution strategy.
              *
              * This strategy trust the underlying middleware and submits
              * selects for each task group an adaptor, submits all the tasks 
              * to it, and does _not_ care, wheater the execution was successfull
              * or not. So it trusts without asking.
              *
              * If tasks or task groups can not be executed within a bulk
              * they are executed one-by-one.
              */
            virtual void apply(sorted_tc_type& analyser_res);
        };
    }
}

#endif // BULK_STRATEGY_SIMPLE_SORT_HPP
