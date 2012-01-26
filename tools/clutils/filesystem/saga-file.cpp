//  Copyright (c) 2008 Ole Weidner
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <string>

#include <saga/saga.hpp>
#include "saga-file.hpp"

#include <cstdlib>
#include <cstring>

#include <boost/lexical_cast.hpp>


///////////////////////////////////////////////////////////////////////////////

int main (int argc, char* argv[])
{
#if !defined(SAGA_HAVE_PACKAGE_FILESYSTEM) || !defined(SAGA_HAVE_PACKAGE_NAMESPACE)

  std::cerr << "saga-file: saga has been compiled without the file or "
               "namespace packages, bailing out" << std::endl;
  return -1;

#else

  saga_tools::common c;

  c.usage ().map ()["list_dir"] = "<url> [...]  ";
  c.usage ().map ()["copy"]     = "<src>  <tgt> ";
  c.usage ().map ()["link"]     = "<src>  <tgt> ";
  c.usage ().map ()["move"]     = "<src>  <tgt> ";
  c.usage ().map ()["remove"]   = "<url> [...]  ";
  c.usage ().map ()["cat"]      = "<url> [...]  ";
  c.usage ().map ()["get_size"] = "<url> [...]  ";
  c.usage ().map ()["is_link"]  = "<url> [...]  ";

  c.init (argc, argv);
  c.need_args_min (1);

  std::vector <std::string> args = c.args ();


  try
  {
    std::string cmd = args[0];

    if ( cmd == "list_dir" )
    {
      c.need_args_min (1);

      for (  int i = 1; i <= c.nargs (); i++ )
      {
        file_list_dir (c, args[i]);
      }
    }  

    
    else if ( cmd == "find_dir" )
    {
      c.need_args (2);

      file_find_dir (c, args[1], args[2]);
    }


    else if ( cmd == "run_tests" )
    {
      c.need_args_min (2);

      for ( int i = 1; i <= c.nargs (); i++ )
      {
        file_run_test (c, args[i]);
      }
    }  


    else if ( cmd == "copy" )
    {
      c.need_args (2);

      file_copy (c, args[1], args[2]);
    }  


    else if ( cmd == "link" )
    {
      c.need_args (2);

      file_link (c, args[1], args[2]);
    }  


    else if ( cmd == "move" )
    {
      c.need_args (2);

      file_move (c, args[1], args[2]);
    }  


    else if ( cmd == "remove" )
    {
      c.need_args_min (1);

      for (  int i = 1; i <= c.nargs (); i++ )
      {
        file_remove (c, args[i]);
      }
    }  


    else if ( cmd == "cat" )
    {
      c.need_args_min (1);

      for (  int i = 1; i <= c.nargs (); i++ )
      {
        file_cat (c, args[i]);
      }
    }  


    else if ( cmd == "is_link" )
    {
      c.need_args_min (1);

      for (  int i = 1; i <= c.nargs (); i++ )
      {
        file_is_link (c, args[i]);
      }
    }  


    else if ( cmd == "get_size" )
    {
      c.need_args_min (1);

      for (  int i = 1; i <= c.nargs (); i++ )
      {
        file_get_size (c, args[i]);
      }
    }  


    else
    {
      c.print_usage ("Unkown command");
    }
  }
  catch ( const saga::exception & e )
  {
    c.fail (e);
  }

  return 0;

#endif

}

