
//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga/adaptors/serialization.hpp>


// we define the engine as a package here, to allow to load it via its own
// package loading mechanism.  That sounds strange, but really simplifies the
// code for serialization, as that needs to pull those packages for which it
// creates objects while de-serializing.  Some of these objects (session,
// context) are defined in the engine...

SAGA_PACKAGE_REGISTER(saga::impl::engine_serialization)


