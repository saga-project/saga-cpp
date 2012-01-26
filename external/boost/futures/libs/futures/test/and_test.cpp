//-----------------------------------------------------------------------------
// boost-libs futures/test/and_test.cpp source file
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
  using namespace boost::fusion;
  using namespace boost;


  future<int> f1 = bind(fac, 4);
  future<int> f2 = bind(fac, 8);
  tuple<int, int> t = futurize(f1 && f2);

  BOOST_CHECK(   (get<0>(t) == 24)     
	      && (get<1>(t) == 40320));

  return boost::exit_success;
}
