//  Copyright (c) 2005-2008 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>

#include <saga/saga.hpp>

int main (int    argc, 
          char * argv[])
{
  if ( argc > 2 ) 
  {
    std::cerr << "\n\tUsage: stream_client [url]\n"
              << "\n\tDefault url is 'any://localhost/'.\n" 
              << std::endl;
    return -2;
  }

  try 
  {
    // retrieve parameter values
    std::string url ("any://localhost/");

    if ( argc > 1 )
    {
      url = argv[1];
    }

    std::cout << "Connecting to " << url << std::endl;

    saga::stream::stream strm (url);
    strm.connect ();

    std::cout << "Connected!" << std::endl;

    std::string msg ("test from client\n");
    std::cout << "Sending : " << msg;
    strm.write (saga::buffer (msg));

    int act = strm.wait (saga::stream::Read, 0.0);

    if ( act & saga::stream::Read )
    {
      char buff[255];
      saga::ssize_t read_bytes = strm.read (saga::buffer (buff));

      std::cout << "Received: " << std::string (buff, read_bytes) << std::endl;
    }

    strm.close ();

  } 
  catch ( saga::exception const & e )
  {
    std::cerr << "saga::exception caught: " << e.what () << std::endl;
    return -1;
  }
  
  return 0;
}

