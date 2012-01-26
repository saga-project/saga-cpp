//  Copyright (c) 2005-2011 Hartmut Kaiser
// 
//  Use, modification and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <string>
#include <iostream>

#include <saga/saga/url.hpp>
#include <saga/saga/detail/call.hpp>
#include <saga/impl/url.hpp>
#include <saga/impl/exception.hpp>
#include <saga/impl/session.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga 
{
  url::url (std::string const & urlstr)
    : saga::object (new impl::url (urlstr))
  {
  }
  
  url::url (char const * urlstr)
    : saga::object (new impl::url (std::string(urlstr)))
  {
  }

  url::url (std::string const & urlstr, adaptors::nocheck)
    : saga::object (new impl::url(urlstr, adaptors::nocheck()))
  {
  }
  
  url::url (impl::url *impl)
    : saga::object (impl)
  {
  }
  
  url::url (saga::object obj)
    : saga::object (obj.clone())
  {
    if (this->get_type() != saga::object::URL)
    {
        SAGA_THROW("Bad type conversion.", saga::BadParameter);
    }
  }
  
  url::url (saga::url const& rhs)
    : saga::object (rhs.clone())
  {
  }
  
  url::url (void)
    : saga::object (new impl::url(""))
  {
  }
  
  url::~url (void)
  {
  }
  
  url& url::operator=(char const * urlstr)
  {
    set_url(urlstr);
    return *this;
  }

  url& url::operator=(std::string const & urlstr)
  {
    set_url(urlstr);
    return *this;
  }

  url& url::operator=(url const& rhs)
  {
    if (&rhs != this) 
        this->saga::object::operator=(rhs.clone());
    return *this;
  }

  url& url::operator=(object const& rhs)
  {
    if (rhs.get_type() != saga::object::URL)
    {
        SAGA_THROW("Bad type conversion.", saga::BadParameter);
    }
    
    if (&rhs != this) 
        this->saga::object::operator=(rhs.clone());

    return *this;
  }

  saga::impl::url* url::get_impl() const
  { 
    typedef saga::object base_type;
    return static_cast<impl::url*>(this->base_type::get_impl());
  }

  TR1::shared_ptr <saga::impl::url> url::get_impl_sp() const
  { 
    typedef saga::object base_type;
    return TR1::static_pointer_cast<impl::url>(this->base_type::get_impl_sp());
  }

  saga::object url::clone (void) const
  {
    return get_impl()->clone();
  }
  
  std::string url::get_url (void) const
  {
    return get_impl()->get_url_escaped();
  }

  void url::set_url(std::string const & url)
  {
    get_impl()->set_url(url);
  }
  
  std::string url::get_string (void) const
  {
    return get_impl()->get_url_escaped(false);
  }

  std::string url::get_url_escaped(bool hide_password) const
  {
    return get_impl()->get_url_escaped(hide_password);
  }

  void url::set_string(std::string const & url)
  {
    get_impl()->set_url(url);
  }
  
  std::string url::get_authority (void) const
  {
    return get_impl()->get_authority ();
  }
  
  std::string url::get_scheme (void) const
  {
    return get_impl()->get_scheme ();
  }
  
  std::string url::get_host (void) const
  {
    return get_impl()->get_host ();
  }
  
  int url::get_port (void) const
  {
    return get_impl()->get_port ();
  }
  
  std::string url::get_fragment (void) const
  {
    return get_impl()->get_fragment_escaped();
  }
  
  std::string url::get_path(void) const
  {
    return get_impl()->get_path_escaped();
  }

  std::string url::get_query (void) const
  {
    return get_impl()->get_query_escaped();
  }

  std::string url::get_userinfo (void) const
  {
    return get_impl()->get_userinfo ();
  }

  std::string url::get_username (void) const
  {
    return get_impl()->get_username ();
  }

  std::string url::get_password (void) const
  {
    return get_impl()->get_password ();
  }

  void url::set_scheme (std::string const & scheme)
  {
    get_impl()->change_scheme (scheme);
  }

  void url::set_scheme_specific_part (std::string const & scheme_specific_part)
  {
    get_impl()->change_scheme_specific_part (scheme_specific_part);
  }

  void url::set_host (std::string const & host)
  {
    get_impl()->change_host (host);
  }

  void url::set_port (int port)
  {
    get_impl()->change_port (port);
  }
  
  void url::set_authority (std::string const & auth)
  {
    return get_impl()->set_authority (auth);
  }
  
  void url::set_fragment (std::string const & fragment)
  {
    get_impl()->change_fragment (fragment);
  }

  void url::set_path (std::string const & path)
  {
    get_impl()->change_path (path);
  }

  void url::set_query (std::string const & query)
  {
    get_impl()->change_query (query);
  }

  void url::set_userinfo(std::string const & userinfo)
  {
    get_impl()->change_userinfo (userinfo);
  }

  void url::set_username(std::string const & username)
  {
    get_impl()->change_username (username);
  }

  void url::set_password(std::string const & password)
  {
    get_impl()->change_password (password);
  }

  std::ostream& operator<< (std::ostream& os, url const& u)
  {
    os << u.get_string();
    return os;
  }

  std::istream& operator>> (std::istream& is, url& u)
  {
    std::string s;
    is >> s;
    u = s;
    return is;
  }

  bool operator== (saga::url const& lhs, saga::url const& rhs)
  {
      return lhs.get_url_escaped() == rhs.get_url_escaped();
  }
  
  bool operator!= (saga::url const& lhs, saga::url const& rhs)
  {
      return ! (lhs.get_url_escaped() == rhs.get_url_escaped());
  }
  
  bool operator< (saga::url const& lhs, saga::url const& rhs)
  {
      return lhs.get_url_escaped() < rhs.get_url_escaped();
  }

  std::string url::unescape(std::string const&in)
  {
      return saga::impl::unescape_url(in);
  }
  
  std::string url::escape(std::string const&in)
  {
      return saga::impl::escape_url(in);
  }

  /////////////////////////////////////////////////////////////////////////////
  saga::task 
  url::translatepriv(std::string scheme, saga::task_base::Sync) const
  {
    return get_impl()->translate_impl(detail::get_the_session(), scheme, true);
  }

  saga::task 
  url::translatepriv(std::string scheme, saga::task_base::Async) const
  {
    return saga::detail::run(get_impl()->translate_impl(
        detail::get_the_session(), scheme));
  }

  saga::task 
  url::translatepriv(std::string scheme, saga::task_base::Task) const
  {
    return get_impl()->translate_impl(detail::get_the_session(), scheme);
  }

  saga::task 
  url::translatepriv(saga::session s, std::string scheme, saga::task_base::Sync) const
  {
    return get_impl()->translate_impl(s, scheme, true);
  }

  saga::task 
  url::translatepriv(saga::session s, std::string scheme, saga::task_base::Async) const
  {
    return saga::detail::run(get_impl()->translate_impl(s, scheme));
  }

  saga::task 
  url::translatepriv(saga::session s, std::string scheme, saga::task_base::Task) const
  {
    return get_impl()->translate_impl(s, scheme);
  }

} // namespace saga
///////////////////////////////////////////////////////////////////////////////

