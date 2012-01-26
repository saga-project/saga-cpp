//  Copyright (c) 2005-2008 Andre Merzky
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "saga-shell.hpp"

int shell::c_env (std::vector <std::string> & args, 
                  props & p)
{
  if ( !args.empty () )
  {
    carp ("format: env", p);
    return FAILURE;
  }

  std::map <std::string, std::string> :: iterator iter = env_.begin ();
  std::map <std::string, std::string> :: iterator end  = env_.end   ();

  for ( /**/; iter != end; ++iter )
  {
    p.out += iter->first;
    p.out += " \t ";
    p.out += iter->second;
    p.out += "\n";
  }

  return SUCCESS;
}

