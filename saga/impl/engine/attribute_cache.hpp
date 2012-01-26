//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_IMPL_ENGINE_ATTRIBUTE_CACHE_HPP
#define SAGA_IMPL_ENGINE_ATTRIBUTE_CACHE_HPP

#include <saga/saga/export_definitions.hpp>

// include stl
#include <map>
#include <set>
#include <vector>
#include <string>

#include <boost/version.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/ptr_container/ptr_map.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable : 4251 4231 4660)
#endif

#if BOOST_VERSION >= 103400 && BOOST_VERSION <= 103500
// workaround for broken ptr_contain implementation on V1.34.x and 1.35
namespace boost { namespace ptr_container_detail 
{
    template <typename F, typename S>
    bool operator==(ref_pair<F, S> const& lhs, ref_pair<F, S> const& rhs)
    {
        return *lhs.second == *rhs.second;
    }
}}
#endif

///////////////////////////////////////////////////////////////////////////////
namespace saga
{
  namespace impl
  {
    ///////////////////////////////////////////////////////////////////////////
    class attrib_base 
    {
    public:
        enum attrib_type
        {
            Scalar = 0,
            Vector = 1
        };

    protected:
        attrib_type type_;  // scalar or vector attrib?
        bool ro_;           // is attrib read only?
        bool ext_;          // is attrib an extension?
        bool has_value_;    // value has been assigned

    public:
        explicit attrib_base (attrib_type type = Scalar, bool ro = true, 
              bool ext  = false)
          : type_(type), ro_(ro), ext_(ext), has_value_(false)
        {}
        attrib_base(attrib_base const& rhs)
          : type_(rhs.type_), ro_(rhs.ro_), ext_(rhs.ext_), 
            has_value_(rhs.has_value_)
        {}

        virtual attrib_base* clone() const = 0;
        virtual ~attrib_base() {}

        bool is_ro() const { return ro_; }
        bool is_ext() const { return ext_; }
        bool is_vec() const { return type_ == Vector ? true : false; }
        bool has_value() const { return has_value_; }

        friend bool operator==(attrib_base const& lhs, attrib_base const&rhs);
    };

    inline attrib_base* new_clone(attrib_base const& a)
    {
        return a.clone();
    }

    ///////////////////////////////////////////////////////////////////////////
    class SAGA_EXPORT attribute_cache 
    {
    public:
        typedef std::vector<std::string> strvec_type;
        typedef std::set<std::string> strset_type;
        typedef std::map<std::string, std::string> strmap_type;
        typedef boost::ptr_map<
            std::string const, saga::impl::attrib_base> a_map_type;

    private:
        typedef boost::recursive_mutex mutex_type;

        // the attributes
        mutable mutex_type mtx_;
        a_map_type a_map_;
        mutable a_map_type::const_iterator last_it_;

        // allowed key names (used, if extensible is false)
        strset_type keynames_;

        // have attributes been initialized?
        bool inited_;

        // can custom attributes be defined?
        bool extensible_;

        void init_attributes(strmap_type const& data, 
            attrib_base::attrib_type type, bool ro, bool ext = false);
        void init_attributes(char const* const* data, 
            attrib_base::attrib_type type, bool ro, bool ext = false);

        bool is_valid_key_locked(std::string const& key) const;

        // maintain last accessed entry
        a_map_type::const_iterator get_end_iter() const { return a_map_.end(); }

        a_map_type::const_iterator find_entry(std::string const& key) const;
        a_map_type::iterator find_entry(std::string const& key);

      public:
        bool is_valid_key(std::string const& key) const;

        attribute_cache (void);
        attribute_cache (attribute_cache const& rhs);

        void init (strmap_type const& ro_scalar, strmap_type const& rw_scalar,
            strmap_type const& ro_vector, strmap_type const& rw_vector);
        void init (char const* const* ro_scalar, 
            char const* const* rw_scalar, char const* const* ro_vector, 
            char const* const* rw_vector);

        void init (bool extensible);

        void init_keynames(strvec_type const& keynames);
        void init_keynames(char const* const* keynames);

        // setters/getters
        std::string  get_attribute           (std::string const& key) const;
        void         set_attribute           (std::string const& key,
                                              std::string const& val);
        void         get_vector_attribute    (std::string const& key, strvec_type&) const;
        void         set_vector_attribute    (std::string const& key,
                                              strvec_type const& val);
                                                          
        void         remove_attribute        (std::string const& key);

        // inspection
        void         list_attributes         (strvec_type&) const;
        void         find_attributes         (std::string const& pattern, strvec_type&) const;
        bool         attribute_exists        (std::string const& key) const;
        bool         attribute_is_readonly   (std::string const& key) const;
        bool         attribute_is_writable   (std::string const& key) const;
        bool         attribute_is_vector     (std::string const& key) const;
        bool         attribute_is_extended   (std::string const& key) const;
        bool         attributes_extensible   () const;

        attribute_cache clone() const;
        bool is_equal(attribute_cache const& rhs) const;
    };

  } // namespace impl

} // namespace saga

#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

#endif // SAGA_IMPL_ENGINE_ATTRIBUTE_CACHE_HPP


