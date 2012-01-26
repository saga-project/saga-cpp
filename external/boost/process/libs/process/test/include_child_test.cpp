//
// Boost.Process
// Checks that child.hpp can be included without errors.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt.)
//

#include <boost/process/child.hpp>

namespace bp = ::boost::process;
namespace bpd = ::boost::process::detail;

// ------------------------------------------------------------------------

namespace boost {
namespace process {
namespace detail {

struct factories {
public:
    static
    void*
    test_it(void)
    {
        bp::child::handle_type h = (bp::child::handle_type)0;
        bpd::file_handle fh;
        return new bp::child(h, fh, fh, fh);
    }
};

} // namespace detail
} // namespace process
} // namespace boost

// ------------------------------------------------------------------------

void*
test_it(void)
{
    return bp::detail::factories::test_it();
}
