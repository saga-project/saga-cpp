// Copyright (c) 2006 Hartmut Kaiser
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_IMPL_ENGINE_BUFFER_HPP
#define SAGA_IMPL_ENGINE_BUFFER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif 

#include <saga/saga/util.hpp>
#include <saga/saga/buffer.hpp>
#include <saga/impl/exception.hpp>
#include <saga/impl/engine/object.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable: 4251 4231 4660 4275)
#endif

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl
{
    ///////////////////////////////////////////////////////////////////////////
    void SAGA_EXPORT not_owned_deleter(void* data);
    
    ///////////////////////////////////////////////////////////////////////////
    class SAGA_EXPORT const_buffer
      : public saga::impl::object
    {
    public:
        const_buffer(void const* data, saga::ssize_t size,
            saga::mutable_buffer::buffer_deleter cb = not_owned_deleter,
            saga::object::type t = saga::object::Buffer);
        
        ~const_buffer();
        
        saga::ssize_t get_size() const;
        void const* get_data() const;
        
        void close(double timeout);
        
    protected:
        void tidy();
    
        void* data_;
        saga::ssize_t size_;
        saga::mutable_buffer::buffer_deleter cb_;
    };
    
    ///////////////////////////////////////////////////////////////////////////
    class SAGA_EXPORT buffer
      : public saga::impl::const_buffer
    {
    public:
        buffer(saga::ssize_t size, saga::object::type t = saga::object::Buffer);
        buffer(void* data, saga::ssize_t size, 
            saga::mutable_buffer::buffer_deleter cb,
            saga::object::type t = saga::object::Buffer);
        
        ~buffer();
        
        void set_size(saga::ssize_t size);
        void set_data(void* data, saga::ssize_t size,
            saga::mutable_buffer::buffer_deleter cb);
        void* get_data();
    };
    
}} // namespace saga::impl

// re-enable warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

#endif // SAGA_IMPL_ENGINE_BUFFER_HPP
