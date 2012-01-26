
//  Copyright (c) 2005-2007 Andre Merzky <andre@merzky.net>
// 
//  Distributed under the Boost Software License, Version 1.0. 
//  (See accompanying file LICENSE file or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga.hpp>
#include <boost/filesystem/path.hpp>

///////////////////////////////////////////////////////////////////////////////
int main (int argc, char* argv[])
{
  try
  {
    saga::url u ("gridftp://qb.loni.org/tmp");

    saga::filesystem::directory d (u);
    std::cout << u            << std::endl;
    std::cout << d.get_url () << std::endl;
  }
  catch ( const saga::exception & e )
  {
    std::cerr << "error: " << e.what () << std::endl;
  }

  return 0;

  try
  {

    saga::url tmp ("ssh://localhost/.com.apple.timemachine.supported");

    std::cout << tmp << std::endl;

    std::string url;

    if ( argc > 1 )
    {
      url = argv[1];
    }
    else
    {
      url = "any:///etc/passwd"; 
    }

    saga::filesystem::file f (url);

    std::cout <<   "size: " << f.get_size () << std::endl;

    std::cout <<   "a/b/..   "; std::cout <<   "a        "; saga::url u_0 ("a/b/.." ); std::cout << u_0.get_string () << "\n\n"; 
    std::cout <<   "a/b/.    "; std::cout <<   "a/b      "; saga::url u_1 ("a/b/."  ); std::cout << u_1.get_string () << "\n\n"; 
    std::cout <<   "a//b     "; std::cout <<   "a/b      "; saga::url u_2 ("a//b"   ); std::cout << u_2.get_string () << "\n\n"; 
    std::cout <<   "a/./b    "; std::cout <<   "a/b      "; saga::url u_3 ("a/./b"  ); std::cout << u_3.get_string () << "\n\n"; 
    std::cout <<   "/a/../b  "; std::cout <<   "/b       "; saga::url u_4 ("/a/../b"); std::cout << u_4.get_string () << "\n\n"; 
    std::cout <<   "a/../b   "; std::cout <<   "b        "; saga::url u_5 ("a/../b" ); std::cout << u_5.get_string () << "\n\n"; 
    std::cout <<   "/../a    "; std::cout <<   "/a       "; saga::url u_6 ("/../a"  ); std::cout << u_6.get_string () << "\n\n"; 
    std::cout <<   "../a/..  "; std::cout <<   "..       "; saga::url u_7 ("../a/.."); std::cout << u_7.get_string () << "\n\n"; 
  }
  catch ( const saga::exception & e )
  {
    std::cerr << "error: " << e.what () << std::endl;
  }

  return 0;
}

