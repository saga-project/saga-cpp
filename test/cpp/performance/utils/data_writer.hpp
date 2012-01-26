//  Copyright (c) 2011 Ole Weidner (oweidner@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_PERF_TESTS_DATA_WRITER
#define SAGA_PERF_TESTS_DATA_WRITER 1

#include <iostream>
#include <string>
#include <fstream>

namespace saga_perf_tests 
{
  class data_writer {
  
  private:
  
    std::ofstream _file;
    
  public:
  
    ////////////////////////////////////////////////////////////////
    //
    data_writer(std::string const & filename)
    {
      _file.open(filename.c_str());
    }
    
    ////////////////////////////////////////////////////////////////
    //
    void write(int count, std::string const & timing)
    {
      _file << timing << std::endl;
    }
    
    ////////////////////////////////////////////////////////////////
    //
    ~data_writer()
    {
      _file.close();
    }
  };
}

#endif