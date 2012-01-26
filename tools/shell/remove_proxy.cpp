//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "saga-shell.hpp"
#include "proxy_data.hpp"

///////////////////////////////////////////////////////////////////////////////
// remove a credential proxy from the used session object
int shell::c_remove_proxy (std::vector<std::string>& args, props& p)
{
    if (args.empty())
    {
        carp ("format: remove_proxy <type>");
        return FAILURE;
    }

    // try to find the given proxy type
    proxy_type const* kp = known_proxies;
    for (/**/; NULL != kp->type; ++kp)
    {
        if (args[0] == kp->type)
            break;
    }

    // complain if type is not found
    if (!kp->type)
    {
        std::string msg("remove_proxy: unknown proxy type. Known types are: ");
        for (proxy_type const* p = known_proxies; NULL != p->type; ++p)
        {
            msg += std::string("'") + p->type + "', ";
        }
        carp (msg);
        return FAILURE;
    }
    
    // remove the proxies of the given type
    std::string type (kp->context_type);
    std::vector<saga::context> ctxs = session_.list_contexts();

    bool found_one = false;
    std::vector<saga::context>::iterator end = ctxs.end();
    for (std::vector<saga::context>::iterator it = ctxs.begin(); it != end; ++it)
    {
        if ((*it).attribute_exists(saga::attributes::context_type) && 
            type == (*it).get_attribute(saga::attributes::context_type))
        {
            session_.remove_context(*it);
            found_one = true;
        }
    }
    
    if (!found_one)
    {
        carp("remove_proxy: unknown proxy type.");
        return FAILURE;
    }
    
    return SUCCESS;
}
