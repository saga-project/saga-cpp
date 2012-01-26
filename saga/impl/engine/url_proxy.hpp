//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(SAGA_IMPL_URL_PROXY_FEB_10_2009_0133PM)
#define SAGA_IMPL_URL_PROXY_FEB_10_2009_0133PM

#include <string>

#include <saga/saga/session.hpp>
#include <saga/impl/url_cpi.hpp>
#include <saga/impl/engine/proxy.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl
{
    class url_proxy : public proxy
    {
    private:
        typedef v1_0::url_cpi url_cpi;
        typedef v1_0::preference_type preference_type;

      // initialize newly attached CPI instance
      void init();

    public:
        url_proxy(saga::session const& s, saga::url const& url);
        ~url_proxy();

        SAGA_CALL_IMPL_DECL_1(translate, std::string)
    };

}}

#endif
