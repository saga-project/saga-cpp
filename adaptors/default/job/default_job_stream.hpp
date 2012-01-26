//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga-defs.hpp>

#ifdef SAGA_WINDOWS

#if !defined(ADAPTORS_DEFAULT_JOB_DEFAULT_JOB_STREAM_HPP)
#define ADAPTORS_DEFAULT_JOB_DEFAULT_JOB_STREAM_HPP

#include <iosfwd>

//#include <saga/saga/util.hpp>
//#include <saga/saga/base.hpp>
#include <saga/impl/engine/cpi.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace impl
{
    template <typename Base>
    class default_job_stream
    :   public Base
    {
    private:
        typedef Base base_type;
        typedef typename Base::handle_type handle_type;
        
    public:
        default_job_stream(saga::impl::v1_0::job_cpi* cpi, std::streambuf *buf,
                handle_type h)
        :   base_type(buf, h),
            cpi_(cpi->shared_from_this()),
            proxy_(cpi->get_proxy()->shared_from_this())
        {
        }
        ~default_job_stream()
        {
            // need to release CPI first since its destructor depends on proxy
            cpi_.reset();
            proxy_.reset();
        }

    private:
        // a saga stream has to keep alive the proxy and the cpi instance
        TR1::shared_ptr<saga::impl::v1_0::cpi> cpi_;
        TR1::shared_ptr<saga::impl::proxy> proxy_;
    };
    
///////////////////////////////////////////////////////////////////////////////
}   // namespace impl

#endif // !defined(ADAPTORS_DEFAULT_JOB_DEFAULT_JOB_STREAM_HPP)

#endif // SAGA_WINDOWS

