//  Copyright (c) 2005-2011 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <string>

#include <boost/thread.hpp>

#include <saga/impl/config.hpp>
#include <saga/impl/object.hpp>

#ifndef SAGA_IMPL_ENGINE_URL_HPP
#define SAGA_IMPL_ENGINE_URL_HPP

///////////////////////////////////////////////////////////////////////////////
namespace saga 
{ 
  namespace impl 
  {
    ///////////////////////////////////////////////////////////////////////////////
    class url : public impl::object
    {
     private:
       typedef boost::recursive_mutex mutex_type;
       
       // this global mutex is used to protect the Spirit grammar
       static boost::mutex &mutex_instance()
       {
           static boost::mutex mutex;
           return mutex;
       }
       static void mutex_init()
       {
           mutex_instance();
       }
       static void initialize_mutex()
       {
           static boost::once_flag been_here = BOOST_ONCE_INIT;
           boost::call_once(mutex_init, been_here);
       }

       // this mutex is used to protect this instance of url
       mutable mutex_type mtx_;
       
       std::string scheme_;
       std::string scheme_specific_part_;
       std::string username_;
       std::string password_;
       std::string host_;
       int         port_;
       std::string fragment_;
       std::string path_;
       std::string params_;
       std::string query_;
       std::string old_urlstr_;
       bool was_checked_;

       std::string normalize (std::string const& inpath);

     protected:
       void check (std::string const& urlstr);
       void check_constructing (std::string const& urlstr);
       bool verify (std::string const& urlstr);
       void reset (void);

     public:
       url (url         const & rhs);       // used for cloning
       url (std::string const & urlstr);
       url (std::string const & urlstr, adaptors::nocheck);
      ~url (void);

       std::string get_url (bool hide_password = false);
       void set_url (std::string const& newurl);

       std::string get_scheme               (void);
       std::string get_host                 (void);
       int         get_port                 (void);
       std::string get_authority (bool hide_password = false); 
       std::string get_fragment             (void);
       std::string get_path                 (void);
       std::string get_query                (void);
       std::string get_userinfo (bool hide_password = false);
       std::string get_username             (void);
       std::string get_password             (void);

       // interface to change the members (and re-check the url)
       void change_scheme               (std::string const & scheme);
       void change_scheme_specific_part (std::string const & scheme_specific_part);
       void change_host                 (std::string const & host);
       void change_port                 (int                 port);
       void change_fragment             (std::string const & fragment);
       void change_path                 (std::string const & path);
       void change_query                (std::string const & query);
       void change_userinfo             (std::string const & userinfo);
       void change_username             (std::string const & userinfo);
       void change_password             (std::string const & userinfo);

       // interface to directly write to the members
       void set_scheme                  (std::string const & scheme);
       void set_scheme_specific_part    (std::string const & scheme_specific_part);
       void set_host_userinfo           (std::string const & host, 
                                         std::string const & userinfo);
       void set_host                    (std::string const & host);
       void set_userinfo                (std::string const & userinfo);
       void set_port                    (int                 port);
       void set_authority               (std::string const & auth); 
       void set_fragment                (std::string const & fragment);
       void set_path                    (std::string const & path);
       void set_query                   (std::string const & query);
       void set_params                  (std::string const & params);

       std::string get_url_escaped(bool hide_password = false);        // used by streaming operator
       std::string get_authority_escaped(bool hide_password = false);
       std::string get_userinfo_escaped(bool hide_password = false);
       std::string get_path_escaped(void);
       std::string get_query_escaped(void);
       std::string get_fragment_escaped(void);

       // saga::object related functionality
       saga::object  clone() const;

       // tell the url it has been checked
       void set_checked(bool checked = true);

       // this function is special as it is implemented using adaptor support
       saga::task translate_impl(saga::session s, std::string url, bool sync = false);
    };

    std::string unescape_url(std::string const&in);
    std::string escape_url(std::string const&in);
  
  } // namespace impl

} // namespace saga
///////////////////////////////////////////////////////////////////////////////

#endif // SAGA_IMPL_ENGINE_URL_HPP

