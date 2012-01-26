//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(SAGA_SHELL_PROXY_DATA_APR_21_2008_0652PM)
#define SAGA_SHELL_PROXY_DATA_APR_21_2008_0652PM

///////////////////////////////////////////////////////////////////////////////
//  proxy_info holds info about a key/value pair, value might refer to a env 
//  variable
struct proxy_info
{
    char const* const key;
    char const* const value;
    bool doecho;
};

///////////////////////////////////////////////////////////////////////////////
//  proxy_type holds everything known for a specific proxy type
struct proxy_type
{
    char const* type;
    char const* context_type;
    proxy_info const* info;
};

///////////////////////////////////////////////////////////////////////////////
// add_proxy omii
proxy_info const omii_proxy_info[] =
{
//  { saga::attributes::context_type, "omii_gridsam" },
    { saga::attributes::context_usercert, "${HOME}/.globus/usercert.pem", true },
    { saga::attributes::context_userkey, "${HOME}/.globus/userkey.pem", true },
    { saga::attributes::context_certrepository, "${HOME}/.globus/certificates/", true },
    { saga::attributes::context_userpass, NULL, false },
    { NULL, NULL }
};

///////////////////////////////////////////////////////////////////////////////
// add_proxy ssh
proxy_info const ssh_proxy_info[] =
{
//  { saga::attributes::context_type, "ssh" },
    { saga::attributes::context_usercert, "${HOME}/.ssh/id_dsa.pub", true },
    { saga::attributes::context_userkey,  "${HOME}/.ssh/id_dsa",     true },
    { saga::attributes::context_userpass, NULL, false },
    { NULL, NULL }
};

///////////////////////////////////////////////////////////////////////////////
//  pull everything together
proxy_type const known_proxies[] =
{
    { "omii", "omii_gridsam", omii_proxy_info },
    { "ssh",  "ssh",          ssh_proxy_info },
    { NULL }
};

#endif
