//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/plugin/dll.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/shared_ptr.hpp>

#include <saga/saga/util.hpp>
#include <saga/impl/runtime.hpp>
#include <saga/saga/version.hpp>

#include <saga/impl/engine/context_base.hpp>
#include <saga/impl/context_serialization.hpp>
#include <saga/impl/engine/session.hpp>
#include <saga/saga/adaptors/serialization.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace adaptors
{

// retrieve the name of the package containing for the implementation of the
// given SAGA object type
inline std::string 
get_package_name(saga::object obj, saga::object::type type)
{
// The sequence of package names in this array should be equivalent to the 
// definition sequence of the corresponding saga object types 
// (see saga/object.hpp)
    struct modulename
    {
        char const* const package_name;
        bool is_package;
    }; 

    static modulename package_names[] = 
    {
        { "core", false },          // Exception        =   1,
        { "core", false },          // URL              =   2,
        { "core", false },          // Buffer           =   3,
        { "core", false },          // Session          =   4,
        { "core", false },          // Context          =   5,
        { "core", false },          // Task             =   6,
        { "core", false },          // TaskContainer    =   7,
        { "core", false },          // Metric           =   8,
        { "namespace", true },      // NSEntry          =   9,
        { "namespace", true },      // NSDirectory      =  10,
        { "filesystem", true },     // IOVec            =  11,
        { "filesystem", true },     // File             =  12,
        { "filesystem", true },     // Directory        =  13,
        { "replica", true },        // LogicalFile      =  14,
        { "replica", true },        // LogicalDirectory =  15,
        { "job", true },            // JobDescription   =  16,
        { "job", true },            // JobService       =  17,
        { "job", true },            // Job              =  18,
        { "job", true },            // JobSelf          =  19,
        { "stream", true },         // StreamServer     =  20,
        { "stream", true },         // Stream           =  21,
        { "rpc", true },            // Parameter        =  22,
        { "rpc", true },            // RPC              =  23,

        // SAGA Message API extension
        { "message", true },        // Msg              =  24,
        { "message", true },        // Endpoint         =  25,

        // SAGA Information services extension
        { "advert", true },         // Advert           =  26,
        { "advert", true },         // AdvertDirectory  =  27,

        // SAGA Service Discovery extension
        { "sd", true },             // ServiceDescription =  28,
        { "sd", true },             // ServiceDiscoverer =  29,
        { "sd", true },             // ServiceData       =  30,

        // SAGA Checkpoint and Recovery extension
        { "cpr", true },            // CPRJobDescription = 31,
        { "cpr", true },            // CPRJobService     = 32,
        { "cpr", true },            // CPRJob            = 33,
        { "cpr", true },            // CPRJobSelf        = 34,
        { "cpr", true },            // CPRCheckpoint     = 35,
        { "cpr", true },            // CPRDirectory      = 36,

      // SAGA Information System Navigator extension
        { "isn", true },            // EntityData         = 37,
        { "isn", true },            // EntityDataSet      = 38,
        { "isn", true },            // EntityNavigator    = 39
    };

    if (type < saga::object::Exception || type > saga::object::ServiceData)
    {
        SAGA_THROW_VERBATIM(obj, "Attempt to retrieve package name from "
            "unknown SAGA object type.", saga::BadParameter);
    }

    std::string package(package_names[type-1].package_name);
    return package_names[type-1].is_package ?
        SAGA_MANGLE_PACKAGE_NAME_STR(package) :
        SAGA_MANGLE_ENGINE_NAME_STR(package);
}

// serialize a saga::object into a string representation
std::string serialize(saga::object obj)
{
    using namespace boost::serialization;
    using namespace boost::archive;

    SAGA_OSSTREAM strm;
    try {
        text_oarchive oa(strm);

        // save the version of the SAGA engine
        unsigned int version = SAGA_VERSION_FULL;
        oa << make_nvp("saga_version", version);

        // save the object type
        saga::object::type type = obj.get_type();
        oa << make_nvp("type", type);

        saga::uuid objid (obj.get_id());
        const std::string str(objid.string());
        oa << make_nvp("id", str);

        // save the object implementation
        TR1::shared_ptr<saga::impl::object> impl(
            saga::impl::runtime::get_impl_object_sp(obj));

        // load the corresponding package and delegate the serialization
        std::string package (get_package_name (obj, type));
        package += SAGA_SHARED_LIB_EXTENSION;

        boost::plugin::dll d (package);
        boost::plugin::plugin_factory <saga::adaptors::serialization> pf (d);

        TR1::shared_ptr <saga::adaptors::serialization> ser (pf.create ("serialization")); 

        // serialize the object
        std::string data (ser->serialize(impl));

        oa << make_nvp ("object", data);
    }
    catch (archive_exception const& e) {
        SAGA_THROW_VERBATIM(obj, 
            std::string("Serialization failed (archive exception): ") + 
                e.what(), saga::NoSuccess);
    }
    catch (std::logic_error const& e) {
        SAGA_THROW_VERBATIM(obj, std::string("Serialization failed: ") + 
            e.what(), saga::NoSuccess);
    }
    return SAGA_OSSTREAM_GETSTRING(strm);
}

// create a saga::object from a given string representation
saga::object deserialize(std::string const& data)
{
    return deserialize(saga::detail::get_the_session(), data);
}

saga::object deserialize(saga::session s, std::string const& data)
{
    using namespace boost::serialization;
    using namespace boost::archive;

    TR1::shared_ptr<saga::impl::object> impl;
    std::string objid;

    try {
        SAGA_ISSTREAM strm(data);
        text_iarchive ia(strm);

        // retrieve the version of the SAGA engine and make sure it is compatible
        // with our current version
        unsigned int version = 0;
        ia >> make_nvp("saga_version", version);  // load version
        if (!SAGA_VERSION_ISCOMPATIBLE_EX(version)) 
        {
            SAGA_THROW_VERBATIM(s, "Deserialization failed: "
                "incompatible version of SAGA modules.", saga::NoSuccess);
        }

        // retrieve the object type of the stored object
        saga::object::type type;
        ia >> make_nvp("type", type);
        ia >> make_nvp("id", objid);

        // load the package corresponding to the object type
        std::string package (get_package_name (s, type));
        package += SAGA_SHARED_LIB_EXTENSION;

        boost::plugin::dll d(package);
        boost::plugin::plugin_factory <saga::adaptors::serialization> pf (d);
        TR1::shared_ptr <saga::adaptors::serialization> ser (pf.create("serialization")); 

        // deserialize the given string
        std::string data;
        ia >> make_nvp("object", data);
        impl = ser->deserialize(s, type, data);
    }
    catch (archive_exception const& e) {
        SAGA_THROW_VERBATIM(s, 
            std::string("Deserialization failed (archive exception): ") + 
                e.what(), saga::NoSuccess);
    }
    catch (std::logic_error const& e) {
        SAGA_THROW_VERBATIM(s, std::string("Deserialization failed: ") + 
            e.what(), saga::NoSuccess);
    }

    // set the objid
    impl->set_uuid(saga::uuid(objid.c_str()));
    return saga::impl::runtime::get_object(impl);
}

///////////////////////////////////////////////////////////////////////////////
}}    // namespace saga::adaptors

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl 
{
    // serialization support for the engine
    engine_serialization::~engine_serialization()
    {
    }

    std::string engine_serialization::serialize(TR1::shared_ptr<saga::impl::object> obj)
    {
        // get the type of the object (has been serialized already)
        saga::object::type type = obj->get_type();
        if (saga::object::Session != type && saga::object::Context != type) 
        {
            SAGA_THROW_VERBATIM(obj.get(), "engine_serialization::serialize: " 
                "Unknown object type", saga::BadParameter);
        }

        SAGA_OSSTREAM strm;
        {
            using namespace boost::serialization;
            using namespace boost::archive;

            // save package version
            text_oarchive oa(strm);
            unsigned int version = SAGA_VERSION_FULL;
            oa << make_nvp("version", version);

            if (saga::object::Session == type) {
                // serialize the session data
                TR1::shared_ptr<saga::impl::session> s(
                    TR1::static_pointer_cast<saga::impl::session>(obj));
                saga::impl::session* sp = s.get();
                oa << make_nvp("session", sp);
            }
            else {
            // saga::object::Context == type: serialize the context data
                saga::context ctx = saga::impl::runtime::get_object(
                    TR1::static_pointer_cast<saga::impl::context>(obj));
                oa << make_nvp("context", ctx);
            }
        }
        return SAGA_OSSTREAM_GETSTRING(strm);
    }

    TR1::shared_ptr<saga::impl::object> engine_serialization::deserialize(
        saga::session s, saga::object::type type, std::string const& data)
    {
        if (saga::object::Session != type && saga::object::Context != type) 
        {
            SAGA_THROW_VERBATIM(s, "engine_serialization::deserialize: " 
                "Unknown object type", saga::BadParameter);
        }

        using namespace boost::serialization;
        using namespace boost::archive;

        SAGA_ISSTREAM strm(data);
        text_iarchive ia(strm);

        // save package version
        unsigned int version = 0;
        ia >> make_nvp("version", version);
        if (!SAGA_VERSION_ISCOMPATIBLE_EX(version)) 
        {
            SAGA_THROW_VERBATIM(s, "engine_serialization::deserialize: "
                "Deserialization failed: incompatible version of SAGA engine "
                "module.", saga::NoSuccess);
        }

        TR1::shared_ptr<saga::impl::object> impl;
        if (saga::object::Session == type) {
            // deserialize the session data
            saga::impl::session* s = 0;
            ia >> make_nvp("session", s);
            impl.reset(s);
        }
        else {
            // saga::object::Context == type: deserialize the context data
            saga::context ctx;
            ia >> make_nvp("context", ctx);
            impl = saga::impl::runtime::get_impl_sp(ctx);
        }
        return impl;
    }

///////////////////////////////////////////////////////////////////////////////
}}    // namespace saga::impl

