/*=============================================================================
    Copyright (c) 2001-2005 Joel de Guzman
    Copyright (c) 2004-2005 Hartmut Kaiser

    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at 
    http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(FUTURES_COMPOSITE_HPP)
#define FUTURES_COMPOSITE_HPP

#include <boost/futures/config.hpp>

#include <boost/spirit/fusion/sequence/tuple.hpp>
#include <boost/spirit/fusion/sequence/at.hpp>
#include <boost/spirit/fusion/sequence/generate.hpp>
#include <boost/spirit/fusion/algorithm/transform.hpp>

#include <boost/futures/future.hpp>

namespace boost { namespace futures {

    ////////////////////////////////////////////////////////////////////////////    
    namespace detail
    {
        template <typename Op, typename Tuple>
        struct composite_result;

        template <typename Op, typename Tuple, typename Result>
        class threaded_evaluator;
    }

    ////////////////////////////////////////////////////////////////////////////    
    template <typename Op, typename Tuple>
    struct composite 
    :   Tuple, future_base<composite<Op, Tuple> >
    {
        typedef Tuple tuple_type;
        typedef Tuple base_type;
        typedef Op op_type;

        typedef typename detail::composite_result<Op, Tuple>::type result_type;

    private:
        typedef detail::threaded_evaluator<Op, base_type, result_type> 
            evaluator_type;

    public:        
    // construction
        composite()
        :   base_type(), 
            internal(new evaluator_type(*this)) 
        {}

        composite(base_type const& base)
        :   base_type(base), 
            internal(new evaluator_type(*this)) 
        {}

        template <typename U0>
        composite(U0& _0)
        :   base_type(_0), 
            internal(new evaluator_type(*this))  
        {}

        template <typename U0, typename U1>
        composite(U0& _0, U1& _1)
        :   base_type(_0, _1), 
            internal(new evaluator_type(*this))  
        {}

        //  Bring in the rest of the constructors
        #include <boost/futures/detail/composite_impl.hpp>

    // futures interface functions
        bool done() const
        {
            return internal->done();
        }
        
        result_type operator()() const
        {
            return internal->get_value();
        }

    // every future should be able to notify about it's status    
        int done_or_register(boost::function<void (result_type)> callback) const
        {
            return internal->done_or_register(callback);
        }

        void unregister(int& id) const
        {
            internal->unregister(id);
        }

    private:
        boost::shared_ptr<evaluator_type> internal;
    };

// the following isn't currently needed, but may be useful in the future
    template <int N, typename Op, typename Tuple>
    inline typename fusion::meta::at_c<Tuple const, N>::type
    get(composite<Op, Tuple> const& c)
    {
        return fusion::at<N>(c);
    }
}}

#endif // !defined(FUTURES_COMPOSITE_HPP)

