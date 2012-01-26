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
#include <saga/saga/adaptors/packages/job_cpi_instance_data.hpp>
#include <saga/saga/adaptors/packages/job_service_cpi_instance_data.hpp>

#include <saga/saga/packages/job/version.hpp>
#include <saga/impl/packages/job/job_serialization.hpp>

#include <saga/impl/packages/job/job.hpp>
#include <saga/impl/packages/job/job_description.hpp>
#include <saga/impl/packages/job/job_service.hpp>

///////////////////////////////////////////////////////////////////////////////
// Register the serialization factory type
SAGA_PACKAGE_REGISTER(saga::impl::job_serialization);

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl
{

    job_serialization::~job_serialization()
    {
    }

    void 
    serialize_jd(boost::archive::text_oarchive& oa, saga::job::description jd)
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

    saga::job::description deserialize_jd(boost::archive::text_iarchive& ia)
    {
        using namespace boost::serialization;

        saga::job::description jd;
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
    job_serialization::serialize(TR1::shared_ptr<saga::impl::object> obj)
    {
        // get the type of the object (has been serialized already)
        saga::object::type type = obj->get_type();
        if (saga::object::Job != type && saga::object::JobService != type &&
            saga::object::JobDescription != type && saga::object::JobSelf != type) 
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
            unsigned int version = SAGA_VERSION_PACKAGE_JOB_FULL;
            oa << make_nvp("version", version);
            
            if (saga::object::Job == type || saga::object::JobSelf == type) {
            // serialize the job_cpi_instance data
                typedef saga::adaptors::v1_0::job_cpi_instance_data 
                    instance_data;
                    
            // so the following cast should succeed
            saga::impl::proxy *proxy = dynamic_cast<saga::impl::proxy *>(obj.get());

                if (NULL == proxy)
                {
                    SAGA_THROW_VERBATIM(obj.get(), "job_serialization::serialize: " 
                        "Unknwon object type", saga::BadParameter);
                }
                
            saga::adaptors::instance_data<instance_data> data(proxy);
            std::string rm (data->rm_.get_url());
            
                oa << make_nvp("resourcemanager", rm);
                oa << make_nvp("jobid", data->jobid_);
                
                // serialize the job description
                serialize_jd(oa, data->jd_);
            }
            else if (saga::object::JobService == type) {
            // serialize the job_service_cpi_instance_data data
                typedef saga::adaptors::v1_0::job_service_cpi_instance_data 
                    instance_data;

            // so the following cast should succeed
            saga::impl::proxy *proxy = dynamic_cast<saga::impl::proxy *>(obj.get());

                if (NULL == proxy)
                {
                    SAGA_THROW_VERBATIM(obj.get(), "job_serialization::serialize: " 
                        "Unknwon object type", saga::BadParameter);
                }
                
            saga::adaptors::instance_data<instance_data> data(proxy);
            std::string rm (data->rm_.get_url());
            
                oa << make_nvp("resourcemanager", rm);
            }
            else {
            // saga::object::JobDescription == type 
                serialize_jd(oa, saga::job::description(
                    saga::impl::runtime::get_object(obj)));
            }
        }
        return SAGA_OSSTREAM_GETSTRING(strm);
    }

    TR1::shared_ptr<saga::impl::object> 
    job_serialization::deserialize(saga::session s, saga::object::type type,
        std::string const& data)
    {
        if (saga::object::Job != type && saga::object::JobService != type &&
            saga::object::JobDescription != type && saga::object::JobSelf != type) 
        {
            SAGA_THROW_VERBATIM(s, "job_serialization::deserialize: " 
                "Unknwon object type", saga::BadParameter);
        }

        using namespace boost::serialization;
        using namespace boost::archive;

        SAGA_ISSTREAM strm(data);
        text_iarchive ia(strm);

        // save package version
        unsigned int version = 0;
        ia >> make_nvp("version", version);
        if (!SAGA_VERSION_PACKAGE_JOB_ISCOMPATIBLE_EX(version)) 
        {
            SAGA_THROW_VERBATIM(s, "job_serialization::deserialize: "
                "Deserialization failed: incompatible version of SAGA job "
                "package module.", saga::NoSuccess);
        }

        TR1::shared_ptr<saga::impl::object> impl;
        if (saga::object::Job == type || saga::object::JobSelf == type) {
        // deserialize the job
        std::string rm;
        std::string jobid;
        
            ia >> make_nvp("resourcemanager", rm);
            ia >> make_nvp("jobid", jobid);

        saga::job::description jd(deserialize_jd(ia));
        saga::job::service js(s, rm);
        saga::job::job j (js.get_job(jobid));

            impl = saga::impl::runtime::get_impl_object_sp(j);
        }
        else if (saga::object::JobService == type) {
        // deserialize the service_cpi_instance data
        std::string rm;

            ia >> make_nvp("resourcemanager", rm);
            impl.reset(new saga::impl::job_service(s, rm));
        }
        else {
        // saga::object::JobDescription == type 
            impl = saga::impl::runtime::get_impl_object_sp(deserialize_jd(ia));
        }
        return impl;
    }
    
///////////////////////////////////////////////////////////////////////////////
}}    // namespace saga::impl
