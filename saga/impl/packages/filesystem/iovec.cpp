//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// include dependent spec sections
#include <saga/saga/filesystem.hpp>
#include <saga/impl/filesystem.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl
{
    ///////////////////////////////////////////////////////////////////////////
    const_iovec::const_iovec(void const* data, saga::ssize_t size, 
            saga::ssize_t len_in)
      : saga::impl::const_buffer(data, size, not_owned_deleter, saga::object::IOVec), 
        len_in_(-1 == len_in ? size : len_in), len_out_(-1)
    {
        if (size != -1 && len_in > size)
        {
            SAGA_THROW("len_in is larger than size", saga::BadParameter);
        }
    }
    
    const_iovec::~const_iovec()
    {
    }
    
    saga::ssize_t const_iovec::get_len_in() const
    {
        return len_in_;
    }
    
    saga::ssize_t const_iovec::get_len_out() const
    {
        return len_out_;
    }

    ///////////////////////////////////////////////////////////////////////////
    iovec::iovec(void* data, saga::ssize_t size, saga::ssize_t len_in,
            saga::mutable_buffer::buffer_deleter cb)
      : saga::impl::buffer(data, size, cb, saga::object::IOVec), 
        len_in_(-1 == len_in ? size : len_in), len_out_(-1)
    {
        if (size != -1 && len_in > size)
        {
            SAGA_THROW("len_in is larger than size", saga::BadParameter);
        }
    }
    
    iovec::~iovec()
    {
    }
    
    void iovec::set_len_in(saga::ssize_t len_in)
    {
        if (this->size_ != -1 && len_in > this->size_)
        {
            SAGA_THROW("len_in is larger than size", saga::BadParameter);
        }
        len_in_ = len_in;
    }
    
    saga::ssize_t iovec::get_len_in() const
    {
        return len_in_;
    }
    
    saga::ssize_t iovec::get_len_out() const
    {
        return len_out_;
    }

///////////////////////////////////////////////////////////////////////////////
}}   // namespace saga::impl

