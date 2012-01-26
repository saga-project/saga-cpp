#if defined(__WAVE__)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/directory.cpp")
#endif
//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if defined(__WAVE__)
#pragma wave option(preserve: 0, output: null)
#endif
// include directory api and implementation
#include <string>
#include <vector>

// include the package we implement
#include <saga/saga/call.hpp>
#include <saga/saga/filesystem.hpp>
#include <saga/impl/filesystem.hpp>
#include <saga/saga/detail/call.hpp>

#ifdef SAGA_DEBUG
#include <saga/saga/packages/filesystem/preprocessed/directory.cpp>
#else

#if defined(__WAVE__)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/directory.cpp")
#endif
///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace filesystem 
{
  directory::directory (saga::impl::directory * init)
    : name_space::directory (init)
  {
  }

  directory::directory (session const & s, saga::url url, int mode)
    : name_space::directory (new saga::impl::directory (s, url, mode))
  {
    this->saga::object::get_impl()->init();
  }

  directory::directory (saga::url url, int mode)
    : name_space::directory (new saga::impl::directory (detail::get_the_session (), url, mode))
  {
    this->saga::object::get_impl()->init();
  }

  directory::directory (saga::object const& o)
    : name_space::directory(o)
  {
      if (this->get_type() != saga::object::Directory)
      {
          SAGA_THROW("Bad type conversion.", saga::BadParameter);
      }
  }

  directory::directory (void)
  {
  }

  directory::~directory (void)
  {
  }

  directory &directory::operator= (saga::object const& o)
  {
      return this->saga::name_space::directory::operator=(o), *this;
  }

  saga::impl::directory* directory::get_impl() const
  { 
    typedef saga::object base_type;
    return static_cast<saga::impl::directory*>(this->base_type::get_impl()); 
  }

  TR1::shared_ptr <saga::impl::directory> directory::get_impl_sp() const
  { 
    typedef saga::object base_type;
    return TR1::static_pointer_cast<saga::impl::directory>(
        this->base_type::get_impl_sp()); 
  }

  // factory
  SAGA_CALL_CREATE_IMP_3(directory, impl::directory, session const&, saga::url, int)

  // get file info
  SAGA_CALL_IMP_2 (directory, get_size, saga::url, int)
  SAGA_CALL_IMP_1 (directory, is_file,  saga::url)

  // overload open methods from namespace_dir
  SAGA_CALL_IMP_2 (directory, open, saga::url, int)
  SAGA_CALL_IMP_2 (directory, open_dir, saga::url, int)

///////////////////////////////////////////////////////////////////////////////////
}} // namespace saga

#endif

