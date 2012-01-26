//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_CONTEXT_SERIALIZATION_HPP
#define SAGA_CONTEXT_SERIALIZATION_HPP

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <saga/saga/context.hpp>

namespace boost { namespace serialization
{
    template<class Archive>
    void load(Archive& ia, saga::context& ctx, const unsigned int version)
    {
        using namespace boost::serialization;

        bool has_type = false;
        ia >> make_nvp("has_type", has_type);
        if (has_type) {
            std::string type;
            ia >> make_nvp("type", type);
            ctx.set_attribute(saga::attributes::context_type, type);
        }

        std::size_t count = 0;
        ia >> make_nvp("attribute_count", count);
        for (std::size_t i = 0; i < count; ++i)
        {
            bool is_vector_attribute = false;
            ia >> make_nvp("is_vector", is_vector_attribute);
            if (is_vector_attribute) {
                std::string key;
                std::vector<std::string> values;

                ia >> make_nvp("key", key);
                ia >> make_nvp("values", values);
                ctx.set_vector_attribute(key, values);
            }
            else {
                std::string key, value;

                ia >> make_nvp("key", key);
                ia >> make_nvp("value", value);
                ctx.set_attribute(key, value);
            }
        }
    }

    template<class Archive>
    void save(Archive& oa, saga::context const& ctx, const unsigned int version)
    {
        using namespace boost::serialization;

        bool has_type = ctx.attribute_exists(saga::attributes::context_type);
        oa << make_nvp("has_type", has_type);
        if (has_type) {
            std::string type = ctx.get_attribute(saga::attributes::context_type);
            oa << make_nvp("type", type);
        }

        std::vector<std::string> keys (ctx.list_attributes());
        std::size_t count = keys.size();

        oa << make_nvp("attribute_count", count);
        std::vector<std::string>::iterator end = keys.end();
        for (std::vector<std::string>::iterator it = keys.begin(); it != end; 
             ++it)
        {
            bool is_vector_attribute = ctx.attribute_is_vector(*it);

            oa << make_nvp("is_vector", is_vector_attribute);
            if (is_vector_attribute) {
                std::vector<std::string> values (ctx.get_vector_attribute(*it));
                oa << make_nvp("key", *it);
                oa << make_nvp("values", values);
            }
            else {
                std::string value (ctx.get_attribute(*it));
                oa << make_nvp("key", *it);
                oa << make_nvp("value", value);
            }
        }
    }

}}

BOOST_SERIALIZATION_SPLIT_FREE(saga::context);

#endif 

