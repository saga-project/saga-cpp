//  Copyright (c) 2005 Stephan Hirmer (shirmer@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_IMPL_ENGINE_BULK_ANALYSER_HPP
#define SAGA_IMPL_ENGINE_BULK_ANALYSER_HPP

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
          * This class analysis a given task container.
          *
          * It searches for similar tasks, in terms of same function on
          * the same cpi class.
          *
          */
        class bulk_analyser 
        : private boost::noncopyable
        { 
        private: 
            enum mode 
            {
                Default        = 0,
                Same_Operation = 1,
                Same_Cpi_Type  = 2
            };
        
        public:
            typedef std::map<std::string, saga::task_container> sorted_tc_type;

            /*! the default constructor*/
            bulk_analyser(void);
            
            /*! the destructor*/
            ~bulk_analyser(void);
            
            /**
              * applys the simple sorting strategy to the given taskcontainer.
              *
              * clustering tasks together, which encapsulate the same
              * function on the same cpi class
              */
            void analyse_bulk(task_container& tc, sorted_tc_type& ret, 
                                                           mode m = Default);
        }; // class bulk_analyser

    } // namespace impl

} // namespace saga

#endif  // SAGA_IMPL_ENGINE_BULK_ANALYSER_HPP

