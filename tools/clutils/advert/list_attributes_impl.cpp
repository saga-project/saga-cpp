//  Copyright (c) 2008-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga.hpp>
#include "saga-advert.hpp"

#if defined(SAGA_HAVE_PACKAGE_ADVERT) && defined(SAGA_HAVE_PACKAGE_ADVERT)
///////////////////////////////////////////////////////////////////////////////
namespace 
{
  template <typename Advert>
  struct print_attributes
  {
    Advert e_;

    print_attributes (Advert e) : e_(e) {}

    template <typename T>
    void operator()(T const& key) const
    {
      if ( e_.attribute_is_vector (key) )
      {
        std::cout << "  (vector) " << key << " \t:";

        std::vector <std::string>                 vec = e_.get_vector_attribute (key);
        std::vector <std::string>::const_iterator it  = vec.begin ();

        if ( it != vec.end () )
        {
          std::cout << (*it);
          ++it;
        }

        for ( ; it != vec.end (); ++it )
        {
          std::cout << ", " << (*it);
        }

        std::cout << std::endl;
      }
      else 
      {
        std::cout << "  (scalar) "  << key << " \t: " 
                  << e_.get_attribute (key) << std::endl;
      }
    }
  };

  template <typename Advert>
  inline void list_attributes (Advert a)
  {
    std::vector <std::string> keys (a.list_attributes ());
    std::for_each (keys.begin (), keys.end (), print_attributes <Advert> (a));
  }
}

///////////////////////////////////////////////////////////////////////////////
void advert_list_attributes (saga_tools::common & c,
                             std::string          aname)
{
  // instantiate name_space entry
  saga::url u (aname);
  saga::name_space::entry e (c.session (), u);

  if ( saga::object::Advert == e.get_type () )
  {
    // list attributes of entry
    list_attributes (saga::advert::entry (u));
  }
  else 
  {
    // list attributes of directory
    list_attributes (saga::advert::directory (u));
  }
}

#endif

