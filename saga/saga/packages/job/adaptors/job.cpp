//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga/task.hpp>
#include <saga/saga/adaptors/metric.hpp>
#include <saga/saga/packages/job/adaptors/job.hpp>

#include <saga/impl/job.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace adaptors {

    job::job (saga::url rm, saga::job::description jd, saga::session const& s)
        : saga::job::job(new saga::impl::job(rm, jd, s))
    {
        this->saga::object::get_impl()->init();
    }

    job::job (saga::url rm, saga::job::description jd)
        : saga::job::job(new saga::impl::job(rm, jd, detail::get_the_session()))
    {
        this->saga::object::get_impl()->init();
    }

    job::job (saga::url rm, std::string jobid, saga::session const& s)
        : saga::job::job(new saga::impl::job(rm, jobid, s))
    {
        this->saga::object::get_impl()->init();
    }

    job::job (saga::url rm, std::string jobid)
        : saga::job::job(new saga::impl::job(rm, jobid, detail::get_the_session()))
    {
        this->saga::object::get_impl()->init();
    }

    job::job (TR1::shared_ptr<saga::impl::job> impl)
      : saga::job::job (saga::impl::runtime::get_object(
            TR1::static_pointer_cast<saga::impl::object>(impl)))
    {
        this->saga::object::get_impl()->init();
    }

    job::~job()
    {
    }

///////////////////////////////////////////////////////////////////////////////
}}    // namespace saga::adaptors

