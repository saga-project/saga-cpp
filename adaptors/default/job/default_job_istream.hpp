//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga-defs.hpp>

#ifdef SAGA_WINDOWS

#if !defined(ADAPTORS_DEFAULT_JOB_DEFAULT_JOB_ISTREAM_HPP)
#define ADAPTORS_DEFAULT_JOB_DEFAULT_JOB_ISTREAM_HPP

#include <saga/saga/util.hpp>
#include <saga/saga/base.hpp>

#include <saga/impl/job.hpp>

#include "default_job_stream.hpp"

///////////////////////////////////////////////////////////////////////////////
class default_job_istream 
:   public saga::job::istream
{
private:
    typedef impl::default_job_stream<saga::adaptors::istream_ptr> impl_type;
    
public:
    template <typename Stream>
    default_job_istream(saga::impl::v1_0::job_cpi* cpi, Stream& child_istream)
    :   saga::job::istream(
            new impl_type(cpi, child_istream.rdbuf(), child_istream.get_handle().disown()))
    {
    }
};

#endif // !defined(ADAPTORS_DEFAULT_JOB_DEFAULT_JOB_ISTREAM_HPP)

#endif // SAGA_WINDOWS

