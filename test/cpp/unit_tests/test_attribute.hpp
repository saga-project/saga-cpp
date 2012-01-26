//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <algorithm>

#include <boost/test/unit_test.hpp>
#include <boost/assign.hpp>
#include <boost/assign/std/vector.hpp>

#include <saga/saga/adaptors/attribute.hpp>
#include "macros.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace test_attributes {

    ///////////////////////////////////////////////////////////////////////////
    inline bool
    compare_vector_attribute_values(saga::attribute::strvec_type& lhs,
        saga::attribute::strvec_type& rhs)
    {
        if (lhs.size() != rhs.size())
            return false;

        std::sort(lhs.begin(), lhs.end());
        std::sort(rhs.begin(), rhs.end());
        
        typedef saga::attribute::strvec_type::const_iterator const_iterator;
        const_iterator lhs_it = lhs.begin();
        const_iterator lhs_end = lhs.end();
        const_iterator rhs_it = rhs.begin();
        
        for (/**/; lhs_it != lhs_end; ++lhs_it, ++rhs_it)
            if (*lhs_it != *rhs_it)
                return false;
        
        return true;    // the sequences are equal
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename Target>
    inline void 
    check_ro_attrib (Target t, std::string key, std::string val, 
        bool must_exist = false)
    {
        SAGA_CHECK(!must_exist || t.attribute_exists(key));
        SAGA_CHECK(!t.attribute_exists(key) || t.attribute_is_readonly(key));
        SAGA_CHECK(!t.attribute_exists(key) || !t.attribute_is_writable(key));
        SAGA_CHECK(!t.attribute_exists(key) || !t.attribute_is_vector(key));

        // this should throw, because the attribute is readonly
        saga::error err = saga::IncorrectState;
        SAGA_REQUIRE_THROW(t.set_attribute(key, val), err);
        SAGA_CHECK_EQUAL(err, saga::IncorrectState);

        // this circumvents the write protection
        saga::adaptors::attribute(t).set_attribute(key, val);
        std::string out = t.get_attribute (key);
        SAGA_CHECK_EQUAL(val, out);

        err = saga::IncorrectState;
        saga::attribute::strvec_type result;
        SAGA_REQUIRE_THROW(result = t.get_vector_attribute(key), err);
        SAGA_CHECK_EQUAL(err, saga::IncorrectState);
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename Target>
    inline void 
    check_attrib (Target t, std::string key, std::string val, 
        bool must_exist = false)
    {
        using namespace boost::assign;

        SAGA_CHECK(!must_exist || t.attribute_exists(key));
        SAGA_CHECK(!t.attribute_exists(key) || !t.attribute_is_readonly(key));
        SAGA_CHECK(!t.attribute_exists(key) || t.attribute_is_writable(key));
        SAGA_CHECK(!t.attribute_exists(key) || !t.attribute_is_vector(key));

        saga::error err = saga::IncorrectState;
        saga::attribute::strvec_type v;
        v += val, val;
        SAGA_REQUIRE_THROW(t.set_vector_attribute(key, v), err);
        SAGA_CHECK_EQUAL(err, saga::IncorrectState);

        // set new value through API interface
        t.set_attribute (key, val);
        SAGA_CHECK_EQUAL(val, t.get_attribute (key));

        SAGA_CHECK(t.attribute_exists(key));
        SAGA_CHECK(!t.attribute_is_readonly(key));
        SAGA_CHECK(t.attribute_is_writable(key));
        SAGA_CHECK(!t.attribute_is_vector(key));

        // set new value through adaptor interface
        saga::adaptors::attribute(t).set_attribute(key, val);
        SAGA_CHECK_EQUAL(val, t.get_attribute (key));

        SAGA_CHECK(t.attribute_exists(key));
        SAGA_CHECK(!t.attribute_is_readonly(key));
        SAGA_CHECK(t.attribute_is_writable(key));
        SAGA_CHECK(!t.attribute_is_vector(key));

        err = saga::IncorrectState;
        saga::attribute::strvec_type result;
        SAGA_REQUIRE_THROW(result = t.get_vector_attribute(key), err);
        SAGA_CHECK_EQUAL(err, saga::IncorrectState);
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename Target>
    inline void 
    check_attrib_invalid (Target t, std::string key)
    {
        SAGA_CHECK(!t.attribute_exists(key));

        saga::error err = saga::DoesNotExist;
        SAGA_REQUIRE_THROW(t.attribute_is_readonly(key), err);
        SAGA_CHECK_EQUAL(err, saga::DoesNotExist);

        err = saga::DoesNotExist;
        SAGA_REQUIRE_THROW(t.attribute_is_writable(key), err);
        SAGA_CHECK_EQUAL(err, saga::DoesNotExist);

        err = saga::DoesNotExist;
        SAGA_REQUIRE_THROW(t.attribute_is_vector(key), err);
        SAGA_CHECK_EQUAL(err, saga::DoesNotExist);

        err = saga::DoesNotExist;
        SAGA_REQUIRE_THROW(t.get_attribute (key), err);
        SAGA_CHECK_EQUAL(err, saga::DoesNotExist);

        err = saga::DoesNotExist;
        SAGA_REQUIRE_THROW(t.set_attribute (key, ""), err);
        SAGA_CHECK_EQUAL(err, saga::DoesNotExist);

        err = saga::DoesNotExist;
        SAGA_REQUIRE_THROW(t.get_vector_attribute (key), err);
        SAGA_CHECK_EQUAL(err, saga::DoesNotExist);

        err = saga::DoesNotExist;
        SAGA_REQUIRE_THROW(t.set_vector_attribute (key, std::vector<std::string>()), err);
        SAGA_CHECK_EQUAL(err, saga::DoesNotExist);
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename Target>
    inline void 
    check_attrib_vector (Target t, std::string key, std::string val1, 
        std::string val2, bool must_exist = false)
    {
        using namespace boost::assign;

        SAGA_CHECK(!must_exist || t.attribute_exists(key));
        SAGA_CHECK(!t.attribute_exists(key) || !t.attribute_is_readonly(key));
        SAGA_CHECK(!t.attribute_exists(key) || t.attribute_is_writable(key));
        SAGA_CHECK(!t.attribute_exists(key) || t.attribute_is_vector(key));

        saga::error err = saga::IncorrectState;
        SAGA_REQUIRE_THROW(t.set_attribute(key, val1), err);
        SAGA_CHECK_EQUAL(err, saga::IncorrectState);

        saga::attribute::strvec_type v;
        v += val1, val2;

        t.set_vector_attribute (key, v);
        saga::attribute::strvec_type out = t.get_vector_attribute (key);

        SAGA_CHECK(t.attribute_exists(key));
        SAGA_CHECK(!t.attribute_is_readonly(key));
        SAGA_CHECK(t.attribute_is_writable(key));
        SAGA_CHECK(t.attribute_is_vector(key));
        SAGA_CHECK(compare_vector_attribute_values(v, out));

        err = saga::IncorrectState;
        std::string result;
        SAGA_REQUIRE_THROW(result = t.get_attribute(key), err);
        SAGA_CHECK_EQUAL(err, saga::IncorrectState);
    }

///////////////////////////////////////////////////////////////////////////////
}   // namespace test_attributes
