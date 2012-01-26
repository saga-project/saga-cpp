//  Copyright (c) 2005-2008 Andre Merzky
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "saga-shell.hpp"

int shell::c_ln (std::vector <std::string> & args,
                 props & p)
{
  if ( args.size () < 2 )
  {
    carp ("format: ln <src> [...] <tgt>", p);
    return FAILURE;
  }

  std::string tgt = args.back ();
  args.erase ((++args.rbegin()).base());

  if ( args.size () > 2 )
  {
    if ( ! cwd_.is_dir (tgt) )
    {
      carp ("format: ln <src> [...] <tgt>", p);
      carp ("error:  for multiple sources, target must be a directory.", p);
      return FAILURE;
    }
  }

  std::vector <std::string> :: iterator begin = args.begin ();
  std::vector <std::string> :: iterator end   = args.end ();
  std::vector <std::string> :: iterator iter;

  for ( iter = begin; iter != end; ++iter )
  {
    std::string src = *iter;

    // sanity checks
    if ( ! cwd_.exists  (src) )
    {
      carp ("No such file: " + src, p);
      return FAILURE;
    }

    if (   cwd_.exists (tgt) && 
         ! cwd_.is_dir (tgt) )
    {
      carp ("already exists: " + tgt, p);
      return FAILURE;
    }

    // do!
    cwd_.link (saga::url (src), saga::url (tgt));
  }

  return SUCCESS;
}

