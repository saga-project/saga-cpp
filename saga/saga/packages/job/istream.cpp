//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga/job.hpp>
#include <saga/saga/adaptors/task.hpp>
#include <saga/impl/job.hpp>
#include <saga/impl/packages/job/job_runtime.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace job 
{
    istream::istream(saga::impl::istream_interface *impl)
    :   std::ios(0), base_type(impl->get_streambuf()), impl_(impl)
    {
    }

    istream::istream(istream const& rhs)
    :   std::ios(0), base_type(rhs.rdbuf()), impl_(rhs.impl_)
    {
        this->copyfmt(rhs);
        this->clear(rhs.rdstate());
    }

    istream::istream()
    :   std::ios(0), base_type(0)
    {
        this->clear(std::ios::eofbit);
    }

    istream& istream::operator=(istream const& rhs)
    {
        if (this != &rhs) {
            this->copyfmt(rhs);
            this->clear(rhs.rdstate());
            this->std::basic_ios<char>::rdbuf(rhs.rdbuf());
            impl_ = rhs.impl_;
        }
        return *this;
    }

    ///////////////////////////////////////////////////////////////////////////
    namespace detail
    {
        handle_type get_handle(saga::job::istream const& istrm, bool detach) 
        {
            return saga::impl::job_runtime::get_handle(istrm, detach);
        }
    }

///////////////////////////////////////////////////////////////////////////////
}}   // namespace saga

