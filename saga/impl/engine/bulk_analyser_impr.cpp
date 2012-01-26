//  Copyright (c) 2006 Stephan Hirmer (shirmer@cct.lsu.edu)
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <vector>
#include <algorithm>
#include <utility>

#include <saga/saga/util.hpp>
#include <saga/saga/adaptors/task_declaration.hpp>
#include <saga/impl/runtime.hpp>
#include <saga/impl/session.hpp>

#include <saga/impl/engine/bulk_analyser_impr.hpp>
#include <saga/impl/engine/task_container.hpp>


namespace saga { 
    
    namespace impl {

        bulk_analyser_impr::bulk_analyser_impr(void)
        {
        
        }

        bulk_analyser_impr::~bulk_analyser_impr(void)
        {
        
        }

        bool inline session_compare_tasks (saga::task const& lhs, saga::task const& rhs)
        {
            return (runtime::get_impl(runtime::get_impl(lhs)->get_session())->get_uuid() < 
                    runtime::get_impl(runtime::get_impl(rhs)->get_session())->get_uuid());
        }

        bool inline task_equal_session (saga::task const& lhs, saga::task const& rhs)
        {
            return ( (runtime::get_impl(runtime::get_impl(lhs)->get_session())->get_uuid()) == 
                     (runtime::get_impl(runtime::get_impl(rhs)->get_session())->get_uuid()) );
        }

        void bulk_analyser_impr::analyse_bulk(saga::impl::task_container& tc, 
                                             sorted_tc_type& ret,
                                             mode m /* = Default */)
        {
            std::vector<saga::task> tmp_ret = tc.list_tasks();
            session_sort(tmp_ret);
            // std::cout<<"analyse bulk: res of session_sort size"<< tmp_ret.size()<<std::endl;
            operation_sort(tmp_ret, ret);
            return;
            
        } // end analyse

        void bulk_analyser_impr::operation_sort(std::vector<saga::task>& session_sorted_,
                                                sorted_tc_type& ret)
        {
            std::vector<saga::task> tmp = session_sorted_;
            std::vector<saga::task>::iterator new_last_it = 
                          unique(tmp.begin(), tmp.end(), task_equal_session);
            
            // std::cout<<"operation sort: res of unique size : " << tmp.size()<<std::endl;
            
            std::vector<saga::task>::iterator tmp_it = tmp.begin();
            
            for(/**/; tmp_it != new_last_it; ++tmp_it)
            {
                std::pair< std::vector<saga::task>::iterator, 
                           std::vector<saga::task>::iterator > my_range = 
                    std::equal_range(session_sorted_.begin(), session_sorted_.end(),
                                *tmp_it, task_equal_session);

                if( my_range.first == my_range.second )
                {
                    std::string op_qualifier = runtime::get_impl(*(my_range.first))->get_func_name(); 
                    op_qualifier += "::";
                    op_qualifier += (runtime::get_impl(runtime::get_impl(*my_range.first)
                                  ->get_session())->get_uuid()).string();

                    sorted_tc_type::iterator p = ret.find(op_qualifier);
                    if(p!=ret.end())
                        std::cout<<"COUCOU" << std::endl;
                                   
                    saga::task_container tmp;
                    tmp.add_task(*(my_range.first));
                    ret.insert(std::pair<std::string, saga::task_container>
                                                            (op_qualifier,tmp) );
                    continue;
                }
                operation_add_tc(my_range, session_sorted_, ret);    
            }
           // std::cout<<"operation sort: res of unique size : "<< ret.size()<<std::endl;
        }

        void bulk_analyser_impr::operation_add_tc(
                std::pair< std::vector<saga::task>::iterator, 
                            std::vector<saga::task>::iterator >& sorted_session_range_, 
                std::vector<saga::task>& session_sorted_,
                sorted_tc_type& ret)
        {
            std::vector<saga::task>::iterator tc_iter = sorted_session_range_.first;
            
            for (/**/; tc_iter != sorted_session_range_.second; ++tc_iter)
            {
                std::string op_qualifier = runtime::get_impl(*tc_iter)->get_func_name(); 
                
                op_qualifier += "::";
                op_qualifier += (runtime::get_impl(runtime::get_impl(*tc_iter)
                                  ->get_session())->get_uuid()).string();
                
                // search in the existing return map, if there is already
                // a task_container holding operations as we have currently. 
                
                sorted_tc_type::iterator p = ret.find(op_qualifier);
                                
                // if there is already an other task_container with the same operation
                // add the current one to the appropriate group
                
                bool create_new_tc = true;
                if ( p != ret.end() )
                {
                    std::vector<saga::task> tlist = p->second.list_tasks();
                    std::vector<saga::task>::iterator t_it = tlist.begin();
                    saga::task ttask = *(t_it);
                    
                    if (task_equal_session(ttask, *tc_iter))
                    {
                        (p->second).add_task(*tc_iter);
                        create_new_tc = false;
                    }
                }
                // if not, create a new group
                if (create_new_tc)
                {
                    saga::task_container tmp;
                    tmp.add_task(*tc_iter);
                    std::cout << "Adding " << op_qualifier <<std::endl;
                    ret.insert(std::pair<std::string, saga::task_container>
                                                          (op_qualifier,tmp) );
                }
            } // end for
            return;
        }
        
        void bulk_analyser_impr::session_sort(std::vector<saga::task>& task_list_)
        {
            sort(task_list_.begin(), task_list_.end(), session_compare_tasks);
            return;
        }
        
        
//         void bulk_analyser_impr::session_sort_BACKUP(saga::impl::task_container& tc,
//                                               std::vector<saga::task_container>& ret)
//         {
//             std::vector<task> tmp_task_list = tc.list_tasks();
//             std::vector<task>::iterator task_iter = tmp_task_list.begin();
//             bool task_added = false;.
//             for(/**/;task_iter != tmp_task_list.end(); ++task_iter)
//             {
//                 std::vector<saga::task_container>::iterator tc_iter = ret.begin();
//                 for(/**/; tc_iter!=ret.end();++tc_iter)
//                 {
//                     std::vector<task> tmp = tc_iter->list_tasks();
//                     task first_task = tmp.begin();
//                     // TODO: Add get_session in saga::impl::task
//                     //       and get_uuid in session
//                     if(first_task.get_session().get_uuid() == task_iter->get_session()->get_uuid() )
//                     {
//                         tc_iter->add_task(*task_iter);
//                         task_added = true;
//                         break;
//                     }
//                 }
//                 if (!task_added)
//                 {
//                     saga::task_container tc;
//                     tc.add_task(*task_iter);
//                     task_added = false;
//                 }
//             }
//         }    
//         void bulk_analyser_impr::operation_sort(std::vector<saga::task_container>& session_sorted,
//                                                 sorted_tc_type& ret)
//         {
//             std::vector<saga::task_container>::iterator session_sorted_iter = 
//                                                          session_sorted.begin();
//             
//             sorted_tc_type::iterator ret_first_current_session = ret.begin();                                             
//             
//             for(/**/;session_sorted_iter!=session_sorted.end(); ++session_sorted_iter)
//             {
//             
//                 std::vector<task> tmp_task_list = session_sorted_iter->list_tasks();
//                 
//                 std::vector<task>::iterator tc_iter = tmp_task_list.begin(); 
//                 
//                 for(/**/;tc_iter != tmp_task_list.end(); ++tc_iter)
//                 {
//                     std::string op_qualifier = runtime::get_impl(*tc_iter)->get_func_name(); 
//                                                                 /*concat with uuid here*/
//                     
//                     // search in the existing return map, if there is already
//                     // a task_container holding operations as we have currently. 
//                     
//                     sorted_tc_type::iterator p=NULL;
//                     p = find(ret_first_current_session, ret.end(), op_qualifier);
//                                     
//                     // if there is already an other task_container with the same operation
//                     // add the current one to the appropriate group
//                     if ( p != ret.end() )
//                     {
//                         (p->second).add_task(*tc_iter);
//                     }
//                     // if not, create a new group
//                     else
//                     {
//                         saga::task_container tmp;
//                         tmp.add_task(*tc_iter);
//                         ret.insert(std::pair<std::string, saga::task_container>
//                                                                 (op_qualifier,tmp) );
//                     }
//                     
//                 } // end for
//                 ret_first_current_session = ret.end();
//                 
//             } // end for
//         }

    
    } // namespace impl
} // namespace saga

