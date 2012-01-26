#if defined(__WAVE__)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/file.cpp")
#endif
//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  job LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if defined(__WAVE__)
#pragma wave option(output: null)
#endif

// include file API and implementation
#include <saga/saga/filesystem.hpp>
#include <saga/impl/filesystem.hpp>
#include <saga/saga/detail/call.hpp>

#ifdef SAGA_DEBUG
#include <saga/saga/packages/filesystem/preprocessed/file.cpp>
#else

#if defined(__WAVE__)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/file.cpp")
#endif
///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace filesystem 
{
  file::file (saga::impl::file * init)
    : entry (init)
  {
  }

  file::file (session const& s, saga::url url, int mode)
    : entry (new saga::impl::file (s, url, mode))
  {
    this->saga::object::get_impl()->init();
  }

  file::file (session const& s, saga::url url, int mode, saga::off_t pointer)
    : entry (new saga::impl::file (s, url, mode, pointer))
  {
    this->saga::object::get_impl()->init();
  }

  file::file (saga::url url, int mode)
    : entry (new saga::impl::file (detail::get_the_session (), url, mode))
  {
    this->saga::object::get_impl()->init();
  }

  file::file (saga::object const& o)
    : entry(o)
  {
      if (this->get_type() != saga::object::File)
      {
          SAGA_THROW("Bad type conversion.", saga::BadParameter);
      }
  }

  file::file (void) 
  {
  }
  
  file::~file (void)
  {
  }

  file &file::operator= (saga::object const& o)
  {
      return this->saga::name_space::entry::operator=(o), *this;
  }

  saga::impl::file* file::get_impl (void) const
  { 
    typedef saga::object base_type;
    return static_cast <saga::impl::file*> (this->base_type::get_impl ());
  }

  TR1::shared_ptr <saga::impl::file> file::get_impl_sp (void) const
  { 
    // FIXME: this needs documentation
    typedef saga::object base_type;
    return TR1::static_pointer_cast <saga::impl::file> (
        this->base_type::get_impl_sp());
  }

  // factory
  SAGA_CALL_CREATE_IMP_3(file, impl::file, session const&, saga::url, int)

  // methods not covered by the namespace_entry
  SAGA_CALL_IMP_0 (file, get_size)
  SAGA_CALL_IMP_2 (file, read, saga::mutable_buffer, saga::ssize_t)
  SAGA_CALL_IMP_2 (file, write, saga::const_buffer, saga::ssize_t)
  SAGA_CALL_IMP_2 (file, seek, saga::off_t, saga::filesystem::seek_mode)

  // scattered I/O
  SAGA_CALL_IMP_1 (file, read_v, std::vector<saga::filesystem::iovec>)
  SAGA_CALL_IMP_1 (file, write_v, std::vector<saga::filesystem::const_iovec>)

  // pattern based I/O
  SAGA_CALL_IMP_1 (file, size_p, std::string)
  SAGA_CALL_IMP_2 (file, read_p, std::string, saga::mutable_buffer)
  SAGA_CALL_IMP_2 (file, write_p, std::string, saga::const_buffer)

  // extended based I/O
  SAGA_CALL_IMP_0 (file, modes_e)
  SAGA_CALL_IMP_2 (file, size_e, std::string, std::string)
  SAGA_CALL_IMP_3 (file, read_e, std::string, std::string, saga::mutable_buffer)
  SAGA_CALL_IMP_3 (file, write_e, std::string, std::string, saga::const_buffer)
  
///////////////////////////////////////////////////////////////////////////////////
}} // namespace saga

#endif

