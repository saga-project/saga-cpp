//  Copyright (c) 2005-2008 Andre Merzky
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "saga-shell.hpp"

int shell::c_unsetenv (std::vector <std::string> & args,
                       props & p)
{
  // sanity checks
  if ( 1 != args.size () )
  {
    carp ("format: unsetenv <var>", p);
    return FAILURE;
  }

  bool found = false;

  if ( env_.find (args[0]) != env_.end () )
  {
    // remove from provate env
    env_.erase (args[0]);

    // remove from process environment - ignore errors
    ::unsetenv (args[0].c_str ());

    found = true;
  }

  if ( ! found )
  {
    carp ("no such variable", p);
    return FAILURE;
  }

  return SUCCESS;
}

