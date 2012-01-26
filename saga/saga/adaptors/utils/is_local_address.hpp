//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(SAGA_ADAPTORS_UTILS_IS_LOCAL_ADDRESS_JAN_10_2009_0951AM)
#define SAGA_ADAPTORS_UTILS_IS_LOCAL_ADDRESS_JAN_10_2009_0951AM

#include <string>
#include <saga/saga/util.hpp>
#include <saga/saga/url.hpp>

namespace saga { namespace adaptors { namespace utils
{
    // forward declaration only
    SAGA_EXPORT bool is_local_address(std::string const & host);

    inline bool is_local_address(saga::url const & url)
    {
        return is_local_address(url.get_host());
    }

    inline bool is_local_address(const char * url)
    {
        return is_local_address(saga::url (url));
    }

}}}

#endif
