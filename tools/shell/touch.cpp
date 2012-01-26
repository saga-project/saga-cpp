//  Copyright (c) 2005-2008 Andre Merzky
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "saga-shell.hpp"

int shell::c_touch (std::vector <std::string> & args,
                    props & p)
{
  // sanity checks
  if ( args.empty () )
  {
    carp ("format: touch <file> [...]", p);
    return FAILURE;
  }

  std::vector <std::string> :: iterator begin = args.begin ();
  std::vector <std::string> :: iterator end   = args.end ();
  std::vector <std::string> :: iterator iter;

  for ( iter = begin; iter != end; ++iter )
  {
    std::string arg = *iter;

    if ( cwd_.exists (arg) )
    {
      // nothing to do
      return SUCCESS;
    }


    saga::name_space::directory d (cwd_.get_url ());
    saga::name_space::entry f = d.open (arg, saga::filesystem::Create);
  }
 
  return SUCCESS;
}

