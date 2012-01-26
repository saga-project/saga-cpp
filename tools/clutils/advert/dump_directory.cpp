//  Copyright (c) 2008 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <iomanip>

#include <saga/saga.hpp>

#include "saga-advert.hpp"

#if defined(SAGA_HAVE_PACKAGE_ADVERT) && defined(SAGA_HAVE_PACKAGE_ADVERT)

void print_dir_attribs (saga_tools::common & c, 
                        saga::url const    & u, 
                        std::string          indent = "    ")
{
  std::cout.setf (std::ios::left);

  saga::advert::directory ad (c.session (), u, saga::advert::Read);

  std::vector <std::string> keys (ad.list_attributes ());

  for ( std::size_t i = 0; i < keys.size (); i++ )
  {
    std::cout << indent << "  " << std::setw (10) << keys[i] << "\t: ";

    if ( ad.attribute_is_vector (keys[i]) )
    {
      std::vector <std::string> vals = ad.get_vector_attribute (keys[i]);

      for ( std::size_t j = 0; j < vals.size (); j++ )
      {
        std::cout << vals[j];

        if ( j < vals.size () - 1 ) 
        {
          std::cout << ", ";
        }
      }
      std::cout << std::endl;
    }
    else
    {
      std::cout << ad.get_attribute (keys[i]) << std::endl;
    }
  }

  return;
}


void print_entry_attribs (saga_tools::common & c, 
                          saga::url const    & u, 
                          std::string          indent = "    ")
{
  std::cout << indent + "  "   << u << std::endl;

  saga::advert::entry ad (c.session (), u, saga::advert::Read);

  std::vector <std::string> keys (ad.list_attributes ());

  for ( std::size_t i = 0; i < keys.size (); i++ )
  {
    std::cout << indent << "    " << std::setw (10) << keys[i] << "\t: ";

    if ( ad.attribute_is_vector (keys[i]) )
    {
      std::vector <std::string> vals = ad.get_vector_attribute (keys[i]);
      
      for ( std::size_t j = 0; j < vals.size (); j++ )
      {
        std::cout << vals[j];
       
        if ( j < vals.size () - 1 ) 
        {
          std::cout << ", ";
        }
      }
      std::cout << std::endl;
    }
    else
    {
      std::cout << ad.get_attribute (keys[i]) << std::endl;
    }
  }

  return;
}

///////////////////////////////////////////////////////////////////////////////
void advert_dump_directory (saga_tools::common & c,
                            std::string          uname, 
                            std::string          indent)
{
  using namespace saga;

  saga::url u (uname);

  // print self
  std::cout << indent << u << std::endl;
  print_dir_attribs (c, u, indent);

  // read advert entries
  advert::directory dir (c.session (), u);
  std::vector <saga::url> entries (dir.list ());

  // print dirs
  for ( unsigned int i = 0; i < entries.size (); i++ )
  {
    if ( dir.is_dir (entries[i]) )
    {
      saga::url next (u);

      std::string path = next.get_path ();

      if ( path[path.size() - 1] != '/' )
      {
        path += '/';
      }
      
      path += entries[i].get_string ();
      next.set_path (path);

      advert_dump_directory (c, next.get_string (), indent + "  ");
    }
  }

  // print entries
  for ( unsigned int i = 0; i < entries.size (); i++ )
  {
    if ( ! dir.is_dir (entries[i]) )
    {
      saga::url next (u);
      std::string path = next.get_path ();
      path += "/";
      path += entries[i].get_string ();
      next.set_path (path);

      print_entry_attribs (c, next, indent + "  ");
    }
  }
}

#endif

