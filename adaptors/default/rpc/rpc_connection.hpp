//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(ADAPTORS_DEFAULT_RPC_RPC_CONNECTION_HPP)
#define ADAPTORS_DEFAULT_RPC_RPC_CONNECTION_HPP

#include <XmlRpc.h>

///////////////////////////////////////////////////////////////////////////////
namespace rpc
{
    ///////////////////////////////////////////////////////////////////////////
    class connection 
    :   public XmlRpc::XmlRpcClient
    {
    public:
        connection(std::string const& host, int port, std::string const& funcname);
        ~connection();
        
        bool is_open() const { return _connectionState != NO_CONNECTION; }
        
        bool call (std::vector<saga::rpc::parameter>& args);
        
    private:
        std::string funcname_;
    };

///////////////////////////////////////////////////////////////////////////////
}   // namespace rpc

#endif // !ADAPTORS_DEFAULT_RPC_RPC_CONNECTION_HPP
