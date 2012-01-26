//  Copyright (c) 2008 Ole Weidner
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <vector>

#include <saga/saga.hpp>
#include "saga-file.hpp"

///////////////////////////////////////////////////////////////////////////////
#if defined(SAGA_HAVE_PACKAGE_FILESYSTEM) && defined(SAGA_HAVE_PACKAGE_NAMESPACE)

void file_list_dir (saga_tools::common & c, 
                    std::string directoryURL)
{
    saga::filesystem::directory dir (c.session (), 
                                     directoryURL);
    std::vector<saga::url> entries = dir.list();

    std::cout << dir.get_url () << ":" << std::endl;

    for ( std::size_t i = 0; i < entries.size (); ++i )
    {
        std::cout <<  "  " << entries[i].get_string ();

        // check if child is directory, mark it
        //if ( dir.is_dir (entries[i]) )
        //{
        //    std::cout << "/";
        //}

        // if child is link, print link target
        //if ( dir.is_link (entries[i]) )
        //{
        //    std::cout <<  " -> ";
        //    std::cout <<  dir.read_link (entries[i]).get_string ();
        //}

        std::cout << std::endl;
    }
}

#endif

