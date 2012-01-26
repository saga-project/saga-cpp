//  Copyright (c) 2008, 2009 João Abecasis
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga/adaptors/utils/is_local_address.hpp>

#include <boost/assert.hpp>

#include <iostream>
#include <string>

bool test_address(std::string const & addr)
{
    using saga::adaptors::utils::is_local_address;

    bool is_alias;

    std::cout << " * " << addr << ((is_alias = is_local_address(addr))
            ? ": resolves to localhost.\n"
            : ": does NOT resolve to localhost.\n");

    return is_alias;
}

int main(int argc, char ** argv)
{
    if (argc >= 2)
        for (int i = 1; i < argc; ++i)
            test_address(argv[i]);
    else
    {
        std::cout << "No arguments specified. Entering self-test mode.\n"
            "  Usage: " << argv[0] << " [list of addresses]\n\n"
            << std::flush;

        BOOST_ASSERT(test_address("localhost"));
        BOOST_ASSERT(test_address("127.0.0.1"));

        BOOST_ASSERT(!test_address("example.com"));
    }
}
