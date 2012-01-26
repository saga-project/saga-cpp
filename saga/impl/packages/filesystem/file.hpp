#ifndef SAGA_IMPL_PACKAGES_FILESYSTEM_FILE_HPP
#define SAGA_IMPL_PACKAGES_FILESYSTEM_FILE_HPP

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/file.hpp")
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

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif 

#include <boost/shared_ptr.hpp>

#include <string>

#include <saga/saga/session.hpp>
#include <saga/saga/buffer.hpp>
#include <saga/impl/call.hpp>
#include <saga/impl/engine/proxy.hpp>
#include <saga/impl/packages/filesystem/file_cpi.hpp>

#include <saga/impl/packages/namespace/namespace_entry.hpp>

#ifdef SAGA_DEBUG
#include <saga/impl/packages/filesystem/preprocessed/file.hpp>
#else

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: "preprocessed/file.hpp")
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
     * and don't have to worry about adaptors. It extends saga::file::file_cpi.
     */
    class file 
    :   public saga::impl::namespace_entry
    {
      typedef v1_0::file_cpi file_cpi;
      typedef v1_0::preference_type preference_type;

      // initialize newly attached CPI instance
      void init();

     public:
      /**
       * Constructor of file, sets the location of the file, and gets
       * an instance of the Saga engine
       *
       * @param url the full path and name of the file
       * @param s   the session handle
       */
      file (saga::session const & s, saga::url url, int mode,
        saga::off_t pointer = 0);

      /*! destructor of file, destroys file and the adaptor it is 
       *  bound to */
      ~file (void);

      // factory support
      void create_impl_sync(filesystem::file& retval);
      static saga::task create_impl_async(saga::session const& s, saga::url const& url, int mode);

      // API functions not covered by namespace_entry
      SAGA_CALL_IMPL_DECL_0(get_size)

      SAGA_CALL_IMPL_DECL_2(read, saga::mutable_buffer, saga::ssize_t)
      SAGA_CALL_IMPL_DECL_2(write, saga::const_buffer, saga::ssize_t)
      SAGA_CALL_IMPL_DECL_2(seek, saga::off_t, saga::filesystem::seek_mode)

      // scattered I/O
      SAGA_CALL_IMPL_DECL_1(read_v, std::vector<saga::filesystem::iovec>)
      SAGA_CALL_IMPL_DECL_1(write_v, std::vector<saga::filesystem::const_iovec>)

      // pattern based I/O
      SAGA_CALL_IMPL_DECL_1(size_p, std::string)
      SAGA_CALL_IMPL_DECL_2(read_p, std::string, saga::mutable_buffer)
      SAGA_CALL_IMPL_DECL_2(write_p, std::string, saga::const_buffer)

      // extended based I/O
      SAGA_CALL_IMPL_DECL_0(modes_e)
      SAGA_CALL_IMPL_DECL_2(size_e, std::string, std::string)
      SAGA_CALL_IMPL_DECL_3(read_e, std::string, std::string, saga::mutable_buffer)
      SAGA_CALL_IMPL_DECL_3(write_e, std::string, std::string, saga::const_buffer)

       // Generate a exact deep copy of this object
       saga::object clone() const;
    };

  } // namespace impl
} // namespace saga
///////////////////////////////////////////////////////////////////////////////

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif
#endif // !defined(SAGA_DEBUG)

#endif // SAGA_IMPL_PACKAGES_FILESYSTEM_FILE_HPP
