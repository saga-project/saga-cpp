//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/serialization/serialization.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <saga/saga/error.hpp>
#include <saga/saga/adaptors/instance_data.hpp>
#include <saga/saga/adaptors/packages/rpc_cpi_instance_data.hpp>

#include <saga/saga/packages/rpc/version.hpp>
#include <saga/impl/packages/rpc/rpc_serialization.hpp>

#include <saga/impl/packages/rpc/rpc.hpp>

///////////////////////////////////////////////////////////////////////////////
// Register the serialization factory type
SAGA_PACKAGE_REGISTER(saga::impl::rpc_serialization);

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl
{

    rpc_serialization::~rpc_serialization()
    {
    }

    std::string 
    rpc_serialization::serialize(TR1::shared_ptr<saga::impl::object> obj)
    {
        // get the type of the object (has been serialized already)
        saga::object::type type = obj->get_type();
        if (saga::object::RPC != type) 
        {
            SAGA_THROW_VERBATIM(obj.get(), "rpc_serialization::serialize: " 
                "Unknwon object type", saga::BadParameter);
        }

        SAGA_OSSTREAM strm;
        {
            using namespace boost::serialization;
            using namespace boost::archive;

            // the object actually is a impl::rpc_entry or 
            // impl::rpc_dir, so the following cast should succeed
            saga::impl::proxy *proxy = dynamic_cast<saga::impl::proxy *>(obj.get());
            if (NULL == proxy)
            {
                SAGA_THROW_VERBATIM(obj.get(), "rpc_serialization::serialize: " 
                    "Unknwon object type", saga::BadParameter);
            }
            
            // save package version
            text_oarchive oa(strm);
            unsigned int version = SAGA_VERSION_PACKAGE_RPC_FULL;
            oa << make_nvp("version", version);
            
        // serialize the rpc_entry_cpi_instance data
            typedef saga::adaptors::v1_0::rpc_cpi_instance_data 
                instance_data;
                
        saga::adaptors::instance_data<instance_data> data(proxy);
        std::string url(data->funcname_.get_url());
        
            oa << make_nvp("name", url);
        }
        return SAGA_OSSTREAM_GETSTRING(strm);
    }

    TR1::shared_ptr<saga::impl::object> 
    rpc_serialization::deserialize(saga::session s, saga::object::type type,
        std::string const& data)
    {
        if (saga::object::RPC != type) 
        {
            SAGA_THROW_VERBATIM(s, "rpc_serialization::deserialize: " 
                "Unknwon object type", saga::BadParameter);
        }

        using namespace boost::serialization;
        using namespace boost::archive;

        SAGA_ISSTREAM strm(data);
        text_iarchive ia(strm);

        // save package version
        unsigned int version = 0;
        ia >> make_nvp("version", version);
        if (!SAGA_VERSION_PACKAGE_RPC_ISCOMPATIBLE_EX(version)) 
        {
            SAGA_THROW_VERBATIM(s, "rpc_serialization::deserialize: "
                "Deserialization failed: incompatible version of SAGA rpc "
                "package module.", saga::NoSuccess);
        }

    // deserialize the rpc_entry_cpi_instance data
        TR1::shared_ptr<saga::impl::object> impl;
        std::string name;

        ia >> make_nvp("name", name);
        impl.reset(new saga::impl::rpc(s, name));

        return impl;
    }
    
///////////////////////////////////////////////////////////////////////////////
}}    // namespace saga::impl
