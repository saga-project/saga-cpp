//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(SAGA_PACKAGES_JOB_ADAPTORS_JOB_HPP)
#define SAGA_PACKAGES_JOB_ADAPTORS_JOB_HPP

#include <saga/saga/util.hpp>
#include <saga/saga/base.hpp>
#include <saga/saga/session.hpp>

#include <saga/saga/job.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace adaptors {

    class SAGA_JOB_PACKAGE_EXPORT job
    :   public saga::job::job
    {
    public:
        job (saga::url rm, saga::job::description jd, saga::session const& s);
        job (saga::url rm, saga::job::description jd);

        job (saga::url rm, std::string jobid, saga::session const& s);
        job (saga::url rm, std::string jobid);

        job (TR1::shared_ptr<saga::impl::job> impl);

        ~job();
    };

///////////////////////////////////////////////////////////////////////////////
}}    // namespace saga::adaptors

#endif // !defined(SAGA_PACKAGES_JOB_ADAPTORS_JOB_HPP)
