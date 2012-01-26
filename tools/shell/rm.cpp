//  Copyright (c) 2005-2008 Andre Merzky
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "saga-shell.hpp"

int shell::c_rm (std::vector <std::string> & args,
                 props & p)
{
  std::vector <std::string> :: iterator begin = args.begin ();
  std::vector <std::string> :: iterator end   = args.end ();
  std::vector <std::string> :: iterator iter;

  for ( iter = begin; iter != end; ++iter )
  {
    std::string arg = *iter;

    // sanity checks
    if ( ! cwd_.exists (arg) )
    {
      carp ("No such file: " + arg, p);
      return FAILURE;
    }

    if ( cwd_.is_dir (arg) )
    {
      carp ("cannot remove directory: " + arg, p);
      return FAILURE;
    }

    // check with the user again if she REALLY wants to 
    // remove the file...
    std::cout << "Do you want to remove " << arg << "? [y/N] > " << std::flush;

    std::string input;

    getline (std::cin, input);

    if ( input == "y" || input == "Y" )
    {
      // remove it
      saga::url u(arg);
      cwd_.remove (u);
    }
  }

  return SUCCESS;
}

