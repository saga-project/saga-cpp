//  Copyright (c) 2005 Stephan Hirmer (shirmer@cct.lsu.edu)
// 
//  Use, modification and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/tokenizer.hpp>

#include <saga/saga/util.hpp>
#include <saga/impl/engine/task_base.hpp>
#include <saga/saga/adaptors/task_declaration.hpp>
#include <saga/impl/runtime.hpp>

#include <saga/saga/adaptors/bulk_strategy_base.hpp>
#include <saga/impl/engine/task_container.hpp>

namespace saga
{
    namespace adaptors
    {
        
        saga::impl::task_container* tc;
        
        // task_blocker_for_bulk
        // moved here, to avoid messing up include dependencies.        
        // constructor
        task_blocker_for_bulk ::task_blocker_for_bulk(impl::task_container* tc_)
        : tc(tc_)
        {
            tc->set_tasks_for_bulk_treatement(
                  impl::task_base::actual_bulk_treated);
        }

        task_blocker_for_bulk ::~task_blocker_for_bulk(void)
        {
            tc->set_tasks_for_bulk_treatement(
                    impl::task_base::was_bulk_treated);
        }
        // end task_blocker_for_bulks
        
        bulk_strategy_base::bulk_strategy_base(void) 
        {
        }
                  
        // default destructor
        bulk_strategy_base::~bulk_strategy_base (void)
        {
        }
        
        void bulk_strategy_base::apply(sorted_tc_type& analyser_res_)
        {
            SAGA_THROW("Don't call apply on strategy base class!", 
                saga::NotImplemented);
        }
        
        void bulk_strategy_base::complete_wait(void)
        {
            this->thread_();
        }
        
        
        void bulk_strategy_base::default_process_func_name(
                      std::string const& func_name, std::string& cpi_name_,
                      std::string& op_name_)
        {
            typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
            boost::char_separator<char> sep("::");
            tokenizer tokens( func_name, sep);
            tokenizer::iterator tok_iter = tokens.begin();
                                        
            cpi_name_ = *tok_iter; 
            op_name_  = "bulk_";
            op_name_ += *(++tok_iter);         
        }
        
        
    }
}

