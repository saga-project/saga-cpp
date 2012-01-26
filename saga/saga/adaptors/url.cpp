//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Use, modification and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <saga/saga/base.hpp>
#include <saga/saga/adaptors/url.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace adaptors
{
    std::ostream& operator<< (std::ostream& os, url const& u)
    {
        saga::url su (static_cast<saga::object>(u));
        return operator<< (os, su);
    }
    
    std::istream& operator>> (std::istream& is, url& u)
    {
        std::string s;
        is >> s;
        u = saga::adaptors::url(s);
        return is;
    }
    
    bool operator== (saga::adaptors::url const& lhs, 
        saga::adaptors::url const& rhs)
    {
        saga::url su1 (static_cast<saga::object>(rhs));
        saga::url su2 (static_cast<saga::object>(lhs));
        return su1 == su2;
    }
    
    bool operator< (saga::adaptors::url const& lhs, 
        saga::adaptors::url const& rhs)
    {
        saga::url su1 (static_cast<saga::object>(rhs));
        saga::url su2 (static_cast<saga::object>(lhs));
        return su1 < su2;
    }

///////////////////////////////////////////////////////////////////////////////
}} // namespace saga::adaptors

