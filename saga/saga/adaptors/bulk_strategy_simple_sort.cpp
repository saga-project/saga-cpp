//  Copyright (c) 2005 Stephan Hirmer (shirmer@cct.lsu.edu)
// 
//  Use, modification and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <utility>
#include <vector>

#include <saga/saga/adaptors/task_declaration.hpp>
#include <saga/impl/engine/task_container.hpp>
#include <saga/saga/adaptors/bulk_strategy_simple_sort.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga
{
    namespace adaptors
    {
        bulk_strategy_simple_sort::bulk_strategy_simple_sort()
        {
        }
        
        void bulk_strategy_simple_sort::apply(sorted_tc_type & analyser_res)
        {
//             this->thread_ = simple_future <int> (
//                         TR1::bind (&bulk_strategy_simple_sort::bond_apply, 
//                                               this, TR1::ref(analyser_res)) );
//             //this->complete_wait();                               
            this->bond_apply(analyser_res);
        }
          
        int bulk_strategy_simple_sort::bond_apply (sorted_tc_type& analyser_res)
        {
            using namespace boost;
            using namespace saga::impl;

            // iterate over the sorted result
            sorted_tc_type::iterator it = analyser_res.begin ();

            while ( it != analyser_res.end () )
            {
                saga::task_container tmp_tc = it->second;

                // just for testing
//                 std::cout << "task container: " << it->first << std::endl;
//                 std::cout << "----------> Number in there: " 
//                           << (unsigned)(it->second.list_tasks () ).size () << std::endl;

                saga::impl::v1_0::preference_type prefs;

                std::string cpi_name;
                std::string op_name; 

                this->default_process_func_name ((it->first), cpi_name, op_name);

                // asking the first task, to search an appropriate adaptor for 
                // an possible bulk execution 
                std::vector<saga::task> bulk_task_list = tmp_tc.list_tasks();

                // we search for a bulk adaptor, if the bulk contains more than
                // one task.

                // TR1::shared_ptr <v1_0::cpi> my_bulk_adaptor;
                std::vector<saga::task>::iterator bulk_task_list_iter = bulk_task_list.begin ();

                if ( bulk_task_list.size () > 1 )
                {
                    my_bulk_adaptor = TR1::shared_ptr<v1_0::cpi> 
                                          (runtime::get_impl (*bulk_task_list_iter)
                                                  ->get_bulk_adaptor (cpi_name, op_name, prefs) );
                }

                // if we have an appropriate bulk_adaptor, we execute the
                // bulk using this bulk_adaptor
                if ( my_bulk_adaptor )
                {
//                    std::cout<<"found bulk adaptor ... "<<std::endl;
                    my_bulk_adaptor->init_bulk ();

                    // prepare the arguments 
                    while ( bulk_task_list_iter != bulk_task_list.end () )
                    {
                      runtime::get_impl (*bulk_task_list_iter)
                              ->visit_args (my_bulk_adaptor.get ());
                      ++bulk_task_list_iter;
                    }
                    
                    my_bulk_adaptor->execute_bulk();
                    
                    // set the state of all the tasks to Done
                    boost::get_pointer(runtime::get_impl (tmp_tc))
                            ->set_state_for_all (saga::task::Done);
                            

                }
                // if not, we execute the task one by one.
                else
                {
//                    std::cout << "NO bulk adaptor found ... " << std::endl;
                    // execute this task_container one by one!
                    runtime::get_impl (it->second)->simple_run ();                 
                }

                ++it;

            } // end while
            return 1;
        }
      
    } // namespace adaptors

} // namespace saga

