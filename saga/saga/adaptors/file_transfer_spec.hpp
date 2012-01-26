//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(FILE_TRANSFER_SPEC_JUL_13_2007_0345PM)
#define FILE_TRANSFER_SPEC_JUL_13_2007_0345PM

#include <string>
#include <saga/saga/util.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace adaptors
{
    ///////////////////////////////////////////////////////////////////////////
    //  possible file transfer operations
    enum file_transfer_operator
    {
        unknown_mode,
        //  Copies the local file to the remote file before the job starts.
        //  Overwrites the remote file if it exists.
        copy_local_remote,    
        //  Copies the local file to the remote file before the job starts.
        //  Appends to the remote file if it exists.
        append_local_remote,
        //  Copies the remote file to the local file after the job finishes.
        //  Overwrites the local file if it exists.
        copy_remote_local,
        //  Copies the remote file to the local file after the job finishes.
        //  Appends to the local file if it exists.
        append_remote_local
    };
    
    ///////////////////////////////////////////////////////////////////////////
    // parse the given string for a file transfer specification and return true
    // if successful
    SAGA_EXPORT bool parse_file_transfer_specification(std::string spec, 
        std::string& left_url, file_transfer_operator& mode, 
        std::string& right_url);
    
///////////////////////////////////////////////////////////////////////////////
}}

#endif
