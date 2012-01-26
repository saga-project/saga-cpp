//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/thread/tss.hpp>
#include <boost/lexical_cast.hpp>

#include <saga/saga/exception.hpp>
#include <saga/impl/exception.hpp>
#include <saga/saga/rpc.hpp>

#include "rpc_connection.hpp"

#ifndef SAGA_SKIP_CONFIG_HPP
# include "saga/saga-defs.hpp"
#endif

///////////////////////////////////////////////////////////////////////////////
namespace rpc
{
    ///////////////////////////////////////////////////////////////////////////
    struct error_handler 
    :   public XmlRpc::XmlRpcErrorHandler
    {
        virtual ~error_handler (void) { };

        error_handler()
        {
            // install the single global instance of this as the current error
            // handler
            XmlRpc::XmlRpcErrorHandler::setErrorHandler(this);
        }
        
        void error(const char* msg) 
        { 
            if (NULL == error_.get())
                error_.reset(new std::string(msg));
            else
                *error_.get() = msg; 
        }
        
        std::string get_error() const
        {
            return error_.get() ? *error_.get() : std::string();
        }
        
        boost::thread_specific_ptr<std::string> error_;
    };
    error_handler handler;
    
    ///////////////////////////////////////////////////////////////////////////
    connection::connection(std::string const& host, int port, 
                           std::string const& funcname)
#if SAGA_XMLRPC_NEEDS_BOOL == 1
      :   XmlRpc::XmlRpcClient(host.c_str(), port, NULL, false), 
#else
      :   XmlRpc::XmlRpcClient(host.c_str(), port, NULL), 
#endif
          funcname_(funcname)
    {
        // try to connect the server
        XmlRpc::XmlRpcValue noArgs, result;
        if (!this->execute("system.listMethods", noArgs, result))
        {
            SAGA_ADAPTOR_THROW_NO_CONTEXT(handler.get_error(), 
                saga::BadParameter);
        }
        
        // we can verify the function name only if the server supports 
        // introspection
        if (!this->isFault()) 
        {
            // make sure return value has the right type
            if (XmlRpc::XmlRpcValue::TypeArray != result.getType())
            {
                SAGA_OSSTREAM strm;
                strm << "Server returned wrong data type (" << result << ")";
                SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                    saga::NoSuccess);
            }
            
            // try to find specified function in the array of supported functions
            bool found = false;
            for (int i = 0; i < result.size(); ++i )
            {
                if (std::string (result[i]) == funcname)
                {
                    found = true;
                    break;
                }
            }

            // function must be supported
            if (!found) {
                SAGA_ADAPTOR_THROW_NO_CONTEXT(
                    "Server does not support function: " + funcname, 
                    saga::DoesNotExist);
            }
        }
    }

    connection::~connection()
    {
        if (is_open())
            this->close();
    }

    bool connection::call (std::vector<saga::rpc::parameter>& args)
    {
        int out_idx = -1;
        int i = 0;
        XmlRpc::XmlRpcValue rpcargs;
        std::vector<saga::rpc::parameter>::iterator end = args.end();
        for (std::vector<saga::rpc::parameter>::iterator it = args.begin();
             it != end; ++it, ++i)
        {
            switch (args[i].get_mode()) {
            case saga::rpc::In:
                {
                    rpcargs[i] = XmlRpc::XmlRpcValue(
                        args[i].get_data(), (int)args[i].get_size());
                }
                break;
            
            case saga::rpc::Out:
                {
                    if (-1 != out_idx) {
                        SAGA_ADAPTOR_THROW_NO_CONTEXT(
                            "More than one 'Out' parameters are not supported",
                            saga::BadParameter);
                    }
                    out_idx = i;
                }
                break;
                
            case saga::rpc::InOut:
                SAGA_ADAPTOR_THROW_NO_CONTEXT(
                    "InOut RPC parameter type for parameter: " + 
                        boost::lexical_cast<std::string>(i) + 
                        " is not supported.", saga::BadParameter);
                break;
                
            default:
                SAGA_ADAPTOR_THROW_NO_CONTEXT(
                    "Wrong RPC parameter type for parameter: " + 
                        boost::lexical_cast<std::string>(i), 
                        saga::BadParameter);
                break;
            }
        }

        // execute the call
        XmlRpc::XmlRpcValue rpcretval;
        if (!this->execute(funcname_.c_str(), rpcargs, rpcretval)) 
        {
            SAGA_ADAPTOR_THROW_NO_CONTEXT(handler.get_error(), 
                saga::NoSuccess);
        }

        // if the function does not exist, we should report this as an error
        if (this->isFault())
        {
            SAGA_OSSTREAM strm;
            strm << "Server does not support function: " << funcname_
                 << " (" << rpcretval << ")";
            SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                saga::NoSuccess);
        }

        // make sure return value gets propagated back to caller
        if (-1 != out_idx) 
        {
            if (XmlRpc::XmlRpcValue::TypeBase64 != rpcretval.getType())
            {
                SAGA_OSSTREAM strm;
                strm << "Server returned wrong data type (" << rpcretval << ")";
                SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                    saga::NoSuccess);
            }

            // allocate the argument buffer
            args[out_idx].set_size(rpcretval.size());
            if (args[out_idx].get_size() < (saga::ssize_t)rpcretval.size())
            {
                SAGA_ADAPTOR_THROW_NO_CONTEXT(
                    "Allocated buffer is to small for returned data.", 
                    saga::NoSuccess);
            }

            // copy over output argument data
            XmlRpc::XmlRpcValue::BinaryData const& data = rpcretval;
            saga::uint8_t* buffer = (saga::uint8_t*)args[out_idx].get_data();
            for (std::size_t i = 0; i < data.size(); ++i)
                buffer[i] = data[i];
        }
        return true;
    }

///////////////////////////////////////////////////////////////////////////////
}   // namespace rpc

