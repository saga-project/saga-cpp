#ifndef SAGA_PACKAGES_CPR_JOB_SELF_HPP
#define SAGA_PACKAGES_CPR_JOB_SELF_HPP

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/job_self.hpp")
#endif
//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2007 Ole Weidner (oweidner@cct.lsu.edu)
//  Copyright (c) 2008 Andre Merzky (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif
#include <string>
#include <vector>

// include dependent spec sections
#include <saga/saga/util.hpp>
#include <saga/saga/base.hpp>
#include <saga/saga/types.hpp>
#include <saga/saga/session.hpp>
#include <saga/saga/call.hpp>

#include <saga/saga/packages/cpr/config.hpp>
#include <saga/saga/packages/cpr/cpr_job.hpp>
#include <saga/saga/detail/steerable.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable: 4251 4231 4275 4660)
#endif

#ifdef SAGA_DEBUG
#include <saga/saga/packages/cpr/preprocessed/cpr_job_self.hpp>
#else

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/cpr_job_self.hpp")
#endif
///////////////////////////////////////////////////////////////////////////////
namespace saga
{
  namespace cpr 
  {
    
    /*! \brief The job_self class %is %a job which represents the current application.     
     *
     *   It can only by created by calling get_self() on a job service (that
     * call can fail though). The motivation to introduce this class is twofold:
     * (1) it allows to actively handle the current application as a grid job (e.g.
     * to migrate it, or to obtain its job description for cloning/spawning); (2)
     * as the class implements the steerable interface, it is possible to add
     * ReadWrite metrics to its instance - that way it is possible to expose these
     * metrics to other external applications, which in fact allows to steer the current
     * application. 
     */
    class SAGA_CPR_PACKAGE_EXPORT self 
        : public saga::cpr::job,
          public saga::detail::steerable<self>
    {
        /// @cond
        /** These methods are not within API scope */
        friend struct saga::detail::steerable <self>;   
        /// @endcond
        
    protected:
        /// @cond
        /** These methods are not within API scope */
        explicit self(saga::impl::cpr_job* impl);
        /// @endcond
          
    public:
        /*! \brief Creates the object.     
         *
         *   Detailed %description starts here.
         */
        self (void);
          
        /*! \brief Destroys the object.     
         *
         *   Detailed %description starts here.
         */
        ~self (void);
    }; 
    
    }   // namespace cpr
} 
 
///////////////////////////////////////////////////////////////////////////////
#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif

#endif // !defined(SAGA_DEBUG)

// re-enable warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

#endif // !defined(SAGA_PACKAGES_CPR_JOB_SELF_HPP)

