//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// include dependent spec sections
#include <saga/saga/rpc.hpp>
#include <saga/impl/rpc.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga
{
    namespace rpc {
    parameter::parameter(void* data, saga::ssize_t size, io_mode mode,
            buffer_deleter cb)
      : saga::mutable_buffer(new saga::impl::parameter(data, size, mode, cb))
    {
    }

    parameter::~parameter()
    {
    }

    saga::impl::parameter* parameter::get_impl() const
    { 
        typedef saga::object base_type;
        return static_cast<saga::impl::parameter*>(this->base_type::get_impl()); 
    }

    TR1::shared_ptr <saga::impl::parameter> parameter::get_impl_sp(void) const
    {
        typedef saga::object base_type;
        return TR1::static_pointer_cast <saga::impl::parameter> (
            this->base_type::get_impl_sp());
    }

    saga::rpc::io_mode parameter::get_mode() const
    {
        return get_impl()->get_mode();
    }
}
///////////////////////////////////////////////////////////////////////////////
}   // namespace saga

