//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// include dependent spec sections
#include <saga/saga/filesystem.hpp>
#include <saga/impl/filesystem.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga
{
    namespace filesystem {
    ///////////////////////////////////////////////////////////////////////////
    const_iovec::const_iovec(void const* data, saga::ssize_t size, 
            saga::ssize_t len_in)
      : saga::const_buffer(new saga::impl::const_iovec(data, size, len_in))
    {
    }

    const_iovec::~const_iovec()
    {
    }

    saga::impl::const_iovec* const_iovec::get_impl (void) const
    { 
      typedef saga::object base_type;
      return static_cast <saga::impl::const_iovec*> (this->base_type::get_impl ());
    }

    TR1::shared_ptr <saga::impl::const_iovec> const_iovec::get_impl_sp (void) const
    {
        typedef saga::object base_type;
        return TR1::static_pointer_cast <saga::impl::const_iovec> (
            this->base_type::get_impl_sp());
    }

    saga::ssize_t const_iovec::get_len_in() const
    {
        return get_impl()->get_len_in();
    }

    saga::ssize_t const_iovec::get_len_out() const
    {
        return get_impl()->get_len_out();
    }

    ///////////////////////////////////////////////////////////////////////////
    iovec::iovec(void* data, saga::ssize_t size, saga::ssize_t len_in,
            buffer_deleter cb)
      : saga::mutable_buffer(new saga::impl::iovec(data, size, len_in, cb))
    {
    }
    
    iovec::~iovec()
    {
    }

    saga::impl::iovec* iovec::get_impl (void) const
    { 
      typedef saga::object base_type;
      return static_cast <saga::impl::iovec*> (this->base_type::get_impl ());
    }

    TR1::shared_ptr <saga::impl::iovec> iovec::get_impl_sp (void) const
    {
        typedef saga::object base_type;
        return TR1::static_pointer_cast <saga::impl::iovec> (
            this->base_type::get_impl_sp());
    }

    void iovec::set_len_in(saga::ssize_t len_in)
    {
        get_impl()->set_len_in(len_in);
    }

    saga::ssize_t iovec::get_len_in() const
    {
        return get_impl()->get_len_in();
    }

    saga::ssize_t iovec::get_len_out() const
    {
        return get_impl()->get_len_out();
    }
}
///////////////////////////////////////////////////////////////////////////////
}   // namespace saga

