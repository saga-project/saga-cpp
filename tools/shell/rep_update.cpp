//  Copyright (c) 2005-2008 Andre Merzky
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "saga-shell.hpp"

// update a given replica location
int shell::c_rep_update (std::vector <std::string> & args,
                         props & p)
{
#if !defined(SAGA_HAVE_PACKAGE_REPLICA) || !defined(SAGA_HAVE_PACKAGE_NAMESPACE)
    carp("saga has been compiled without the replica package, unable to use this command");
    return FAILURE;
#else
  if ( args.size () != 3 )
  {
    carp ("format: rep_update <logical_file> <old_url> <new_url>", p);
    return FAILURE;
  }

  std::string src  = args[0];
  std::string url1 = args[1];
  std::string url2 = args[2];

  // sanity checks
  if ( ! cwd_.exists (src) )
  {
    carp ("No such logical file: " + src, p);
    return FAILURE;
  }

  if ( cwd_.is_dir (src) )
  {
    carp ("Cannot update replica locations on a directory: " + src, p);
    return FAILURE;
  }

  // open the current directory as logical dir
  saga::replica::logical_directory dir (cwd_.get_url ());

  // open the logical file
  saga::replica::logical_file lf = 
         dir.open (src, saga::replica::Write);

  // update the replica location
  lf.update_location (saga::url (url1), 
                      saga::url (url2));

  // logical file and dir close when going out of scope
  return SUCCESS;
#endif
}

