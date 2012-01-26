//  Copyright (c) 2008-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/filesystem/path.hpp>

#include <saga/saga.hpp>
#include <saga/saga/path_leaf.hpp>

#include "saga-advert.hpp"

///////////////////////////////////////////////////////////////////////////////
#if defined(SAGA_HAVE_PACKAGE_ADVERT) && defined(SAGA_HAVE_PACKAGE_NAMESPACE)

void advert_list_directory (saga_tools::common & c,
                            std::string          aname, 
                            std::string          pattern)
{
    // instantiate advert directory 
    saga::url ad(aname);
    saga::advert::directory dir (ad);

    // read replica entries
    std::vector<saga::url> entries(dir.list(pattern));
    if (!entries.empty()) {
        std::vector<saga::url>::const_iterator it;
        for(it = entries.begin(); it != entries.end(); ++it)
        {
            boost::filesystem::path p((*it).get_path());
            if (dir.is_dir(saga::detail::leaf(p)))
                std::cout << "d " << (*it) << std::endl;
            else
                std::cout << "  " << (*it) << std::endl;
        }
    }
}

#endif
