//  Copyright (c) 2009 STFC
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <string>
#include <vector>

#include <saga/saga/adaptors/task.hpp>
#include <saga/saga/sd.hpp>
#include <saga/impl/sd.hpp>
#include <saga/saga/detail/call.hpp>
#include <saga/saga/exception.hpp>

///@cond
namespace saga
{
  namespace sd {
    discoverer::discoverer (session const &s, saga::url loc)
       SAGA_THROW_SPEC(throw(saga::not_implemented,
                             saga::incorrect_url,
                             saga::not_implemented,
                             saga::authorization_failed,
                             saga::authentication_failed,
                             saga::timeout,
                             saga::no_success))
       : saga::object(new saga::impl::discoverer(s, loc))
    {
         this->saga::object::get_impl()->init();
    }

    discoverer::discoverer (saga::impl::discoverer* impl) 
       SAGA_THROW_SPEC(throw(saga::not_implemented,
                             saga::incorrect_url,
                             saga::not_implemented,
                             saga::authorization_failed,
                             saga::authentication_failed,
                             saga::timeout,
                             saga::no_success))
       : saga::object(impl)
    {
    }

    discoverer::discoverer(saga::url loc)
       SAGA_THROW_SPEC(throw(saga::not_implemented,
                             saga::incorrect_url,
                             saga::not_implemented,
                             saga::authorization_failed,
                             saga::authentication_failed,
                             saga::timeout,
                             saga::no_success))
       : saga::object(new saga::impl::discoverer(saga::detail::get_the_session(), loc))
    {
       this->saga::object::get_impl()->init();
    }

    discoverer::~discoverer(void) SAGA_THROW_SPEC(throw())
    {
    }

    discoverer &discoverer::operator= (saga::object const& o) 
        SAGA_THROW_SPEC(throw())
    {
        if (this != &o)
           this->saga::object::operator=(o);
        return *this;
    }

    saga::impl::discoverer* discoverer::get_impl() const
    { 
        typedef saga::object base_type;
        return static_cast<saga::impl::discoverer*>(this->base_type::get_impl()); 
    }

    TR1::shared_ptr <saga::impl::discoverer> discoverer::get_impl_sp(void) const 
    {
        typedef saga::object base_type;
        return TR1::static_pointer_cast <saga::impl::discoverer> (
            this->base_type::get_impl_sp());
    }

    // factory
    SAGA_CALL_CREATE_IMP_2(discoverer,
                           impl::discoverer,
                           session const&,
                           saga::url)

    // methods for service discoverer
    SAGA_CALL_IMP_2_EX (discoverer, list_services, list_services2, std::string, std::string)
    SAGA_CALL_IMP_3_EX (discoverer, list_services, list_services3, std::string, std::string, std::string)

 } // namespace sd
} // namespace saga

///@endcond
