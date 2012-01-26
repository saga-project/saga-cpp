//  helper class for composite_future<and_op, T>

//  Copyright (c) 2005 Thorsten Schuett. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef FUTURES_DETAIL_THREADED_AND_EVALUATOR_HPP
#define FUTURES_DETAIL_THREADED_AND_EVALUATOR_HPP 1

#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/remove_reference.hpp>

#include <boost/spirit/fusion/sequence/get.hpp>
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
    class threaded_evaluator<and_op, Tuple, Result>
    {
        typedef threaded_evaluator<and_op, Tuple, Result> self_type;

    public:
        threaded_evaluator(Tuple operands_)
        : operands(operands_), joined(false)
        { }

        Result get_value()
        {
            // there will be never more than one thread joining because of the lock
            boost::mutex::scoped_lock scoped_lock(lock);

            if (joined)
                return data;

	    missing = done_or_register();
	    if(missing == 0){
            // all of the referenced futures have joined in the meantime
                cleanup();
                return data;
            }
            
        // not all of the referenced futures have joined yet, wait for the rest
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
        template <typename RT, int I>
        struct set_value
        {
            set_value(self_type& self) : self(self) {}
            
            void operator()(RT data_)
            {
                boost::mutex::scoped_lock scoped_lock(self.data_lock);
                fusion::get<I>(self.data) = data_;
		self.missing--;
		if(self.missing == 0)
		  self.joined_condition.notify_one();
            }
            
            self_type &self;
        };

        template <typename First, typename Last, int Idx>
        int done_or_register(First first, Last last, mpl::false_){
	  typedef typename fusion::meta::value_of<First>::type element_type;
	  typedef typename element_type::first_type::result_type first_type;
	  typedef typename fusion::meta::next<First>::type next_type;
	  typedef typename fusion::meta::equal_to<
	    BOOST_DEDUCED_TYPENAME fusion::meta::next<First>::type, 
	    Last
	    >::type done_type;
	  
	  int id = (*first).first.done_or_register(
	    boost::bind<void>(set_value<first_type, Idx>(*this), _1));
	  
	  (*first).second = id;
	  
	  int result = done_or_register<next_type, Last, Idx + 1>(
	    fusion::next(first), last, done_type()); 
	  if(-1 == id)
	    return result;     //future has already joined
	  else
	    return 1 + result; //future has not joined
	}

        template <typename First, typename Last, int Idx>
        int done_or_register(First first, Last last, mpl::true_){
	    return 0;
	}

        int done_or_register()
        {
	  typedef typename fusion::meta::begin<Tuple>::type first_type;
	  typedef typename fusion::meta::end<Tuple>::type last_type;
	  typedef typename fusion::meta::equal_to<
	    BOOST_DEDUCED_TYPENAME fusion::meta::begin<Tuple>::type, 
	    BOOST_DEDUCED_TYPENAME fusion::meta::end<Tuple>::type
	    >::type done_type;

	  return done_or_register<first_type, last_type, 0>(                
		fusion::begin(operands), fusion::end(operands), done_type());
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
        boost::mutex lock;
        boost::mutex data_lock;
        boost::condition joined_condition;
        int missing;
        bool joined;
        Result data;
    };

///////////////////////////////////////////////////////////////////////////////
}}}  // namespace boost::futures::detail

#endif /* FUTURES_DETAIL_THREADED_OR_EVALUATOR_HPP */
