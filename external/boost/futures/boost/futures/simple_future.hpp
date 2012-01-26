//  simple future using threads

//  Copyright (c) 2005 Thorsten Schuett. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_SIMPLEFUTURE_HPP
#define BOOST_SIMPLEFUTURE_HPP 1

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

#include <boost/futures/future_base.hpp>
#include <boost/futures/detail/threaded_future.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace boost {  namespace futures { 
  
  /////////////////////////////////////////////////////////////////////////////
  template <typename T>
  class simple_future 
  :   public future_base<simple_future<T> >
  {
  public:
      typedef T result_type;

      simple_future(void) 
      {
      }

      template <typename Func>
      simple_future(Func actor) 
        : internal(new boost::futures::detail::threaded_future<T>(actor))
      {
      }

  // main future interface
      bool done() const
      {
          return internal->done();
      }

      T operator()() const
      {
          return internal->get();
      }

      void cancel()
      {
          internal->cancel();
      }

      bool canceled() const
      {
          return internal->canceled();
      }

    // I still like it, basta ;-)
//       operator T() const
//       {
//           return internal->get();
//       }

  // every future should be able to notify about it's status    
      int done_or_register(boost::function<void (T)> callback) const
      {
          return internal->done_or_register(callback);
      }

      void unregister(int& id) const
      {
          internal->unregister(id);
      }
      
  private:
      boost::shared_ptr<boost::futures::detail::threaded_future<T> > internal;
  }; 

///////////////////////////////////////////////////////////////////////////////
}}  // namespace boost::futures

#endif /* BOOST_SIMPLEFUTURE_HPP */
