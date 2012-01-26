//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// include dependent spec sections
#include <saga/saga/rpc.hpp>
#include <saga/impl/rpc.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl
{
    parameter::parameter(void* data, saga::ssize_t size, 
            saga::rpc::io_mode mode, saga::mutable_buffer::buffer_deleter cb)
      : saga::impl::buffer(data, size, cb, saga::object::Parameter), 
        mode_(mode)
    {
    }
    
    parameter::~parameter()
    {
    }
    
    saga::rpc::io_mode parameter::get_mode() const
    {
        return mode_;
    }

///////////////////////////////////////////////////////////////////////////////
}}   // namespace saga::impl

