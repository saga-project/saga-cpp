//  Copyright (c) 2005-2008 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>

#include <boost/lexical_cast.hpp>

#include <saga/saga.hpp>

int main (int    argc, 
          char * argv[])
{
  if ( argc > 2 )
  {
    std::cerr << "\n\tUsage: stream_server [url]\n"
              << "\n\tDefault url is 'any://localhost/'.\n" 
              << std::endl;
    return -2;
  }

  try 
  {
    // retrieve parameter values
    saga::url url ("any://localhost/");

    if ( argc > 1 )
    {
      url = argv[1];
    }

    std::cout << "Serving " << url << std::endl;

    // actual functionality
    saga::stream::server service (url);

    while ( 1 )
    {
      saga::stream::stream strm = service.serve ();

      std::cout << "Established connection from: " 
                << strm.get_url () 
                << std::endl;

      char buff[255];
      saga::ssize_t read_bytes = strm.read (saga::buffer(buff));
      strm.write (saga::buffer (buff, read_bytes));
    }
  } 
  catch ( saga::exception const & e )
  {
    std::cerr << "saga::exception caught: " << e.what () << std::endl;
    return -1;
  }

  return 0;
}

