//  Copyright (c) 2009 STFC
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <string>
#include <vector>

#include <saga/saga/adaptors/task.hpp>
#include <saga/saga/isn.hpp>
#include <saga/impl/isn.hpp>
#include <saga/saga/detail/call.hpp>
#include <saga/saga/exception.hpp>

///@cond
namespace saga
{
  namespace isn {
    navigator::navigator (std::string const& model,
                          session const &s,
                          saga::url loc)
       SAGA_THROW_SPEC(throw(saga::not_implemented,
                             saga::incorrect_url,
                             saga::not_implemented,
                             saga::authorization_failed,
                             saga::authentication_failed,
                             saga::timeout,
                             saga::no_success))
       : saga::object(new saga::impl::navigator(model, s, loc))
    {
         this->saga::object::get_impl()->init();
    }

    navigator::navigator (saga::impl::navigator* impl) 
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

    navigator::navigator(std::string const& model, saga::url loc)
       SAGA_THROW_SPEC(throw(saga::not_implemented,
                             saga::incorrect_url,
                             saga::not_implemented,
                             saga::authorization_failed,
                             saga::authentication_failed,
                             saga::timeout,
                             saga::no_success))
       : saga::object(new saga::impl::navigator(model,
                                                saga::detail::get_the_session(),
                                                loc))
    {
       this->saga::object::get_impl()->init();
    }

    navigator::~navigator(void) SAGA_THROW_SPEC(throw())
    {
    }

    navigator &navigator::operator= (saga::object const& o) 
        SAGA_THROW_SPEC(throw())
    {
        if (this != &o)
           this->saga::object::operator=(o);
        return *this;
    }

    saga::impl::navigator* navigator::get_impl() const
    { 
        typedef saga::object base_type;
        return static_cast<saga::impl::navigator*>(this->base_type::get_impl()); 
    }

    TR1::shared_ptr <saga::impl::navigator> navigator::get_impl_sp(void) const 
    {
        typedef saga::object base_type;
        return TR1::static_pointer_cast <saga::impl::navigator> (
            this->base_type::get_impl_sp());
    }

    // factory
    SAGA_CALL_CREATE_IMP_3(navigator,
                           impl::navigator,
                           std::string const&,
                           session const&,
                           saga::url)

    // Methods for isn
    SAGA_CALL_IMP_1 (navigator, list_related_entity_names, std::string)
    SAGA_CALL_IMP_4 (navigator,
                     get_related_entities,
                     std::string,
                     std::string,
                     std::string,
                     std::vector<saga::isn::entity_data>)

    SAGA_CALL_IMP_2 (navigator,
                     get_entities,
                     std::string,
                     std::string)

 } // namespace isn
} // namespace saga

///@endcond

