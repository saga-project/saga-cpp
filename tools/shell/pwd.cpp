//  Copyright (c) 2005-2008 Andre Merzky
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "saga-shell.hpp"

// print the current directory
int shell::c_pwd (std::vector <std::string> & args,
                  props & p)
{
  if ( ! args.empty () )
  {
    carp ("format: pwd", p);
    return FAILURE;
  }

  // easy... ;-)
  p.out += saga::url::unescape(cwd_.get_url().get_string());
  p.out += "\n";

  return SUCCESS;
}

