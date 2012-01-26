//  Copyright (c) 2005-2008 Andre Merzky
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "saga-shell.hpp"

int shell::c_mkdir (std::vector <std::string> & args,
                    props & p)
{
  if ( args.size () != 1 )
  {
    carp ("format: mkdir <tartget> [...]", p);
    return FAILURE;
  }

  std::vector <std::string> :: iterator begin = args.begin ();
  std::vector <std::string> :: iterator end   = args.end ();
  std::vector <std::string> :: iterator iter;

  for ( iter = begin; iter != end; ++iter )
  {
    std::string arg = *iter;


    // sanity checks
    if ( cwd_.exists (arg) )
    {
      carp ("Already exists: " + arg, p);
      return FAILURE;
    }

    // create the directory
    saga::filesystem::directory d (cwd_.get_url ());

    d.open_dir (arg, saga::filesystem::Create);
  }

  return SUCCESS;
}

