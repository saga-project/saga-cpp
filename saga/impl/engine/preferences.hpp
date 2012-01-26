//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_IMPL_ENGINE_PREFERENCES_HPP
#define SAGA_IMPL_ENGINE_PREFERENCES_HPP

#include <string>
#include <set>

#include <saga/saga/context.hpp>
#include <boost/assert.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl { namespace v1_0 
{
    // used for adaptor and method preferences
    struct preferences
    {
        typedef std::set<std::string> data_type;
        typedef data_type::iterator iterator;
        typedef data_type::const_iterator const_iterator;

        void set(std::string const& key, std::string const& value)
        {
            BOOST_ASSERT(saga::attributes::context_type == key);
            data_.insert(value);
        }
        void clear()
        {
            data_.clear();
        }

        friend bool operator< (preferences const& lhs, preferences const& rhs) 
        {
            return lhs.data_ < rhs.data_;
        }

        data_type data_;
    };

    typedef preferences preference_type;

}}}

#endif

