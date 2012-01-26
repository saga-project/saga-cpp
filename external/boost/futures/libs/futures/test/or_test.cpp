//-----------------------------------------------------------------------------
// boost-libs futures/test/or_test.cpp source file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2005
// Thorsten Schuett
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <boost/test/minimal.hpp>

#if BOOST_MSVC != 0
#pragma warning (disable: 4355)   // this is used in member initialiser list
#endif

#include <boost/bind.hpp>
#include <boost/futures.hpp>

int fac(int n)
{
    if(n == 0)
        return 0;
    if(n == 1)
        return 1;
    return n * fac(n - 1);
}

double fac8()
{
    return (double)fac(8);
}

struct fac_functor
{
    fac_functor(int n) : n(n) {}
    
    int operator()()
    {
        return fac(n);
    } 
    
    int n;
};

int test_main(int , char* [])
{
    using namespace boost::futures;
    using namespace boost;

    future<int> f1 = bind(fac, 4);
    future<double> f2 = fac8;
    future<int> f3 = fac_functor(6);

    variant<int, double> v = futurize(f1 || f2 || f3); 

    std::cout << v.which() << ": " << v << std::endl;
    BOOST_CHECK((0 == v.which() && boost::get<int>(v) == 24) || 
                (0 == v.which() && boost::get<int>(v) == 720) ||
                (1 == v.which() && boost::get<double>(v) == 40320.0));
        
    BOOST_CHECK(f1() == 24);
    BOOST_CHECK(f2() == 40320);
    BOOST_CHECK(f3() == 720);

    return boost::exit_success;
}
