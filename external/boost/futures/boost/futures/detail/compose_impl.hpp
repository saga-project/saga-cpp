/*=============================================================================
    Copyright (c) 2001-2005 Joel de Guzman
    Copyright (c) 2004-2005 Hartmut Kaiser

    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at 
    http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(FUTURES_DETAIL_COMPOSE_IMPL_HPP)
#define FUTURES_DETAIL_COMPOSE_IMPL_HPP

#include <boost/spirit/fusion/sequence/tuple.hpp>
#include <boost/spirit/fusion/sequence/joint_view.hpp>
#include <boost/spirit/fusion/algorithm/push_back.hpp>
#include <boost/spirit/fusion/algorithm/push_front.hpp>

#include <boost/futures/composite.hpp>

////////////////////////////////////////////////////////////////////////////////
namespace boost { namespace futures {

    namespace detail
    {
        template <typename Op, typename A, typename B>
        struct compose_composite
        {
            // case: A and B are both primitives or composites 
            //       with different Ops. i.e. A::op_type != B::op_type

            typedef std::pair<A, int> data_a_type;
            typedef std::pair<B, int> data_b_type;
            typedef composite<Op, 
                    fusion::tuple<data_a_type, data_b_type> 
                > const type;
            
            static type
            eval(A const& a, B const& b)
            {
                data_a_type data_a(a, -1);
                data_b_type data_b(b, -1);
                return type(data_a, data_b);
            }
        };

        template <typename Op, typename A, typename B>
        struct compose_composite<Op, composite<Op, A>, B>
        {
            // case: B is a primitive or a composite with 
            //       a different Op, i.e. B::op_type != Op
            //       A is a composite where A::op_type == Op

            typedef std::pair<B, int> data_b_type;
            typedef typename fusion::meta::push_back<A, data_b_type>::type 
                join_type;
            typedef typename fusion::meta::generate<join_type>::type tuple_type;
            typedef composite<Op, tuple_type> const type;

            static type
            eval(composite<Op, A> const& a, B const& b)
            {
                data_b_type data_b(b, -1);
                return fusion::generate(fusion::push_back(a, data_b));
            }
        };

        template <typename Op, typename A, typename B>
        struct compose_composite<Op, A, composite<Op, B> >
        {
            // case: A is a primitive or a composite with 
            //       a different Op, i.e. A::op_type != Op
            //       B is a composite where B::op_type == Op

            typedef std::pair<A, int> data_a_type;
            typedef typename fusion::meta::push_front<B, data_a_type>::type 
                join_type;
            typedef typename fusion::meta::generate<join_type>::type tuple_type;
            typedef composite<Op, tuple_type> const type;

            static type
            eval(A const& a, composite<Op, B> const& b)
            {
                data_a_type data_a(a, -1);
                return fusion::generate(fusion::push_front(b, data_a));
            }
        };

        template <typename Op, typename A, typename B>
        struct compose_composite<
            Op,
            composite<Op, A>,
            composite<Op, B> >
        {
            // case: A and B are both composites 
            //       where A::op_type == B::op_type 

            typedef typename fusion::joint_view<A, B>::type join_type;
            typedef typename fusion::meta::generate<join_type>::type tuple_type;
            typedef composite<Op, tuple_type> const type;

            static type
            eval(composite<Op, A> const& a, composite<Op, B> const& b)
            {
                return fusion::generate(join_type(a, b));
            }
        };
    } 
}}

#endif // !defined(FUTURES_DETAIL_COMPOSE_IMPL_HPP)
