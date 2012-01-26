//  Copyright (c) 2010 Ole Weidner
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga.hpp>

using namespace std;

int main(int argc, char** argv)
{
    saga::uuid u;
    saga::url d1("file://localhost/tmp/"+u.string());

    try 
    {
        std::cout << "If #640 is not fixed, the code below will produce an exception. " << std::endl;
        std::cout << "Please check: http://faust.cct.lsu.edu/trac/saga/ticket/640 for details." << std::endl;

        saga::filesystem::directory sfd1(d1, saga::filesystem::Create);
    
        saga::url d2("file://localhost/tmp/");
        saga::filesystem::directory sfd2(d2);
    
        sfd2.remove(d1);
    
        return 0;
    }
    catch(saga::exception const & e)
    {
        std::cerr << "\nFAILED: " << e.what() << std::endl;
        return 255;
    }
}
