//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/serialization/serialization.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <saga/saga/error.hpp>
#include <saga/saga/adaptors/instance_data.hpp>
#include <saga/saga/adaptors/packages/logical_file_cpi_instance_data.hpp>
#include <saga/saga/adaptors/packages/logical_directory_cpi_instance_data.hpp>

#include <saga/saga/packages/replica/version.hpp>
#include <saga/impl/packages/replica/replica_serialization.hpp>

#include <saga/impl/packages/replica/logical_file.hpp>
#include <saga/impl/packages/replica/logical_directory.hpp>

///////////////////////////////////////////////////////////////////////////////
// Register the serialization factory type
SAGA_PACKAGE_REGISTER(saga::impl::replica_serialization);

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl
{

    replica_serialization::~replica_serialization()
    {
    }

    std::string 
    replica_serialization::serialize(TR1::shared_ptr<saga::impl::object> obj)
    {
        // get the type of the object (has been serialized already)
        saga::object::type type = obj->get_type();
        if (saga::object::LogicalFile != type && 
            saga::object::LogicalDirectory != type) 
        {
            SAGA_THROW_VERBATIM(obj.get(), "replica_serialization::serialize: " 
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
                SAGA_THROW_VERBATIM(obj.get(), "replica_serialization::serialize: " 
                    "Unknwon object type", saga::BadParameter);
            }
            
            // save package version
            text_oarchive oa(strm);
            unsigned int version = SAGA_VERSION_PACKAGE_REPLICA_FULL;
            oa << make_nvp("version", version);
            
            if (saga::object::LogicalFile == type) {
            // serialize the replica_cpi_instance data
                typedef saga::adaptors::v1_0::logical_file_cpi_instance_data 
                    instance_data;
                    
            saga::adaptors::instance_data<instance_data> data(proxy);
            std::string url (data->location_.get_url());
            
                oa << make_nvp("location", url);
                oa << make_nvp("mode", data->mode_);
            }
            else {
            // saga::object::LogicalDirectory == type: serialize the 
            // logical_directory_cpi_instance data
                typedef saga::adaptors::v1_0::logical_directory_cpi_instance_data 
                    instance_data;

            saga::adaptors::instance_data<instance_data> data(proxy);
            std::string url (data->location_.get_url());
            
                oa << make_nvp("location", url);
                oa << make_nvp("mode", data->mode_);
            }
        }
        return SAGA_OSSTREAM_GETSTRING(strm);
    }

    TR1::shared_ptr<saga::impl::object> 
    replica_serialization::deserialize(saga::session s, saga::object::type type,
        std::string const& data)
    {
        if (saga::object::LogicalFile != type && 
            saga::object::LogicalDirectory != type) 
        {
            SAGA_THROW_VERBATIM(s, "replica_serialization::deserialize: " 
                "Unknwon object type", saga::BadParameter);
        }

        using namespace boost::serialization;
        using namespace boost::archive;

        SAGA_ISSTREAM strm(data);
        text_iarchive ia(strm);

        // restore package version
        unsigned int version = 0;
        ia >> make_nvp("version", version);
        if (!SAGA_VERSION_PACKAGE_REPLICA_ISCOMPATIBLE_EX(version)) 
        {
            SAGA_THROW_VERBATIM(s, "replica_serialization::deserialize: "
                "Deserialization failed: incompatible version of SAGA replica "
                "package module.", saga::NoSuccess);
        }

        // restore instance data
        std::string location;
        int mode = 0;    
        ia >> make_nvp("location", location);
        ia >> make_nvp("mode", mode);
        
        TR1::shared_ptr<saga::impl::object> impl;
        if (saga::object::LogicalFile == type) {
        // deserialize the logicalfile object
            impl.reset(new saga::impl::logical_file(s, location, mode));
        }
        else {
        // saga::object::LogicalDirectory == type: deserialize the 
        // logical_directory object
            impl.reset(new saga::impl::logical_directory(s, location, mode));
        }

        return impl;
    }
    
///////////////////////////////////////////////////////////////////////////////
}}    // namespace saga::impl
