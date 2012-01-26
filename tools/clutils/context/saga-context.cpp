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
//
// print all contexts of the default session.  If a specific type is given,
// print only contexts of that type.  If no matching context is found, return an
// error.
//
int main (int argc, char* argv[])
{
  std::string type = "";

  if ( argc > 1 )
  {
    type = argv[1];
  }

  try 
  {
    int n = 0;
    saga::session s;
    std::vector <saga::context> contexts = s.list_contexts ();

    for ( unsigned int i = 0; i < contexts.size (); i++ )
    {
      if ( type == "" ||
           type == contexts[i].get_attribute (saga::attributes::context_type) )
      {
        dump_context (contexts[i]);
        n++;
      }
    }

    if ( 0 == n )
    {
      return 1;
    }
  }
  catch ( const saga::exception & e )
  {
    std::cerr << e.what () << std::endl;
    return 2;
  }
  
  return 0; 
}

