//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(SAGA_ADAPTORS_URL_HPP)
#define SAGA_ADAPTORS_URL_HPP

#include <saga/saga/url.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable: 4251 4231 4275 4660)
#endif

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace adaptors
{
    /*! \brief Brief %description starts here     
     *
     *   The only difference of this class if compared to the saga::url class 
     *   is that it doesn't check the url on construction time, allowing to 
     *   speed up the creation of large amounts of objects. The url's will be 
     *   checked on first access to one of the member functions.
     */
    class SAGA_EXPORT url : public saga::url
    {
    public:
        url(std::string const& urlstr) : saga::url(urlstr, nocheck()) {}
    };

    /////////////////////////////////////////////////////////////////////////////
    SAGA_EXPORT std::ostream& operator<< (std::ostream& os, url const& u);
    SAGA_EXPORT std::istream& operator>> (std::istream& is, url& u);
    SAGA_EXPORT bool operator== (saga::adaptors::url const& lhs, 
        saga::adaptors::url const& rhs);
    SAGA_EXPORT bool operator< (saga::adaptors::url const& lhs, 
        saga::adaptors::url const& rhs);

///////////////////////////////////////////////////////////////////////////////
}} // namespace saga::adaptor

// re-enable warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

#endif // SAGA_ADAPTORS_URL_HPP

