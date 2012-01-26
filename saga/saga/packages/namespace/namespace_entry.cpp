#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/namespace_entry.cpp")
#endif
//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 0, output: null)
#endif

//  this is needed in every file including detail/monitorable_impl.hpp and not 
//  belonging to the engine
#define SAGA_NO_IMPORT_MONITORABLE
#define SAGA_NO_IMPORT_PERMISSIONS
#define SAGA_PERMISSIONS_EXPORTS

// include the package and implementation
#include <saga/saga/util.hpp>
#include <saga/saga/namespace.hpp>
#include <saga/impl/namespace.hpp>

#include <saga/saga/detail/monitorable_impl.hpp>
#include <saga/saga/detail/permissions_impl.hpp>
#include <saga/saga/detail/call.hpp>

#include <string>
#include <vector>

#ifdef SAGA_DEBUG
#include <saga/saga/packages/namespace/preprocessed/namespace_entry.cpp>
#else

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/namespace_entry.cpp")
#endif
///////////////////////////////////////////////////////////////////////////////
namespace saga 
{
  namespace name_space 
  {
    entry::entry (saga::impl::namespace_entry * init)
      : saga::object (init)
    {
    }

    entry::entry (session const & session, saga::url url, int mode, 
            saga::object::type t)
      : saga::object (new saga::impl::namespace_entry (
          session, url, mode, t))
    {
      saga::impl::object* obj (this->saga::object::get_impl());
      obj->init();
      if (t == saga::object::NSEntry)
        obj->set_type((saga::object::type)obj->get_cpitype());
    }

    entry::entry (saga::url url, int mode, saga::object::type t)
      : saga::object (new saga::impl::namespace_entry (
            saga::detail::get_the_session(), url, mode, t))
    {
      saga::impl::object* obj (this->saga::object::get_impl());
      obj->init();
      if (t == saga::object::NSEntry)
        obj->set_type((saga::object::type)obj->get_cpitype());
    }

    entry::entry (saga::object const& o)
      : saga::object (o)
    {
    }

    entry::entry ()
    {
    }

    entry::~entry (void)
    {
    }

    entry &entry::operator= (saga::object const& o)
    {
        return saga::object::operator=(o), *this;
    }

    saga::impl::namespace_entry* entry::get_impl() const
    { 
        typedef saga::object base_type;
        return static_cast<saga::impl::namespace_entry*>(this->base_type::get_impl()); 
    }

    TR1::shared_ptr <saga::impl::namespace_entry> entry::get_impl_sp(void) const
    { 
      // FIXME: this needs documentation
      typedef saga::object base_type;
      return TR1::static_pointer_cast<saga::impl::namespace_entry>( 
          this->base_type::get_impl_sp());
    }

    ///////////////////////////////////////////////////////////////////////////
    SAGA_CALL_CREATE_IMP_3(entry, impl::namespace_entry, session const&, saga::url, int)

    // basic properties
    SAGA_CALL_CONST_IMP_0(entry, get_url)
    SAGA_CALL_CONST_IMP_0(entry, get_cwd)
    SAGA_CALL_CONST_IMP_0(entry, get_name)

    // navigation/query methods
    SAGA_CALL_CONST_IMP_0(entry, read_link)
    SAGA_CALL_CONST_IMP_0(entry, is_dir)
    SAGA_CALL_CONST_IMP_0(entry, is_entry)
    SAGA_CALL_CONST_IMP_0(entry, is_link)

    // entry management methods
    SAGA_CALL_IMP_2(entry, copy, saga::url, int)
    SAGA_CALL_IMP_2(entry, link, saga::url, int)
    SAGA_CALL_IMP_2(entry, move, saga::url, int)
    SAGA_CALL_IMP_1(entry, remove, int)
    SAGA_CALL_IMP_1(entry, close, double)

    // permissions with flags
    SAGA_CALL_IMP_3(entry, permissions_allow, std::string, int, int)
    SAGA_CALL_IMP_3(entry, permissions_deny, std::string, int, int)

  }

  namespace detail
  {
    /////////////////////////////////////////////////////////////////////////////
    //  implement the monitorable functions (we need to explicitly specialize 
    //  the template because the functions are not implemented inline)
    template struct SAGA_NAMESPACE_PACKAGE_EXPORT monitorable<name_space::entry>;

    /////////////////////////////////////////////////////////////////////////////
    //  implement the permissions functions (we need to explicitly specialize 
    //  the template because the functions are not implemented inline)
    template struct SAGA_NAMESPACE_PACKAGE_EXPORT permissions<name_space::entry>;

    template struct SAGA_NAMESPACE_PACKAGE_EXPORT permissions_priv<name_space::entry, task_base::Sync>;
    template struct SAGA_NAMESPACE_PACKAGE_EXPORT permissions_priv<name_space::entry, task_base::Async>;
    template struct SAGA_NAMESPACE_PACKAGE_EXPORT permissions_priv<name_space::entry, task_base::Task>;
  }

///////////////////////////////////////////////////////////////////////////////
} // namespace saga

#endif // SAGA_DEBUG

