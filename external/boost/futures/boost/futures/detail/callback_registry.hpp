//  helper class for future

//  Copyright (c) 2005 Thorsten Schuett. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DETAIL_CALLBACK_REGISTRY_HPP
#define BOOST_DETAIL_CALLBACK_REGISTRY_HPP 1

#include <map>

#include <boost/assert.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace boost {  namespace futures {  namespace detail { 

  /////////////////////////////////////////////////////////////////////////////
  //
  //  The visitor_registry class wraps the registration and notfication of 
  //  nullary functors. Note: this class is not thread safe by itself. Threading
  //  issues have to be taken into account at the call point.
  //
  /////////////////////////////////////////////////////////////////////////////
  template <typename T>
  class visitor_registry
  {
      typedef std::map<int, boost::function<void (T)> > callbacks_type;
    
  public:
      visitor_registry()
      : count(0)
      {
      }

      ~visitor_registry()
      {
          BOOST_ASSERT(0 == callbacks.size());
      }

      template <typename Func>
      int register_callback(Func const& cb)
      {
          boost::mutex::scoped_lock scoped_lock(lock);

          callbacks[count] = boost::function<void (T)>(cb);
          return count++;
      }
      
      void unregister_callback(int cookie)
      {
          boost::mutex::scoped_lock scoped_lock(lock);

          BOOST_ASSERT(callbacks.find(cookie) != callbacks.end());
          callbacks.erase(cookie);
      }
          
      void notify_callbacks(T const& data)
      {
          boost::mutex::scoped_lock scoped_lock(lock);

          typedef typename callbacks_type::iterator iterator;
          iterator end = callbacks.end();
          for (iterator it = callbacks.begin(); 
	       it != end; 
	       ++it)
          {
              (it->second)(data);
          }
      }
    
  private:
      boost::mutex lock;
      callbacks_type callbacks;
      int count;
  };

///////////////////////////////////////////////////////////////////////////////
}}}  // namespace boost::futures::detail

#endif /* BOOST_DETAIL_CALLBACK_REGISTRY_HPP */
