//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_IMPL_PACKAGES_JOB_ISTREAM_HPP
#define SAGA_IMPL_PACKAGES_JOB_ISTREAM_HPP

#include <boost/config.hpp>
#if defined(BOOST_WINDOWS)
#include <windows.h>    // for CloseHandle
#endif

#include <saga/saga/job.hpp>
#include <saga/impl/job.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga {

    namespace impl { 
        ///////////////////////////////////////////////////////////////////////
        class istream_interface
        {
        public:
#if defined(BOOST_WINDOWS)
            typedef void* handle_type;
#else
            typedef int handle_type;
#endif

            virtual ~istream_interface() {}
            virtual std::streambuf *get_streambuf() = 0;
            virtual handle_type get_handle(bool detach) const 
            { 
                return (handle_type)(-1); 
            }
        };

    ///////////////////////////////////////////////////////////////////////////
    }
    
    ///////////////////////////////////////////////////////////////////////////
    namespace adaptors {

        ///////////////////////////////////////////////////////////////////////
        //  Use this template to implement your istream implementation if you
        //  need to provide your own streambuf implementation
        template <typename StreamBuf>
        class istream 
        :   public saga::impl::istream_interface
        {
        public:
            istream(handle_type h) : h_(h) {}
            ~istream() 
            {
                if ((handle_type)(-1) != h_) {
#if defined(BOOST_WINDOWS)
                    ::CloseHandle(h_);
#else
                    ::close(h_);
#endif
                }
            }
            std::streambuf *get_streambuf() 
            {
                return &buffer_;
            }
            handle_type get_handle(bool detach) const 
            {
                handle_type h = h_;
                if (detach)
                    h_ = (handle_type)(-1);
                return h; 
            }

        private:
            StreamBuf buffer_;
            handle_type h_;
        };

        ///////////////////////////////////////////////////////////////////////
        //  Use this template to implement your istream implementation if you
        //  need to use a existing streambuf instance
        class istream_ptr
        :   public saga::impl::istream_interface
        {
        public:            
            istream_ptr(std::streambuf *buffer, handle_type h) 
                : buffer_(buffer), h_(h) {}
            ~istream_ptr() 
            {
                if ((handle_type)(-1) != h_) {
#if defined(BOOST_WINDOWS)
                    ::CloseHandle(h_);
#else
                    ::close(h_);
#endif
                }
            }
            std::streambuf *get_streambuf() 
            {
                return buffer_;
            }
            handle_type get_handle(bool detach) const 
            {
                handle_type h = h_;
                if (detach)
                    h_ = (handle_type)(-1);
                return h; 
            }

        private:
            std::streambuf *buffer_;
            mutable handle_type h_;
        };

    ///////////////////////////////////////////////////////////////////////////
    }

///////////////////////////////////////////////////////////////////////////////
}    // namespace saga

#endif // SAGA_IMPL_PACKAGES_JOB_ISTREAM_HPP

