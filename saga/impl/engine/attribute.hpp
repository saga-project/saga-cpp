//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_IMPL_ENGINE_ATTRIBUTE_HPP
#define SAGA_IMPL_ENGINE_ATTRIBUTE_HPP

// include stl
#include <map>
#include <vector>
#include <string>

#include <boost/thread.hpp>
#include <saga/impl/call.hpp>
#include <saga/impl/engine/attribute_interface.hpp>
#include <saga/impl/engine/attribute_cache.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable : 4251 4231 4660)
#endif

///////////////////////////////////////////////////////////////////////////////
namespace saga
{
  namespace impl
  {
    // in SAGA, attribute is actually an interface.  However, our implementation
    // does not shy back from multiple inheritance, and implement it here...
    class SAGA_EXPORT attribute : public attribute_interface
    {
      public:
        typedef std::vector <std::string> strvec_type;
        typedef std::map <std::string, std::string> strmap_type;

      private:
        // the attributes
        bool cache_only_;

      protected:
        attribute_cache attributes_;
        bool is_valid_key(std::string const& key) const;

      public:
        attribute (void);
        attribute (attribute const& rhs);

        virtual ~attribute (void) {}

        void init (strmap_type const& ro_scalar, strmap_type const& rw_scalar,
            strmap_type const& ro_vector, strmap_type const& rw_vector);
        void init (char const* const* ro_scalar, char const* const* rw_scalar,
            char const* const* ro_vector, char const* const* rw_vector);
        void init (bool extensible, bool cache_only = false);

        void init_keynames(strvec_type const& keynames);
        void init_keynames(char const* const* keynames);

        // setters/getters 
        SAGA_CALL_IMPL_DECL_1(get_attribute, std::string)
        SAGA_CALL_IMPL_DECL_2(set_attribute, std::string, std::string)
        SAGA_CALL_IMPL_DECL_1(get_vector_attribute, std::string)
        SAGA_CALL_IMPL_DECL_2(set_vector_attribute, std::string, strvec_type)
        SAGA_CALL_IMPL_DECL_1(remove_attribute, std::string)

        // inspection
        SAGA_CALL_IMPL_DECL_0(list_attributes)
        SAGA_CALL_IMPL_DECL_1(find_attributes, std::string)

        SAGA_CALL_IMPL_DECL_1(attribute_exists, std::string)
        SAGA_CALL_IMPL_DECL_1(attribute_is_readonly, std::string)
        SAGA_CALL_IMPL_DECL_1(attribute_is_writable, std::string)
        SAGA_CALL_IMPL_DECL_1(attribute_is_vector, std::string)
        SAGA_CALL_IMPL_DECL_1(attribute_is_extended, std::string)

        // special optimized synchronous interface
        std::string get_attribute_sync(std::string const& key);
        void set_attribute_sync(std::string const& key, std::string const& val);

        void get_vector_attribute_sync(std::string const& key, strvec_type& result);
        void set_vector_attribute_sync(std::string const& key, strvec_type const& val);
        void remove_attribute_sync(std::string const& key);

        void list_attributes_sync(strvec_type& result);
        void find_attributes_sync(std::string const& pattern, strvec_type& result);

        bool attribute_exists_sync(std::string const& key);
        bool attribute_is_readonly_sync(std::string const& key);
        bool attribute_is_writable_sync(std::string const& key);
        bool attribute_is_vector_sync(std::string const& key);
        bool attribute_is_extended_sync(std::string const& key);

        bool attributes_extensible () const;

        attribute clone() const;
        bool is_equal(attribute const& rhs) const;

        // get_proxy returns the internal implementation of the 
        // impl::object interface
        virtual saga::impl::proxy* get_proxy();
        virtual saga::impl::proxy const* get_proxy() const;

        attribute_cache *get_cache() { return &attributes_; }
    };

  } // namespace impl
} // namespace saga

#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

#endif // SAGA_IMPL_ENGINE_ATTRIBUTE_HPP


