/*=============================================================================
    Copyright (c) 2005 Hartmut Kaiser

    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at 
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(FUTURES_DETAIL_COMPOSITE_OR_RESULT_HPP)
#define FUTURES_DETAIL_COMPOSITE_OR_RESULT_HPP

#include <boost/mpl/remove.hpp>
#include <boost/mpl/unique.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/size.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/variant.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/enum.hpp>

#include <boost/spirit/fusion/sequence/tuple.hpp>
#include <boost/spirit/fusion/sequence/at.hpp>
#include <boost/spirit/fusion/sequence/generate.hpp>
#include <boost/spirit/fusion/algorithm/push_front.hpp>

namespace boost { namespace futures {

    namespace detail
    {
        template <typename Pair>
        struct future_result
        {
            typedef typename Pair::first_type::result_type type;
        };
        
        ////////////////////////////////////////////////////////////////////////
        //
        //  future_result<or_op, Tuple> is a metafunction usable for calculation 
        //  of the variant type used to hold the return type of the future 'or' 
        //  composite.
        //
        ////////////////////////////////////////////////////////////////////////
        template <typename Tuple>
        struct composite_result<or_op, Tuple>
        {
        // return_types: the list of return types of the referenced futures
            typedef typename mpl::transform<
                    Tuple, future_result<mpl::_1> 
                >::type result_types;

        // remove duplicate (adjacent) types
            typedef typename mpl::unique<
                    result_types, boost::is_same<mpl::_1, mpl::_2> 
                >::type unique_types;
            
        // build a variant from the type sequence 
            typedef typename mpl::if_c<
                    1 == mpl::size<unique_types>::value,
                    typename mpl::at_c<unique_types, 0>::type,
                    typename boost::make_variant_over<unique_types>::type
                >::type type;
        };

        ////////////////////////////////////////////////////////////////////////
        //
        //  future_result<and_op, Tuple> is a metafunction usable for calculation 
        //  of the tuple type used to hold the return type of the future 'and' 
        //  composite.
        //
        ////////////////////////////////////////////////////////////////////////
        template <typename Tuple>
        struct composite_result<and_op, Tuple>
        {
        // return_types: the list of return types of the referenced futures
            typedef typename mpl::transform<
                    Tuple, future_result<mpl::_1> 
                >::type result_types;

        // build a tuple from the type sequence
            typedef typename fusion::meta::generate<result_types>::type type;
        };
    }
    
}}    // boost::futures

#endif // !defined(FUTURES_DETAIL_COMPOSITE_OR_RESULT_HPP)
