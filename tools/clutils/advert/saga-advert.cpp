//  Copyright (c) 2008-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <string>

#include <saga/saga.hpp>
#include "saga-advert.hpp"

#include <cstdlib>
#include <cstring>

///////////////////////////////////////////////////////////////////////////////

int main (int argc, char* argv[])
{
#if !defined(SAGA_HAVE_PACKAGE_ADVERT) || !defined(SAGA_HAVE_PACKAGE_NAMESPACE)

    std::cerr << "saga-advert: saga has been compiled without the advert or "
                 "namespace packages, bailing out" << std::endl;
    return -1;

#else

    saga_tools::common c;

    c.usage ().map ()["list_directory"   ] = "<advert-url> <pattern>   ";
    c.usage ().map ()["dump_directory"   ] = "<advert-url>             ";
    c.usage ().map ()["add_directory"    ] = "<advert-url>             ";
    c.usage ().map ()["remove_directory" ] = "<advert-url>             ";
    c.usage ().map ()["add_entry"        ] = "<advert-url>             ";
    c.usage ().map ()["remove_entry"     ] = "<advert-url>             ";
    c.usage ().map ()["store_string"     ] = "<advert-url> <string>    ";
    c.usage ().map ()["retrieve_string"  ] = "<advert-url>             ";
    c.usage ().map ()["list_attributes"  ] = "<advert-url>             ";
    c.usage ().map ()["set_attribute"    ] = "<advert-url> <attr> <val>";
    c.usage ().map ()["get_attribute"    ] = "<advert-url> <attr>      ";
    c.usage ().map ()["remove_attribute "] = "<advert-url> <attr>      ";

    c.init (argc, argv);
    c.need_args_min (1);

    std::vector <std::string> args = c.args ();

    try 
    {
      std::string cmd = args[0];

      if ( cmd == "list_directory" )
      {
        c.need_args_min (1);

        std::string pat = "*";  // default pattern

        // collect pattern into single string
        if ( c.nargs () > 1 )
        {
          pat = args[2];

          for ( int i = 3; i <= c.nargs (); i++ )
          {
            pat += " " + args[i];
          }
        }

        advert_list_directory (c, args[1], pat);
      }


      else if ( cmd == "add_directory" )
      {
        c.need_args_min (1);

        for ( int i = 1; i <= c.nargs (); i++ )
        {
          advert_add_directory (c, args[i]);
        }
      }


      else if ( cmd == "dump_directory" )
      {
        c.need_args_min (1);

        for ( int i = 1; i <= c.nargs (); i++ )
        {
          advert_dump_directory (c, args[i]);
        }  
      }


      else if ( cmd == "add_entry" )
      {
        c.need_args_min (1);

        for ( int i = 1; i <= c.nargs (); i++ )
        {
          advert_add_entry (c, args[i]);
        }
      }


      else if ( cmd == "remove_entry" )
      {
        c.need_args_min (1);

        for ( int i = 1; i <= c.nargs (); i++ )
        {
          advert_remove_entry (c, args[i]);
        }
      }


      else if ( cmd == "remove_directory" )
      {
        c.need_args_min (1);

        for ( int i = 1; i <= c.nargs (); i++ )
        {
          advert_remove_directory (c, args[i]);
        }
      }


      else if ( cmd == "store_string" )
      {
        c.need_args_min (2);

        for ( int i = 1; i <= c.nargs (); i+=2 )
        {
          c.need_args_min (i + 2);
          advert_store_string (c, args[i], args[i+1]);
        }
      }


      else if ( cmd == "retrieve_string" )
      {
        c.need_args_min (1);

        for ( int i = 1; i <= c.nargs (); i++ )
        {
          advert_retrieve_string (c, args[i]);
        }
      }


      else if ( cmd == "find_entries" )
      {
        c.need_args_min (1);

        for ( int i = 1; i <= c.nargs (); i++ )
        {
          advert_list_attributes (c, args[i]);
        }
      }


      else if ( cmd == "list_attributes" )
      {
        c.need_args_min (1);

        for ( int i = 1; i <= c.nargs (); i++ )
        {
          advert_list_attributes (c, args[i]);
        }
      }


      else if ( cmd == "set_attribute" )
      {
        c.need_args_min (3);

        for ( int i = 1; i <= c.nargs (); i+=3 )
        {
          c.need_args_min (i + 2);
          advert_set_attribute (c, args[i], args[i+1], args[i+2]);
        }
      }


      else if ( cmd == "get_attribute" )
      {
        c.need_args_min (2);

        for ( int i = 1; i <= c.nargs (); i+=3 )
        {
          c.need_args_min (i + 1);
          advert_get_attribute (c, args[i], args[i+1]);
        }
      }


      else if ( cmd == "remove_attribute" )
      {
        c.need_args_min (2);  

        for ( int i = 1; i <= c.nargs (); i+=2 )
        {
          c.need_args_min (i + 1);
          advert_remove_attribute (c, args[i], args[i+1]);
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

#endif

}

