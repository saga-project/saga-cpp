//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2007 Ole Weidner (oweidner@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(SAGA_PACKAGES_JOB_ADAPTORS_OSTREAM_HPP)
#define SAGA_PACKAGES_JOB_ADAPTORS_OSTREAM_HPP

#include <iosfwd>

#include <boost/config.hpp>

#include <saga/saga/util.hpp>
#include <saga/saga/base.hpp>
#include <saga/saga/packages/job/config.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable: 4251 4231 4275 4660)
#endif

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace job 
{
    /*! \brief Brief %description starts here     
     *
     *
     */
    class SAGA_JOB_PACKAGE_EXPORT ostream
        :   public std::basic_ostream<char>
    {
    private:
        typedef std::basic_ostream<char> base_type;
        TR1::shared_ptr<saga::impl::ostream_interface> impl_;

    private:
         friend struct saga::impl::job_runtime;    // needed to allow access to get_handle

    protected:
        /// @cond
        /** These methods are not within API scope */
        ostream(saga::impl::ostream_interface *impl);
        /// @endcond

    public:
        /*! \brief Brief %description starts here     
         *
         *
         */
        ostream();

        /*! \brief Brief %description starts here     
         *
         *
         */
        ostream(ostream const& rhs);

        /*! \brief Brief %description starts here     
         *
         *
         */
        ~ostream();

        /*! \brief Brief %description starts here     
         *
         *
         */
        ostream& operator=(ostream const& rhs);
    };

    namespace detail
    {
#if defined(BOOST_WINDOWS)
        typedef void* handle_type;
#else
        typedef int handle_type;
#endif

        /*! \brief access the underlying file descriptor
         * 
         *  \note This functionality is for internal use only and has been 
         *        introduced solely for language bindings implemented on top 
         *        of this SAGA implementation
         */
         SAGA_JOB_PACKAGE_EXPORT handle_type 
         get_handle(saga::job::ostream const& ostream, bool detach = false);
    }

///////////////////////////////////////////////////////////////////////////////
}}    // namespace saga

// re-enable warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

#endif // !defined(SAGA_PACKAGES_JOB_ADAPTORS_OSTREAM_HPP)

