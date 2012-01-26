// Copyright (c) 2006 Hartmut Kaiser
// Copyright (c) 2007 Ole Weidner (oweidner@cct.lsu.edu)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_SAGA_BUFFER_HPP
#define SAGA_SAGA_BUFFER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif 

#include <saga/saga/util.hpp>
#include <saga/saga/base.hpp>
#include <saga/saga/types.hpp>
#include <saga/saga/object.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable: 4251 4231 4660)
#endif

///////////////////////////////////////////////////////////////////////////////
namespace saga 
{
    /*! \brief Encapsulates I/O operations.     
     *
     *   The SAGA API includes a number of calls which perform byte-level I/O
     * operations, e.g. read()/write() on files and streams, and call() on rpc
     * instances. Future SAGA API extensions are expected to increase the number
     * of I/O methods. The saga::buffer class encapsulates a sequence of bytes to
     * be used for such I/O operations - that allows for uniform I/O syntax and semantics
     * over the various SAGA API packages. 
     */
    class SAGA_EXPORT const_buffer
      : public saga::object
    {
    protected:
        /// @cond
        /** These methods are not within API scope */
        TR1::shared_ptr<saga::impl::const_buffer> get_impl_sp (void) const;
        saga::impl::const_buffer* get_impl (void) const;
        friend class saga::impl::const_buffer;

        const_buffer(saga::impl::const_buffer* impl);
        /// @endcond
    
    public:
        /*! \brief Creates an I/O buffer 
         * \param data Data to be used
         * \param size Size of data to be used.
         * \return The newly created buffer.
         *
         *
         */
        const_buffer(void const* data, saga::ssize_t size);
        
        /*! \brief Destroys a buffer.     
         *
         *
         */
        ~const_buffer();
        
        /*! \brief Retrieves the current value for size. 
         * \return Value of size.
         *
         *
         */
        saga::ssize_t get_size() const;
        
        /*! \brief Retrieves the buffer data.     
         * \return Buffer data to retrieve.
         *
         *
         */
        void const* get_data() const;

        /*! \brief Closes the object.  
         * \param timeout Seconds to wait
         *
         *
         */
        void close(double timeout = 0.0);
    };
    
    /*! \brief Brief %description starts here     
     *
     *
    */
    class SAGA_EXPORT mutable_buffer
      : public const_buffer
    {
    protected:
        /// @cond
        /** These methods are not within API scope */
        TR1::shared_ptr <saga::impl::buffer> get_impl_sp (void) const;
        saga::impl::buffer* get_impl (void) const;

        friend class saga::impl::buffer;

        mutable_buffer(saga::impl::buffer* impl);
        /// @endcond

    public:
        /// @cond
        /** These methods are not within API scope */
        typedef void buffer_deleter_type(void* data);
        typedef TR1::function<buffer_deleter_type> buffer_deleter;

        //  The default deleter simply does nothing
        static void default_buffer_deleter(void* data);
        /// @endcond
    
    public:
        /*! \brief Brief %description starts here     
         *
         *
         */
        mutable_buffer(saga::ssize_t size = -1);
        
        /*! \brief Brief %description starts here     
         *
         *
         */
        mutable_buffer(void* data, saga::ssize_t size);
        
        /*! \brief Brief %description starts here     
         *
         *
         */
        mutable_buffer(void* data, saga::ssize_t size, buffer_deleter cb);
        
        /*! \brief Brief %description starts here     
         *
         *
         */
        ~mutable_buffer();
    
        /*! \brief Brief %description starts here     
         *
         *
         */
        void set_size(saga::ssize_t size = -1);
        
        /*! \brief Brief %description starts here     
         *
         *
         */
        void set_data(void* data, saga::ssize_t size,
            buffer_deleter cb = default_buffer_deleter);

        /*! \brief Brief %description starts here     
         *
         *
         */
        void* get_data();
    };
    
    ///////////////////////////////////////////////////////////////////////////
    inline mutable_buffer 
    buffer(void* data, saga::size_t size)
    {
        return mutable_buffer(data, size);
    }
    
    inline const_buffer 
    buffer(void const* data, saga::size_t size)
    {
        return const_buffer(data, size);
    }
    
    ///////////////////////////////////////////////////////////////////////////
    template <typename PodType, std::size_t N>
    inline mutable_buffer 
    buffer(PodType (&data)[N])
    {
        return mutable_buffer(data, N * sizeof(PodType));
    }
    
    template <typename PodType, std::size_t N>
    inline const_buffer 
    buffer(PodType const (&data)[N])
    {
        return const_buffer(data, N * sizeof(PodType));
    }
    
    ///////////////////////////////////////////////////////////////////////////
    template <typename PodType, std::size_t N>
    inline mutable_buffer 
    buffer(PodType (&data)[N], saga::size_t max_size)
    {
        return mutable_buffer(data, 
            N * sizeof(PodType) < max_size ? N * sizeof(PodType) : max_size);
    }
    
    template <typename PodType, std::size_t N>
    inline const_buffer 
    buffer(PodType const (&data)[N], saga::size_t max_size)
    {
        return const_buffer(data, 
            N < max_size ? N * sizeof(PodType) : max_size * sizeof(PodType));
    }
    
    ///////////////////////////////////////////////////////////////////////////
    template <typename PodType, typename Allocator>
    inline mutable_buffer 
    buffer(std::vector<PodType, Allocator>& data)
    {
        return mutable_buffer(&data[0], data.size() * sizeof(PodType));
    }
    
    template <typename PodType, typename Allocator>
    inline const_buffer 
    buffer(std::vector<PodType, Allocator> const& data)
    {
        return const_buffer(&data[0], data.size() * sizeof(PodType));
    }
    
    ///////////////////////////////////////////////////////////////////////////
    template <typename PodType, typename Allocator>
    inline mutable_buffer 
    buffer(std::vector<PodType, Allocator>& data, saga::size_t max_size)
    {
        return mutable_buffer(&data[0], 
            data.size() < max_size ? 
                data.size() * sizeof(PodType) : max_size * sizeof(PodType));
    }
    
    template <typename PodType, typename Allocator>
    inline const_buffer 
    buffer(std::vector<PodType, Allocator> const& data, saga::size_t max_size)
    {
        return const_buffer(&data[0], 
            data.size() < max_size ? 
                data.size() * sizeof(PodType) : max_size * sizeof(PodType));
    }
    
    ///////////////////////////////////////////////////////////////////////////
    template <typename Char, typename Traits, typename Allocator>
    inline const_buffer 
    buffer(std::basic_string<Char, Traits, Allocator> const& data)
    {
        return const_buffer(data.data(), data.size());
    }
    
    template <typename Char, typename Traits, typename Allocator>
    inline const_buffer 
    buffer(std::basic_string<Char, Traits, Allocator> const& data, 
        saga::size_t max_size)
    {
        return const_buffer(data.data(), data.size() < max_size ? 
            data.size() * sizeof(Char) : max_size * sizeof(Char));
    }
    
 } // namespace saga

// re-enable warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

#endif // SAGA_SAGA_BUFFER_HPP
