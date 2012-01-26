//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Use, modification and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/thread.hpp>

// include the session implementation we want to use
#include <saga/saga/session.hpp>
#include <saga/impl/session.hpp>
#include <saga/saga/adaptors/manage_threads.hpp>

#include <boost/plugin/dll.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga 
{
  // Instantiate the default session object
  //
  // Most saga objects have a session attached.  If no session is explicitly
  // specified on object creation, this default session is attached instead.
  
  ///@cond
  namespace detail 
  {
      /////////////////////////////////////////////////////////////////////////
      //  Helper object allowing to break the cyclic dependency between a 
      //  session and the contexts held inside this session
      class session_helper
      {
      public:
          session_helper(session& s)
            : session_(s)
          {}
          ~session_helper()
          {
              impl::runtime::get_impl(session_)->release_contexts();
          }

      private:
          session& session_;
      };

      /////////////////////////////////////////////////////////////////////////
      //  Helper functions to allow thread safe initialization of the static session 
      //  object
      session& get_instance()
      {
          static session s(true);
          static session_helper helper(s);
          return s;
      }

      static void call_once_session()
      {
          get_instance();
      }

      /////////////////////////////////////////////////////////////////////////////
      //  This is an API function
      session& get_the_session (void)
      {
          static boost::once_flag once_flag = BOOST_ONCE_INIT;
          boost::call_once(&call_once_session, once_flag);

          return get_instance();
      }
  }
  ///@endcond

  session get_default_session (void)
  {
      return detail::get_the_session();
  }

  namespace adaptors
  {
//       struct managed_threads : boost::thread_group
//       {
//           ~managed_threads() 
//           {
//               join_all();
//           }
//       };
//       
//       static managed_threads& get_thread_group_helper()
//       {
//           static managed_threads threads;
//           return threads;
//       }
// 
//       static void call_once_thread_group()
//       {
//           get_thread_group_helper();
//       }
// 
//       static managed_threads& get_thread_group()
//       {
//           static boost::once_flag once_flag = BOOST_ONCE_INIT;
//           boost::call_once(&call_once_thread_group, once_flag);
//           
//           return get_thread_group_helper();
//       }

      /////////////////////////////////////////////////////////////////////////
      void add_managed_thread (saga::session s, boost::thread* thrd)
      {
//           get_thread_group().add_thread(thrd);
      }
      
      void remove_managed_thread (saga::session s, boost::thread* thrd)
      {
//           get_thread_group().remove_thread(thrd);
      }
  }

  /////////////////////////////////////////////////////////////////////////////
  session::session () 
    : saga::object (new saga::impl::session(false))
  {
      BOOST_ASSERT(get_impl());
  }

  session::session (bool default_session) 
    : saga::object (new saga::impl::session(default_session))
  {
      BOOST_ASSERT(get_impl());
  }

  session::session (saga::object const& obj) 
    : saga::object(obj)
  {
      if (this->get_type() != saga::object::Session)
      {
          SAGA_THROW("Bad type conversion.", saga::BadParameter);
      }
  }

  session& session::operator=(saga::object const& obj)
  {
      saga::object::operator=(obj);
      if (this->get_type() != saga::object::Session)
      {
          SAGA_THROW("Bad type conversion.", saga::BadParameter);
      }
      return *this;
  }

  session::~session (void)
  {
  }

  saga::impl::session* session::get_impl (void) const
  { 
      typedef saga::object base_type;
      return static_cast<saga::impl::session*>(this->base_type::get_impl ());
  }

  TR1::shared_ptr<saga::impl::session> session::get_impl_sp(void) const
  { 
      typedef saga::object base_type;
      return TR1::static_pointer_cast<saga::impl::session>(
          this->base_type::get_impl_sp());
  }

  void session::add_context (context const & c)
  {
      get_impl()->add_context (c);
  }

  void session::remove_context (context const & c)
  {
      get_impl()->remove_context (c);
  }

  std::vector <context> 
  session::list_contexts() const
  {
      return get_impl()->list_contexts();
  }

  void session::close(double timeout)
  {
      return get_impl()->close(timeout);
  }

  /// @cond 
  /** Doxygen generates namespace function from friend impls... */  
  bool operator== (session const & lhs,
                   session const & rhs)
  {
      if ( ! lhs.is_impl_valid() )
      {
        // IncorrrectState
        SAGA_THROW_VERBATIM(lhs, "The lhs session is not initialized", 
            saga::IncorrectState);
      }

      if ( ! rhs.is_impl_valid() )
      {
        // IncorrrectState
        SAGA_THROW_VERBATIM(rhs, "The rhs session is not initialized", 
            saga::IncorrectState);
      }

      return (lhs.get_impl() == rhs.get_impl()); 
  }
  /// @endcond

  /////////////////////////////////////////////////////////////////////////////
  /// @cond
  namespace detail
  {
      preference_type const& get_preferences(saga::session const& s)
      {
          return saga::impl::runtime::get_impl(s)->get_preferences();
      }
  }
  /// @endcond

} // namespace saga
///////////////////////////////////////////////////////////////////////////////

