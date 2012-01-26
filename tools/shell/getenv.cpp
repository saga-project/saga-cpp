//  Copyright (c) 2005-2008 Andre Merzky
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "saga-shell.hpp"

int shell::c_getenv (std::vector <std::string> & args, 
                     props & p)
{
  if ( args.empty () )
  {
    return c_env (args, p);
  }

  std::vector <std::string> :: iterator begin = args.begin ();
  std::vector <std::string> :: iterator end   = args.end ();
  std::vector <std::string> :: iterator iter;

  for ( iter = begin; iter != end; ++iter )
  {
    std::string key = *iter;

    if ( env_.find (key) != env_.end () )
    {
      p.out += env_[key];
      p.out += " ";
    }
    else
    {
      carp ("No such environment variable " + key, p);
      return FAILURE;
    }
  }

  p.out += "\n";

  return SUCCESS;
}

