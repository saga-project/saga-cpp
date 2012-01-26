//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/lexical_cast.hpp>
#include <saga/impl/engine/session.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { 
    /// @cond
    /** These methods are not within API scope TODO wrong namespace?!? */
    std::string get_object_type_name(saga::object::type t)
    {
        switch(t) {
        case saga::object::Exception:        return "Exception";
        case saga::object::URL:              return "URL";
        case saga::object::Buffer:           return "Buffer";
        case saga::object::Session:          return "Session";
        case saga::object::Context:          return "Context";
        case saga::object::Task:             return "Task";
        case saga::object::TaskContainer:    return "TaskContainer";
        case saga::object::Metric:           return "Metric";
        case saga::object::NSEntry:          return "NSEntry";
        case saga::object::NSDirectory:      return "NSDirectory";
        case saga::object::IOVec:            return "IOVec";
        case saga::object::File:             return "File";
        case saga::object::Directory:        return "Directory";
        case saga::object::LogicalFile:      return "LogicalFile";
        case saga::object::LogicalDirectory: return "LogicalDirectory";
        case saga::object::JobDescription:   return "JobDescription";
        case saga::object::JobService:       return "JobService";
        case saga::object::Job:              return "Job";
        case saga::object::JobSelf:          return "JobSelf";
        case saga::object::StreamServer:     return "StreamServer";
        case saga::object::Stream:           return "Stream";
        case saga::object::Parameter:        return "Parameter";
        case saga::object::RPC:              return "RPC";
        case saga::object::Msg:              return "Msg";
        case saga::object::Endpoint:         return "Endpoint";
        case saga::object::Advert:           return "Advert";
        case saga::object::AdvertDirectory:  return "AdvertDirectory";
        case saga::object::CPRJob:           return "CPRJob";
        case saga::object::CPRJobService:    return "CPRJobService";
        case saga::object::CPRCheckpoint:    return "CPRCheckpoint";
        case saga::object::CPRDirectory:     return "CPRDirectory";
        case saga::object::EntityData:       return "EntityData";
        case saga::object::EntityDataSet:    return "EntityDataSet";
        case saga::object::EntityNavigator:  return "EntityNavigator";
        default:
            break;
        }
        return "<Unknown>";
    }
    // @endcond

namespace impl 
{
    object::object(saga::object::type type)
    :  type_(type)
    {
    }

    saga::object::type object::get_type() const
    {
        return type_;
    }

    saga::session& object::get_session()
    {
        SAGA_THROW("This object has no associated session", saga::DoesNotExist);
        return detail::get_the_session();
    }
    saga::session const& object::get_session() const
    {
        SAGA_THROW("This object has no associated session", saga::DoesNotExist);
        return detail::get_the_session();
    }

    saga::uuid object::get_uuid (void) const
    {
      return uuid_;
    }

    void object::set_uuid(saga::uuid const& id)
    {
        uuid_ = id;
    }

    void object::set_type(saga::object::type t)
    {
        type_ = t;
    }

    saga::object::type object::get_cpitype() const
    {
      SAGA_THROW(
        "This object does not implement the object::get_cpitype() function: " 
        "objecttype: " + get_object_type_name(type_), 
        saga::NotImplemented);
      return (saga::object::type)saga::impl::v1_0::cpi::UnknownCPI;
    }
    
    saga::object object::clone() const
    {
      SAGA_THROW(
        "This object does not implement the object::clone() function: " 
        "objecttype: " + get_object_type_name(type_), 
        saga::NotImplemented);
      return saga::object();
    }
    
    bool object::is_equal(saga::impl::object *impl) const
    {
        SAGA_THROW(
          "This object does not implement the object::is_equal() function: " 
          "objecttype: " + get_object_type_name(type_), 
          saga::NotImplemented);
        return false;
    }
    
    void object::init()
    {
      SAGA_THROW(
        "This object does not implement the object::init() function: " + 
        "objecttype: " + get_object_type_name(type_), 
        saga::NotImplemented);
    }

    ///////////////////////////////////////////////////////////////////////////
    saga::impl::attribute* object::get_attributes()
    {
      SAGA_THROW(
        "This object does not expose the SAGA attribute interface: " + 
        "objecttype: " + get_object_type_name(type_), 
        saga::NotImplemented);
      return NULL;
    }

    saga::impl::attribute const* object::get_attributes() const
    {
      SAGA_THROW(
        "This object does not expose the SAGA attribute interface: " + 
        "objecttype: " + get_object_type_name(type_), 
        saga::NotImplemented);
      return NULL;
    }

    ///////////////////////////////////////////////////////////////////////////
    saga::impl::monitorable* object::get_monitorable()
    {
      SAGA_THROW(
        "This object does not expose the SAGA monitorable interface: " + 
        "objecttype: " + get_object_type_name(type_), 
        saga::NotImplemented);
      return NULL;
    }

    saga::impl::monitorable const* object::get_monitorable() const
    {
      SAGA_THROW(
        "This object does not expose the SAGA monitorable interface: " + 
        "objecttype: " + get_object_type_name(type_), 
        saga::NotImplemented);
      return NULL;
    }

    ///////////////////////////////////////////////////////////////////////////
    saga::impl::steerable* object::get_steerable()
    {
      SAGA_THROW(
        "This object does not expose the SAGA steerable interface: " + 
        "objecttype: " + get_object_type_name(type_), 
        saga::NotImplemented);
      return NULL;
    }

    saga::impl::steerable const* object::get_steerable() const
    {
      SAGA_THROW(
        "This object does not expose the SAGA steerable interface: " + 
        "objecttype: " + get_object_type_name(type_), 
        saga::NotImplemented);
      return NULL;
    }

    ///////////////////////////////////////////////////////////////////////////
    saga::impl::task_interface* object::get_task_interface()
    {
      SAGA_THROW(
        "This object does not expose the SAGA task_interface interface: " + 
        "objecttype: " + get_object_type_name(type_), 
        saga::NotImplemented);
      return NULL;
    }

    saga::impl::task_interface const* object::get_task_interface() const
    {
      SAGA_THROW(
        "This object does not expose the SAGA task_interface interface: " + 
        "objecttype: " + get_object_type_name(type_), 
        saga::NotImplemented);
      return NULL;
    }

    ///////////////////////////////////////////////////////////////////////////
    saga::impl::permissions* object::get_permissions()
    {
      SAGA_THROW(
        "This object does not expose the SAGA permissions interface: " + 
        "objecttype: " + get_object_type_name(type_), 
        saga::NotImplemented);
      return NULL;
    }

    saga::impl::permissions const* object::get_permissions() const
    {
      SAGA_THROW(
        "This object does not expose the SAGA permissions interface: " + 
        "objecttype: " + get_object_type_name(type_), 
        saga::NotImplemented);
      return NULL;
    }

///////////////////////////////////////////////////////////////////////////////
}} // namespace saga::impl

