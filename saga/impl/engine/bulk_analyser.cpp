//  Copyright (c) 2006 Stephan Hirmer (shirmer@cct.lsu.edu)
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <vector>
#include <utility>

#include <saga/saga/util.hpp>
#include <saga/saga/adaptors/task_declaration.hpp>
#include <saga/impl/runtime.hpp>

#include <saga/impl/engine/bulk_analyser.hpp>
#include <saga/impl/engine/task_container.hpp>


///////////////////////////////////////////////////////////////////////////////
namespace saga { 
    
    namespace impl {

        bulk_analyser::bulk_analyser(void)
        {
        
        }
        
        bulk_analyser::~bulk_analyser(void)
        {
        
        }
        
        
        void bulk_analyser::analyse_bulk(saga::impl::task_container& tc, 
                                         sorted_tc_type& ret,
                                         mode m /* = Default */)
        {

            std::vector<saga::task> tmp_task_list = tc.list_tasks();

            std::vector<saga::task>::iterator tc_iter = tmp_task_list.begin(); 

            while(tc_iter != tmp_task_list.end())
            {
                std::string op_qualifier = runtime::get_impl(*tc_iter)->get_func_name(); 
                                                            /*concat with uuid here*/

                // search in the existing return map, if there is already
                // a task_container holding operations as we have currently. 

                sorted_tc_type::iterator p = ret.find(op_qualifier);

                // if there is already an other task_container with the same operation
                // add the current one to the appropriate group
                if ( p != ret.end() )
                {
                    (p->second).add_task(*tc_iter);
                }
                // if not, create a new group
                else
                {
                    saga::task_container tmp;
                    tmp.add_task(*tc_iter);
                    ret.insert(std::pair<std::string, saga::task_container>
                                                            (op_qualifier,tmp) );
                }

                ++tc_iter;
            } // end while

        } // end analyse
    } // namespace impl

} // namespace saga

