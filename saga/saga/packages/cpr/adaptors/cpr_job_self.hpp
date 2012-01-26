//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(SAGA_PACKAGES_CPR_ADAPTORS_JOB_SELF_HPP)
#define SAGA_PACKAGES_CPR_ADAPTORS_JOB_SELF_HPP

#include <saga/saga/util.hpp>
#include <saga/saga/base.hpp>
#include <saga/saga/session.hpp>

#include <saga/saga/cpr.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace adaptors {

    class SAGA_CPR_PACKAGE_EXPORT cpr_self
    :   public saga::cpr::self
    {
    public:
        cpr_self (std::string rm, 
                  saga::cpr::description jd_start, 
                  saga::cpr::description jd_restart, 
                  saga::session const& s);
        cpr_self (std::string rm, 
                  saga::cpr::description jd_start, 
                  saga::cpr::description jd_restart);

        cpr_self (std::string rm, 
                  saga::cpr::description jd, 
                  saga::session const& s);
        cpr_self (std::string rm, 
                  saga::cpr::description jd);

        cpr_self (std::string rm, 
                  std::string jobid, 
                  saga::session const& s);
        cpr_self (std::string rm, 
                  std::string jobid);

        ~cpr_self();
    };

///////////////////////////////////////////////////////////////////////////////
}}    // namespace saga::adaptors

#endif // !defined(SAGA_PACKAGES_CPR_ADAPTORS_JOB_SELF_HPP)

