//
//  Copyright (c) 2008 João Abecasis
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//
// This is a compile time test, to make sure there is no ambiguity when using
// C-strings and std::strings as URLs.
//

#include <saga/saga/url.hpp>
#include <saga/saga/packages/namespace/namespace_dir.hpp>

#include <boost/assert.hpp>

#include <iostream>
#include <string>

template <class T>
void namespace_directory_test(T const & url)
{
    BOOST_ASSERT(false && "This is strictly a compile-time test."
        "Test function is not supposed to be called, it might harm kittens.");

    saga::name_space::directory dir;

    dir.change_dir(url);

    dir.read_link(url);
    dir.exists(url);
    dir.is_dir(url);
    dir.is_entry(url);
    dir.is_link(url);

    dir.copy(url, url, 1);
    dir.link(url, url, 1);
    dir.move(url, url, 1);
    dir.remove(url, 1);

    dir.open(url, 1);
    dir.open_dir(url, 1);

    dir.permissions_allow(url, std::string(), 1, 1);
    dir.permissions_deny(url, std::string(), 1, 1);
}

void compile_time_test()
{
    namespace_directory_test(saga::url());
    namespace_directory_test(std::string());
    namespace_directory_test("");
}

int main()
{
    std::cout << "This is a compile-time test. If you've got this far...\n"
        "Test PASSED!\n" << std::flush;
}

