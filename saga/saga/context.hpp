//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2007 Ole Weidner (oweidner@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_CONTEXT_HPP
#define SAGA_CONTEXT_HPP

// include dependent spec sections
#include <saga/saga/util.hpp>
#include <saga/saga/base.hpp>
#include <saga/saga/task.hpp>

#include <saga/saga/object.hpp>
#include <saga/saga/detail/attribute.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable: 4251 4231 4660)
#endif

///////////////////////////////////////////////////////////////////////////////
namespace saga
{
  namespace attributes 
  {
    ///////////////////////////////////////////////////////////////////////////
    //  attribute names for context

    //  read write attributes
    char const* const context_type            = "Type";
    char const* const context_server          = "Server";
    char const* const context_certrepository  = "CertRepository";
    char const* const context_userproxy       = "UserProxy";
    char const* const context_usercert        = "UserCert";
    char const* const context_userkey         = "UserKey";
    char const* const context_userid          = "UserID";
    char const* const context_userpass        = "UserPass";
    char const* const context_uservo          = "UserVO";
    char const* const context_lifetime        = "LifeTime";
    char const* const context_remoteid        = "RemoteID";
    char const* const context_remotehost      = "RemoteHost";
    char const* const context_remoteport      = "RemotePort";
  }

  /*! \brief The saga::context class provides the functionality of
   * a security information container.     
   *
   *   A context gets created, and attached to a session handle. As such
   * it is available to all objects instantiated in that session. Multiple 
   * contexts can co-exist in one session - it is up to the implementation to
   * choose the correct context for a specific method call. Also, a single
   * saga::context instance can be shared between multiple sessions. SAGA
   * objects created from other SAGA objects inherit its session and thus also
   * its context(s).
   */
  class SAGA_EXPORT context
    : public saga::object
    , public saga::detail::attribute<context>
  {
    protected:
      /// @cond
      /** These methods are not within API scope */
      TR1::shared_ptr <saga::impl::context> get_impl_sp (void) const;
      saga::impl::context* get_impl (void) const;

      friend struct saga::detail::attribute<context>;  // needs to access get_impl()
      friend class saga::impl::context;
      friend struct saga::impl::runtime;

      explicit context (saga::impl::context *impl);
      explicit context (TR1::shared_ptr<saga::impl::context> init);
      /// @endcond

    public:

      /*! \brief Brief %description starts here
       *
       *
       */
      explicit context(std::string const& ctxtype = "");

      /*! \brief Creates a security context 
       * \return The newly created object.
       *
       *
       */
      explicit context(saga::object const& o);

      /*! \brief Destroys a security context
       *
       *
       */
     ~context();

      /*! \brief Brief %description starts here
       *
       *
       */
      context &operator= (saga::object const& o);

      /*! \brief Brief %description starts here
       *
       *
       */
      friend SAGA_EXPORT 
      bool operator== (context const& lhs, context const& rhs);

      // has been removed as of newest SAGA spec (is implicitely called in
      // saga::session::add_context now.
      // void set_defaults();
  }; 

} // namespace saga
///////////////////////////////////////////////////////////////////////////////

// re-enable warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

#endif // SAGA_CONTEXT_HPP

