//  Copyright (c) 2011 Ole Weidner (oweidner@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "./utils/arg_checker.hpp"
#include "./utils/test_executor.hpp"

#include <saga.hpp>

#define STEPS 500

using namespace saga_perf_tests;

////////////////////////////////////////////////////////////////
//
class test_priv {

  private: 
  
    int _steps_left;
    saga::filesystem::file * saga_file;

  public:
    ////////////////////////////////////////////////////////////
    //
    test_priv()
    {
      _steps_left = STEPS;
    }
    
    ////////////////////////////////////////////////////////////
    //
    int total_steps()
    {
      return STEPS;
    }
    
    ////////////////////////////////////////////////////////////
    //
    int remaining_steps()
    {
      return _steps_left;
    }
    
    ////////////////////////////////////////////////////////////
    //
    void setup()
    {
      saga_file = new saga::filesystem::file("file://localhost//etc/passwd");
      
      // create target dir
      saga::filesystem::directory d("file://localhost/tmp/saga-perf_tmp",
                                    saga::filesystem::Create);
    }
    
    ////////////////////////////////////////////////////////////
    //
    void do_step()
    {
      saga_file->copy("file://localhost/tmp/saga-perf_tmp/",
                      saga::filesystem::Overwrite);
      _steps_left -= 1;
    }
    
    
    ////////////////////////////////////////////////////////////
    //
    void teardown()
    {
      // delete tmp directory
      saga::filesystem::directory d("file://localhost/tmp/saga-perf_tmp");
      d.remove(saga::filesystem::Recursive);
      
      //delete saga_file;
    }
};

////////////////////////////////////////////////////////////////
//
int main (int argc, char * argv[])
{
  arg_checker a("Copy a lot of small files with the default file adaptor.", 
    argc, argv);
  if(false == a.check())
    return 255;
  
  test_executor<test_priv> te(a.get_filename());
  te.run();
}
//
////////////////////////////////////////////////////////////////
