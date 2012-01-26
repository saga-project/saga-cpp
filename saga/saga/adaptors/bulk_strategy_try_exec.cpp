//  Copyright (c) 2005 Stephan Hirmer (shirmer@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  job LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <utility>

#include <saga/saga/base.hpp>
#include <saga/saga/util.hpp>
#include <saga/saga/adaptors/task.hpp>
#include <saga/impl/runtime.hpp>
#include <saga/saga/adaptors/bulk_strategy_try_exec.hpp>

#include <saga/impl/engine/task_container.hpp>

namespace saga
{
    namespace adaptors
    {
        bulk_strategy_try_exec::bulk_strategy_try_exec()
        {
        }
        
        saga::task bulk_strategy_try_exec::get_first_task(saga::task_container& tc)
        {
            std::vector<saga::task> bulk_task_list = tc.list_tasks();
            std::vector<saga::task>::iterator tmp_first_task = 
                                                 bulk_task_list.begin();
            
            return *tmp_first_task;
        }
        
        bool bulk_strategy_try_exec::prepare_args_for_adaptor(saga::task_container& tc, 
                                      TR1::shared_ptr<saga::impl::v1_0::cpi> adp)
        {
            std::vector<saga::task> bulk_task_list = tc.list_tasks();
            std::vector<saga::task>::iterator it_ = bulk_task_list.begin();
            
            while ( it_ != bulk_task_list.end () )
            {
                if( (saga::task_base::New == it_->get_state())
                 || (saga::task_base::Running == it_->get_state())  )
               // Remark: the second condition is necessary because
               //         of the fact that tasks treated in a task_container
               //         return RUNNING as soon as task_container.run()
               //         is called. This is necessary to avoid race conditions.
                {
                    try
                    {
                        impl::runtime::get_impl(*it_)->visit_args (adp.get ());
                    }
                    catch (std::exception e)
                    {
                        return false;
                    }
                }
                ++it_;
            }
            return true;
        }
        
        void bulk_strategy_try_exec::apply(sorted_tc_type & analyser_res)
        {
//             this->thread_ = simple_future <int> (
//                         TR1::bind (&bulk_strategy_try_exec::bond_apply, 
//                                               this, TR1::ref(analyser_res)) );
//             //this->complete_wait(); 
               this->bond_apply(analyser_res);                               
        }
        
        int bulk_strategy_try_exec::bond_apply(sorted_tc_type & analyser_res)
        {
            using namespace boost;
            using namespace saga::impl;

//            std::cout<< "bulk_strategy_try_exec::apply size INPUT : "<< analyser_res.size() << std::endl;
            
            // iterate over each pair: string -- taskcontainer 
            // sorted_tc_type::iterator it = analyser_res.begin();
            sorted_tc_type::iterator it_end = analyser_res.end();
            for (sorted_tc_type::iterator it = analyser_res.begin(); 
                 it != it_end; ++it)
            {
                // get the taskcontainer from out the pair.
                saga::task_container tmp_tc = it->second;
                
                // just for testing
//                 std::cout << "Try_exec: task container: " << it->first << std::endl;
//                 std::cout << "Try_exec: ----------> Number in there: " << 
//                               (unsigned int)(it->second.list_tasks() ).size()
//                           << std::endl;
                
                saga::impl::v1_0::preference_type prefs;
                
                std::string cpi_name;
                std::string op_name; 
                this->default_process_func_name((it->first), cpi_name, op_name);
                std::vector<saga::uuid> used_adaptors;
                
                size_t nb_tasks_todo = (tmp_tc.list_tasks()).size();
                
                saga::task first_task = get_first_task(tmp_tc);
                TR1::shared_ptr <saga::impl::v1_0::cpi> my_bulk_adaptor;
                
                // as long as there is as least 1 task, which couldn't have been
                // treated
                while (nb_tasks_todo >= 1) 
                {
                    if (nb_tasks_todo > 1)
                    {
                        // asking the first task, to search an appropriate adaptor for 
                        // an possible bulk execution 
                        my_bulk_adaptor.reset(
                            runtime::get_impl(first_task)->get_bulk_adaptor(
                                cpi_name, op_name, prefs) );
                    }

                    bool adaptor_used_before = true;

                    // check if we found a bulk adaptor
                    if (my_bulk_adaptor)
                    {
                        std::vector<saga::uuid>::iterator res_find =
                                              std::find (used_adaptors.begin(), 
                                                         used_adaptors.end(), 
                                                         my_bulk_adaptor->get_adaptor_uuid() );

                        // check if the found bulk adaptor is a new one.
                        if ( res_find == used_adaptors.end() )
                        {
                            adaptor_used_before = false;
                            
//                             std::cout<<"found bulk adaptor ... "<<std::endl;

                            my_bulk_adaptor->init_bulk();
                            
                            if (prepare_args_for_adaptor(tmp_tc, my_bulk_adaptor) )
                            {
                                std::vector<saga::uuid> done_tasks = my_bulk_adaptor->execute_bulk();

                                runtime::get_impl(tmp_tc)->set_state_by_uuid(done_tasks,
                                                    saga::task_base::Done);
                                                    
                                nb_tasks_todo -= done_tasks.size();
                            }
                            used_adaptors.push_back(my_bulk_adaptor->get_adaptor_uuid() );
                        }
                    }
                    // if there is no suitable adaptor, we execute the tasks
                    // one by one, and switch to the next package.
                    if ( (!my_bulk_adaptor) || (adaptor_used_before)  )
                    {
//                         std::cout<<"NO bulk adaptor found ... "<<std::endl;

                        // execute this task_container one by one!
                        runtime::get_impl(tmp_tc)->simple_run();
                        break;
                    }
                } // end while
            } // end for
            
            return 1; // just for the future;
        }
    } // namespace adaptors
}     // namespace saga
