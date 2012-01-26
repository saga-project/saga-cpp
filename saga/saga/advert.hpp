//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_SAGA_ADVERT_HPP
#define SAGA_SAGA_ADVERT_HPP

#include <saga/saga-defs.hpp>

# ifdef SAGA_HAVE_PACKAGE_ADVERT
// advert comes from the data package
#include <saga/saga/packages/advert/version.hpp>
#include <saga/saga/packages/advert/advert.hpp>
#include <saga/saga/packages/advert/advert_directory.hpp>

#if !defined(SAGA_ADVERT_PACKAGE_EXPORTS) || defined(SAGA_USE_AUTO_LINKING)
#define SAGA_AUTOLINK_LIB_NAME "advert"
#include <saga/saga/autolink.hpp>
#endif

#else

#if defined(_MSC_VER) || defined(__BORLANDC__) || defined(__DMC__)
#  pragma message ("Warning: The saga-advert package has been disabled at configuration time.")
#elif defined(__GNUC__) || defined(__HP_aCC) || defined(__SUNPRO_CC) || defined(__IBMCPP__)
#  warning "The saga-advert package has been disabled at configuration time."
#endif

#endif

#endif //  SAGA_SAGA_ADVERT_HPP

