//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_IMPL_PACKAGES_COMM_RPC_PARAMETER_HPP
#define SAGA_IMPL_PACKAGES_COMM_RPC_PARAMETER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif 

// include dependent spec sections
#include <saga/impl/buffer.hpp>
#include <saga/saga/rpc.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl
{
    ///////////////////////////////////////////////////////////////////////////
    class parameter 
      : public saga::impl::buffer
    {
    public:
        parameter(void* data, saga::ssize_t size, 
            saga::rpc::io_mode mode, saga::mutable_buffer::buffer_deleter cb);
        ~parameter();
        
        saga::rpc::io_mode get_mode() const;
        
    private:
        saga::rpc::io_mode mode_;
    };  // iovec

///////////////////////////////////////////////////////////////////////////////
}}   // namespace saga::impl

#endif // !defined(SAGA_IMPL_PACKAGES_COMM_RPC_PARAMETER_HPP)

