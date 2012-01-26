//  Copyright (c) 2005-2008 Andre Merzky
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "saga-shell.hpp"

int shell::c_mv (std::vector <std::string> & args,
                 props & p)
{
  std::string tgt = args.back ();
  args.erase ((++args.rbegin()).base());

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

    if ( cwd_.is_dir (src) )
    {
      carp ("cannot copy directory: " + src, p);
      return FAILURE;
    }

    if (   cwd_.exists (tgt) && 
           ! cwd_.is_dir (tgt) )
    {
      carp ("already exists: " + tgt, p);
      return FAILURE;
    }

    // move the file
    cwd_.move (saga::url (src), saga::url (tgt));
  }

  return SUCCESS;
}

