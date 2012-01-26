//  future class

//  Copyright (c) 2005 Hartmut Kaiser. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUTURE_HPP
#define BOOST_FUTURE_HPP 1

#include <boost/shared_ptr.hpp>

#include <boost/futures/future_base.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace boost {  namespace futures { 
  
    ///////////////////////////////////////////////////////////////////////////
    namespace detail {

        template <typename FutureResult>
        struct future_impl_base
        {
            virtual ~future_impl_base() {};
            virtual FutureResult get() = 0;
            virtual bool done() = 0;
        };

        template <typename Future, typename FutureResult>
        struct future_impl
        :   future_impl_base<FutureResult>
        {
            future_impl(Future const& f) : f(f) {}
            ~future_impl() {}
            
            FutureResult
            get()
            {
                return f();
            }

            bool
            done()
            {
                return f.done();
            }
            
            Future f;
        };
    }  

    ///////////////////////////////////////////////////////////////////////////
    template <typename FutureResult>
    struct future : future_base<future<FutureResult> >
    {
        typedef FutureResult result_type;

        future() 
        {}
        
        template <typename Future>
        future(future_base<Future> const& f)
            : impl(new detail::future_impl<Future, FutureResult>(f.derived()))
        {}
        
        template <typename Future>
        future
        operator= (future_base<Future> const& f)
        {
            impl.reset(new detail::future_impl<Future, FutureResult>(f.derived()));
            return *this;
        }
        
        FutureResult
        operator()()
        {
            return impl->get();
        }

        bool
        done()
        {
            return impl->done();
        }

        boost::shared_ptr<detail::future_impl_base<FutureResult> > impl;
    };
  
///////////////////////////////////////////////////////////////////////////////
}}  // namespace boost::futures

#endif /* BOOST_FUTURIZED_HPP */
