//  Copyright (c) 2011 Ole Weidner (oweidner@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_PERF_TESTS_ARG_CHECKER
#define SAGA_PERF_TESTS_ARG_CHECKER 1

#include <vector>
#include <iostream>

namespace saga_perf_tests 
{
  class arg_checker {
  
  private:
  
    std::vector<std::string> _arg_vector;
    std::string _test_desc;
    std::string _exe_name;
    
    void _print_info()
    {
      std::cout << std::endl
                << "This executable is part of the 'SAGA Performance Tests'."
                << std::endl;
      std::cout << "Description: " << _test_desc 
                << std::endl << std::endl;
      std::cout << "Usage: " << _exe_name << " <output_filename>" 
                << std::endl << std::endl; 
    }
  
  public:
  
    ////////////////////////////////////////////////////////////////
    //
    arg_checker(std::string const & test_desc, int argc, char * argv[] )
    {
      for(int i=1; i<argc; ++i) {
        _arg_vector.push_back(std::string(argv[i]));
      }
      
      _exe_name = std::string(argv[0]);
      _test_desc = test_desc;
    }
    
    ////////////////////////////////////////////////////////////////
    //
    ~arg_checker()
    {
    
    }
    
    ////////////////////////////////////////////////////////////////
    // true on success, false on error
    bool check()
    {
      if(_arg_vector.size() != 1)
      {
        _print_info();
        return false;
      }
      
      return true;
    }
    
    ////////////////////////////////////////////////////////////////
    // returns the path for the data file
    std::string get_filename()
    {
      if(_arg_vector.size() == 1)
      {
        return _arg_vector[0];
      }
      else
      {
        return "";
      }
    }
    
  };
}

#endif
