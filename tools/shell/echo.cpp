//  Copyright (c) 2005-2008 Andre Merzky
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "saga-shell.hpp"

// open a file, and read chunks of data until we reach EOF, 
// printing the data found to stdout
int shell::c_echo (std::vector <std::string> & args, 
                   props & p)
{
  if ( args.empty () )
  {
    carp ("format: echo <string> [...]", p);
    return FAILURE;
  }

  std::vector <std::string> :: iterator begin = args.begin ();
  std::vector <std::string> :: iterator end   = args.end ();
  std::vector <std::string> :: iterator iter;

  for ( iter = begin; iter != end; ++iter )
  {
    p.out += (*iter);
    p.out += " ";
  }

  p.out += "\n";

  return SUCCESS;
}

