//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(SAGA_IMPL_PACKAGES_CPR_CPR_JOB_SERIALIZATION_HPP)
#define SAGA_IMPL_PACKAGES_CPR_CPR_JOB_SERIALIZATION_HPP

#include <saga/saga/adaptors/serialization.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl 
{

// serialization support for the cpr package
class cpr_job_serialization
:   public saga::adaptors::serialization
{
public:
    ~cpr_job_serialization();
    std::string serialize(TR1::shared_ptr<saga::impl::object>);
    TR1::shared_ptr<saga::impl::object> deserialize(
        saga::session s, saga::object::type type, std::string const& data);
};

///////////////////////////////////////////////////////////////////////////////
}}    // namespace saga::impl

#endif // !SAGA_IMPL_PACKAGES_CPR_CPR_JOB_SERIALIZATION_HPP
