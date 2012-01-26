//  Copyright (c) 2005-2008 Andre Merzky
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "saga-shell.hpp"

int shell::c_setenv (std::vector <std::string> & args,
                     props & p)
{
  // sanity checks
  if ( args.size () < 2)
  {
    carp ("format: setenv <key> <val> [...]", p);
    return FAILURE;
  }


  std::string key = args[0];
  std::string val = args[1];

  args.erase (args.begin ());
  args.erase (args.begin ());

  std::vector <std::string> :: iterator begin = args.begin ();
  std::vector <std::string> :: iterator end   = args.end ();
  std::vector <std::string> :: iterator iter;

  for ( iter = begin; iter != end; ++iter )
  {
    val += *iter;
    val += " ";
  }


  // export env setting
  if ( 0 != ::setenv (key.c_str (), val.c_str (), 1) )
  {
    carp ("cannot export into environment", p);
    return FAILURE;
  }


  // make setting available to the shell
  env_[key] = val;


  // update shell settings with new environment
  init ();

  return SUCCESS;
}

