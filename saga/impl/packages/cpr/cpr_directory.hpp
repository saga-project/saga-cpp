#ifndef SAGA_IMPL_PACKAGES_CPR_DIRECTORY_HPP
#define SAGA_IMPL_PACKAGES_CPR_DIRECTORY_HPP

#if defined(__WAVE__)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/cpr_directory.hpp")
#endif
//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if defined(__WAVE__)
#pragma wave option(output: null)
#endif

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif 

#include <boost/shared_ptr.hpp>

#include <string>

#include <saga/saga/session.hpp>

#include <saga/impl/call.hpp>
#include <saga/impl/engine/proxy.hpp>
#include <saga/impl/engine/attribute.hpp>
#include <saga/impl/packages/cpr/cpr_directory_cpi.hpp>
#include <saga/impl/packages/namespace/namespace_dir.hpp>

#ifdef SAGA_DEBUG
#include <saga/impl/packages/cpr/preprocessed/cpr_directory.hpp>
#else

#if defined(__WAVE__)
#pragma wave option(output: "preprocessed/cpr_directory.hpp")
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
     * and don't have to worry about adaptors. It extends saga::file::directory_cpi.
     */
    class cpr_directory 
        : public namespace_dir, // v1_0::directory_cpi
          public saga::impl::attribute
    {
     private:
       typedef v1_0::cpr_directory_cpi cpr_directory_cpi;
       typedef v1_0::preference_type preference_type;
       typedef saga::impl::attribute attribute_base;

       // initialize newly attached CPI instance
       void init();
       
      explicit cpr_directory(attribute const& attr);

     public:
       /**
        * Constructor of directory, sets the location of the directory, and gets
        * an instance of the Saga engine
        *
        * @param url the full path and name of the directory
        * @param s   the session handle
        */
       cpr_directory (saga::session const& s, saga::url url, int mode);

       /*! destructor of directory, destroys directory and the adaptor it is 
        *  bound to */
       ~cpr_directory (void);

      // factory support
      void create_impl_sync(saga::cpr::directory& retval);
      static saga::task create_impl_async(saga::session const& s, 
          saga::url const& url, int mode);

       // overload open methods of name space
       SAGA_CALL_IMPL_DECL_1 (is_checkpoint,   saga::url)
       SAGA_CALL_IMPL_DECL_4 (find,            std::string, std::vector<std::string>, int, std::string)
       SAGA_CALL_IMPL_DECL_3 (set_parent,      saga::url, saga::url, int)
       SAGA_CALL_IMPL_DECL_2 (get_parent,      saga::url, int)
       SAGA_CALL_IMPL_DECL_1 (get_file_num,    saga::url)
       SAGA_CALL_IMPL_DECL_1 (list_files,      saga::url)
       SAGA_CALL_IMPL_DECL_2 (add_file,        saga::url, saga::url)
       SAGA_CALL_IMPL_DECL_2 (get_file,        saga::url, int)
       SAGA_CALL_IMPL_DECL_3 (open_file,       saga::url, saga::url, int)
       SAGA_CALL_IMPL_DECL_3 (open_file_idx,   saga::url, int,       int)
       SAGA_CALL_IMPL_DECL_2 (remove_file,     saga::url, saga::url)
       SAGA_CALL_IMPL_DECL_2 (remove_file_idx, saga::url, int)
       SAGA_CALL_IMPL_DECL_3 (update_file,     saga::url, saga::url, saga::url)
       SAGA_CALL_IMPL_DECL_3 (update_file_idx, saga::url, int,       saga::url)
       SAGA_CALL_IMPL_DECL_3 (stage_file,      saga::url, saga::url, saga::url)
       SAGA_CALL_IMPL_DECL_3 (stage_file_idx,  saga::url, int,       saga::url)
       SAGA_CALL_IMPL_DECL_2 (stage_file_all,  saga::url, saga::url)
       SAGA_CALL_IMPL_DECL_2 (open,            saga::url, int)
       SAGA_CALL_IMPL_DECL_2 (open_dir,        saga::url, int)


       saga::impl::attribute* get_attributes() { return this; }
       saga::impl::attribute const* get_attributes() const { return this; }
          
       // Generate a exact deep copy of this object
       saga::object clone() const;
    };                        
                              
///////////////////////////////////////////////////////////////////////////////
}} // namespace saga::impl

#if defined(__WAVE__)
#pragma wave option(output: null)
#endif

#endif // !defined(SAGA_DEBUG)
#endif // SAGA_IMPL_PACKAGES_CPR_DIRECTORY_HPP

