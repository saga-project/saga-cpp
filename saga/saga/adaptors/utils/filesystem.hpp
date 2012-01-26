//  Copyright (c) 2008 João Abecasis
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_ADAPTORS_UTILS_FILESYSTEM_HPP_INCLUDED
#define SAGA_ADAPTORS_UTILS_FILESYSTEM_HPP_INCLUDED

//
//  Here, we attempt to hide idiosyncrasies of Boost.Filesystem's evolution.
//  This is not meant to be complete, stuff should be added *only* as needed.
//

#include <boost/version.hpp>
#include <saga/saga-defs.hpp>

#if SAGA_BOOST_FILESYSTEM_VERSION > 103500
#   include <boost/filesystem.hpp>
#else

//  There are serious issues with this version of the API and the workarounds in
//  this file. E.g. file_status has race-conditions which can't be avoided other
//  than by reimplementing Boost.Filesystem.

#   include <boost/filesystem/path.hpp>
#   include <boost/filesystem/operations.hpp>
#   include <boost/filesystem/exception.hpp>

#endif

namespace boost { namespace system {

    // Error reporting was split into Boost.System in Boost 1.35

#if SAGA_BOOST_FILESYSTEM_VERSION < 103400
    // An enum in Boost 1.33
    typedef boost::filesystem::error_code error_code;
#elif SAGA_BOOST_FILESYSTEM_VERSION < 103500
    // An integer type in Boost 1.34
    typedef boost::filesystem::system_error_type error_code;
#endif

}} // boost::system

namespace boost { namespace filesystem {

#if SAGA_BOOST_FILESYSTEM_VERSION < 103400
    // Introduced in Boost 1.34
    struct file_status
    {
        file_status()
            : error_(other_error)
        {
        }

        file_status(path const &p)
            : error_(no_error)
            , exists_(exists(p))
        {
            // Yes, there's a race condition here. Upgrade your Boost!
            if (exists_)
            {
                is_directory_ = is_directory(p);
                is_symbolic_link_ = symbolic_link_exists(p);
            }
        }

        error_code error_;
        bool exists_;
        bool is_symbolic_link_;
        bool is_directory_;
    };

    // Introduced in Boost 1.34
    inline file_status status(path const &p)
    {
        return file_status(p);
    }

    // Introduced in Boost 1.34
    inline file_status status(path const &p, boost::system::error_code &err)
    {
        file_status status;
        err = no_error;

        try
        {
            status = file_status(p);
        }
        catch (boost::filesystem::filesystem_error const &exc)
        {
            err = exc.error();
            status.error_ = err;
        }

        return status;
    }

    // Introduced in Boost 1.34
    inline bool status_known(file_status status)
    {
        return status.error_ == no_error;
    }

    // Introduced in Boost 1.34
    inline bool exists(file_status status)
    {
        return status.exists_;
    }

    // Introduced in Boost 1.34
    inline bool is_directory(file_status status)
    {
        return status.is_directory_;
    }

    // is_regular(file_status) was introduced in Boost 1.34 and later renamed to
    // is_regular_file.
    inline bool is_regular_file(file_status status)
    {
        // This implementation of is_regular_file is less strict than what later
        // versions of Boost provide.
        //
        // If you have a problem with this, upgrade your Boost! ;-)

        return status.exists_
            && !status.is_symbolic_link_
            && !status.is_directory_;
    }

#elif SAGA_BOOST_FILESYSTEM_VERSION <= 103500

    // Introduced in Boost 1.36, deprecating is_regular(file_status)
    inline bool is_regular_file(file_status status)
    {
        return is_regular(status);
    }

#endif

    ////////////////////////////////////////////////////////////////////////////
    // Evolution of basic_path member functions is catered to with inline
    // forwarding non-member functions that can be found through ADL.
    ////////////////////////////////////////////////////////////////////////////

    // basic_path::leaf() was deprecated in Boost 1.36 and renamed to filename
#if SAGA_BOOST_FILESYSTEM_VERSION < 103400
    inline std::string filename(path const &p)
#else
    template <class Path>
    inline typename Path::string_type filename(Path const &p)
#endif
    {
    #if SAGA_BOOST_FILESYSTEM_VERSION < 103600
        return p.leaf();
    #else
        #if BOOST_FILESYSTEM_VERSION == 3
        return p.filename().string();
        #else
        return p.filename();
        #endif    
    #endif
    }

    // basic_path::branch_path() was deprecated in Boost 1.36 and renamed to
    // parent_path
    template <class Path>
    inline Path parent_path(Path const &p)
    {
    #if SAGA_BOOST_FILESYSTEM_VERSION < 103600
        return p.branch_path();
    #else
        return p.parent_path();
    #endif
    }

}} // namespace boost::filesystem

#endif // include guard
