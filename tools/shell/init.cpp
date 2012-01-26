//  Copyright (c) 2005-2008 Andre Merzky
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "saga-shell.hpp"

// re-init the session, which should cause all adaptors to be reloaded.
// Note that the old session may linger around until all async ops completed.
// But then again, we don't use async calls at the moment ;-)
int shell::c_init (std::vector <std::string> & args,
                   props & p)
{
  if ( ! args.empty () )
  {
    carp ("format: init", p);
    return FAILURE;
  }

  // create the new session.  This loads new adaptors etc.
  saga::session new_session = saga::get_default_session();

  // if the above fails, and throws, we keep the old session: the line below
  // would not be executed, bt the exception gets catched by the shell
  session_ = new_session;

  // re-init js, cwd etc.
  init ();

  return SUCCESS;
}

