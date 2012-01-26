//  Copyright (c) 2005-2008 Andre Merzky
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "saga-shell.hpp"

// list all known replicas on a logical file
int shell::c_rep_find (std::vector <std::string> & args,
                       props & p)
{
#if !defined(SAGA_HAVE_PACKAGE_REPLICA) || !defined(SAGA_HAVE_PACKAGE_NAMESPACE)
    carp("saga has been compiled without the replica package, unable to use this command");
    return FAILURE;
#else
  if ( args.size () < 1 )
  {
    carp ("format: rep_find <pat> [<key=val>] [...]", p);
    return FAILURE;
  }

  std::string pattern = args[0];
  args.erase (args.begin());

  // open the current directory as logical dir
  saga::replica::logical_directory dir (cwd_.get_url ());

  std::vector <saga::url> list = dir.find (pattern, args);

  p.out += "\n";

  // iterate over replica locations
  for ( std::size_t i = 0; i < list.size (); ++i )
  {
    p.out += "  ";
    p.out += saga::url::unescape(list[i].get_string());
    p.out += "\n";
  }

  p.out += "\n";

  return SUCCESS;
#endif
}

