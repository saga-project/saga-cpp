//  helper class for future

//  Copyright (c) 2005 Thorsten Schuett. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_THREADED_FUTURE_HPP
#define BOOST_THREADED_FUTURE_HPP 1

#include <boost/function.hpp>
#include <boost/thread.hpp>
#include <boost/assert.hpp>

#include <boost/futures/futures_forward.hpp>
#include <boost/futures/detail/callback_registry.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace boost {  namespace futures {  namespace detail { 

    ////////////////////////////////////////////////////////////////////////////
    template<typename T>
    class threaded_future
    {
    private:
        template<typename Func>
        class threaded_internal
        {
        public:
            threaded_internal(Func actor_, threaded_future<T> *outer_) 
            : actor(actor_), outer(outer_) {}

            void operator()()
            {
                outer->notify(actor());
            }

        private:
            Func actor;
            threaded_future<T> *outer;
        };
        
        threaded_future* this_() { return this; }
        
    public:
        template<typename Func>
        threaded_future(Func actor) 
          : joined(false), 
#if BOOST_VERSION >= 103500
            canceled_(false), 
#endif
            thr(threaded_internal<Func>(actor, this_()))
        { }

        ~threaded_future()
        {
            if (!done()) {
#if BOOST_VERSION >= 103500
                try {
                    thr.join();
                }
                catch (boost::thread_interrupted const&) {
                    canceled_ = true;
                }
#else
                thr.join();
#endif
            }
        }

        T get()
        {
            if (!done()) {
#if BOOST_VERSION >= 103500
                try {
                    thr.join();
                }
                catch (boost::thread_interrupted const&) {
                    canceled_ = true;
                }
#else
                thr.join();
#endif
            }
            return data;
        }

        bool done()
        {
            return joined;
        }

        template <typename Func>
        int done_or_register(Func const& setter)
        {
            boost::mutex::scoped_lock scoped_lock(lock);
            if (done()) {
                setter(data);
                return -1;
            }
            return registry.register_callback(setter);
        }

        void unregister(int& id)
        {
            if (-1 != id) {
                boost::mutex::scoped_lock scoped_lock(lock);
                registry.unregister_callback(id);
                id = -1;            // reset the cookie
            }
        }

        void cancel()
        {
            boost::mutex::scoped_lock scoped_lock(lock);
            if (!joined) {
#if BOOST_VERSION >= 103500
                thr.interrupt();
#endif
                joined = true;
            }
        }

        bool canceled() const
        {
#if BOOST_VERSION >= 103500
            return canceled_;
#else
            return false;
#endif
        }

    private:
        void notify(T const& data_)
        {
            boost::mutex::scoped_lock scoped_lock(lock);
            data = data_;
            joined = true;
            registry.notify_callbacks(data_);
        }

        visitor_registry<T> registry;
        boost::mutex lock;
        T data;
        bool joined;
#if BOOST_VERSION >= 103500
        bool canceled_;
#endif
        boost::thread thr;
    };

///////////////////////////////////////////////////////////////////////////////
}}}  // namespace boost::futures::detail

#endif /* BOOST_THREADED_FUTURE_HPP */
