//  Copyright (c) 2005-2008 Andre Merzky
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "saga-shell.hpp"

// trivial: create a shell instance and run it!
int main (int argc, char* argv[], char* envp[])
try 
{
#if !defined(SAGA_HAVE_PACKAGE_FILESYSTEM) || !defined(SAGA_HAVE_PACKAGE_NAMESPACE)
    std::cerr << "saga-shell: saga has been compiled without the filesystem or "
                 "namespace packages, bailing out" << std::endl;
#else
    shell sh;
    sh.run (argc, argv, envp);
#endif
    return 0;
}
catch ( std::exception const & e ) 
{
  std::cerr << "exception caught: " << e.what () << std::endl;
}
catch (...) 
{
  std::cerr << "unexpected exception caught" << std::endl;
}

