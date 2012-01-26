//  helper class for composite_future<or_op, T>

//  Copyright (c) 2005 Thorsten Schuett. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef FUTURES_DETAIL_THREADED_OR_EVALUATOR_HPP
#define FUTURES_DETAIL_THREADED_OR_EVALUATOR_HPP 1

#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/remove_reference.hpp>

#include <boost/spirit/fusion/iterator/equal_to.hpp>
#include <boost/spirit/fusion/iterator/next.hpp>
#include <boost/spirit/fusion/iterator/deref.hpp>
#include <boost/spirit/fusion/iterator/value_of.hpp>
#include <boost/spirit/fusion/algorithm/for_each.hpp>

#include <boost/futures/future.hpp>

////////////////////////////////////////////////////////////////////////////////
namespace boost {  namespace futures {  namespace detail { 
  
    ////////////////////////////////////////////////////////////////////////////
    template <typename Tuple, typename Result>
    class threaded_evaluator<or_op, Tuple, Result>
    {
        typedef threaded_evaluator<or_op, Tuple, Result> self_type;

    public:
        threaded_evaluator(Tuple operands_)
        : operands(operands_), joined(false)
        {
        }

        Result get_value()
        {
            // there will be never more than one thread joining because of the lock
            boost::mutex::scoped_lock scoped_lock(lock);

            if (joined)
                return data;

            if (!done_or_register()) {
            // one of the referenced futures has joined in the meantime
                cleanup();
                return data;
            }
            
        // none of the referenced futures has joined yet, wait for the first
            joined_condition.wait(scoped_lock);
            joined = true;

            cleanup();
            return data;
        }

        bool done()
        {
            boost::mutex::scoped_lock scoped_lock(lock);
            return joined;
        }
      
    private:
    //  register our callback with all futures referenced by this composite
        template <typename RT>
        struct set_value
        {
            set_value(self_type& self) : self(self) {}
            
            void operator()(RT data_)
            {
                boost::mutex::scoped_lock scoped_lock(self.data_lock);
                self.data = data_;
                self.joined_condition.notify_one();
            }
            
            self_type &self;
        };

        template <typename First, typename Last>
        bool done_or_register(First const&, Last const&, mpl::true_)
        {
            return true;
        }
        
        template <typename First, typename Last>
        bool done_or_register(First first, Last last, mpl::false_)
        {
            typedef typename fusion::meta::value_of<First>::type element_type;
            typedef typename element_type::first_type::result_type first_type;
            
            int id = (*first).first.done_or_register(
                boost::bind<void>(set_value<first_type>(*this), _1));
            if (-1 == id)
                return false;
            
            (*first).second = id;
            
            return done_or_register(fusion::next(first), last, 
                fusion::meta::equal_to<
                    BOOST_DEDUCED_TYPENAME fusion::meta::next<First>::type, 
                    Last
                >());
        }
        
        bool done_or_register()
        {
            return done_or_register(
                fusion::begin(operands), fusion::end(operands), 
                fusion::meta::equal_to<
                    BOOST_DEDUCED_TYPENAME fusion::meta::begin<Tuple>::type,
                    BOOST_DEDUCED_TYPENAME fusion::meta::end<Tuple>::type
                >());
        }

    //  cleanup the registration of our callback function with all referenced 
    //  futures        
        struct cleanup_functor
        {
            template <typename Value>
            void operator()(Value& v) const
            {
                v.first.unregister(v.second);
            }
        };
        
        void cleanup()
        {
            fusion::for_each(operands, cleanup_functor());
        }

        Tuple operands;
        bool joined;
        Result data;
        boost::condition joined_condition;
        boost::mutex lock;
        boost::mutex data_lock;
    };

///////////////////////////////////////////////////////////////////////////////
}}}  // namespace boost::futures::detail

#endif /* FUTURES_DETAIL_THREADED_OR_EVALUATOR_HPP */
