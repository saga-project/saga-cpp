//-----------------------------------------------------------------------------
// boost-libs futures/test/join_test.cpp source file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2005
// Thorsten Schuett
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/minimal.hpp>

#include <boost/futures.hpp>

int fac(int n){
  if(n == 0)
    return 0;
  if(n == 1)
    return 1;
  return n * fac(n - 1);
}

int test_main(int , char* [])
{
  using namespace boost::futures;
  using namespace boost;


  future<int> f = bind(fac, 4);

  BOOST_CHECK(f() == 24);
  BOOST_CHECK(f == 24);

  return boost::exit_success;
}
