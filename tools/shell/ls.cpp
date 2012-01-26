//  Copyright (c) 2005-2008 Andre Merzky
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "saga-shell.hpp"

// print a list of entries in the curent directory.  
// resolve symbolic links, and mark directories with /
int shell::c_ls (std::vector <std::string> & args,
                 props & p)
{
  if ( args.empty () )
  {
    args.push_back (".");
  }

  std::vector <std::string> :: iterator begin = args.begin ();
  std::vector <std::string> :: iterator end   = args.end ();
  std::vector <std::string> :: iterator iter;

  for ( iter = begin; iter != end; ++iter )
  {
    std::string arg = *iter;

    // get the list
    std::vector <saga::url> entries = cwd_.list (arg);

    // iterate over children
    for ( std::size_t i = 0; i < entries.size (); ++i )
    {
      p.out += "  ";
      p.out += saga::url::unescape(entries[i].get_string());

      // check if child is directory, mark it
      if ( cwd_.is_dir (entries[i]) )
      {
        p.out += "/";
      }

      // if child is link, print link target
      if ( cwd_.is_link (entries[i]) )
      {
        p.out += " -> ";
        p.out += saga::url::unescape(cwd_.read_link(entries[i]).get_string());
      }

      p.out += "\n";

      // FIXME: more could be done here, e.g. checking for file 
      // sizes, recursive listings etc.  That is left as an
      // excercise to the reader ;-)
    }
  }

  return SUCCESS;
}

