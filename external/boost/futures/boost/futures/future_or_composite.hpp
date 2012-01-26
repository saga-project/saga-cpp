//  or operator on futures

//  Copyright (c) 2005 Thorsten Schuett. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUTURE_OR_COMPOSITE_HPP
#define BOOST_FUTURE_OR_COMPOSITE_HPP 1

#include <boost/smart_ptr.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/size.hpp>

#include <boost/futures/future.hpp>
#include <boost/futures/compose.hpp>
#include <boost/futures/composite.hpp>
#include <boost/futures/detail/composite_result.hpp>
#include <boost/futures/detail/threaded_or_evaluator.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace boost {  namespace futures { 
  
  /////////////////////////////////////////////////////////////////////////////
  //
  //  This is the operator||()
  //
  /////////////////////////////////////////////////////////////////////////////

  template<typename F1, typename F2>
  inline typename
      lazy_enable_if<
          mpl::and_<
              futures::detail::can_convert<F1>, 
              futures::detail::can_convert<F2> 
          >,
          as_composite<or_op, F1, F2>
      >::type
  operator|| (F1 const& f1, F2 const& f2)
  {
      return compose<or_op>(f1, f2);
  }

///////////////////////////////////////////////////////////////////////////////
}}  // namespace boost::futures

#endif /* BOOST_FUTURE_OR_COMPOSITE_HPP */
