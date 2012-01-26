//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_SAGA_PATH_LEAF_HPP
#define SAGA_SAGA_PATH_LEAF_HPP

#include <string>
#include <boost/version.hpp>
#include <boost/filesystem/path.hpp>

namespace saga { namespace detail
{
    inline SAGA_EXPORT std::string leaf(boost::filesystem::path const& p)
    {
#if BOOST_FILESYSTEM_VERSION == 3
        return p.filename().string();
#else
#if BOOST_VERSION >= 103600
        return p.empty() ? std::string() : *--p.end();
#else
        return p.leaf();
#endif
#endif
    }

    inline SAGA_EXPORT std::string parent(boost::filesystem::path const& p)
    {
#if BOOST_VERSION >= 103600
        return p.parent_path().string();
#else
        return p.branch_path().string();
#endif
    }

    // workaround for broken V1.44 filesystem library directory functions 
    // canonize should have been called on the argument
    inline void remove_trailing_dot(boost::filesystem::path& p)
    {
//#if BOOST_VERSION == 104400
        std::string str = p.string();
        if (str[str.size()-1] == '.')
            p = boost::filesystem::path(str.substr(0, str.size()-1));
//#endif
    }

}}

#endif

