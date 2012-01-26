//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Use, modification and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/config.hpp>
#include <boost/lexical_cast.hpp>   // lexical_cast will be in TR2

#include <saga/saga/exception.hpp>
#include <saga/saga/context.hpp>

#include <saga/impl/context.hpp>
#include <saga/impl/engine/object.hpp>
#include <saga/impl/engine/proxy.hpp>
#include <saga/impl/engine/metric.hpp>

#include <saga/saga/detail/attribute_impl.hpp>

#include <boost/assign/std/vector.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga 
{
//   namespace detail
//   {
//     ///@cond
//       std::string get_userid()
//       {
// #if defined(BOOST_WINDOWS)
//           char chBuffer[MAX_PATH + 1];
//           DWORD cchBuff = sizeof(chBuffer)-1;
//           GetUserName (chBuffer, &cchBuff);     // Get and display the user name.
//           return chBuffer;
// #else
//           return boost::lexical_cast<std::string>(getuid());
// #endif
//       }
//     ///@endcond
//   }
  
  /////////////////////////////////////////////////////////////////////////////
  context::context (std::string const& ctxtype) 
    : saga::object (new impl::context())
  {
      using namespace boost::assign;

      std::vector<std::string> valid_keys;
      valid_keys += 
          attributes::context_type,
          attributes::context_server,
          attributes::context_certrepository,
          attributes::context_userproxy,
          attributes::context_usercert,
          attributes::context_userkey,
          attributes::context_userid,
          attributes::context_userpass,
          attributes::context_uservo,
          attributes::context_lifetime,
          attributes::context_remoteid, 
          attributes::context_remotehost,
          attributes::context_remoteport;
      this->init_keynames(valid_keys);

      // initialize this context if the type is given
      strmap_type attributes_scalar_rw;
      insert(attributes_scalar_rw)
              (attributes::context_type, ctxtype)
              (attributes::context_server, "")
              (attributes::context_certrepository, "")
              (attributes::context_userproxy, "")
              (attributes::context_usercert, "")
              (attributes::context_userkey, "")
              (attributes::context_userid, "")
              (attributes::context_userpass, "")
              (attributes::context_uservo, "")
              (attributes::context_lifetime, "")
              (attributes::context_remoteid, "")
              (attributes::context_remotehost, "")
              (attributes::context_remoteport, "")
          ;

      // initialize attribute implementation
      this->init(strmap_type(), attributes_scalar_rw);

      // FIXME: attribute_cache only
      // making this not cache only may lead to infinite recursions while
      // the default session is initialized
      this->init(true, true);

// This has been removed as per latest Spec errata, has been moved to 
// session::add_context
//       if (!ctxtype.empty()) {
//       // initialize the context
//           get_impl()->set_defaults(true);
//       }

      // initialize implementation object
      if (!ctxtype.empty()) 
          this->saga::object::get_impl()->init();
  }

  context::context(saga::impl::context *impl)
    : saga::object(impl)
  {
  }
  context::context (TR1::shared_ptr<saga::impl::context> init)
    : saga::object(init)
  {
  }

  context::context(saga::object const& obj)
    : saga::object (obj)
  {
      if (this->get_type() != saga::object::Context)
      {
          SAGA_THROW("Bad type conversion.", saga::BadParameter);
      }
  }
  context& context::operator=(saga::object const& obj)
  {
      saga::object::operator=(obj);
      if (this->get_type() != saga::object::Context)
      {
          SAGA_THROW("Bad type conversion.", saga::BadParameter);
      }
      return *this;
  }
  
  context::~context (void)
  {
  }

  saga::impl::context* context::get_impl (void) const
  { 
    typedef saga::object base_type;
    return static_cast<saga::impl::context*>(this->base_type::get_impl());
  }

  TR1::shared_ptr <saga::impl::context> context::get_impl_sp(void) const
  { 
    typedef saga::object base_type;
    return TR1::static_pointer_cast<saga::impl::context>(
        this->base_type::get_impl_sp());
  }

// has been removed as of newest SAGA spec
//   void context::set_defaults()
//   {
//       get_impl()->set_defaults(true);
//   }

  /// @cond 
  /** Doxygen generates namespace function from friend impls... */
  bool operator== (context const & lhs, context const & rhs)
  {
      return lhs.get_impl()->is_equal(rhs.get_impl()); 
  }
  /// @endcond

  namespace detail
  {
    /////////////////////////////////////////////////////////////////////////////
    //  implement the attribute functions (we need to explicitly specialize 
    //  the template because the functions are not implemented inline)
    template struct SAGA_EXPORT_REPEAT attribute<context>;

    template struct SAGA_EXPORT attribute_priv<context, task_base::Sync>;
    template struct SAGA_EXPORT attribute_priv<context, task_base::Async>;
    template struct SAGA_EXPORT attribute_priv<context, task_base::Task>;

    template struct SAGA_EXPORT attribute_sync<context>;
  }

///////////////////////////////////////////////////////////////////////////////
} // namespace saga 

