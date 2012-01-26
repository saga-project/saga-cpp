//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga/task.hpp>
#include <saga/saga/adaptors/attribute.hpp>
#include <saga/saga/adaptors/metric.hpp>
#include <saga/saga/packages/cpr/adaptors/cpr_job_self.hpp>

#include <saga/impl/cpr.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace adaptors {

    cpr_self::cpr_self (std::string rm,
                        saga::cpr::description jd_start, 
                        saga::cpr::description jd_restart, 
                        saga::session const& s)
        : saga::cpr::self(new saga::impl::cpr_job(rm, jd_start, jd_restart, s, saga::object::CPRJobSelf))
    {
        this->saga::object::get_impl()->init();
    }

    cpr_self::cpr_self (std::string rm, 
                        saga::cpr::description jd_start,
                        saga::cpr::description jd_restart)
        : saga::cpr::self(new saga::impl::cpr_job(rm, jd_start, jd_restart, detail::get_the_session(), 
              saga::object::CPRJobSelf))
    {
        this->saga::object::get_impl()->init();
    }

    cpr_self::cpr_self (std::string rm, 
                        saga::cpr::description jd, 
                        saga::session const& s)
        : saga::cpr::self(new saga::impl::cpr_job(rm, jd, s, saga::object::CPRJobSelf))
    {
        this->saga::object::get_impl()->init();
    }

    cpr_self::cpr_self (std::string rm, 
                        saga::cpr::description jd)
        : saga::cpr::self(new saga::impl::cpr_job(rm, jd, detail::get_the_session(), 
              saga::object::CPRJobSelf))
    {
        this->saga::object::get_impl()->init();
    }

    cpr_self::cpr_self (std::string rm, 
                        std::string jobid, 
                        saga::session const& s)
        : saga::cpr::self(new saga::impl::cpr_job(rm, jobid, s,
                                                  saga::object::CPRJobSelf))
    {
        this->saga::object::get_impl()->init();
    }

    cpr_self::cpr_self (std::string rm, 
                        std::string jobid)
        : saga::cpr::self(new saga::impl::cpr_job(rm, jobid, detail::get_the_session(),
              saga::object::CPRJobSelf))
    {
        this->saga::object::get_impl()->init();
    }

    cpr_self::~cpr_self()
    {
    }

///////////////////////////////////////////////////////////////////////////////
}}    // namespace saga::adaptors

