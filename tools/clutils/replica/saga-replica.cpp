//  Copyright (c) 2008 Ole Weidner
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <string>

#include <saga/saga.hpp>
#include "saga-replica.hpp"

#include <cstdlib>
#include <cstring>


///////////////////////////////////////////////////////////////////////////////

int main (int argc, char* argv[])
{
#if !defined(SAGA_HAVE_PACKAGE_REPLICA) || !defined(SAGA_HAVE_PACKAGE_NAMESPACE)
  std::cerr << "saga-replica: saga has been compiled without the replica or "
               "namespace packages, bailing out" << std::endl;
  return -1;
#endif

  saga_tools::common c;

  c.usage ().map ()["list_directory  "] = "<ldn url>                "; 
  c.usage ().map ()["add_directory   "] = "<ldn url>                "; 
  c.usage ().map ()["remove_directory"] = "<ldn url>                "; 
  c.usage ().map ()["add_lfn         "] = "<lfn url>                "; 
  c.usage ().map ()["remove_lfn      "] = "<lfn url>                "; 
  c.usage ().map ()["list_pfns       "] = "<lfn url>                "; 
  c.usage ().map ()["add_pfn         "] = "<lfn url>  <pfn>         "; 
  c.usage ().map ()["remove_pfn      "] = "<lfn url>  <pfn>         "; 
  c.usage ().map ()["list_attributes "] = "<lfn url>                "; 
  c.usage ().map ()["set_attribute   "] = "<lfn url>  <attr>  <val> "; 
  c.usage ().map ()["remove_attribute"] = "<lfn url>  <attr>        "; 

  c.init (argc, argv);
  c.need_args_min (1);

  std::vector <std::string> args = c.args ();

  try
  {
    std::string cmd = args[0];

    if ( cmd == "list_directory" )
    {
      c.need_args_min (1);

      for ( int i = 1; i <= c.nargs (); i++ )
      {
        replica_list_directory (c, args[i]);
      }
    }  


    else if ( cmd == "add_directory" )
    {
      c.need_args_min (1);

      for ( int i = 1; i <= c.nargs (); i++ )
      {
        replica_add_directory (c, args[i]);
      }
    }


    else if (cmd == "remove_directory" )
    {
      c.need_args_min (1);

      for ( int i = 1; i <= c.nargs (); i++ )
      {
        replica_remove_directory (c, args[i]);
      }
    }

    
    else if ( cmd == "list_pfns" )
    {
      c.need_args_min (1);

      for ( int i = 1; i <= c.nargs (); i++ )
      {
        replica_list_pfns (c, args[i]);
      }
    }

    
    else if ( cmd == "add_lfn" )
    {
      c.need_args_min (1);

      for ( int i = 1; i <= c.nargs (); i++ )
      {
        replica_add_lfn (c, args[i]);
      }
    }

    
    else if ( cmd == "remove_lfn" )
    {
      c.need_args_min (1);

      for ( int i = 1; i <= c.nargs (); i++ )
      {
        replica_remove_lfn (c, args[i]);
      }
    }

    
    else if ( cmd == "add_pfn" )
    {
      c.need_args_min (2);

      for ( int i = 1; i <= c.nargs (); i+=2 )
      {
        c.need_args_min (i + 1);
        replica_add_pfn (c, args[i], args[i+1]);
      }
    }

    
    else if ( cmd == "remove_pfn" )
    {
      c.need_args_min (2);

      for ( int i = 1; i <= c.nargs (); i+=2 )
      {
        c.need_args_min (i + 1);
        replica_remove_pfn (c, args[i], args[i+1]);
      }
    }

    
    else if ( cmd == "list_attributes" )
    {
      c.need_args_min (1);

      for ( int i = 1; i <= c.nargs (); i++ )
      {
        replica_list_attributes (c, args[i]);
      }
    }

    
    else if ( cmd == "set_attribute" )
    {
      c.need_args_min (3);

      for ( int i = 1; i <= c.nargs (); i+=3 )
      {
        c.need_args_min (i + 2);
        replica_set_attribute (c, args[i], args[i+1], args[i+2]);
      }
    }

    
    else if ( cmd == "remove_attribute" )
    {
      c.need_args_min (2);  

      for ( int i = 1; i <= c.nargs (); i+=2 )
      {
        c.need_args_min (i + 1);
        replica_remove_attribute (c, args[i], args[i+1]);
      }
    }

    
    else
    {
      c.print_usage ("Unknown command");
    }
  }
  catch ( const saga::exception & e )
  {
    c.fail (e);
  }
  

  return 0;
}

