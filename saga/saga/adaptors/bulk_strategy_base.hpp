//  Copyright (c) 2005 Stephan Hirmer (stephan.hirmer@gmail.com)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BULK_STRATEGY_BASE_HPP
#define BULK_STRATEGY_BASE_HPP

#include <boost/futures.hpp>
#include <saga/impl/engine/bulk_analyser.hpp>

#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable: 4251 4231 4275 4660)
#endif

namespace saga
{
    namespace adaptors
    {
        /**
          * Thread safe struct, to block tasks, while they are tried to
          * execute by this strategy implementation.
          *
          */
        struct task_blocker_for_bulk 
        {
            saga::impl::task_container* tc;
            
            /** 
              * constructor
              */
            task_blocker_for_bulk(impl::task_container* tc_);          
            
            /** 
              * destructor
              */
            ~task_blocker_for_bulk(void);
        };
        
        /**
          * common base class for bulk strategy execution.
          */
        class bulk_strategy_base
        {
        protected:
             
             /**
               * retrieves the cpi name and the function name from a
               * given function name.
               *
               */       
             void default_process_func_name(std::string const& func_name, 
                                            std::string& cpi_name_,
                                            std::string& op_name_);
             typedef saga::impl::bulk_analyser::sorted_tc_type sorted_tc_type;
             
             boost::futures::simple_future<int> thread_;
                        
        public:
            /**
              * constructor
              */ 
            bulk_strategy_base(void); 
                     
            /** 
              * destructor
              */
            virtual ~bulk_strategy_base (void);
            
            /**
              * virtual function to apply the implemented execution strategy
              * to a given set of tasks.
              */
            virtual void apply(sorted_tc_type& analyser_res_);
            
            virtual void complete_wait(void);            
                        
        };
    }
}

#endif // BULK_STRATEGY_BASE_HPP
