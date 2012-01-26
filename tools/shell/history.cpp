//  Copyright (c) 2005-2008 Andre Merzky
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "saga-shell.hpp"

// open a file, and read chunks of data until we reach EOF,
// printing the data found to stdout
int shell::c_history (std::vector <std::string> & args,
                      props & p)
{
  if ( ! args.empty () )
  {
    carp ("format: history", p);
    return FAILURE;
  }

#ifndef SAGA_HAVE_HISTORY_LIST

  carp ("Sorry, your libreadline does not support history listing", p);
  return FAILURE;

#else // SAGA_HAVE_HISTORY_LIST

  HIST_ENTRY ** hist = history_list ();

  if ( hist )
  {
    for ( int i = 0; hist[i]; i++ )
    {
      p.out += (i + history_base);
      p.out += ":";
      p.out += hist[i]->line;
      p.out += "\n";
    }
  }

  return SUCCESS;
#endif // SAGA_HAVE_HISTORY_LIST

}

