//  Copyright (c) 2011 Ole Weidner (oweidner@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "./utils/arg_checker.hpp"
#include "./utils/test_executor.hpp"

#include <saga.hpp>

#define STEPS 50

using namespace saga_perf_tests;

////////////////////////////////////////////////////////////////
//
class test_priv {

  private: 
  
    int _steps_left;
    saga::filesystem::file f;

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
      f = saga::filesystem::file("file://localhost//etc/passwd");
    }
    
    ////////////////////////////////////////////////////////////
    //
    void do_step()
    {
      saga::filesystem::file f2 = f;

      f2.is_dir();
      _steps_left -= 1;
    }
    
    
    ////////////////////////////////////////////////////////////
    //
    void teardown()
    {

    }
};

////////////////////////////////////////////////////////////////
//
int main (int argc, char * argv[])
{
  arg_checker a("Runs is_dir on exclusive file objects. (Hot vs. cold start).", 
    argc, argv);
  if(false == a.check())
    return 255;
  
  test_executor<test_priv> te(a.get_filename());
  te.run();
}
//
////////////////////////////////////////////////////////////////
