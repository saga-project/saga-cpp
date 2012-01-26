//
// Boost.Process
// Miscellaneous utilities for the tests.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt.)
//

#include <boost/process/config.hpp>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/test/unit_test.hpp>

// ------------------------------------------------------------------------

enum bp_api_type {
    posix_api,
    win32_api,
}
#if defined(BOOST_PROCESS_POSIX_API)
bp_api_type = posix_api;
#elif defined(BOOST_PROCESS_WIN32_API)
bp_api_type = win32_api;
#else
#   error "Unsupported platform."
#endif

// ------------------------------------------------------------------------

inline
std::string
get_helpers_path(void)
{
    boost::filesystem::path hp = boost::filesystem::initial_path();

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    hp /= "./bin/msvc/debug/link-static/runtime-link-static/helpers.exe";
#elif defined(__APPLE__)
    hp /= "./bin/darwin/debug/link-static/runtime-link-static/helpers";
#else
    hp /= "./bin/gcc/debug/link-static/runtime-link-static/helpers";
#endif

    BOOST_REQUIRE(boost::filesystem::exists(hp));

    return hp.string();
}

// ------------------------------------------------------------------------

inline
std::istream&
portable_getline(std::istream& is, std::string& str)
{
    std::getline(is, str);
    std::string::size_type pos = str.rfind('\r');
    if (pos != std::string::npos)
        str.erase(pos);
    return is;
}
