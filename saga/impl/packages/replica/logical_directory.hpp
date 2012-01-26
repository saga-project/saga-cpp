#ifndef SAGA_IMPL_PACKAGES_DATA_REPLICA_LOGICAL_DIRECTORY_HPP
#define SAGA_IMPL_PACKAGES_DATA_REPLICA_LOGICAL_DIRECTORY_HPP

#if defined(__WAVE__)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/logical_directory.hpp")
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
#include <saga/impl/packages/replica/logical_directory_cpi.hpp>
#include <saga/impl/packages/namespace/namespace_dir.hpp>

#ifdef SAGA_DEBUG
#include <saga/impl/packages/replica/preprocessed/logical_directory.hpp>
#else

#if defined(__WAVE__)
#pragma wave option(output: "preprocessed/logical_directory.hpp")
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
    class logical_directory 
        : public namespace_dir, // v1_0::directory_cpi
          public saga::impl::attribute
    {
     private:
       typedef v1_0::logical_directory_cpi logical_directory_cpi;
       typedef v1_0::preference_type preference_type;
       typedef saga::impl::attribute attribute_base;

       // initialize newly attached CPI instance
       void init();

      explicit logical_directory(attribute const& attr);

     public:
       /**
        * Constructor of directory, sets the location of the directory, and gets
        * an instance of the Saga engine
        *
        * @param url the full path and name of the directory
        * @param s   the session handle
        */
       logical_directory (saga::session const& s, saga::url url, int mode);

       /*! destructor of directory, destroys directory and the adaptor it is 
        *  bound to */
       ~logical_directory (void);

       // factory support
       void create_impl_sync(replica::logical_directory& retval);
       static saga::task create_impl_async(saga::session const& s, 
          saga::url const& url, int mode);

       // get file info
       SAGA_CALL_IMPL_DECL_1 (is_file, saga::url)

       // overload open methods of name space
       SAGA_CALL_IMPL_DECL_2 (open, saga::url, int)
       SAGA_CALL_IMPL_DECL_2 (open_dir, saga::url, int)
       SAGA_CALL_IMPL_DECL_3 (find, std::string, std::vector<std::string>, int)

       saga::impl::attribute* get_attributes() { return this; }
       saga::impl::attribute const* get_attributes() const { return this; }

       // get_proxy returns the internal implementation of the 
       // impl::object interface
       saga::impl::proxy* get_proxy() { return this; }
       saga::impl::proxy const* get_proxy() const { return this; }

       // Generate a exact deep copy of this object
       saga::object clone() const;
    };

///////////////////////////////////////////////////////////////////////////////
}} // namespace saga::impl

#if defined(__WAVE__)
#pragma wave option(output: null)
#endif

#endif // !defined(SAGA_DEBUG)
#endif // SAGA_IMPL_PACKAGES_DATA_REPLICA_LOGICAL_DIRECTORY_HPP

