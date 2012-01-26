//
// Boost.Process
// Checks that detail/factories.hpp can be included without errors.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt.)
//

#include <boost/process/detail/factories.hpp>

namespace bp = ::boost::process;
namespace bpd = ::boost::process::detail;

// ------------------------------------------------------------------------

void*
test_it_1(void)
{
    bpd::file_handle fh;
    bp::child c = bpd::factories::create_child(0, fh, fh, fh);
    void* ptr = &c;
    return ptr;
}

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
void*
test_it_2(void)
{
    bpd::info_map info;
    bp::posix_child c = bpd::factories::create_posix_child(0, info, info);
    void* ptr = &c;
    return ptr;
}
#endif
