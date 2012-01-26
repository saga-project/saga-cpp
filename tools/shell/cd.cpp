//  Copyright (c) 2005-2008 Andre Merzky
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "saga-shell.hpp"

// change the working directory
int shell::c_cd (std::vector <std::string> & args, 
                 props & p)
{
  if ( args.size () > 1 )
  {
    carp ("format: cd <dir>", p);
    return FAILURE;
  }

  // have a nice default
  std::string target;

  if ( ! args.empty () )
  {
    target = args[0];
  }
  else
  {
    target = home_.get_string ();
  }

  // we actually try a real cd.  If that fails, we try to create a new dir
  // instance with the arg, in case its switching to another namespace...
  // That's also why we don't bother too much about sanity checks at the moment.
  try
  {
    // do! :-)
    cwd_.change_dir (target);
  }

  catch ( const saga::exception & /*e*/ )
  {
    // lets try the fallback
    // FIXME: cd ssh://localhost/; cd .. ; 
    // this results in cwd.get_url () to be '../'.  
    // I wonder what cwd_.get_cwd () is :-)
    saga::name_space::directory new_cwd (target, saga::name_space::ReadWrite);
    cwd_ = new_cwd;
  }

  set_prompt ();

  return SUCCESS;
}

