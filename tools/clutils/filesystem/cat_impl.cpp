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

void file_cat (saga_tools::common & c, 
               std::string fileURL)
{
    saga::filesystem::file f (c.session (), 
                              saga::url (fileURL), 
                              saga::filesystem::Read);

    while (true)
    {
        saga::size_t const n = 1024*64;
        saga::uint8_t data[n+1];

        std::memset(data, n+1, '\0');

        // read a chunk into the buffer
        if ( f.read (saga::buffer (data, n), n) )
        {
            // show what we found
            std::cout << data;
        }
        else
        {
            break;
        }
    }
}

#endif

