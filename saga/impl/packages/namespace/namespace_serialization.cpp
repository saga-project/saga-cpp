//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga/util.hpp>
#include <saga/saga/error.hpp>
#include <saga/saga/adaptors/instance_data.hpp>
#include <saga/saga/adaptors/packages/namespace_entry_cpi_instance_data.hpp>
#include <saga/saga/adaptors/packages/namespace_dir_cpi_instance_data.hpp>

#include <saga/saga/packages/namespace/version.hpp>
#include <saga/impl/packages/namespace/namespace_serialization.hpp>

#include <saga/impl/packages/namespace/namespace_entry.hpp>
#include <saga/impl/packages/namespace/namespace_dir.hpp>

#include <boost/serialization/serialization.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

///////////////////////////////////////////////////////////////////////////////
// Register the serialization factory type
SAGA_PACKAGE_REGISTER(saga::impl::namespace_serialization);

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl
{

    namespace_serialization::~namespace_serialization()
    {
    }

    std::string 
    namespace_serialization::serialize(TR1::shared_ptr<saga::impl::object> obj)
    {
        // get the type of the object (has been serialized already)
        saga::object::type type = obj->get_type();
        if (saga::object::NSEntry != type && saga::object::NSDirectory != type) 
        {
            SAGA_THROW_VERBATIM(obj.get(), "namespace_serialization::serialize: " 
                "Unknwon object type", saga::BadParameter);
        }

        SAGA_OSSTREAM strm;
        {
            using namespace boost::serialization;
            using namespace boost::archive;

            // the object actually is a impl::namespace_entry or 
            // impl::namespace_dir, so the following cast should succeed
            saga::impl::proxy *proxy = dynamic_cast<saga::impl::proxy *>(obj.get());
            if (NULL == proxy)
            {
                SAGA_THROW_VERBATIM(obj.get(), "namespace_serialization::serialize: " 
                    "Unknwon object type", saga::BadParameter);
            }
            
            // save package version
            text_oarchive oa(strm);
            unsigned int version = SAGA_VERSION_PACKAGE_NAMESPACE_FULL;
            oa << make_nvp("version", version);
            
            if (saga::object::NSEntry == type) {
            // serialize the namespace_entry_cpi_instance data
                typedef saga::adaptors::v1_0::namespace_entry_cpi_instance_data 
                    instance_data;
                    
            saga::adaptors::instance_data<instance_data> data(proxy);
            std::string url (data->location_.get_url());
            
                oa << make_nvp("location", url);
                oa << make_nvp("mode", data->mode_);
            }
            else {
            // saga::object::NSDirectory == type: serialize the 
            // namespace_dir_cpi_instance data
                typedef saga::adaptors::v1_0::namespace_dir_cpi_instance_data 
                    instance_data;

            saga::adaptors::instance_data<instance_data> data(proxy);
            std::string url;
            
                oa << make_nvp("location", url);
                data->location_.set_url(url);
                oa << make_nvp("mode", data->mode_);
            }
        }
        return SAGA_OSSTREAM_GETSTRING(strm);
    }

    TR1::shared_ptr<saga::impl::object> 
    namespace_serialization::deserialize(saga::session s, saga::object::type type,
        std::string const& data)
    {
        if (saga::object::NSEntry != type && saga::object::NSDirectory != type) 
        {
            SAGA_THROW_VERBATIM(s, "namespace_serialization::deserialize: " 
                "Unknwon object type", saga::BadParameter);
        }

        using namespace boost::serialization;
        using namespace boost::archive;

        SAGA_ISSTREAM strm(data);
        text_iarchive ia(strm);

        // save package version
        unsigned int version = 0;
        ia >> make_nvp("version", version);
        if (!SAGA_VERSION_PACKAGE_NAMESPACE_ISCOMPATIBLE_EX(version)) 
        {
            SAGA_THROW_VERBATIM(s, "namespace_serialization::deserialize: "
                "Deserialization failed: incompatible version of SAGA namespace "
                "package module.", saga::NoSuccess);
        }

        TR1::shared_ptr<saga::impl::object> impl;
        if (saga::object::NSEntry == type) {
        // deserialize the namespace_entry_cpi_instance data
        std::string location;
        int mode = 0;

            ia >> make_nvp("location", location);
            ia >> make_nvp("mode", mode);
            impl.reset(new saga::impl::namespace_entry(s, location, mode, 
                saga::object::NSEntry));
        }
        else {
        // saga::object::NSDirectory == type: deserialize the 
        // namespace_dir_cpi_instance data
        std::string location;
        int mode = 0;    
        
            ia >> make_nvp("location", location);
            ia >> make_nvp("mode", mode);
            impl.reset(new saga::impl::namespace_dir(s, location, mode, 
                saga::object::NSDirectory));
        }

        return impl;
    }
    
///////////////////////////////////////////////////////////////////////////////
}}    // namespace saga::impl
