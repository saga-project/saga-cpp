//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <vector>
#include <string>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>

#include <saga/saga/error.hpp>
#include <saga/saga/adaptors/instance_data.hpp>
#include <saga/saga/adaptors/attribute.hpp>
#include <saga/saga/adaptors/packages/cpr_job_cpi_instance_data.hpp>
#include <saga/saga/adaptors/packages/cpr_job_service_cpi_instance_data.hpp>

#include <saga/saga/packages/cpr/version.hpp>
#include <saga/impl/packages/cpr/cpr_job_serialization.hpp>

#include <saga/impl/packages/cpr/cpr_job.hpp>
#include <saga/impl/packages/cpr/cpr_job_service.hpp>
#include <saga/impl/packages/job/job_description.hpp>

///////////////////////////////////////////////////////////////////////////////
// Register the serialization factory type
SAGA_PACKAGE_REGISTER(saga::impl::cpr_job_serialization);

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl
{

    cpr_job_serialization::~cpr_job_serialization()
    {
    }

    void 
    serialize_cpr_jd(boost::archive::text_oarchive& oa, saga::cpr::description jd)
    {
        using namespace boost::serialization;

        saga::adaptors::attribute attr(jd);
        std::vector<std::string> keys (attr.list_attributes());
        std::size_t count = keys.size();

        oa << make_nvp("attribute_count", count);
        std::vector<std::string>::iterator end = keys.end();
        for (std::vector<std::string>::iterator it = keys.begin(); it != end; 
             ++it)
        {
            bool is_vector_attribute = attr.attribute_is_vector(*it);

            oa << make_nvp("is_vector", is_vector_attribute);
            if (is_vector_attribute) {
            std::vector<std::string> values (attr.get_vector_attribute(*it));

                oa << make_nvp("key", *it);
                oa << make_nvp("values", values);
            }
            else {
            std::string value (attr.get_attribute(*it));

                oa << make_nvp("key", *it);
                oa << make_nvp("value", value);
            }
        }
    }
        
    saga::cpr::description deserialize_cpr_jd(boost::archive::text_iarchive& ia)
    {
        using namespace boost::serialization;

        saga::cpr::description jd;
        saga::adaptors::attribute attr(jd);
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
                attr.set_vector_attribute(key, values);
            }
            else {
            std::string key, value;

                ia >> make_nvp("key", key);
                ia >> make_nvp("value", value);
                attr.set_attribute(key, value);
            }
        }
        return jd;
    }
    
    std::string 
    cpr_job_serialization::serialize(TR1::shared_ptr<saga::impl::object> obj)
    {
        // get the type of the object (has been serialized already)
        saga::object::type type = obj->get_type();
        if (saga::object::CPRJob     != type  &&
            saga::object::JobService != type  )
        {
            SAGA_THROW_VERBATIM(obj.get(), "job_serialization::serialize: " 
                "Unknwon object type", saga::BadParameter);
        }

        SAGA_OSSTREAM strm;
        {
            using namespace boost::serialization;
            using namespace boost::archive;

            // save package version
            text_oarchive oa(strm);
            unsigned int version = SAGA_VERSION_PACKAGE_CPR_FULL;
            oa << make_nvp("version", version);
            
            if (saga::object::CPRJob == type ) {
            // serialize the cpr_job_cpi_instance data
                typedef saga::adaptors::v1_0::cpr_job_cpi_instance_data 
                    instance_data;
                    
            // so the following cast should succeed
            saga::impl::proxy *proxy = dynamic_cast<saga::impl::proxy *>(obj.get());

                if (NULL == proxy)
                {
                    SAGA_THROW_VERBATIM(obj.get(), "cpr_job_serialization::serialize: " 
                        "Unknwon object type", saga::BadParameter);
                }
                
            saga::adaptors::instance_data<instance_data> data(proxy);
            std::string rm (data->rm_.get_url());
            
                oa << make_nvp("resourcemanager", rm);
                oa << make_nvp("jobid", data->jobid_);
                
                // serialize the start job description
                serialize_cpr_jd(oa, data->jd_start_);

                // serialize the restart job description
                serialize_cpr_jd(oa, data->jd_restart_);
            }
            else if (saga::object::CPRJobService == type) {
            // serialize the cpr_job_service_cpi_instance_data data
                typedef saga::adaptors::v1_0::cpr_job_service_cpi_instance_data 
                    instance_data;

            // so the following cast should succeed
            saga::impl::proxy *proxy = dynamic_cast<saga::impl::proxy *>(obj.get());

                if (NULL == proxy)
                {
                    SAGA_THROW_VERBATIM(obj.get(), "cpr_job_serialization::serialize: " 
                        "Unknwon object type", saga::BadParameter);
                }
                
            saga::adaptors::instance_data<instance_data> data(proxy);
            std::string rm (data->rm_.get_url());
            
                oa << make_nvp("resourcemanager", rm);
            }
        }
        return SAGA_OSSTREAM_GETSTRING(strm);
    }

    TR1::shared_ptr<saga::impl::object> 
    cpr_job_serialization::deserialize(saga::session s, saga::object::type type,
        std::string const& data)
    {
        if (saga::object::CPRJob        != type && 
            saga::object::CPRJobService != type )
        {
            SAGA_THROW_VERBATIM(s, "cpr_job_serialization::deserialize: " 
                "Unknwon object type", saga::BadParameter);
        }

        using namespace boost::serialization;
        using namespace boost::archive;

        SAGA_ISSTREAM strm(data);
        text_iarchive ia(strm);

        // save package version
        unsigned int version = 0;
        ia >> make_nvp("version", version);
        if (!SAGA_VERSION_PACKAGE_CPR_ISCOMPATIBLE_EX(version)) 
        {
            SAGA_THROW_VERBATIM(s, "cpr_job_serialization::deserialize: "
                "Deserialization failed: incompatible version of SAGA cpr_job "
                "package module.", saga::NoSuccess);
        }

        TR1::shared_ptr<saga::impl::object> impl;
        if (saga::object::CPRJob == type) {
        // deserialize the job
        std::string rm;
        std::string jobid;
        
            ia >> make_nvp("resourcemanager", rm);
            ia >> make_nvp("jobid", jobid);

        saga::cpr::description jd(deserialize_cpr_jd(ia));
        saga::cpr::service js(s, rm);
        saga::cpr::job j (js.get_job(jobid));

            impl = saga::impl::runtime::get_impl_object_sp(j);
        }
        else if (saga::object::CPRJobService == type) {
        // deserialize the service_cpi_instance data
        std::string rm;

            ia >> make_nvp("resourcemanager", rm);
            impl.reset(new saga::impl::cpr_job_service(s, rm));
        }
        return impl;
    }
    
///////////////////////////////////////////////////////////////////////////////
}}    // namespace saga::impl

