#ifndef SAGA_IMPL_PACKAGES_CPR_CHECKPOINT_HPP
#define SAGA_IMPL_PACKAGES_CPR_CHECKPOINT_HPP

#if defined(__WAVE__)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/cpr_checkpoint.hpp")
#endif
//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if defined(__WAVE__)
#pragma wave option(preserve: 0, output: null)
#endif

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif 

#include <boost/shared_ptr.hpp>

#include <string>
#include <vector>

#include <saga/saga/session.hpp>
#include <saga/impl/call.hpp>
#include <saga/impl/engine/proxy.hpp>
#include <saga/impl/packages/cpr/cpr_checkpoint_cpi.hpp>
#include <saga/impl/engine/attribute.hpp>

#include <saga/impl/packages/namespace/namespace_entry.hpp>

#ifdef SAGA_DEBUG
#include <saga/impl/packages/cpr/preprocessed/cpr_checkpoint.hpp>
#else

#if defined(__WAVE__)
#pragma wave option(output: "preprocessed/cpr_checkpoint.hpp")
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
    class cpr_checkpoint 
    :   public saga::impl::namespace_entry, // v1_0::cpr_checkpoint_cpi
        public saga::impl::attribute
    {
      typedef v1_0::cpr_checkpoint_cpi cpr_checkpoint_cpi;
      typedef v1_0::preference_type preference_type;
      typedef saga::impl::attribute attribute_base;

      // initialize newly attached CPI instance
      void init();

      explicit cpr_checkpoint(attribute const& attr);

     public:
      /**
       * Constructor of file, sets the location of the file, and gets
       * an instance of the Saga engine
       *
       * @param url the full path and name of the file
       * @param s   the session handle
       */
      cpr_checkpoint (saga::session const& s, saga::url url, int mode);

      /*! destructor of cpr_checkpoint, destroys file and the adaptor it is 
       *  bound to */
      ~cpr_checkpoint (void);

      // factory support
      void create_impl_sync(saga::cpr::checkpoint& retval);
      static saga::task create_impl_async(saga::session const& s, 
          saga::url const& url, int mode);

      // API functions not covered by namespace_entry
      SAGA_CALL_IMPL_DECL_2(set_parent,      saga::url, int)
      SAGA_CALL_IMPL_DECL_1(get_parent,      int)
      SAGA_CALL_IMPL_DECL_0(get_file_num)
      SAGA_CALL_IMPL_DECL_0(list_files)
      SAGA_CALL_IMPL_DECL_1(add_file,        saga::url)
      SAGA_CALL_IMPL_DECL_1(get_file,        int)
      SAGA_CALL_IMPL_DECL_2(open_file,       saga::url, int)
      SAGA_CALL_IMPL_DECL_2(open_file_idx,   int,       int)
      SAGA_CALL_IMPL_DECL_1(remove_file,     saga::url)
      SAGA_CALL_IMPL_DECL_1(remove_file_idx, int)
      SAGA_CALL_IMPL_DECL_2(update_file,     saga::url, saga::url)
      SAGA_CALL_IMPL_DECL_2(update_file_idx, int,       saga::url)
      SAGA_CALL_IMPL_DECL_2(stage_file,      saga::url, saga::url)
      SAGA_CALL_IMPL_DECL_2(stage_file_idx,  int,       saga::url)
      SAGA_CALL_IMPL_DECL_1(stage_file_all,  saga::url)
      
  
      saga::impl::attribute* get_attributes() { return this; }
      saga::impl::attribute const* get_attributes() const { return this; }
        
      // Generate a exact deep copy of this object
      saga::object clone() const;
    };

  } // namespace impl

} // namespace saga
///////////////////////////////////////////////////////////////////////////////
#if defined(__WAVE__)
#pragma wave option(output: null)
#endif

#endif // !defined(SAGA_DEBUG)
#endif // SAGA_IMPL_PACKAGES_CPR_CHECKPOINT_HPP
