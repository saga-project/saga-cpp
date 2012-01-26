//  Copyright (c) 2011 Ole Weidner (oweidner@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_PERF_TESTS_TEST_EXECUTOR
#define SAGA_PERF_TESTS_TEST_EXECUTOR 1

#include "./data_writer.hpp"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <iostream>

using namespace boost::posix_time;

namespace saga_perf_tests 
{
  template <class T> class test_executor {
  
  private:
  
    test_executor() { };
    T * _test_instance;
    saga_perf_tests::data_writer _dw;
    
  public:
  
    ////////////////////////////////////////////////////////////////
    //
    test_executor(std::string const & filename) : _dw(filename)
    {
      _test_instance = new T();
    }
    
    ////////////////////////////////////////////////////////////////
    //
    ~test_executor()
    {
      delete _test_instance;
    }
    
     ////////////////////////////////////////////////////////////////
    //
    void run()
    {
    
    int total_steps = _test_instance->total_steps();
    
    std::cout << "setup()" << std::endl;
      _test_instance->setup();
    
      while(_test_instance->remaining_steps() > 1)
      {
        int remaining_steps = _test_instance->remaining_steps();
        
        std::cout << "step: " << total_steps-remaining_steps;
        ptime t_start(microsec_clock::local_time());
        
        _test_instance->do_step();
        
        ptime t_end(microsec_clock::local_time());
        time_duration t_diff = t_end - t_start;
        std::cout << " - " << t_diff.total_microseconds() << " ms" << std::endl;
        
        // write to data file
        _dw.write(total_steps-remaining_steps, boost::lexical_cast<std::string>(t_diff.total_microseconds()) );
      }
      
      std::cout << "teardown()" << std::endl;
      _test_instance->teardown();
    }
  };
}

# endif
