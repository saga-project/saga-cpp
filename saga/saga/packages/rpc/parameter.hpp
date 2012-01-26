//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_PACKAGES_COMM_RPC_PARAMETER_HPP
#define SAGA_PACKAGES_COMM_RPC_PARAMETER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif 

#include <saga/saga/packages/rpc/config.hpp>

// include dependent spec sections
#include <saga/saga/buffer.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable: 4251 4231 4275 4660)
#endif

///////////////////////////////////////////////////////////////////////////////
namespace saga
{
    namespace rpc {
    
    /*! \brief The io mode enum specifies the modus of the rpc::parameter instances:
     * In, Out and InOut.     
     *
     * In: The parameter is an input parameter: its initial value will be evaluated,
     * and its data buffer will not be changed during the invocation of call(). 
     *
     * Out: The parameter is an output parameter: its initial value will not be
     * evaluated, and its data buffer will likely be changed during the invocation
     * of call(). 
     *
     * InOut: The parameter is input and output parameter: its initial value will not
     * evaluated, and its data buffer will likely be changed during the invocation 
     * of call().
     */  
    enum io_mode
    {
        Unknown = -1,
        In = 1,           // input parameter
        Out = 2,          // output parameter
        InOut = In | Out  // input and output parameter
    };
    
    /*! \brief The parameter class inherits the saga::buffer class.    
     *
     *   It also adds one additional state attribute: io mode, which is
     * read-only. With that addition, the new class can conveniently be used
     * to define input, inout and output parameters for RPC calls. 
     */  
    class SAGA_RPC_PACKAGE_EXPORT parameter 
        : public saga::mutable_buffer
    {
    protected:
        /// @cond
        /** These methods are not within API scope */
        TR1::shared_ptr <saga::impl::parameter> get_impl_sp (void) const;
        saga::impl::parameter* get_impl (void) const;
        friend class saga::impl::parameter;
        /// @endcond

    public:
        /*! \brief Creates a parameter instance.    
         *  \return The newly created parameter.
         * 
         *
         */  
        parameter(void* data = 0, saga::ssize_t size = -1, io_mode mode = In,
                  buffer_deleter cb = default_buffer_deleter);

        /*! \brief Destroys a parameter instance.     
         *
         *
         */  
        ~parameter();

        /*! \brief FIXME: spec says: should be set_io_mode    
         *
         *
         */  
        io_mode get_mode() const;

        // FIXME: missing from spec: set_io_mode
        
    }; 
}}   

// re-enable warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

#endif // !defined(SAGA_PACKAGES_COMM_RPC_PARAMETER_HPP)

