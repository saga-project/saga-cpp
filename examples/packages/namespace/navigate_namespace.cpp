//  Copyright (c) 2005-2008 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga.hpp>

int main(int argc, char *argv[])
{
    try {
        saga::name_space::entry ns(saga::url(argv[1]), saga::filesystem::Read);
    
        std::cout << "Entry: " << argv[1] << " is "
                  << (ns.is_entry() ? "" : "not ") << "an entry.\n";
        std::cout << "Entry: " << argv[1] << " is "
                  << (ns.is_dir() ? "" : "not ") << "a directory.\n";
        
        std::cout << "Entry: " << argv[1] << " is a saga::" 
                  << saga::get_object_type_name(ns.get_type()) 
                  << std::endl;
    }
    catch (std::exception const& e) {
        std::cerr << "saga exception caught: " << e.what () << std::endl;
        return -1;
    }
    catch (...) {
        std::cerr << "unexpected exception caught" << std::endl;
        return -1;
    }
    return 0;
}
