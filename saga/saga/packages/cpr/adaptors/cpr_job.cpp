//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga/task.hpp>
#include <saga/saga/adaptors/metric.hpp>
#include <saga/saga/packages/cpr/adaptors/cpr_job.hpp>

#include <saga/impl/cpr.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace adaptors {

    cpr_job::cpr_job (std::string rm,
                      saga::cpr::description jd_start, 
                      saga::cpr::description jd_restart, 
                      saga::session const& s)
        : saga::cpr::job(new saga::impl::cpr_job(rm, jd_start, jd_restart, s))
    {
        this->saga::object::get_impl()->init();
    }

    cpr_job::cpr_job (std::string rm,
                      saga::cpr::description jd_start, 
                      saga::cpr::description jd_restart)
        : saga::cpr::job(new saga::impl::cpr_job(rm, jd_start, jd_restart,
                         detail::get_the_session()))
    {
        this->saga::object::get_impl()->init();
    }

    cpr_job::cpr_job (std::string rm, 
                      saga::cpr::description jd, 
                      saga::session const& s)
        : saga::cpr::job(new saga::impl::cpr_job(rm, jd, s))
    {
        this->saga::object::get_impl()->init();
    }

    cpr_job::cpr_job (std::string rm, 
                      saga::cpr::description jd)
        : saga::cpr::job(new saga::impl::cpr_job(rm, jd, detail::get_the_session()))
    {
        this->saga::object::get_impl()->init();
    }

    cpr_job::cpr_job (std::string rm, 
                      std::string jobid, 
                      saga::session const& s)
        : saga::cpr::job(new saga::impl::cpr_job(rm, jobid, s))
    {
        this->saga::object::get_impl()->init();
    }

    cpr_job::cpr_job (std::string rm, 
                      std::string jobid)
        : saga::cpr::job(new saga::impl::cpr_job(rm, jobid, detail::get_the_session()))
    {
        this->saga::object::get_impl()->init();
    }

    cpr_job::cpr_job (TR1::shared_ptr<saga::impl::cpr_job> impl)
      : saga::cpr::job (saga::impl::runtime::get_object (
          TR1::static_pointer_cast<saga::impl::object>(impl)))
    {
        this->saga::object::get_impl()->init();
    }

    cpr_job::~cpr_job()
    {
    }

///////////////////////////////////////////////////////////////////////////////
}}    // namespace saga::adaptors

