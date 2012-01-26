//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2007 Ole Weidner (oweidner@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_SESSION_HPP
#define SAGA_SESSION_HPP

// include stl
#include <list>
#include <set>

// include dependent spec sections
#include <saga/saga/util.hpp>
#include <saga/saga/base.hpp>
#include <saga/saga/object.hpp>
#include <saga/saga/detail/preferences.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable: 4251 4231 4275 4660)
#endif

///////////////////////////////////////////////////////////////////////////////
namespace saga
{
    ///@cond
    namespace detail 
    {
        // the default session as singleton, and the no-session singleton
        SAGA_EXPORT session& get_the_session();
        #define SAGA_DEFAULT_SESSION detail::get_the_session()

        // helper function to create exactly one instance of a default session
        session& get_instance();
    }
    ///@endcond

  /*! \brief Almost all SAGA objects are created in a SAGA session, and are
   * associated with this (and only this) session for their whole life time.     
   *
   *   A session instance to be used on ob ect instantiation can explicitly be
   * given as first parameter to the SAGA object instantiation call (CONSTRUCTOR).
   * If the session is omitted as first parameter, a default session is used, with
   * default security context(s) attached. The default session can be obtained by
   * passing true to the session CONSTRUCTOR.
   */
  class SAGA_EXPORT session
    : public saga::object
  {
    protected: 
      /// @cond 
      /** These methods are not within API scope */
      TR1::shared_ptr<saga::impl::session> get_impl_sp (void) const;
      saga::impl::session* get_impl (void) const;

      friend class  saga::object; // needs to get impl
      friend struct impl::runtime;
      friend session& detail::get_instance();
      /// @endcond

      // this is protected now to avoid abuse
      explicit session(bool default_session);

    public:
      /*! \brief Creates the object.
       *
       *
       */
      explicit session();

      /*! \brief Creates the object.
       * \return The newly created object.
       *
       *
       */
      explicit session(saga::object const& obj);

      /*! \brief Destroys the object.    
       *
       *
       */
     ~session (void);

      /*! \brief Brief %description starts here     
       *
       *
       */
      session &operator= (saga::object const& o);

      /*! \brief Attaches a security context to a session  
       * \param c Security context to add.
       *
       *
       */
      void add_context(saga::context const& c);

      /*! \brief Detaches a security context from a session. 
       * \param c Security context to remove.
       *
       *
       */
      void remove_context(saga::context const& c);

      /*! \brief Retrieves all contexts attached to a session 
       * \return List of contexts of this session.
       *
       *
       */
      std::vector <saga::context> list_contexts() const;
      
      /*! \brief Brief %description starts here     
       *
       *
       */
      friend SAGA_EXPORT 
      bool operator== (session const& lhs, session const& rhs);

      /*! \brief Closes the object.     
       */
      void close (double timeout = 0.0);
  };

  saga::session SAGA_EXPORT get_default_session();

///////////////////////////////////////////////////////////////////////////////
} // namespace saga

// re-enable warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

#endif // SAGA_SESSION_HPP

