//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_DETAIL_PREFERENCES_HPP
#define SAGA_DETAIL_PREFERENCES_HPP

///////////////////////////////////////////////////////////////////////////////
namespace saga { 

  namespace impl { namespace v1_0 { struct preferences; }}
  namespace detail
  {
      typedef impl::v1_0::preferences preference_type;
      SAGA_EXPORT preference_type const& get_preferences(saga::session const&);
  }

} // namespace saga

#endif
