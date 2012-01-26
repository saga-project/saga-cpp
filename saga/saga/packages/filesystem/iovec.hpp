//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2007 Ole Weidner (oweidner@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_PACKAGES_DATA_FILESYSTEM_IOVEC_HPP
#define SAGA_PACKAGES_DATA_FILESYSTEM_IOVEC_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif 

#include <saga/saga/packages/filesystem/config.hpp>

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
    namespace filesystem {
    /*! \brief Brief %description starts here     
     *
     *
     */
    class SAGA_FILESYSTEM_PACKAGE_EXPORT const_iovec 
        : public saga::const_buffer
    {
    protected:
        /// @cond
        /** These methods are not within API scope */
        TR1::shared_ptr <saga::impl::const_iovec> get_impl_sp(void) const;
        saga::impl::const_iovec* get_impl (void) const;
        friend class saga::impl::const_iovec;
        /// @endcond

    public:
        /*! \brief Brief %description starts here  
         *
         *
         */
        const_iovec(void const* data, saga::ssize_t size, 
            saga::ssize_t len_in = -1);
            
        /*! \brief Brief %description starts here  
         *
         *
         */
        ~const_iovec();

        /*! \brief Brief %description starts here  
         *
         *
         */
        saga::ssize_t get_len_in() const;
        
        /*! \brief Brief %description starts here  
         *
         *
         */
        saga::ssize_t get_len_out() const;

    };  // const_iovec

    /*! \brief The iovec class inherits the saga::buffer class, and 
     * three additional state attributes: offset, len in and len out.
     *
     *   With that addition, the new class can be used very much the 
     * same way as the iovec structure defined by POSIX for readv/writev, 
     * with the buffer len_in being interpreted as the POSIX iov_len, 
     * i.e. the number of bytes to read/write. 
     */
    class SAGA_FILESYSTEM_PACKAGE_EXPORT iovec 
        : public saga::mutable_buffer
    {
    protected:
        /// @cond
        /** These methods are not within API scope */
        TR1::shared_ptr <saga::impl::iovec> get_impl_sp (void) const;
        saga::impl::iovec* get_impl (void) const;
        friend class saga::impl::iovec;
        /// @endcond

    public:
        /*! \brief Creates an iovec instance.     
         *
         *
         */
        iovec(void* data = 0, saga::ssize_t size = -1, 
            saga::ssize_t len_in = -1, 
            buffer_deleter cb = default_buffer_deleter);
            
        /*! \brief Destroys an iovec instance.     
         *
         *
         */
        ~iovec();

        /*! \brief Sets len_in.     
         *
         *
         */
        void set_len_in(saga::ssize_t len_in);
        
        /*! \brief Retrieves the current value for len_in.          
         *
         *
         */
        saga::ssize_t get_len_in() const;
        
        /*! \brief Retrieves the current value for len_out.      
         *
         *
         */
        saga::ssize_t get_len_out() const;

    };  // iovec
}
///////////////////////////////////////////////////////////////////////////////
}   // namespace saga

// re-enable warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

#endif // !defined(SAGA_PACKAGES_DATA_FILESYSTEM_IOVEC_HPP)

