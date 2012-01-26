#ifndef SAGA_IMPL_PACKAGES_DATA_ADVERT_ADVERT_HPP
#define SAGA_IMPL_PACKAGES_DATA_ADVERT_ADVERT_HPP

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/advert.hpp")
#endif
//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 0, output: null)
#endif

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif 

#include <string>
#include <vector>

#include <saga/saga/session.hpp>
#include <saga/impl/call.hpp>
#include <saga/impl/engine/proxy.hpp>
#include <saga/impl/packages/advert/advert_cpi.hpp>
#include <saga/impl/engine/attribute.hpp>

#include <saga/impl/packages/namespace/namespace_entry.hpp>

#ifdef SAGA_DEBUG
#include <saga/impl/packages/advert/preprocessed/advert.hpp>
#else

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: "preprocessed/advert.hpp")
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
    class advert 
    :   public saga::impl::namespace_entry, // v1_0::advert_cpi
        public saga::impl::attribute
    {
    private:
        typedef v1_0::advert_cpi advert_cpi;
        typedef v1_0::preference_type preference_type;
        typedef saga::impl::attribute attribute_base;

        // initialize newly attached CPI instance
        void init();

        explicit advert(attribute const& attr);

     public:
      /**
        * Constructor of file, sets the location of the file, and gets
        * an instance of the Saga engine
        *
        * @param url the full path and name of the file
        * @param s   the session handle
        */
        advert (saga::session const& s, saga::url url, int mode);

        /*! destructor of advert, destroys file and the adaptor it is 
        *  bound to */
        ~advert (void);

        // factory support
        void create_impl_sync(saga::advert::entry& retval);
        static saga::task create_impl_async(saga::session const& s, 
            saga::url const& url, int mode);

        // API functions not covered by namespace_entry
        SAGA_CALL_IMPL_DECL_1(store_object, saga::object)
        SAGA_CALL_IMPL_DECL_1(retrieve_object, saga::session)

        SAGA_CALL_IMPL_DECL_1(store_string, std::string)
        SAGA_CALL_IMPL_DECL_0(retrieve_string)

        saga::impl::attribute* get_attributes() { return this; }
        saga::impl::attribute const* get_attributes() const { return this; }

        // get_proxy returns the internal implementation of the 
        // impl::object interface
        saga::impl::proxy* get_proxy() { return this; }
        saga::impl::proxy const* get_proxy() const { return this; }

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
#endif // SAGA_IMPL_PACKAGES_DATA_ADVERT_ADVERT_HPP
