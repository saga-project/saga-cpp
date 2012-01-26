//  Copyright (c) 2010 Ole Weidner
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <saga/saga.hpp>



using namespace std;



int main(int argc, char** argv)

{
    
  try 
    
  {
        
    std::cout << "If #640 is not fixed, the code below will produce an exception. " << std::endl;
  

    std::cout << "Please check: http://faust.cct.lsu.edu/trac/saga/ticket/626 for details." << std::endl;

      

    saga::stream::stream *s = new saga::stream::stream("tcp://www.google.com");
    delete s;

    return 0;
  }
  catch(saga::exception const & e)
    
  {
        
    std::cerr << "\nFAILED: " << e.what() << std::endl;
    
        
    return 255;
    
  }

}

