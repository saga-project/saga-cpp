//  Copyright (c) 2005 Stephan Hirmer (shirmer@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_IMPL_ENGINE_BULK_ANALYSER_IMPR_HPP
#define SAGA_IMPL_ENGINE_BULK_ANALYSER_IMPR_HPP

#include <map>

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

#include <saga/saga/task_container.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga
{
    namespace impl
    {
        /**
          * Class to cluster tasks together from a given task container in 
          * terms of same session, same function and same cpi class.
          */
        class bulk_analyser_impr 
        : private boost::noncopyable
        {
            enum mode 
            {
                Default        = 0,
                Same_Operation = 1,
                Same_Cpi_Type  = 2
            };
            
        public:
            struct compare_keys
            {
                bool operator()(const std::string s1, const std::string s2) const
                {
                    return strcmp(s1.c_str(), s2.c_str()) < 0;
                }
            };
        
            typedef std::map<std::string, saga::task_container, compare_keys> sorted_tc_type;

        private:
            /**
              * Helper function to add a new taskcontainer to the list
              * of already existing task containers.
              */    
            void operation_add_tc(
                    std::pair< std::vector<saga::task>::iterator, 
                               std::vector<saga::task>::iterator >& sorted_session_range_, 
                    std::vector<saga::task>& session_sorted_,
                    sorted_tc_type& ret);
            /**
              * Helper function, which sorts the tasks according to the function.
              */
            void operation_sort(std::vector<saga::task>& session_sorted_,
                                sorted_tc_type& ret);
            /**
              * Helper function to sort the tasks according to their sessions.
              */                                
            void session_sort(std::vector<saga::task>& task_list_);
        public:
            /*the default constructor*/
            bulk_analyser_impr(void);
            
            /*the destructor*/
            ~bulk_analyser_impr(void);

            /**
              * The function to do the actual analysis.
              *
              */            
            void analyse_bulk(task_container& tc, sorted_tc_type& ret, 
                                                           mode m = Default);
        }; // class bulk_analyser

    } // namespace impl

} // namespace saga

#endif  // SAGA_IMPL_ENGINE_BULK_ANALYSER_IMPR_HPP

