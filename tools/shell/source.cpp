//  Copyright (c) 2005-2008 Andre Merzky
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "saga-shell.hpp"

// open a file, read it line by line, and execute the contemts
int shell::c_source (std::vector <std::string> & args,
                     props & p)
{
  if ( args.empty () )
  {
    carp ("format: source <file> [...]", p);
    return FAILURE;
  }

  std::vector <std::string> :: iterator end   = args.end ();
  for (std::vector<std::string>::iterator iter = args.begin(); 
       iter != end; ++iter)
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
      carp ("Cannot source directory: " + arg, p);
      return FAILURE;
    }

    // open the current directory as filesystem dir
    saga::filesystem::directory dir (cwd_.get_url ());

    // open the file
    saga::filesystem::file file = dir.open (arg, saga::filesystem::Read);

    ssize_t size = file.get_size ();
    std::vector<saga::uint8_t> data(size);

// no need to initialize, the vector constructor invokes the default constructor 
// for all created elements
//     for ( ssize_t i = 0; i < size; ++i ) { data[i] = '\0'; }

    // read whole file into the buffer
    if ( size != file.read (saga::buffer(data)) )
    {
      carp ("could not read file", p);
      return FAILURE;
    }

    file.close ();

    bool echo = false;

    std::map<std::string, std::string>::iterator it = env_.find ("ECHO");
    if ( it != env_.end () && (*it).second != "0" )
    {
      echo = true;
    }

    std::string line;
    for ( ssize_t i = 0; i < size; i++ )
    {
      if ( data[i] == '\n')
      {
        // we have to read the echo status on each command
        std::map<std::string, std::string>::iterator it = env_.find("ECHO");

        if ( echo )
        {
          std::cout << line << std::endl;
        }
            
        execute (line, p);
        line = "";
      }
      else
      {
        line += data[i];
      }
    }

    // in case file does not end with newline
    if ( line != "" )
    {
      if ( echo )
      {
        std::cout << line << std::endl;
      }

      execute (line, p);
    }
  }

  return SUCCESS;
}

