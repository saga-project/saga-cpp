//  Copyright (c) 2005-2007 Andre Merzky <andre@merzky.net>
// 
//  Distributed under the Boost Software License, Version 1.0. 
//  (See accompanying file LICENSE file or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#define SAGA_LOG_FACILITY "my facility"

#include <saga/saga.hpp>

///////////////////////////////////////////////////////////////////////////////
int main (int argc, char* argv[])
{
  SAGA_LOG_CRITICAL ("critical message");
  SAGA_LOG_ERROR    ("error    message");
  SAGA_LOG_WARN     ("warning  message");
  SAGA_LOG_INFO     ("info     message");
  SAGA_LOG_DEBUG    ("debug    message");
  SAGA_LOG_BLURB    ("blurb    message");

  return 0;
}

