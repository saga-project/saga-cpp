//  Copyright (c) 2005-2008 Andre Merzky
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "saga-shell.hpp"

// open a file, and read chunks of data until we reach EOF, 
// printing the data found to stdout
int shell::c_cat (std::vector <std::string> & args, 
                  props & p)
{
  if ( args.empty () )
  {
    if ( p.in != "" )
    {
      p.out = p.in;
      p.in  = "";
    }
    else
    {
      carp ("format: cat <file> [...]");
      return FAILURE;
    }
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
      carp ("No such file: " + arg, p);
      return FAILURE;
    }

    if ( cwd_.is_dir (arg) )
    {
      carp ("Cannot cat directory: " + arg, p);
      return FAILURE;
    }

    // open the current directory as filesystem dir
    saga::filesystem::directory dir (cwd_.get_url ());

    // open the file
    saga::filesystem::file file = dir.open (arg, saga::filesystem::Read);

    while ( true )
    {
      size_t const n = 1024*64;
      char   data[n + 1];

      for ( unsigned int i = 0; i <= n; ++i ) { data[i] = '\0'; }

      // read a chunk into the buffer
      if ( file.read (saga::buffer (data, n), n) )
      {
        // show what we found
        p.out += data;
      }
      else
      {
        break;
      }
    }

    // file closes when going out of scope
  }

  return SUCCESS;
}

