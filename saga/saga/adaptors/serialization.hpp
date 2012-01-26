//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_SAGA_ADAPTORS_SERIALIZATION_HPP
#define SAGA_SAGA_ADAPTORS_SERIALIZATION_HPP

#include <boost/plugin.hpp>
#include <boost/plugin/export_plugin.hpp>

#include <saga/saga/util.hpp>
#include <saga/saga/object.hpp>

///////////////////////////////////////////////////////////////////////////////
#ifndef SAGA_PACKAGE_NAME
# ifndef SAGA_ENGINE
#   define SAGA_PACKAGE_NAME engine   // needed for get_name()
# endif
#endif

///////////////////////////////////////////////////////////////////////////////
#ifndef SAGA_PACKAGE_LIB_NAME
// the serialization functions are defined in the cpi part of the package
#define SAGA_PACKAGE_LIB_NAME SAGA_MANGLE_PACKAGE_NAME(SAGA_PACKAGE_NAME)
#endif

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace adaptors
{
    // serialize a saga::object into a string representation
    SAGA_EXPORT std::string serialize(saga::object obj);
    
    // create a saga::object from a given string representation
    SAGA_EXPORT saga::object deserialize(std::string const& data);
    SAGA_EXPORT saga::object deserialize(saga::session s, std::string const& data);
    
    // base class for all the package serialization functionality
    struct serialization
    {
        virtual ~serialization() {}
        virtual std::string serialize(TR1::shared_ptr<saga::impl::object>) = 0;
        virtual TR1::shared_ptr<saga::impl::object> deserialize(
            saga::session s, saga::object::type type, 
            std::string const& data) = 0;
    };
    
///////////////////////////////////////////////////////////////////////////////
}}  // namespace saga::adaptors

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl 
{
    // serialization support for the engine
    class engine_serialization
    :   public saga::adaptors::serialization
    {
    public:
        ~engine_serialization();
        std::string serialize(TR1::shared_ptr<saga::impl::object>);
        TR1::shared_ptr<saga::impl::object> deserialize(
            saga::session s, saga::object::type type, std::string const& data);
    };

///////////////////////////////////////////////////////////////////////////////
}}    // namespace saga::impl

///////////////////////////////////////////////////////////////////////////////
//  Forward the SAGA_PACKAGE_REGISTER macro to the correct BOOST_PLUGIN macro 
//  sequence
#define SAGA_PACKAGE_REGISTER(ActualType)                                     \
        BOOST_PLUGIN_EXPORT_LIST (SAGA_PACKAGE_LIB_NAME)                      \
        BOOST_PLUGIN_EXPORT      (SAGA_PACKAGE_LIB_NAME,                      \
        saga::adaptors::serialization, ActualType, "serialization")           \
    /**/

#endif // !SAGA_SAGA_ADAPTORS_SERIALIZATION_HPP


