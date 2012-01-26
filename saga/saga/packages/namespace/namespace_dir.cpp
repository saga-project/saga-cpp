#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/namespace_dir.cpp")
#endif
//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 0, output: null)
#endif

// include the package and implementation
#include <saga/saga/util.hpp>
#include <saga/saga/namespace.hpp>
#include <saga/impl/namespace.hpp>
#include <saga/saga/detail/call.hpp>

#include <string>
#include <vector>

#ifdef SAGA_DEBUG
#include <saga/saga/packages/namespace/preprocessed/namespace_dir.cpp>
#else

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/namespace_dir.cpp")
#endif
///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace name_space 
{
  directory::directory (saga::impl::namespace_dir *init)
      : entry (init)
  {
  }
  
  directory::directory (session const& s, saga::url url, int mode, 
          saga::object::type t)
    : entry (new saga::impl::namespace_dir (s, url, mode, t))
  {
    saga::impl::object* obj (this->saga::object::get_impl());
    obj->init();
    if (t == saga::object::NSDirectory)
      obj->set_type((saga::object::type)obj->get_cpitype());
  }

  directory::directory (saga::url url, int mode,
          saga::object::type t)
    : entry (new saga::impl::namespace_dir (
          detail::get_the_session(), url, mode, t))
  {
    saga::impl::object* obj (this->saga::object::get_impl());
    obj->init();
    if (t == saga::object::NSDirectory)
      obj->set_type((saga::object::type)obj->get_cpitype());
  }

  directory::directory (saga::object const& o)
    : saga::name_space::entry(o)
  {
  }
  
  directory::directory (void)
  {
  }

  directory::~directory (void)
  {
  }

  directory &directory::operator= (saga::object const& o)
  {
      return this->saga::name_space::entry::operator=(o), *this;
  }

  saga::impl::namespace_dir* directory::get_impl() const
  { 
      typedef saga::object base_type;
      return static_cast<saga::impl::namespace_dir*>(this->base_type::get_impl()); 
  }

  TR1::shared_ptr <saga::impl::namespace_dir> directory::get_impl_sp() const
  { 
    typedef saga::object base_type;
    return TR1::static_pointer_cast<saga::impl::namespace_dir>(
        this->base_type::get_impl_sp()); 
  }

  // factories
  SAGA_CALL_CREATE_IMP_3(directory, impl::namespace_dir, session const&, saga::url, int)

  // navigation/query methods
  SAGA_CALL_IMP_1(directory, change_dir, saga::url)
  SAGA_CALL_CONST_IMP_2(directory, list, std::string, int)
  SAGA_CALL_CONST_IMP_2(directory, find, std::string, int)

  SAGA_CALL_CONST_IMP_1(directory, read_link, saga::url)
  SAGA_CALL_CONST_IMP_1(directory, exists, saga::url)
  SAGA_CALL_CONST_IMP_1(directory, is_dir, saga::url)
  SAGA_CALL_CONST_IMP_1(directory, is_entry, saga::url)
  SAGA_CALL_CONST_IMP_1(directory, is_link, saga::url)

  // Deal with entries by entry number
  SAGA_CALL_CONST_IMP_0(directory, get_num_entries)
  SAGA_CALL_CONST_IMP_1(directory, get_entry, std::size_t)

  // entry management methods
  SAGA_CALL_IMP_3(directory, copy, saga::url, saga::url, int)
  SAGA_CALL_IMP_3(directory, link, saga::url, saga::url, int)
  SAGA_CALL_IMP_3(directory, move, saga::url, saga::url, int)
  SAGA_CALL_IMP_2(directory, remove, saga::url, int)
  SAGA_CALL_IMP_2(directory, make_dir, saga::url, int)

  // open methods
  SAGA_CALL_IMP_2(directory, open, saga::url, int)
  SAGA_CALL_IMP_2(directory, open_dir, saga::url, int)

  // entry management methods - wildcard versions
  SAGA_CALL_IMP_3_EX(directory, copy, copy_wildcard, std::string, saga::url, int)
  SAGA_CALL_IMP_3_EX(directory, link, link_wildcard, std::string, saga::url, int)
  SAGA_CALL_IMP_3_EX(directory, move, move_wildcard, std::string, saga::url, int)
  SAGA_CALL_IMP_2_EX(directory, remove, remove_wildcard, std::string, int)

  // permissions with flags
  SAGA_CALL_IMP_4(directory, permissions_allow, saga::url, std::string, int, int)
  SAGA_CALL_IMP_4(directory, permissions_deny, saga::url, std::string, int, int)
  SAGA_CALL_IMP_4_EX(directory, permissions_allow, permissions_allow_wildcard, std::string, std::string, int, int)
  SAGA_CALL_IMP_4_EX(directory, permissions_deny, permissions_deny_wildcard, std::string, std::string, int, int)

///////////////////////////////////////////////////////////////////////////////////
}} // namespace saga::name_space

#endif
