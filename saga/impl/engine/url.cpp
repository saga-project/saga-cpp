//  Copyright (c) 2005-2011 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <cctype>

#include <boost/version.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/filesystem/path.hpp>

#include <saga/saga/exception.hpp>
#include <saga/saga/url.hpp>

#include <saga/impl/exception.hpp>
#include <saga/impl/engine/url.hpp>
#include <saga/impl/engine/url_grammar.hpp>
#include <saga/impl/engine/url_proxy.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga
{
  namespace impl
  {
    /////////////////////////////////////////////////////////////////////////////
    //
    // utility calls
    //
    namespace
    {
      inline std::string escape_lit (std::string const& in)
      {
          std::string out;
          for (std::string::size_type i = 0; i < in.size(); ++i)
          {
              // leave sequences which look like escape sequences alone
              if (in[i] == '%') {
                  if (i < in.size()-2) {
                      if (std::isxdigit(in[i+1]) && std::isxdigit(in[i+2]))
                      {
                          // found such a sequence
                          out += in.substr(i, 3);
                          i += 2;
                      }
                      else {
                          out += "%25";
                      }
                  }
                  else {
                      out += "%25";
                  }
              }
              else {
                  switch(in[i]) {
                  case ' ': out += "%20"; break;
                  case '&': out += "%26"; break;
                  case '<': out += "%3C"; break;
                  case '>': out += "%3E"; break;
                  case '"': out += "%22"; break;
                  case '#': out += "%23"; break;
                  case '$': out += "%24"; break;
                  case '\'': out += "%27"; break;
                  case '+': out += "%2B"; break;
                  case ',': out += "%2C"; break;
                  case ';': out += "%3B"; break;
                  case '=': out += "%3D"; break;
                  case '?': out += "%3F"; break;
                  case '@': out += "%40"; break;
                  case '[': out += "%5B"; break;
                  case '\\': out += "%5C"; break;
                  case ']': out += "%5D"; break;
                  case '^': out += "%5E"; break;
                  case '`': out += "%60"; break;
                  case '{': out += "%7B"; break;
                  case '|': out += "%7C"; break;
                  case '}': out += "%7D"; break;
                  case '~': out += "%7E"; break;
                  default: out += in[i]; 
                      break;
                  }
              }
          }
          return out;
      }

      inline char
      unescape_char (std::string const      & str,
                     std::string::size_type   pos)
      {
        using namespace boost::spirit;
        using namespace phoenix;

        char result = 0;

        if ( ! parse (&str.c_str ()[pos], hex_escape_p[phoenix::var (result) = arg1]).hit )
          SAGA_THROW_VERBATIM(saga::object(), "invalid url: " + str,
            saga::IncorrectURL);

        return result;
      }

      inline bool
      is_escape_sequence (std::string const      & str,
                          std::string::size_type   pos)
      {
        return (std::string::npos != pos &&
                isxdigit (str[pos + 1])  &&
                isxdigit (str[pos + 2])  );
      }

      inline std::string
      unescape_lit (std::string const & value)
      {
        std::string result;
        std::string::size_type pos  = 0;
        std::string::size_type pos1 = value.find_first_of ("%", 0);

        if ( is_escape_sequence (value, pos1) )
        {
          do
          {
            result += value.substr (pos, pos1 - pos) + unescape_char (value, pos1 + 1);
            pos1    = value.find_first_of ("%", pos = pos1 + 3);
          }
          while ( is_escape_sequence (value, pos1) );

          result += value.substr (pos);
        }
        else
        {
          result = value;
        }

        return result;
      }

      std::string clean_leading_slashs(std::string s)
      {
        if (s.size() > 1  && s[0] == '/') {
            // check if we actually got a relative path
            if (s[1] == '.')
              return s.substr(1);

            // remove leading slash's
            std::size_t i = 0;
            while (i < s.size() && s[++i] == '/') 
              ;
            if (i > 1) 
              s = s.substr(i-1);

#if defined(BOOST_WINDOWS)
            // remove leading '/' if path has a drive letter
            if (s.size() > 2 && std::isalpha(s[1]) && s[2] == ':')
            {
              s = s.substr(1);
              return s;
            }
#endif
        }

//         // make path relative if it doesn't start with a '/' or '.'
//         if (s.size() > 1  && s[0] != '/' && s[0] != '.')
//         {
//           s = "./" + s;
//         }
        return s;
      }

#if defined(BOOST_WINDOWS)
      std::string fix_windows_backslashs(std::string urlstr)
      {
          boost::replace_all(urlstr, "\\", "/");
          return urlstr;
      }
#else
      std::string const& fix_windows_backslashs(std::string const& urlstr)
      {
          return urlstr;
      }
#endif

    }   // namespace

    /////////////////////////////////////////////////////////////////////////////
    //
    // URL method implementations
    //
    url::url (std::string const & urlstr)
      : impl::object(saga::object::URL), port_(-1), was_checked_(false)
    {
        if (!urlstr.empty())
          check_constructing (urlstr);   // throw special exceptions from here
    }

    url::url (std::string const & urlstr, saga::adaptors::nocheck)
      : impl::object(saga::object::URL),
        scheme_(urlstr),
        port_(-1),
        was_checked_(false)
    {
    }

    url::url (url const & rhs)
        : impl::object(saga::object::URL),
          scheme_              (rhs.scheme_),
          scheme_specific_part_(rhs.scheme_specific_part_),
          username_            (rhs.username_),
          password_            (rhs.password_),
          host_                (rhs.host_),
          port_                (rhs.port_),
          fragment_            (rhs.fragment_),
          path_                (rhs.path_),
          params_              (rhs.params_),
          query_               (rhs.query_),
          was_checked_(rhs.was_checked_)
    {
    }

    url::~url (void)
    {
    }

    bool url::verify (std::string const & urlstr)
    {
        // verify == true means that the current components are to be
        // retained after re-parsing. This verification makes sure every
        // components is recognized correctly and does not introduce
        // ambiguities.
        std::string scheme               = scheme_;
        std::string scheme_specific_part = scheme_specific_part_;
        std::string username             = username_;
        std::string password             = password_;
        std::string host                 = host_;
        int         port                 = port_;
        std::string fragment             = fragment_;
        std::string path                 = path_;
        std::string params               = params_;
        std::string query                = query_;

        check(urlstr);

        if (scheme   != scheme_ ||
            username != username_ ||
            password != password_ ||
            host     != host_ ||
            port     != port_ ||
            fragment != fragment_ ||
            params   != params_ ||
            query    != query_)
        {
            // restore previous state
            mutex_type::scoped_lock lock (mtx_);
            scheme_               = scheme;
            scheme_specific_part_ = scheme_specific_part;
            username_             = username;
            password_             = password;
            host_                 = host;
            port_                 = port;
            fragment_             = fragment;
            path_                 = path;
            params_               = params;
            query_                = query;

            return false;
        }

        if (path != path_)
        {
            // these still might point to the same item
            std::string p (normalize(path));
            if ((p != path_) && ("./" + p != path_) && ("/" + p != path_))
            {
                // restore previous state
                mutex_type::scoped_lock lock (mtx_);
                scheme_               = scheme;
                scheme_specific_part_ = scheme_specific_part;
                username_             = username;
                password_             = password;
                host_                 = host;
                port_                 = port;
                fragment_             = fragment;
                path_                 = path;
                params_               = params;
                query_                = query;

                return false;
            }
        }
        return true;
    }

    void url::check (std::string const & urlstr_)
    {
      initialize_mutex();
      boost::mutex::scoped_lock lock (mutex_instance());
      std::string urlstr(fix_windows_backslashs(urlstr_));

      reset();

      url_grammar <url> g (*this);
      boost::spirit::parse_info <> pi = boost::spirit::parse (urlstr.c_str (), g);

      if ( ! pi.full )
      {
        SAGA_THROW("invalid url (could not parse): " + urlstr,
          saga::IncorrectURL);
      }

      // invalid url, if username or port are given but no host
      if ( host_.empty() && (-1 != port_ || ! username_.empty () || !password_.empty()) )
      {
        SAGA_THROW("invalid url (no host): " + urlstr, saga::IncorrectURL);
      }

      // invalid url: invalid character in host
      if ( ! host_.empty () && std::string::npos != host_.find_first_of ("/") )
      {
        SAGA_THROW("invalid url (invalid host character): " + urlstr,
          saga::IncorrectURL);
      }

      // normalize path
      path_ = normalize (path_);

      was_checked_ = true;
    }

    void url::check_constructing (std::string const & urlstr_)
    {
      initialize_mutex();
      boost::mutex::scoped_lock lock (mutex_instance());
      std::string urlstr(fix_windows_backslashs(urlstr_));

      reset ();

      url_grammar <url> g (*this);
      boost::spirit::parse_info <> pi = boost::spirit::parse (urlstr.c_str (), g);

      if ( ! pi.full )
      {
        SAGA_THROW_NO_OBJECT("invalid url (could not parse): " + urlstr,
          saga::IncorrectURL);
      }

      // invalid url, if username or port are given but no host
      if ( host_.empty() && (-1 != port_ || ! username_.empty () || !password_.empty()) )
      {
        SAGA_THROW_NO_OBJECT("invalid url (no host): " + urlstr,
          saga::IncorrectURL);
      }

      // invalid url: invalid character in host
      if ( ! host_.empty () && std::string::npos != host_.find_first_of ("/") )
      {
        SAGA_THROW_NO_OBJECT("invalid url (invalid host character): " + urlstr,
          saga::IncorrectURL);
      }

      // normalize path
      path_ = normalize (path_);

      was_checked_ = true;
    }

    void url::reset (void)
    {
      port_ = -1;
      scheme_.clear   ();
      scheme_specific_part_.clear();
      username_.clear ();
      password_.clear ();
      host_.clear     ();
      fragment_.clear ();
      path_.clear     ();
      params_.clear   ();
      query_.clear    ();
      was_checked_ = false;
    }

    void url::set_checked(bool checked)
    {
        was_checked_ = checked;
    }

    std::string url::get_url (bool hide_password)
    {
      if (!was_checked_ && !scheme_.empty())
        check (scheme_);

      mutex_type::scoped_lock lock (mtx_);

      std::string result (get_scheme());
      if ( ! result.empty () )
      {
        result += ':';
      }

      std::string authority (get_authority(true));
      std::string path(get_path());
      if (!authority.empty())
      {
        result += "//";
        result += authority;
#if defined(BOOST_WINDOWS)
        // if path starts with a drive letter we must insert a '/'
        if (path.empty() || 
            (path.size() >= 2 && std::isalpha(path[0]) && path[1] == ':'))
        {
          result += "/";
        }
        else if (path[0] != '/')
        { 
            if (path.size() > 1 && path[0] != '.' && path[1] != '/')
            {
              result += "/./";
            }
            else
            {
              result += "/";
            }
        }
#else
        if (path.empty())
        {
          result += "/";
        }
        else if (path[0] != '/')
        { 
            if (path.size() > 1 && path[0] != '.' && path[1] != '/')
            {
              result += "/./";
            }
            else
            {
              result += "/";
            }
        }
#endif
      }
      else if (!result.empty())
      {
        result += "//";
        if (path.empty())
        {
          result += "/";
        }
#if defined(BOOST_WINDOWS)
        else if (path.size() >= 2 && std::isalpha(path[0]) && path[1] == ':')
        {
          result += "/";
        }
#endif
        else if (path[0] != '/')
        { 
            if (path.size() > 1 && path[0] != '.' && path[1] != '/')
            {
              result += "/./";
            }
            else
            {
              result += "/";
            }
        }
      }

      if (!path.empty())
      {
        result += path;
      }

      std::string query(get_query());
      if (!query.empty())
      {
        result += '?';
        result += query;
      }

      std::string fragment(get_fragment());
      if (!fragment.empty())
      {
        result += '#';
        result += fragment;
      }

      return result;
    }

    std::string url::get_url_escaped(bool hide_password)
    {
      if (!was_checked_ && !scheme_.empty())
        check (scheme_);

      mutex_type::scoped_lock lock (mtx_);

      std::string result (scheme_);
      if (!result.empty())
      {
        result = scheme_ + ':';
      }

      std::string authority (get_authority(hide_password));
      std::string path(path_);
      if (!authority.empty())
      {
        result += "//";
        result += get_authority_escaped(hide_password);
#if defined(BOOST_WINDOWS)
        // if path starts with a drive letter we must insert a '/'
        if (path.empty() || 
            (path.size() >= 2 && std::isalpha(path[0]) && path[1] == ':'))
        {
          result += "/";
        }
        else if (path[0] != '/')
        { 
            if (path.size() > 1 && path[0] != '.' && path[1] != '/')
            {
              result += "/./";
            }
            else
            {
              result += "/";
            }
        }
#else
        if (path.empty())
        {
          result += "/";
        }
        else if (path[0] != '/')
        { 
            if (path.size() > 1 && path[0] != '.' && path[1] != '/')
            {
              result += "/./";
            }
            else
            {
              result += "/";
            }
        }
#endif
      }
      else if (!result.empty())
      {
        result += "//";
        if (path.empty())
        {
          result += "/";
        }
#if defined(BOOST_WINDOWS)
        else if (path.size() >= 2 && std::isalpha(path[0]) && path[1] == ':')
        {
          result += "/";
        }
#endif
        else if (path[0] != '/')
        { 
            if (path.size() > 1 && path[0] != '.' && path[1] != '/')
            {
              result += "/./";
            }
            else
            {
              result += "/";
            }
        }
      }

      if (!path.empty())
      {
        result += path_;
      }

      std::string query(get_query());
      if (!query.empty())
      {
        result += '?';
        result += query_;
      }

      std::string fragment(get_fragment());
      if (!fragment.empty())
      {
        result += '#';
        result += fragment_;
      }

      return result;
    }

    void url::set_url (std::string const& newurl)
    {
       if (!newurl.empty()) {
         check (newurl);
       }
       else {
         mutex_type::scoped_lock lock (mtx_);
         reset();
       }
    }

    std::string url::get_scheme (void)
    {
      if (!was_checked_ && !scheme_.empty())
        check (scheme_);

      mutex_type::scoped_lock lock (mtx_);
      return unescape_lit(scheme_);
    }

    std::string url::get_host (void)
    {
      if (!was_checked_ && !scheme_.empty())
        check (scheme_);

      mutex_type::scoped_lock lock (mtx_);
      return unescape_lit(host_);
    }

    int url::get_port (void)
    {
      if (!was_checked_ && !scheme_.empty())
        check (scheme_);

      mutex_type::scoped_lock lock (mtx_);
      return port_;
    }

    void url::set_authority (std::string const & auth)
    {
      mutex_type::scoped_lock lock (mtx_);

      if ( auth.empty () )
      {
        // nothing to do
        set_userinfo ("");
        set_host     ("");
        set_port     (-1);

        return;
      }

      // operate on a copy
      std::string auth_ (auth);

      // do we have userinfo?
      size_t pos = auth_.find_first_of ("@");

      if ( pos != std::string::npos )
      {
        if ( pos > 1 )
        {
          set_userinfo (auth_.substr (0, pos));
        }
        else
        {
          set_userinfo ("");
        }

        auth_.erase (0, pos+1);
      }
      else
      {
        set_userinfo ("");
      }

      // do we have a port?
      pos = auth_.find_last_of (":");
      if ( pos != std::string::npos )
      {
        int i;
        std::istringstream iss (auth_.substr (pos+1));

        if ( (iss >> i).fail () )
        {
          SAGA_THROW ("Could not parse port from authority string: " + auth, saga::BadParameter);
        }

        set_port (i);
        auth_.erase (pos);
      }
      else
      {
        set_port (-1);
      }

      // everything else is host - this will reset host if empty
      set_host (auth_);
    }


    std::string url::get_authority (bool hide_password)
    {
      if (!was_checked_ && !scheme_.empty())
        check (scheme_);

      mutex_type::scoped_lock lock (mtx_);

      std::string result(get_userinfo(hide_password));
      if (!result.empty())
        result += '@';

      if (!get_host().empty())
        result += get_host();

      if (-1 != get_port())
        result += ':' + boost::lexical_cast<std::string>(get_port());

      return result;
    }

    std::string url::get_authority_escaped(bool hide_password)
    {
      if (!was_checked_ && !scheme_.empty())
        check (scheme_);

      mutex_type::scoped_lock lock (mtx_);

      std::string result(get_userinfo_escaped(hide_password));
      if (!result.empty())
        result += '@';

      if (!get_host().empty())
        result += host_;

      if (-1 != get_port())
        result += ':' + boost::lexical_cast<std::string>(get_port());

      return result;
    }

    std::string url::get_fragment (void)
    {
      if (!was_checked_ && !scheme_.empty())
        check (scheme_);

      mutex_type::scoped_lock lock (mtx_);
      return unescape_lit(fragment_);
    }

    std::string url::get_fragment_escaped(void)
    {
      if (!was_checked_ && !scheme_.empty())
        check (scheme_);

      mutex_type::scoped_lock lock (mtx_);
      return fragment_;
    }

    std::string url::get_path (void)
    {
      if (!was_checked_ && !scheme_.empty())
        check (scheme_);

      mutex_type::scoped_lock lock (mtx_);
      return unescape_lit(path_);
    }

    std::string url::get_path_escaped(void)
    {
      if (!was_checked_ && !scheme_.empty())
        check (scheme_);

      mutex_type::scoped_lock lock (mtx_);
      return path_;
    }

    std::string url::get_query (void)
    {
      if (!was_checked_ && !scheme_.empty())
        check (scheme_);

      mutex_type::scoped_lock lock (mtx_);
      return unescape_lit(query_);
    }

    std::string url::get_query_escaped(void)
    {
      if (!was_checked_ && !scheme_.empty())
        check (scheme_);

      mutex_type::scoped_lock lock (mtx_);
      return query_;
    }

    std::string url::get_userinfo (bool hide_password)
    {
      if (!was_checked_ && !scheme_.empty())
        check (scheme_);

      mutex_type::scoped_lock lock (mtx_);
      std::string result (get_username());
      std::string password (get_password());
      if (!password.empty())
          result += hide_password ? ":********" : (":" + password);
      return result;
    }

    std::string url::get_userinfo_escaped(bool hide_password)
    {
      if (!was_checked_ && !scheme_.empty())
          check (scheme_);

      mutex_type::scoped_lock lock (mtx_);
      std::string result (username_);
      std::string password (get_password());
      if (!password.empty()) 
          result += hide_password ? ":********" : (":" + password_);

      return result;
    }

    std::string url::get_username (void)
    {
      if (!was_checked_ && !scheme_.empty())
          check (scheme_);

      mutex_type::scoped_lock lock (mtx_);
      return unescape_lit(username_);
    }

    std::string url::get_password (void)
    {
      if (!was_checked_ && !scheme_.empty())
        check (scheme_);

      mutex_type::scoped_lock lock (mtx_);
      return unescape_lit(password_);
    }

    ///////////////////////////////////////////////////////////////////////////////
    void url::set_scheme (std::string const & scheme)
    {
      scheme_ = scheme.substr (0, scheme.find_first_of (':'));
      boost::to_lower (scheme_);
    }

    void url::set_scheme_specific_part (std::string const & scheme_specific_part)
    {
      scheme_specific_part_ = scheme_specific_part;
    }

    void url::set_host_userinfo (std::string const & host,
                                 std::string const & userinfo)
    {
      set_host     (host);
      set_userinfo (userinfo);
    }

    void url::set_host(std::string const & host)
    {
      host_ = host;
    }

    void url::set_userinfo (std::string const & userinfo)
    {
      std::string ui (userinfo);

      std::string::size_type p1 = ui.find_first_of (':');
      std::string::size_type p2 = ui.find_first_of ('@');

      if (p1 != std::string::npos) 
      {
        username_ = ui.substr (0, p1);
        password_ = ui.substr(p1+1, p2-p1-1);
      }
      else 
      {
        username_ = ui.substr (0, p2);
      }
    }

    void url::set_port (int port)
    {
      port_ = port;
    }

    void url::set_fragment (std::string const & fragment)
    {
      fragment_ = fragment;
    }

    void url::set_path (std::string const & path)
    {
      path_ = clean_leading_slashs(path);
    }

    void url::set_query (std::string const & query)
    {
      query_ = query;
    }

    void url::set_params (std::string const & params)
    {
      params_ = params;
    }

    ///////////////////////////////////////////////////////////////////////////////
    void url::change_scheme (std::string const & scheme)
    {
      if (!was_checked_ && !scheme_.empty())
          check (scheme_);

      std::string old_scheme;

      {
          mutex_type::scoped_lock lock (mtx_);
          old_scheme = scheme;
          scheme_ = scheme;
          boost::to_lower(scheme_);
          was_checked_ = true;
      }

      std::string urlstr(get_url_escaped());
      if (!verify(urlstr)) {
          mutex_type::scoped_lock lock (mtx_);
          scheme_ = old_scheme;
          SAGA_THROW("invalid scheme (url re-parse gives inconsistent result): "
              + urlstr, saga::BadParameter);
      }
    }

    void url::change_scheme_specific_part (std::string const & scheme_specific_part)
    {
      if (!was_checked_ && !scheme_.empty())
          check (scheme_);

      std::string old_scheme_specific_part;

      {
          mutex_type::scoped_lock lock (mtx_);
          old_scheme_specific_part = scheme_specific_part_;
          scheme_specific_part_ = scheme_specific_part;
      }

      std::string urlstr(get_url_escaped());
      if (!verify(urlstr)) {
          mutex_type::scoped_lock lock (mtx_);
          scheme_specific_part_ = old_scheme_specific_part;
          SAGA_THROW("invalid scheme specific part (url re-parse gives inconsistent result): "
              + urlstr, saga::BadParameter);
      }
    }

    void url::change_host (std::string const & host)
    {
      if (!was_checked_ && !scheme_.empty())
          check (scheme_);

      std::string old_host;

      {
          mutex_type::scoped_lock lock (mtx_);
          old_host = host_;
          host_ = host;
      }

      std::string urlstr(get_url_escaped());
      if (!verify(urlstr)) {
          mutex_type::scoped_lock lock (mtx_);
          host_ = old_host;
          SAGA_THROW("invalid host (url re-parse gives inconsistent result): "
              + urlstr, saga::BadParameter);
      }
    }

    void url::change_port (int port)
    {
      if (!was_checked_ && !scheme_.empty())
          check (scheme_);

      int old_port;

      {
          mutex_type::scoped_lock lock (mtx_);
          old_port = port_;
          port_ = port;
      }

      std::string urlstr(get_url_escaped());
      if (!verify(urlstr)) {
          mutex_type::scoped_lock lock (mtx_);
          port_ = old_port;
          SAGA_THROW("invalid port (url re-parse gives inconsistent result): "
              + urlstr, saga::BadParameter);
      }
    }

    void url::change_fragment (std::string const & fragment)
    {
      if (!was_checked_ && !scheme_.empty())
          check (scheme_);

      std::string old_fragment;

      {
          mutex_type::scoped_lock lock (mtx_);
          old_fragment = fragment_;
          fragment_ = fragment;
      }

      std::string urlstr(get_url_escaped());
      if (!verify(urlstr)) {
          mutex_type::scoped_lock lock (mtx_);
          fragment_ = old_fragment;
          SAGA_THROW("invalid fragment (url re-parse gives inconsistent result): "
              + urlstr, saga::BadParameter);
      }
    }

    void url::change_path (std::string const & newpath)
    {
      if (!was_checked_ && !scheme_.empty())
          check (scheme_);

      std::string old_path;

      {
          mutex_type::scoped_lock lock (mtx_);
          old_path = path_;
          path_ = escape_lit(clean_leading_slashs(newpath));
      }

      std::string urlstr(get_url_escaped());
      if (!verify(urlstr)) {
          mutex_type::scoped_lock lock (mtx_);
          path_ = old_path;
          SAGA_THROW("invalid path (url re-parse gives inconsistent result): "
              + urlstr, saga::BadParameter);
      }
    }

    void url::change_query (std::string const & query)
    {
      if (!was_checked_ && !scheme_.empty())
          check (scheme_);

      std::string old_query;

      {
          mutex_type::scoped_lock lock (mtx_);
          old_query = query_;
          query_ = query;
      }

      std::string urlstr(get_url_escaped());
      if (!verify(urlstr)) {
          mutex_type::scoped_lock lock (mtx_);
          query_ = old_query;
          SAGA_THROW("invalid query (url re-parse gives inconsistent result): "
              + urlstr, saga::BadParameter);
      }
    }

    void url::change_userinfo (std::string const & userinfo)
    {
      if (!was_checked_ && !scheme_.empty())
          check (scheme_);

      std::string old_username, old_password;

      {
          mutex_type::scoped_lock lock (mtx_);

          old_username = username_;
          old_password = password_;

          std::string::size_type p1 = userinfo.find_first_of (':');
          std::string::size_type p2 = userinfo.find_first_of ('@');
          if (p1 != std::string::npos) {
              username_ = userinfo.substr (0, p1);
              password_ = userinfo.substr(p1+1, p2-p1-1);
          }
          else {
              username_ = userinfo.substr (0, p2);
              password_.clear();
          }
      }

      std::string urlstr(get_url_escaped());
      if (!verify(urlstr)) {
          mutex_type::scoped_lock lock (mtx_);
          username_ = old_username;
          password_ = old_password;
          SAGA_THROW("invalid userinfo (url re-parse gives inconsistent result): "
              + urlstr, saga::BadParameter);
      }
    }

    void url::change_username (std::string const & username)
    {
      if (!was_checked_ && !scheme_.empty())
          check (scheme_);

      std::string old_username;

      {
          mutex_type::scoped_lock lock (mtx_);
          old_username = username_;
          username_ = username;
      }

      std::string urlstr(get_url_escaped());
      if (!verify(urlstr)) {
          mutex_type::scoped_lock lock (mtx_);
          username_ = old_username;
          SAGA_THROW("invalid username (url re-parse gives inconsistent result): "
              + urlstr, saga::BadParameter);
      }
    }

    void url::change_password (std::string const & password)
    {
      if (!was_checked_ && !scheme_.empty())
          check (scheme_);

      std::string old_password;

      {
          mutex_type::scoped_lock lock (mtx_);
          old_password = password_;
          password_ = password;
      }

      std::string urlstr(get_url_escaped());
      if (!verify(urlstr)) {
          mutex_type::scoped_lock lock (mtx_);
          password_ = old_password;
          SAGA_THROW("invalid passwd (url re-parse gives inconsistent result): "
              + urlstr, saga::BadParameter);
      }
    }

// #if BOOST_VERSION < 103400
//     // workaround for ancient Boost versions
//     boost::filesystem::path& canonize(boost::filesystem::path& p)
//     {
//         static const char dot_str[] = { '.', 0 };
// 
//         if (p.empty()) 
//             return p;
// 
//         boost::filesystem::path temp;
//         for (boost::filesystem::path::iterator itr(p.begin()); itr != p.end(); ++itr)
//         {
//             temp /= *itr;
//         };
// 
//         if (temp.empty()) 
//             temp /= dot_str;
// 
//         p = temp;
//         return p;
//     }
//     std::string remove_double_slash(std::string s)
//     {
//         std::string::size_type p = s.find_first_of('/');
//         while (p != std::string::npos) {
//             if (s.size() > p+1 && s[p+1] == '/')
//                 s.erase(p, 1);
//             p = s.find_first_of('/', p+1);
//         }
//         return s;
//     }
// #endif

    // normalize the path element of the url:
    //  - a//b    -> a/b
    //  - a/./b   -> a/b
    //  - a/../b  -> b
    //  - /../a   -> a
    //  - ../a/.. -> ../
    //  - a/b/..  -> a/
    //  - a/b/.   -> a/b/
    std::string url::normalize (std::string const& inpath)
    {
// #if BOOST_VERSION >= 103400
      boost::filesystem::path p (inpath);
// #else
//       boost::filesystem::path p (remove_double_slash(inpath), boost::filesystem::no_check);
// #endif

      // if path ends in ".." we must append a '/'
      if (inpath.size() > 1 && 
          inpath[inpath.size()-1] == '.' && 
          inpath[inpath.size()-2] == '.')
      {
        p /= ".";
      }

      // canonize and normalize to get rid of duplicate '/'
// #if BOOST_VERSION >= 103400
#if BOOST_FILESYSTEM_VERSION == 2
      p.canonize();
#endif
// #else
//       canonize(p);
// #endif
      p.normalize();

      boost::filesystem::path pp = p.root_path();   // includes drive on windows

      boost::filesystem::path::iterator end   = p.end   ();
      boost::filesystem::path::iterator iter  = p.begin ();

      bool absolute = !pp.empty();
      bool first    = true;

      if ( absolute )
      {
        ++iter; // skip root entry, we got that above...
      }

      boost::filesystem::path remainder;
      int append_slash = (inpath.size() > 1 && inpath[inpath.size()-1] == '/') ? 1 : 0;

      // this loop removes leading '/..' and trailing '/.' elements.
      for ( ; iter != end; ++iter )
      {
        if ( first    &&
             absolute &&
             ".."     == (*iter) )
        {
          // '/..' - we are skipping this one
        }
        else if ( "." == (*iter) )
        {
          remainder /= "."; // keep '/.' in case they are not trailing
          ++append_slash;
        }
        else
        {
          // found something useful...
          if (!remainder.empty())
              pp /= remainder;
          pp /= *iter;

          first    = false;
          if (--append_slash < 0)
            append_slash = 0;
          remainder = "";
        }
      }

      // handle single "."
      if (pp.string().empty())
          return remainder.string();

// #if BOOST_VERSION >= 103400
#if BOOST_FILESYSTEM_VERSION == 2
      pp.canonize();
#endif
// #else
//       canonize(pp);
// #endif
      pp.normalize ();

      return (append_slash > 0) ? pp.string() + "/" : pp.string();
    }

    // saga::object related functionality
    saga::object url::clone() const
    {
       return saga::url(new saga::impl::url(*this));  // implies nocheck()
    }

    ///////////////////////////////////////////////////////////////////////////
    saga::task url::translate_impl(saga::session s, std::string scheme, bool is_sync)
    {
        TR1::shared_ptr<saga::impl::url_proxy> prxy(
            new saga::impl::url_proxy(s, saga::url(this)));
        return prxy->translate(scheme, is_sync);
    }

    ///////////////////////////////////////////////////////////////////////////
    std::string unescape_url(std::string const&in)
    {
        return unescape_lit(in);
    }
    std::string escape_url(std::string const&in)
    {
        return escape_lit(in);
    }

///////////////////////////////////////////////////////////////////////////////
}} // namespace saga::impl

