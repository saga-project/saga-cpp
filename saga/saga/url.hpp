//  Copyright (c) 2005-2011 Hartmut Kaiser
//  Copyright (c) 2007 Ole Weidner (oweidner@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <string>
#include <iosfwd>

#include <saga/saga/util.hpp>
#include <saga/saga/base.hpp>
#include <saga/saga/call.hpp>

#include <saga/saga/object.hpp>
#include <saga/saga/session.hpp>

#if !defined(SAGA_SAGA_URL_HPP)
#define SAGA_SAGA_URL_HPP

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable: 4251 4231 4275 4660)
#endif

///////////////////////////////////////////////////////////////////////////////
namespace saga 
{
  /////////////////////////////////////////////////////////////////////////////
  /*! \brief Brief %description starts here     
   *
   *
   */
  SAGA_EXPORT std::ostream& operator<< (std::ostream& os, url const& u);

  /*! \brief Brief %description starts here     
   *
   *
   */
  SAGA_EXPORT std::istream& operator>> (std::istream& is, url& u);

  /*! \brief Brief %description starts here     
   *
   *
   */
  SAGA_EXPORT bool operator== (saga::url const& lhs, saga::url const& rhs);

  /*! \brief Brief %description starts here     
   *
   *
   */
  SAGA_EXPORT bool operator!= (saga::url const& lhs, saga::url const& rhs);

  /*! \brief Brief %description starts here     
   *
   *
   */
  SAGA_EXPORT bool operator< (saga::url const& lhs, saga::url const& rhs);

  /*! \brief SAGA's implementation of an RFC 1630 compliant URL Class
   *  \details URLs (and URIs, see below) are a dominant concept for referencing 
   *  external resources. As such, they are also widely in the Grid world, and
   *  in SAGA. This class helps to manage such URLs. 
   *
   *  <B>Example:</B>  \include misc/url_translate.cpp
   */
  class SAGA_EXPORT url 
    : public saga::object
  {
    private:
      friend class saga::impl::url;
      friend struct saga::impl::runtime;

      SAGA_CALL_CONST_PRIV_1(translate, std::string)
      SAGA_CALL_CONST_PRIV_2(translate, saga::session, std::string)

      std::string get_url_escaped(bool hide_password = false) const;

      friend SAGA_EXPORT std::ostream& operator<< (std::ostream& os, url const& u);
      friend SAGA_EXPORT std::istream& operator>> (std::istream& is, url& u);
      friend SAGA_EXPORT bool operator== (saga::url const& lhs, saga::url const& rhs);
      friend SAGA_EXPORT bool operator!= (saga::url const& lhs, saga::url const& rhs);
      friend SAGA_EXPORT bool operator< (saga::url const& lhs, saga::url const& rhs);

   protected:
     /// @cond
     /** These methods are not within API scope */
     TR1::shared_ptr <impl::url> get_impl_sp() const;
     impl::url* get_impl() const;

     url (impl::url *impl);
//      url (adaptors::nocheck);
//      url (saga::object rhs, adaptors::nocheck);
     url (std::string const & urlstr, adaptors::nocheck);
     /// @endcond

   public:
     url();
     explicit url (saga::object rhs);
     url(saga::url const& rhs);
     url(std::string const& urlstr);
     url(char const * urlstr);
    ~url(void);

    url& operator=(char const * urlstr);
    url& operator=(std::string const & urlstr);
    url& operator=(saga::url const& rhs);
    url& operator=(saga::object const& rhs);
    
    /*! \brief Brief %description starts here     
     *
     *
     */
    saga::object clone() const;
    
    /*! \brief Retrieves the url as string 
     * \return String representing the url.
     *
     *
     */
     std::string get_string(void) const;
     
    /*! \brief Set a new url  
     *
     *
     */
     void set_string(std::string const & url);
     
    /*! \brief Gets an url scheme.   
     * \return The url scheme.
     *
     *
     */
     std::string get_scheme (void) const;
     
    /*! \brief Sets an url scheme.     
     *
     *
     */
     void set_scheme (std::string const & scheme);
     
    /*! \brief  Gets an url host.     
     * \return The host.
     *
     *
     */
     std::string get_host (void) const;
     
    /*! \brief Sets an url hosts.     
     *
     *
     */
     void set_host (std::string const & host);
     
    /*! \brief  Gets an url port.
     * \return the port.     
     *
     *
     */
     int get_port (void) const;

    /*! \brief Sets an url port.    
     *
     *
     */
     void set_port (int port);

    /*! \brief  Gets an url fragment.
     * \return The url fragment.   
     *
     *
     */
     std::string get_fragment  (void) const;
     
    /*! \brief  Sets an url fragment.  
     *
     *
     */
     void set_fragment (std::string const & fragment);
     
    /*! \brief  Gets an url path.
     * \return The path.
     *
     *
     */
     std::string get_path (void) const;
     
    /*! \brief  Sets an url path.    
     *
     *
     */
     void set_path (std::string const & path);
     
    /*! \brief  Gets an url user information
     * \return The user information.    
     *
     *
     */
     std::string get_userinfo (void) const;
     
    /*! \brief  Sets an url user information.    
     *
     *
     */
     void set_userinfo (std::string const & userinfo);

    /*! \brief Translates and URL to a new scheme.
     * \param scheme The new scheme to translate into
     * \return String representation of the translated url.    
     */
    saga::url translate(std::string scheme) const
    {
        saga::task t = translatepriv(scheme, saga::task_base::Sync());
        return t.get_result<saga::url>();
    }
    SAGA_CALL_CONST_PUB_1_DEF_0(translate, std::string)

    /*! \brief Translates and URL to a new scheme.
     * \param scheme The new scheme to translate into
     * \return String representation of the translated url.    
     */
    saga::url translate(saga::session s, std::string scheme) const
    {
        saga::task t = translatepriv(s, scheme, saga::task_base::Sync());
        return t.get_result<saga::url>();
    }
    SAGA_CALL_CONST_PUB_2_DEF_0(translate, saga::session, std::string)

    ///////////////////////////////////////////////////////////////////////////
    // The API functions below are not part of the SAGA specification
    // we provide these as convenience helpers
    ///////////////////////////////////////////////////////////////////////////
    
    /*! \brief Gets an url.    
     *  \return The url.
     *
     *
     */
     std::string get_url       (void) const;
     
    /*! \brief Sets an url.  
     *
     *
     */
     void set_url(std::string const & url);
     
    /*! \brief  Gets an url authority.   
     *
     *
     */
     std::string get_authority (void) const;
     
    /*! \brief  Sets an url authority.   
     *
     *
     */
     void set_authority (std::string const & auth);
     
    /*! \brief Gets an url query.   
     *
     *
     */
     std::string get_query (void) const;
     
    /*! \brief  Sets an url query.     
     *
     *
     */
     void set_query (std::string const & query);

    /*! \brief Brief %description starts here     
     *
     *
     */
     void set_scheme_specific_part (std::string const & scheme_specific_part);
     
    /*! \brief  Gets an username. 
     *
     *
     */
     std::string get_username  (void) const;
     
    /*! \brief Sets the username.     
     *
     *
     */
     void set_username (std::string const & username);
     
    /*! \brief Gets the password.     
     *
     *
     */
     std::string get_password  (void) const;
     
    /*! \brief  Sets the password.     
     *
     *
     */
     void set_password (std::string const & passwd);

    /*! \brief Remove all escape sequence from a string resembling (part of) s
     *         an url
     */
    static std::string unescape(std::string const&in);
  
    /*! \brief Add escape sequences to a string resembling (part of) an url
     */
    static std::string escape(std::string const&in);
  
  };
} // namespace saga

// re-enable warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

#endif // SAGA_SAGA_URL_HPP

