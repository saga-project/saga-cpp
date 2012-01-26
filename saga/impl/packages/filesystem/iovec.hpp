//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_IMPL_PACKAGES_DATA_FILE_IOVEC_HPP
#define SAGA_IMPL_PACKAGES_DATA_FILE_IOVEC_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif 

// include dependent spec sections
#include <saga/impl/buffer.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl
{
    ///////////////////////////////////////////////////////////////////////////
    class const_iovec 
      : public saga::impl::const_buffer
    {
    public:
        const_iovec(void const* data, saga::ssize_t size, 
            saga::ssize_t len_in);
        ~const_iovec();
        
        saga::ssize_t get_len_in() const;
        saga::ssize_t get_len_out() const;

    private:
        saga::ssize_t len_in_;
        saga::ssize_t len_out_;

    };  // const_iovec

    ///////////////////////////////////////////////////////////////////////////
    class iovec 
      : public saga::impl::buffer
    {
    public:
        iovec(void* data, saga::ssize_t size, saga::ssize_t len_in,
            saga::mutable_buffer::buffer_deleter cb);
        ~iovec();
        
        void set_len_in(saga::ssize_t len_in);
        saga::ssize_t get_len_in() const;
        saga::ssize_t get_len_out() const;

    private:
        saga::ssize_t len_in_;
        saga::ssize_t len_out_;

    };  // iovec

///////////////////////////////////////////////////////////////////////////////
}}   // namespace saga::impl

#endif // !defined(SAGA_IMPL_PACKAGES_DATA_FILE_IOVEC_HPP)

