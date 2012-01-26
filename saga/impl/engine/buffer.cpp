// Copyright (c) 2006 Hartmut Kaiser
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga/version.hpp>
#include <saga/impl/buffer.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl
{
    void owned_deleter(void* data)
    {
        delete (boost::uint8_t*) data;
    }
    
    void not_owned_deleter(void* data)
    {
    }

    ///////////////////////////////////////////////////////////////////////////
    const_buffer::const_buffer(void const* data, saga::ssize_t size, 
          saga::mutable_buffer::buffer_deleter cb, saga::object::type t)
      : saga::impl::object(t), 
        data_(const_cast<void*>(data)), size_(size), cb_(cb)
    {
    }
    
    const_buffer::~const_buffer()
    {
        tidy();
    }

    void const_buffer::tidy()
    {
        cb_(data_);     // call provided deleter

        cb_ = saga::impl::not_owned_deleter;
        data_ = 0;
        size_ = -1;
    }

    ///////////////////////////////////////////////////////////////////////////
    saga::ssize_t const_buffer::get_size() const
    {
        return size_;
    }

    void const* const_buffer::get_data() const
    {
        return data_;
    }

    void const_buffer::close(double timeout)
    {
    }

    ///////////////////////////////////////////////////////////////////////////
    buffer::buffer(saga::ssize_t size, saga::object::type t)
      : saga::impl::const_buffer(0, -1, owned_deleter, t)
    {
        set_size(size);
    }
    
    buffer::buffer(void* data, saga::ssize_t size, 
            saga::mutable_buffer::buffer_deleter cb, saga::object::type t)
      : saga::impl::const_buffer(data, size, cb, t)
    {
    }
    
    buffer::~buffer()
    {
        tidy();
    }

    ///////////////////////////////////////////////////////////////////////////
    void buffer::set_size(saga::ssize_t size)
    {
        tidy();               // delete old data if owned by impl

        if (size >= 0) {     // allocate new data if size is given
            data_ = new boost::uint8_t[size];
            cb_ = owned_deleter;
        }
        size_ = size;
    }
    
    void buffer::set_data(void* data, saga::ssize_t size,
        saga::mutable_buffer::buffer_deleter cb)
    {
        tidy();
        
        data_ = data;
        size_ = size;
        cb_ = cb;
    }
    
    void* buffer::get_data() 
    {
        return data_;
    }

///////////////////////////////////////////////////////////////////////////////
}} // namespace saga::impl

