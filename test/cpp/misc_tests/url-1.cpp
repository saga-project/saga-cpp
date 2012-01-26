//
//  Copyright (c) 2008 João Abecasis
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <saga/saga.hpp>
#include <iostream>

int main()
{
    saga::url u1("http://example.com"), u2 = u1, u3;

    std::cout << "u1: " << u1 << std::endl;
    std::cout << "u2: " << u2 << std::endl;
    std::cout << "u3: " << u3 << std::endl;

    std::cout << "Resetting u2." << std::endl;
    u2 = "condor://localhost";

    std::cout << "u1: " << u1 << std::endl;
    std::cout << "u2: " << u2 << std::endl;
    std::cout << "u3: " << u3 << std::endl;

    std::cout << "u1.get_host(): " << u1.get_host() << std::endl;
    std::cout << "u2.get_host(): " << u2.get_host() << std::endl;
    std::cout << "u3.get_host(): " << u3.get_host() << std::endl;

    std::cout << "Resetting all hosts to localhost." << std::endl;
    u1.set_host("localhost");
    u2.set_host("localhost");
    u3.set_host("localhost");

    std::cout << "u1.get_host(): " << u1.get_host() << std::endl;
    std::cout << "u2.get_host(): " << u2.get_host() << std::endl;
    std::cout << "u3.get_host(): " << u3.get_host() << std::endl;

}

