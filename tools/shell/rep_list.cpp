//  Copyright (c) 2005-2008 Andre Merzky
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "saga-shell.hpp"

// list all known replicas on a logical file
int shell::c_rep_list (std::vector <std::string> & args,
                       props & p)
{
#if !defined(SAGA_HAVE_PACKAGE_REPLICA) || !defined(SAGA_HAVE_PACKAGE_NAMESPACE)
    carp("saga has been compiled without the replica package, unable to use this command");
    return FAILURE;
#else
  if ( args.empty () )
  {
    carp ("format: rep_list <logical_file> [...]", p);
    return FAILURE;
  }

  std::vector <std::string> :: iterator begin = args.begin ();
  std::vector <std::string> :: iterator end   = args.end ();
  std::vector <std::string> :: iterator iter;

  for ( iter = begin; iter != end; ++iter )
  {
    std::string arg = *iter;

    // sanity checks
    if ( ! cwd_.exists (arg) )
    {
      carp ("No such logical file: " + arg, p);
      return FAILURE;
    }

    if ( cwd_.is_dir (arg) )
    {
      carp ("Cannot list replicas of directory: " + arg, p);
      return FAILURE;
    }

    // open the current directory as logical dir
    saga::replica::logical_directory dir (cwd_.get_url ());

    // open the logical file
    saga::replica::logical_file lf
      = dir.open (arg, saga::replica::Read);

    std::vector <saga::url> list = lf.list_locations ();

    p.out += "\n";
    p.out += arg;
    p.out += "\n";

    // iterate over replica locations
    for ( std::size_t i = 0; i < list.size (); ++i )
    {
      p.out += " \t " ;
      p.out += saga::url::unescape(list[i].get_string());
      p.out += "\n";
    }

    p.out += "\n";
  }

  return SUCCESS;
#endif
}

