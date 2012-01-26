//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/serialization/serialization.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <saga/saga/error.hpp>
#include <saga/saga/adaptors/instance_data.hpp>
#include <saga/saga/adaptors/packages/stream_cpi_instance_data.hpp>
#include <saga/saga/adaptors/packages/stream_server_cpi_instance_data.hpp>

#include <saga/saga/packages/stream/version.hpp>
#include <saga/impl/packages/stream/stream_serialization.hpp>

#include <saga/impl/packages/stream/stream.hpp>
#include <saga/impl/packages/stream/stream_server.hpp>

///////////////////////////////////////////////////////////////////////////////
// Register the serialization factory type
SAGA_PACKAGE_REGISTER(saga::impl::stream_serialization);

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl
{

    stream_serialization::~stream_serialization()
    {
    }

    std::string 
    stream_serialization::serialize(TR1::shared_ptr<saga::impl::object> obj)
    {
        // get the type of the object (has been serialized already)
        saga::object::type type = obj->get_type();
        if (saga::object::Stream != type && 
            saga::object::StreamServer != type) 
        {
            SAGA_THROW_VERBATIM(obj.get(), "stream_serialization::serialize: " 
                "Unknwon object type", saga::BadParameter);
        }

        SAGA_OSSTREAM strm;
        {
            using namespace boost::serialization;
            using namespace boost::archive;

            // the object actually is a impl::file or impl::directory, so the
            // following cast should succeed
            saga::impl::proxy *proxy = dynamic_cast<saga::impl::proxy *>(obj.get());
            if (NULL == proxy)
            {
                SAGA_THROW_VERBATIM(obj.get(), "stream_serialization::serialize: " 
                    "Unknwon object type", saga::BadParameter);
            }
            
            // save package version
            text_oarchive oa(strm);
            unsigned int version = SAGA_VERSION_PACKAGE_STREAM_FULL;
            oa << make_nvp("version", version);
            
            if (saga::object::Stream == type) {
            // serialize the stream_cpi_instance data
                typedef saga::adaptors::v1_0::stream_cpi_instance_data 
                    instance_data;
                    
            saga::adaptors::instance_data<instance_data> data(proxy);
            std::string url(data->location_.get_url());
            
                oa << make_nvp("location", url);
            }
            else {
            // saga::object::StreamServer == type: serialize the 
            // server_cpi_instance data
                typedef saga::adaptors::v1_0::stream_server_cpi_instance_data 
                    instance_data;

            saga::adaptors::instance_data<instance_data> data(proxy);
            std::string url(data->location_.get_url());
            
                oa << make_nvp("location", url);
            }
        }
        return SAGA_OSSTREAM_GETSTRING(strm);
    }

    TR1::shared_ptr<saga::impl::object> 
    stream_serialization::deserialize(saga::session s, saga::object::type type,
        std::string const& data)
    {
        if (saga::object::Stream != type && 
            saga::object::StreamServer != type) 
        {
            SAGA_THROW_VERBATIM(s, "stream_serialization::deserialize: " 
                "Unknwon object type", saga::BadParameter);
        }

        using namespace boost::serialization;
        using namespace boost::archive;

        SAGA_ISSTREAM strm(data);
        text_iarchive ia(strm);

        // save package version
        unsigned int version = 0;
        ia >> make_nvp("version", version);
        if (!SAGA_VERSION_PACKAGE_STREAM_ISCOMPATIBLE_EX(version)) 
        {
            SAGA_THROW_VERBATIM(s, "stream_serialization::deserialize: "
                "Deserialization failed: incompatible version of SAGA stream "
                "package module.", saga::NoSuccess);
        }

        TR1::shared_ptr<saga::impl::object> impl;
        if (saga::object::Stream == type) {
        // deserialize the stream_cpi_instance data
        std::string location;

            ia >> make_nvp("location", location);
            impl.reset(new saga::impl::stream(s, location));
        }
        else {
            // saga::object::StreamServer == type: deserialize the 
            // server_cpi_instance data
            std::string location;
            // int mode = 0;    
        
            ia >> make_nvp("location", location);
            impl.reset(new saga::impl::server(s, location));
        }

        return impl;
    }
    
///////////////////////////////////////////////////////////////////////////////
}}    // namespace saga::impl
