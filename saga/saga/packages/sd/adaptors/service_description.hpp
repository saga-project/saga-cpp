//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(SAGA_PACKAGES_SD_ADAPTORS_SD_HPP)
#define SAGA_PACKAGES_SD_ADAPTORS_SD_HPP

#include <saga/saga/util.hpp>
#include <saga/saga/base.hpp>
#include <saga/saga/session.hpp>

#include <saga/saga/sd.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace adaptors {

    class SAGA_SD_PACKAGE_EXPORT service_description
    :   public saga::sd::service_description
    {
    public:
        service_description (saga::url loc);

        service_description (TR1::shared_ptr<saga::impl::service_description> impl);

        ~service_description();
    };

///////////////////////////////////////////////////////////////////////////////
}}    // namespace saga::adaptors

#endif // !defined(SAGA_PACKAGES_SD_ADAPTORS_SD_HPP)
