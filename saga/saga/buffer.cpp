// Copyright (c) 2006 Hartmut Kaiser
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga/buffer.hpp>
#include <saga/impl/buffer.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga 
{
    ///////////////////////////////////////////////////////////////////////////
    const_buffer::const_buffer(void const* data, saga::ssize_t size)
      : saga::object(new saga::impl::const_buffer(data, size))
    {
    }

    const_buffer::const_buffer(saga::impl::const_buffer* impl)
      : saga::object(impl)
    {
    }

    const_buffer::~const_buffer()
    {
    }

    ///////////////////////////////////////////////////////////////////////////
    saga::impl::const_buffer* const_buffer::get_impl (void) const
    {
        typedef saga::object base_type;
        return static_cast<saga::impl::const_buffer*>(this->base_type::get_impl());
    }

    TR1::shared_ptr<saga::impl::const_buffer> const_buffer::get_impl_sp(void) const
    {
        typedef saga::object base_type;
        return TR1::static_pointer_cast<saga::impl::const_buffer>(
            this->base_type::get_impl_sp());
    }

    ///////////////////////////////////////////////////////////////////////////
    saga::ssize_t const_buffer::get_size() const
    {
        return get_impl()->get_size();
    }

    void const* const_buffer::get_data() const
    {
        return get_impl()->get_data();
    }

    void const_buffer::close(double timeout)
    {
        get_impl()->close(timeout);
    }
    
    ///////////////////////////////////////////////////////////////////////////
    SAGA_EXPORT void mutable_buffer::default_buffer_deleter(void* data) {}

    ///////////////////////////////////////////////////////////////////////////
    mutable_buffer::mutable_buffer(saga::ssize_t size)
      : saga::const_buffer(new saga::impl::buffer(size))
    {
    }
    
    mutable_buffer::mutable_buffer(void* data, saga::ssize_t size)
      : saga::const_buffer(new saga::impl::buffer(data, size, default_buffer_deleter))
    {
    }

    mutable_buffer::mutable_buffer(void* data, saga::ssize_t size, buffer_deleter cb)
      : saga::const_buffer(new saga::impl::buffer(data, size, cb))
    {
    }

    mutable_buffer::mutable_buffer(saga::impl::buffer* impl)
      : saga::const_buffer(impl)
    {
    }

    mutable_buffer::~mutable_buffer()
    {
    }

    ///////////////////////////////////////////////////////////////////////////
    saga::impl::buffer* mutable_buffer::get_impl (void) const
    {
        typedef saga::object base_type;
        return static_cast<saga::impl::buffer*>(this->base_type::get_impl());
    }

    TR1::shared_ptr<saga::impl::buffer> mutable_buffer::get_impl_sp(void) const
    {
        typedef saga::object base_type;
        return TR1::static_pointer_cast<saga::impl::buffer>(
            this->base_type::get_impl_sp());
    }

    ///////////////////////////////////////////////////////////////////////////
    void mutable_buffer::set_size(saga::ssize_t size)
    {
        get_impl()->set_size(size);
    }

    void mutable_buffer::set_data(void* data, saga::ssize_t size,
        saga::mutable_buffer::buffer_deleter cb)
    {
        get_impl()->set_data(data, size, cb);
    }

    void* mutable_buffer::get_data() 
    {
        return get_impl()->get_data();
    }

///////////////////////////////////////////////////////////////////////////////
} // namespace saga

