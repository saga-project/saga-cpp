//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga/task.hpp>
#include <saga/saga/adaptors/attribute.hpp>
#include <saga/saga/adaptors/metric.hpp>
#include <saga/saga/packages/job/adaptors/job_self.hpp>

#include <saga/impl/job.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace adaptors {

    self::self (saga::url rm, saga::job::description jd, 
              saga::session const& s)
        : saga::job::self(new saga::impl::job(rm, jd, s, saga::object::JobSelf))
    {
        this->saga::object::get_impl()->init();
    }

    self::self (saga::url rm, saga::job::description jd)
        : saga::job::self(new saga::impl::job(rm, jd, detail::get_the_session(), 
              saga::object::JobSelf))
    {
        this->saga::object::get_impl()->init();
    }

    self::self (saga::url rm, std::string jobid, saga::session const& s)
        : saga::job::self(new saga::impl::job(rm, jobid, s, saga::object::JobSelf))
    {
        this->saga::object::get_impl()->init();
    }

    self::self (saga::url rm, std::string jobid)
        : saga::job::self(new saga::impl::job(rm, jobid, detail::get_the_session(),
              saga::object::JobSelf))
    {
        this->saga::object::get_impl()->init();
    }

    self::~self()
    {
    }

///////////////////////////////////////////////////////////////////////////////
}}    // namespace saga::adaptors

