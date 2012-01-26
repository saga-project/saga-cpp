#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/self.cpp")
#endif
//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif

//  this is needed in every file including detail/steerable_impl.hpp and not 
//  belonging to the engine
#define SAGA_NO_EXPORT_STEERABLE

#include <cstdlib>

// include job API's and implementation
#include <saga/saga/job.hpp>
#include <saga/saga/adaptors/task.hpp>
#include <saga/saga/adaptors/attribute.hpp>
#include <saga/impl/job.hpp>

#include <saga/saga/detail/steerable_impl.hpp>

#ifdef SAGA_DEBUG
#include <saga/saga/packages/self/preprocessed/self.cpp>
#else

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/self.cpp")
#endif

#if !defined(MAX_PATH)
#define MAX_PATH _POSIX_PATH_MAX
#endif

///////////////////////////////////////////////////////////////////////////////
namespace 
{
    ///////////////////////////////////////////////////////////////////////////
    // wrapper for gethostname()
    inline std::string get_hostname()
    {
        char buffer[MAX_PATH] = { '\0' };
        gethostname(buffer, sizeof(buffer));
        return std::string(buffer);
    }

///////////////////////////////////////////////////////////////////////////////
}
    
///////////////////////////////////////////////////////////////////////////////
namespace saga 
{
   namespace job {
    self::self (void) 
    {
    }
    
    self::self (saga::impl::job* impl) 
    :   job(impl)
    {
    }
    
    self::~self (void)
    {
    }
  }

  namespace detail
  {
    ///////////////////////////////////////////////////////////////////////////
    //  implement the attribute functions (we need to explicitly specialize 
    //  the template because the functions are not implemented inline)
    template struct SAGA_JOB_PACKAGE_EXPORT steerable<job::self>;
  }

///////////////////////////////////////////////////////////////////////////////
} // namespace saga

#endif

