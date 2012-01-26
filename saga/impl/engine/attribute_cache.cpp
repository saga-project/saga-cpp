//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <vector>
#include <string>
#include <iostream>

#include <boost/function_output_iterator.hpp>
#include <boost/pool/singleton_pool.hpp>

#include <saga/saga/exception.hpp>
#include <saga/impl/exception.hpp>
#include <saga/impl/engine/attribute_cache.hpp>

/////////////////////////////////////////////////////////////////
namespace 
{
    inline std::string const& escape (const std::string & in)
    {
        // FIXME: escape !!
        return in;
    }

    inline std::string const& unescape (const std::string & in)
    {
        // FIXME: unescape !!
        return in;
    }

    inline std::string concat (const std::vector <std::string> & in)
    {
        if (1 == in.size())
            return in[0];

        std::vector <std::string>::const_iterator i = in.begin ();

        // get first entry
        std::string out (escape(*i)); 
        for (++i; i != in.end(); ++i )
        {
            out += ", " + escape(*i);
        }

        return (out);
    }
}

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl
{
    ///////////////////////////////////////////////////////////////////////////
    class scalar_attrib : public attrib_base
    {
    private:
        std::string val_;   // value of attrib

    public:
        explicit scalar_attrib (bool ro = true, bool ext  = false)
          : attrib_base(attrib_base::Scalar, ro, ext)
        {}
        explicit scalar_attrib (std::string const& val, bool ro = true, 
                bool ext = false)
          : attrib_base(attrib_base::Scalar, ro, ext), val_(val)
        {
            has_value_ = true;
        }
        scalar_attrib(scalar_attrib const& rhs)
          : attrib_base(rhs), val_(rhs.val_)
        {}

        void val (std::string const& val) 
            { val_ = val; has_value_ = true; }
        std::string const& val() const 
            { BOOST_ASSERT(has_value_); return val_; }

        attrib_base* clone() const 
        {
            return new scalar_attrib(*this);
        }
    };

    ///////////////////////////////////////////////////////////////////////////
    class vector_attrib : public attrib_base
    {
    public:
        typedef std::vector<std::string> strvec_type;

    private:
        strvec_type val_;       // value of this attrib

    public:
        explicit vector_attrib (bool ro = true, bool ext  = false)
          : attrib_base(attrib_base::Vector, ro, ext)
        {}
        explicit vector_attrib (strvec_type const& val, bool ro = true, 
                bool ext = false)
          : attrib_base(attrib_base::Vector, ro, ext), val_(val)
        {
            has_value_ = true;
        }
        explicit vector_attrib (std::string const& val, bool ro = true, 
                bool ext = false)
          : attrib_base(attrib_base::Vector, ro, ext)
        {
            val_.push_back(val);
            has_value_ = true;
        }
        vector_attrib(vector_attrib const& rhs)
          : attrib_base(rhs), val_(rhs.val_)
        {}

        void val (strvec_type const& val) 
            { val_ = val; has_value_ = true; }
        strvec_type const& val() const 
            { BOOST_ASSERT(has_value_); return val_; }

        attrib_base* clone() const 
        {
            return new vector_attrib(*this);
        }
    };

    inline bool operator==(attrib_base const& lhs, attrib_base const&rhs)
    {
        if (!(rhs.ro_ == lhs.ro_ && rhs.ext_ == lhs.ext_ && 
            rhs.type_ == lhs.type_ && rhs.has_value_ == lhs.has_value_))
        {
            return false;
        }

        if (rhs.has_value()) 
        {
            if (rhs.type_ == attrib_base::Scalar) 
            {
                return static_cast<scalar_attrib const&>(rhs).val() ==
                       static_cast<scalar_attrib const&>(lhs).val();
            }
            else if (rhs.type_ == attrib_base::Vector) 
            {
                return static_cast<vector_attrib const&>(rhs).val() ==
                       static_cast<vector_attrib const&>(lhs).val();
            }

            BOOST_ASSERT(false);    // snh
            return false;
        }
        return true;
    }

    ///////////////////////////////////////////////////////////////////////////
    attribute_cache::attribute_cache (void)
      : last_it_(get_end_iter()), inited_(false), extensible_(false)
    {}

    // copy constructor
#if BOOST_VERSION <= 103401
    // Boost V1.34 and below requires explicit cloning support
    attribute_cache::attribute_cache  (attribute_cache const& rhs)
      : keynames_(rhs.keynames_), 
        inited_(rhs.inited_), extensible_(rhs.extensible_)
    {
        a_map_type::const_iterator end = rhs.a_map_.end();
        for (a_map_type::const_iterator it = rhs.a_map_.begin(); it != end; ++it)
#if BOOST_VERSION <= 103301
            a_map_.insert(it.key(), new_clone(*it));
#else
            a_map_.insert((*it).first, new_clone(*(*it).second));
#endif
        last_it_ = get_end_iter();
    }
#else
    attribute_cache::attribute_cache  (attribute_cache const& rhs)
      : a_map_(rhs.a_map_), last_it_(get_end_iter()), 
        keynames_(rhs.keynames_), 
        inited_(rhs.inited_), extensible_(rhs.extensible_)
    {}
#endif

#if BOOST_VERSION <= 103301
    inline std::string 
    get_key(attribute_cache::a_map_type::iterator const& it)
    {
        return it.key();
    }
    inline attrib_base*
    get_value(attribute_cache::a_map_type::iterator const& it)
    {
        return &*it;
    }

    inline std::string 
    get_key(attribute_cache::a_map_type::const_iterator const& it)
    {
        return it.key();
    }
    inline attrib_base const*
    get_value(attribute_cache::a_map_type::const_iterator const& it)
    {
        return &*it;
    }
#else
    inline std::string const& 
    get_key(attribute_cache::a_map_type::iterator const& it)
    {
        return it->first;
    }
    inline attrib_base*
    get_value(attribute_cache::a_map_type::iterator const& it)
    {
        return it->second;
    }

    inline std::string const& 
    get_key(attribute_cache::a_map_type::const_iterator const& it)
    {
        return it->first;
    }
    inline attrib_base const*
    get_value(attribute_cache::a_map_type::const_iterator const& it)
    {
        return it->second;
    }
#endif

    bool attribute_cache::is_valid_key_locked(std::string const& key) const
    {
        if (extensible_)
          return true;      // all key's are allowed for extensible attribute

        // is key allowed for this object?
        return keynames_.find(key) != keynames_.end();
    }

    bool attribute_cache::is_valid_key(std::string const& key) const
    {
        mutex_type::scoped_lock lock(mtx_);
        return is_valid_key_locked(key);
    }

    void attribute_cache::init (bool extensible)
    {
        mutex_type::scoped_lock lock(mtx_);
        extensible_ = extensible;
    }

    void attribute_cache::init_attributes(strmap_type const& data, 
        attrib_base::attrib_type type, bool ro, bool ext)
    {
        strmap_type::const_iterator end = data.end();
        for (strmap_type::const_iterator it = data.begin(); it != end; ++it)
        {
// FIXME: do we really need to verify that all keys are valid at this point?
//             if (!is_valid_key_locked(it->first)) 
//             {
//                 SAGA_THROW("attribute '" + it->first + "' is not valid for "
//                     "this object", saga::DoesNotExist);
//             }

            if (it->second.empty()) {
                if (type == attrib_base::Scalar) {
                    a_map_.insert(it->first, new scalar_attrib(ro, ext));
                }
                else {
                    a_map_.insert(it->first, new vector_attrib(ro, ext));
                }
            }
            else {
                if (type == attrib_base::Scalar) {
                    a_map_.insert(it->first, new scalar_attrib(it->second, ro, ext));
                }
                else {
                    a_map_.insert(it->first, new vector_attrib(it->second, ro, ext));
                }
            }
        }
    }

    void attribute_cache::init (strmap_type const& ro_scalar, 
        strmap_type const& rw_scalar, strmap_type const& ro_vector, 
        strmap_type const& rw_vector)
    {
        mutex_type::scoped_lock lock(mtx_);

        init_attributes(ro_scalar, saga::impl::attrib_base::Scalar, true);
        init_attributes(rw_scalar, saga::impl::attrib_base::Scalar, false);
        init_attributes(ro_vector, saga::impl::attrib_base::Vector, true);
        init_attributes(rw_vector, saga::impl::attrib_base::Vector, false);

        inited_ = true;
    }

    void attribute_cache::init_attributes(char const* const* data, 
        attrib_base::attrib_type type, bool ro, bool ext)
    {
        if (NULL == data) 
            return;

        for (char const* const* d = data; NULL != *d; ++d)
        {
            std::string key(*d);
// FIXME: do we really need to verify that all keys are valid at this point?
//             if (!is_valid_key_locked(key)) 
//             {
//               SAGA_THROW("attribute '" + key + "' is not valid for "
//                 "this object", saga::DoesNotExist);
//             }
            if (type == attrib_base::Scalar) {
                a_map_.insert(key, new scalar_attrib(ro, ext));
            }
            else {
                a_map_.insert(key, new vector_attrib(ro, ext));
            }
        }
    }

    void attribute_cache::init (char const* const* ro_scalar, 
        char const* const* rw_scalar, char const* const* ro_vector, 
        char const* const* rw_vector)
    {
        mutex_type::scoped_lock lock(mtx_);

        init_attributes(ro_scalar, saga::impl::attrib_base::Scalar, true);
        init_attributes(rw_scalar, saga::impl::attrib_base::Scalar, false);
        init_attributes(ro_vector, saga::impl::attrib_base::Vector, true);
        init_attributes(rw_vector, saga::impl::attrib_base::Vector, false);

        inited_ = true;
    }

    void attribute_cache::init_keynames(strvec_type const& keynames)
    {
        mutex_type::scoped_lock lock(mtx_);

        if (!keynames_.empty())
            keynames_.clear();
        std::copy(keynames.begin(), keynames.end(), 
            std::inserter(keynames_, keynames_.end()));

        if (inited_ && !extensible_) {
            //  go over existing attributes and check for their validity
            a_map_type::const_iterator end = a_map_.end();
            for (a_map_type::const_iterator it = a_map_.begin(); it != end; ++it)
            {
                if (!is_valid_key_locked(get_key(it)))
                {
                    SAGA_THROW("attribute '" + get_key(it) + "' is not valid "
                      "for this object", saga::DoesNotExist);
                }
            }
        }
    }

    void attribute_cache::init_keynames(char const* const* keynames)
    {
        mutex_type::scoped_lock lock(mtx_);

        if (!keynames_.empty())
            keynames_.clear();
        if (NULL != keynames) {
            for (char const* const* k = keynames; NULL != *k; ++k)
            {
                keynames_.insert(keynames_.end(), *k);
            }
        }

        if (inited_ && !extensible_) {
            //  go over existing attributes and check for their validity
            a_map_type::const_iterator end = a_map_.end();
            for (a_map_type::const_iterator it = a_map_.begin(); it != end; ++it)
            {
                if (!is_valid_key_locked(get_key(it)))
                {
                    SAGA_THROW("attribute '" + get_key(it) + "' is not valid "
                      "for this object", saga::DoesNotExist);
                }
            }
        }
    }

    ///////////////////////////////////////////////////////////////////////////
    inline attribute_cache::a_map_type::const_iterator 
    attribute_cache::find_entry(std::string const& key) const
    {
        if (last_it_ != a_map_.end() && get_key(last_it_) == key)
            return last_it_;

        return last_it_ = a_map_.find(key);
    }

    inline attribute_cache::a_map_type::iterator 
    attribute_cache::find_entry(std::string const& key)
    {
        return a_map_.find(key);
    }

    ///////////////////////////////////////////////////////////////////////////
    void attribute_cache::set_attribute (std::string const& key,
        std::string const& val)
    {
        mutex_type::scoped_lock lock(mtx_);

        a_map_type::iterator ai = find_entry(key);
        if (a_map_.end() == ai)
        {
            // make sure the key is a valid one
            if (!is_valid_key_locked(key) && !extensible_) 
            {
                SAGA_THROW("attribute '" + key + "' does not exist, and cannot be "
                    "created (is not valid for this object)", saga::DoesNotExist);
            }

            // this cache needs to extensible
            BOOST_ASSERT(extensible_);

            // does not exist, and needs to be created
            a_map_.insert(key, new scalar_attrib(val, false, true));
        }
        else
        {
            // make sure this is a not vector attribute (as required by the latest spec)
            if (get_value(ai)->is_vec())
            {
                SAGA_THROW("attribute '" + key + "' is a vector attribute", 
                    saga::IncorrectState);
            }

            // exists and can be changed
            static_cast<scalar_attrib*>(get_value(ai))->val(val);
        }
    }

    /////////////////////////////////////////////////////////////////
    std::string attribute_cache::get_attribute (std::string const& key) const
    {
        mutex_type::scoped_lock lock(mtx_);

        a_map_type::const_iterator ai = find_entry(key);
        if (a_map_.end() == ai || !get_value(ai)->has_value())
        {
            // throw the correct error
            if (!is_valid_key_locked(key)) 
            {
                SAGA_THROW("attribute '" + key + "' is not valid for "
                  "this object", saga::DoesNotExist);
            }

            // DoesNotExist
            SAGA_THROW("attribute '" + key + "' does not exist", 
              saga::DoesNotExist);
        }

        // make sure this is not a vector attribute (as required by the latest spec)
        if (get_value(ai)->is_vec())
        {
            SAGA_THROW("attribute '" + key + "' is a vector attribute", 
                saga::IncorrectState);
        }

        // return val as scalar
        return static_cast<scalar_attrib const*>(get_value(ai))->val();
    }

    /////////////////////////////////////////////////////////////////
    void attribute_cache::set_vector_attribute (std::string const& key,
        strvec_type const& val)
    {
        mutex_type::scoped_lock lock(mtx_);

        a_map_type::iterator ai = find_entry(key);
        if (a_map_.end() == ai)
        {
            if (!is_valid_key_locked(key) && !extensible_) 
            {
                SAGA_THROW("attribute '" + key + "' does not exist, and cannot be "
                    "created (is not valid for this object)", saga::DoesNotExist);
            }

            BOOST_ASSERT(extensible_);

            // does not exist, and needs to be created
            a_map_.insert(key, new vector_attrib(val, false, true));
        }
        else
        {
            // make sure this is a vector attribute (as required by the latest spec)
            if (!get_value(ai)->is_vec())
            {
                SAGA_THROW("attribute '" + key + "' is not a vector attribute", 
                    saga::IncorrectState);
            }

            // exists and can be changed
            static_cast<vector_attrib*>(get_value(ai))->val (val);
        }
    }


    /////////////////////////////////////////////////////////////////
    // set/get vector attribute
    void attribute_cache::get_vector_attribute (std::string const& key,
        strvec_type& result) const
    {
        mutex_type::scoped_lock lock(mtx_);

        a_map_type::const_iterator ai = find_entry(key);
        if (a_map_.end() == ai || !get_value(ai)->has_value())
        {
            // throw the correct error
            if (!is_valid_key_locked(key)) 
            {
                SAGA_THROW("attribute '" + key + "' is not valid for "
                  "this object", saga::DoesNotExist);
            }

            // DoesNotExist
            SAGA_THROW("attribute '" + key + "' does not exist", 
                saga::DoesNotExist);
        }

        // make sure this is a vector attribute (as required by the latest spec)
        if (!get_value(ai)->is_vec())
        {
            SAGA_THROW("attribute '" + key + "' is not a vector attribute", 
                saga::IncorrectState);
        }

        // return val as vector
        result = static_cast<vector_attrib const*>(get_value(ai))->val();
    }

    /////////////////////////////////////////////////////////////////
    void attribute_cache::remove_attribute (const std::string & key)
    {
        mutex_type::scoped_lock lock(mtx_);

        a_map_type::iterator ai = find_entry(key);
        if (a_map_.end() == ai || !get_value(ai)->has_value())
        {
            // throw the correct error
            if (!is_valid_key_locked(key)) 
            {
                SAGA_THROW("attribute '" + key + "' is not valid for "
                  "this object", saga::DoesNotExist);
            }

            // DoesNotExist
            SAGA_THROW("attribute '" + key + "' does not exist", 
              saga::DoesNotExist);
        }

        // exists and can be removed:
        a_map_.erase (ai);
    }


    /////////////////////////////////////////////////////////////////
    // list attribute
#if BOOST_VERSION <= 103301
    void attribute_cache::list_attributes (strvec_type& result) const
    {
        mutex_type::scoped_lock lock(mtx_);
        result.reserve(a_map_.size());

        a_map_type::const_iterator end = a_map_.end();
        for (a_map_type::const_iterator it = a_map_.begin(); it != end; ++it)
        {
            if ((*it).has_value())
                result.push_back(it.key());
        }
    }

    void attribute_cache::find_attributes (std::string const &pattern,
        strvec_type& result) const
    {
        // FIXME: implement
        mutex_type::scoped_lock lock(mtx_);
        result.reserve(a_map_.size());

        a_map_type::const_iterator end = a_map_.end();
        for (a_map_type::const_iterator it = a_map_.begin(); it != end; ++it)
        {
            if ((*it).has_value())
                result.push_back(it.key());
        }
    }
#else
    struct string_appender
    {
        typedef attribute_cache::strvec_type strvec_type;

        string_appender(strvec_type& v) : v_(v) {}

        template <typename T>
        void operator()(T const& v) 
        { 
            if (v.second->has_value())
                v_.push_back(v.first); 
        }

        strvec_type& v_;
    };

    void attribute_cache::list_attributes (strvec_type& result) const
    {
        mutex_type::scoped_lock lock(mtx_);
        result.reserve(a_map_.size());
        std::copy(a_map_.begin(), a_map_.end(),
            boost::make_function_output_iterator(string_appender(result)));
    }

    /////////////////////////////////////////////////////////////////
    void attribute_cache::find_attributes (std::string const &pattern,
        strvec_type& result) const
    {
        // FIXME: implement
        mutex_type::scoped_lock lock(mtx_);
        result.reserve(a_map_.size());
        std::copy(a_map_.begin(), a_map_.end(),
            boost::make_function_output_iterator(string_appender(result)));
    }
#endif

    /////////////////////////////////////////////////////////////////
    bool attribute_cache::attribute_exists (std::string const& key) const
    {
        mutex_type::scoped_lock lock(mtx_);

//  FIXME: should we report an error for invalid keys here?      
//       if (!is_valid_key_locked(key)) 
//       {
//         SAGA_THROW("attribute '" + key + "' is not valid for "
//           "this object", saga::DoesNotExist);
//       }

        a_map_type::const_iterator ai = find_entry(key);
        return a_map_.end() != ai && get_value(ai)->has_value();
    }


    /////////////////////////////////////////////////////////////////
    bool attribute_cache::attribute_is_readonly (const std::string & key ) const
    {
        mutex_type::scoped_lock lock(mtx_);

        a_map_type::const_iterator ai = find_entry(key);
        if ( a_map_.end () == ai )
        {
            // throw the correct error
            if (!is_valid_key_locked(key)) 
            {
                SAGA_THROW("attribute '" + key + "' is not valid for "
                  "this object", saga::DoesNotExist);
            }

            // DoesNotExist
            SAGA_THROW("attribute '" + key + "' does not exist", 
              saga::DoesNotExist);
        }

        return get_value(ai)->is_ro();
    }


    /////////////////////////////////////////////////////////////////
    bool attribute_cache::attribute_is_writable (const std::string & key ) const
    {
        return ( ! attribute_is_readonly (key) );
    }


    /////////////////////////////////////////////////////////////////
    bool attribute_cache::attribute_is_vector (const std::string & key ) const
    {
        mutex_type::scoped_lock lock(mtx_);

        a_map_type::const_iterator ai = find_entry(key);
        if ( a_map_.end () == ai )
        {
            // throw the correct error
            if (!is_valid_key_locked(key)) 
            {
                SAGA_THROW("attribute '" + key + "' is not valid for "
                  "this object", saga::DoesNotExist);
            }

            // DoesNotExist
            SAGA_THROW("attribute '" + key + "' does not exist", 
              saga::DoesNotExist);
        }

        return get_value(ai)->is_vec ();
    }


    /////////////////////////////////////////////////////////////////
    bool attribute_cache::attribute_is_extended (const std::string & key ) const
    {
        mutex_type::scoped_lock lock(mtx_);

        a_map_type::const_iterator ai = find_entry(key);
        if ( a_map_.end () == ai )
        {
            // throw the correct error
            if (!is_valid_key_locked(key)) 
            {
                SAGA_THROW("attribute '" + key + "' is not valid for "
                  "this object", saga::DoesNotExist);
            }

            // DoesNotExist
            SAGA_THROW("attribute '" + key + "' does not exist", 
              saga::DoesNotExist);
        }

        return get_value(ai)->is_ext ();
    }

    /////////////////////////////////////////////////////////////////
    bool attribute_cache::attributes_extensible (void) const
    {
        mutex_type::scoped_lock lock(mtx_);
        return extensible_;
    }

    ///////////////////////////////////////////////////////////////////////////
    attribute_cache attribute_cache::clone() const
    {
        mutex_type::scoped_lock lock(mtx_);
        return *this;
    }

    bool attribute_cache::is_equal(attribute_cache const& rhs) const
    {
        mutex_type::scoped_lock lock(mtx_);
        return keynames_ == rhs.keynames_ && a_map_ == rhs.a_map_ && 
               extensible_ == rhs.extensible_;
    }

}}

