#ifndef SAGA_IMPL_PACKAGES_DATA_NAMESPACE_NAMESPACEDIR_HPP
#define SAGA_IMPL_PACKAGES_DATA_NAMESPACE_NAMESPACEDIR_HPP

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/namespace_dir.hpp")
#endif
//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005 Michel Zandstra [michelzandstra@gmail.com]
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif
#include <boost/shared_ptr.hpp>

#include <string>

#include <saga/saga/session.hpp>
#include <saga/impl/call.hpp>
#include <saga/impl/engine/proxy.hpp>

#include <saga/impl/packages/namespace/namespace_dir_cpi.hpp>
#include <saga/impl/packages/namespace/namespace_entry.hpp>

#ifdef SAGA_DEBUG
#include <saga/impl/packages/namespace/preprocessed/namespace_dir.hpp>
#else

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: "preprocessed/namespace_dir.hpp")
#endif
///////////////////////////////////////////////////////////////////////////////
namespace saga 
{ 
  namespace impl 
  {
    /**
     * This class acts as a proxy class, hiding all the nasty Saga stuff
     * (like creating and using adaptors) for the user. Users just call
     * the functions of this class like to would on every local object,
     * and don't have to worry about adaptors. It extends
     * saga::name_space::directory_cpi.
     */
    class SAGA_NAMESPACE_PACKAGE_EXPORT namespace_dir 
        : public saga::impl::namespace_entry   // v1_0::namespace_dir_cpi
    {
      typedef v1_0::namespace_dir_cpi namespace_dir_cpi;
      typedef v1_0::preference_type preference_type;

      bool need_to_free_instance_data;

      // initialize newly attached CPI instance
      void init();

     protected:
      namespace_dir (saga::session const & s, 
          saga::object::type t = saga::object::NSDirectory);
      explicit namespace_dir (saga::object::type t);

     public:
      /**
       * Constructor of namespace_dir, sets the location of the namespace_dir, and gets
       * an instance of the Saga engine
       *
       * @param loc the full path and name of the namespace_dir
       * @param s   the session handle
       */
      namespace_dir (saga::session const& s, saga::url loc, int mode,
          saga::object::type t = saga::object::NSDirectory);

      /*! destructor of namespace_dir, destroys namespace_dir and the adaptor it is 
       *  bound to */
      ~namespace_dir (void);

      // factories
      void create_impl_sync(name_space::directory& retval);
      static saga::task create_impl_async(saga::session const& s, saga::url url, int mode);

      // navigation/query methods
      SAGA_CALL_IMPL_DECL_1(change_dir, saga::url)
      SAGA_CALL_IMPL_DECL_2(list, std::string, int)
      SAGA_CALL_IMPL_DECL_2(find, std::string, int)

      SAGA_CALL_IMPL_DECL_1(read_link, saga::url)
      SAGA_CALL_IMPL_DECL_1(exists, saga::url)
      SAGA_CALL_IMPL_DECL_1(is_dir, saga::url)
      SAGA_CALL_IMPL_DECL_1(is_entry, saga::url)
      SAGA_CALL_IMPL_DECL_1(is_link, saga::url)

      // Deal with entries by entry number
      SAGA_CALL_IMPL_DECL_0(get_num_entries)
      SAGA_CALL_IMPL_DECL_1(get_entry, std::size_t)

      // entry management methods
      SAGA_CALL_IMPL_DECL_3(copy, saga::url, saga::url, int)
      SAGA_CALL_IMPL_DECL_3(link, saga::url, saga::url, int)
      SAGA_CALL_IMPL_DECL_3(move, saga::url, saga::url, int)
      SAGA_CALL_IMPL_DECL_2(remove, saga::url, int)
      SAGA_CALL_IMPL_DECL_2(make_dir, saga::url, int)

      // open methods
      SAGA_CALL_IMPL_DECL_2(open, saga::url, int)
      SAGA_CALL_IMPL_DECL_2(open_dir, saga::url, int)

      // entry management methods - wild card versions
      SAGA_CALL_IMPL_DECL_3(copy_wildcard, std::string, saga::url, int)
      SAGA_CALL_IMPL_DECL_3(link_wildcard, std::string, saga::url, int)
      SAGA_CALL_IMPL_DECL_3(move_wildcard, std::string, saga::url, int)
      SAGA_CALL_IMPL_DECL_2(remove_wildcard, std::string, int)
      
      // permissions with flags
      SAGA_CALL_IMPL_DECL_4(permissions_allow, saga::url, std::string, int, int)
      SAGA_CALL_IMPL_DECL_4(permissions_deny, saga::url, std::string, int, int)
      SAGA_CALL_IMPL_DECL_4(permissions_allow_wildcard, std::string, std::string, int, int)
      SAGA_CALL_IMPL_DECL_4(permissions_deny_wildcard, std::string, std::string, int, int)

      // Generate a exact deep copy of this object
      saga::object clone() const;
    };

  }  // namespace impl
} // namespace saga
///////////////////////////////////////////////////////////////////////////////

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif
#endif // !defined(SAGA_DEBUG)

#endif // SAGA_IMPL_PACKAGES_DATA_NAMESPACE_NAMESPACEDIR_HPP


