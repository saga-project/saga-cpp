//  Copyright (c) 2005-2007 Andre Merzky <andre@merzky.net>
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE file or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga.hpp>

#define COUNT 100

void dump_context (saga::context c)
{
  std::vector <std::string> attribs = c.list_attributes ();

  std::cout << " ------------------------------ " << std::endl;

  for ( unsigned int i = 0; i < attribs.size (); i++ )
  {
    std::cout << " " << attribs[i] << " \t: ";

    if ( c.attribute_is_vector (attribs[i]) )
    {
      std::vector <std::string> vals = c.get_vector_attribute (attribs[i]);

      for ( unsigned int j = 0; j < vals.size (); j++ )
      {
        std::cout << vals[j] << " ";
      }
    }
    else
    {
      std::string val = c.get_attribute (attribs[i]);
      std::cout << val << " ";
    }

    std::cout << std::endl;
  }

  std::cout << " ------------------------------ " << std::endl;

}

///////////////////////////////////////////////////////////////////////////////
int main (int argc, char* argv[])
{
  std::string type = "ssh";

  if ( argc > 1 )
  {
    type = argv[1];
  }

  try 
  {
    saga::context c (type);

    // c.set_attribute ("UserID", "root");
    dump_context (c);

    saga::session s;
    s.add_context (c);

    dump_context (c);

  }
  catch ( const saga::exception & e )
  {
    std::cerr << e.what () << std::endl;
  }
  
  return 0; 
}

